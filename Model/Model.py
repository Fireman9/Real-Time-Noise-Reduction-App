import os
import tensorflow as tf
from keras.layers import Lambda, Input, Conv1D, LSTM, Dense, \
    Dropout, Activation, Multiply
from keras.optimizers import Adam
from keras.callbacks import CSVLogger, EarlyStopping, ReduceLROnPlateau, \
    ModelCheckpoint

from ModelDatasetGenerator import ModelDatasetGenerator


class Model():
    """
    Model class
    """

    def __init__(self):
        """
        Constructor of the Model class.
        """

        # initialising internal class members
        # init model
        self.model = []
        # sample rate
        self.sr = 48000

        # batches of size - batchsize
        self.batch_size = 16384
        # activation function
        self.activation = 'sigmoid'

        # mag mask kernel
        self.units_count = 384
        self.layers_count = 2

        # one time domain frame size = 32 ms for 48k sr
        self.block_len = 1536
        # shift for block_len = 8 ms for 48k sr
        self.block_shift = 384

        self.dropout = 0.25
        self.learning_rate = 1e-3
        self.max_epochs = 200

        # filters count for finding features
        self.filter_count = 768

        # epsilon
        self.eps = 1e-7

        # signal-to-noise ratio cost function
        self.cost_function = self.snr_cost

        # set TensorFlow to use the GPU
        physical_devices = tf.config.experimental.list_physical_devices('GPU')
        if len(physical_devices) > 0:
            for device in physical_devices:
                tf.config.experimental.set_memory_growth(device, enable=True)

    @staticmethod
    def snr_cost(pred_val, true_val, epsilon=1e-10):
        """
        Static signal-to-noise ratio cost function method. The loss is 
        always calculated over the last dimension. 

        Args:
            pred_val: predicted value
            true_val: true value

        Returns:
            float: loss, penalty for a bad prediction
        """

        # calculating the signal-to-noise ratio
        snr = tf.reduce_mean(
            tf.pow(true_val, 2),
            axis=-1,
            keepdims=True
        ) / (
            tf.reduce_mean(
                tf.pow(true_val - pred_val, 2),
                axis=-1,
                keepdims=True
            ) + epsilon
        )

        # log10(snr) = log(snr) / log(10)
        num = tf.math.log(snr + epsilon)
        denom = tf.math.log(tf.constant(10, dtype=num.dtype))
        # calculating loss
        loss = -10 * (num / (denom))

        return loss

    def loss_wrapper(self):
        """
        Wrapper for loss function.
        """

        def loss_function(y_true, y_pred):
            # calculate loss, squeeze it: remove all size 1 dimensions and returns a tensor of the same type
            loss = tf.squeeze(self.cost_function(y_pred, y_true))

            # mean over batches
            loss = tf.reduce_mean(loss)

            return loss

        return loss_function

    def fft_lambda_layer(self, x):
        """
        Method for the calculation of the FFT for the lambda layer of the model. Calculates the FFT from the continuous signal amplitudes and returns magnitude and phase list.

        Args:
            x: time signal layer input

        Returns:
            list: magnitude and phase from FFT values
        """

        # expanding dimension
        frame = tf.expand_dims(x, axis=0)
        # calculate FFT from the continuous signal amplitudes
        fft = tf.signal.rfft(frame)

        # calculate magnitude from complex signal
        mag = tf.abs(fft)
        # calculate phase from complex signal
        phase = tf.math.angle(fft)

        # returning magnitude and phase as list
        return [mag, phase]

    def ifft_lambda_layer(self, x):
        """
        Method for the calculation of the inverse FFT for the model lambda layer. Calculates and returns signals representing the inverse FFT for each input FFT.

        Args:
            x (list): magnitude and phase from FFT values

        Returns:
            Tensor: Tensor of Treal signals representing the inverse FFT for each input of FFT.
        """

        # calculating FFT complex representation
        fft = (tf.cast(x[0], tf.complex64)
               * tf.exp((1j * tf.cast(x[1], tf.complex64))))

        # calculate inverse FFT from complex FFT values
        ifft = tf.signal.irfft(fft)

        # return calculated iSTFT signals
        return ifft

    def mask_kernel(self, layers_count, mask_size, x):
        """
        Method for creating a mask in the separation kernel.

        Args:
            layers_count (int): LSTM layers count
            mask_size (int): size of output mask and Dense layer size
            x: signal magnitude

        Returns:
             : mask
        """

        # creating layers_count LSTM layers
        for i in range(layers_count):
            x = LSTM(self.units_count, return_sequences=True,
                     stateful=True)(x)

            # dropout for regularization
            if i < (layers_count - 1):
                x = Dropout(self.dropout)(x)

        # creating the mask
        mask = Dense(mask_size)(x)
        mask = Activation(self.activation)(mask)

        return mask

    def build_model(self):
        """
        Method to build model.
        """

        # building model
        # input layer for time signal
        time_signal = Input(batch_shape=(1, self.block_len))
        # calculating fft
        mag, phase = Lambda(self.fft_lambda_layer)(time_signal)

        # predicting a magnitude mask with separation kernel
        mask_1 = self.mask_kernel(
            self.layers_count,
            (self.block_len // 2 + 1),
            mag
        )

        # applying a magnitude mask from separation kernel
        processed_mag = Multiply()([mag, mask_1])

        # calculating inverse fft
        x = Lambda(self.ifft_lambda_layer)([processed_mag, phase])

        # finding features
        encoded_frames = Conv1D(self.filter_count, 1,
                                strides=1, use_bias=False)(x)

        # predicting a features mask with separation kernel
        mask_2 = self.mask_kernel(
            self.layers_count, self.filter_count, encoded_frames)
        # applying a features mask from separation kernel
        x = Multiply()([encoded_frames, mask_2])

        # back to time domain
        x = Conv1D(self.block_len, 1, padding='causal', use_bias=False)(x)

        # create model
        self.model = tf.keras.Model(inputs=time_signal, outputs=x)

        # print model summary
        print(self.model.summary())

    def compile_model(self):
        """
        Method to compile the model.
        """

        # compile model with snr loss function
        self.model.compile(
            loss=self.loss_wrapper(),
            optimizer=Adam(learning_rate=self.learning_rate)
        )

    def save_model(self, weights_file_path, target_name):
        """
        Method for saving created model with best weight file.

        Args:
            weights_file_path (str): path to weight file
            target_name (str): saved model name
        """

        # build model
        self.build_model()

        # load weights
        self.model.load_weights(weights_file_path)

        # save model
        tf.saved_model.save(self.model, target_name)

    def train_model(self, run_name,
                    path_train_noisy, path_train_clean,
                    path_valid_noisy, path_valid_clean):
        """
        Method to train the model.

        Args:
            run_name (str): name of training run
            path_train_noisy (str): path to train noised audio
            path_train_clean (str): path to train clean audio
            path_valid_noisy (str): path to validation noised audio
            path_valid_clean (str): path to validation clean audio
        """

        # create save path for model if not created
        save_path = './model_' + run_name + '/'
        if not os.path.exists(save_path):
            os.makedirs(save_path)

        # logger
        csv_logger = CSVLogger(save_path + 'training_' + run_name + '.log')

        # adaptive learning rate
        reduce_lr = ReduceLROnPlateau(monitor='val_loss', factor=0.5,
                                      patience=3, min_lr=1e-8, cooldown=1)

        # callback for early stopping
        early_stopping = EarlyStopping(
            monitor='val_loss', min_delta=0.01, patience=10)

        # save best results
        checkpoints = ModelCheckpoint(save_path + run_name + ".h5",
                                      monitor='val_loss',
                                      verbose=1,
                                      save_best_only=True,
                                      save_weights_only=False)

        # dataset generator for training data
        generator_train = ModelDatasetGenerator(path_train_noisy,
                                                path_train_clean,
                                                self.block_len,
                                                self.sr, True)
        dataset = generator_train.tf_dataset
        dataset = dataset.batch(self.batch_size, drop_remainder=True).repeat()

        # calculate training steps in one epoch
        train_steps = generator_train.total_samples // self.batch_size

        # dataset generator for validation data
        generator_valid = ModelDatasetGenerator(path_valid_noisy,
                                                path_valid_clean,
                                                self.block_len,
                                                self.sr)
        dataset_valid = generator_valid.tf_dataset
        dataset_valid = dataset_valid.batch(
            self.batch_size, drop_remainder=True).repeat()

        # calculate validation steps
        valid_steps = generator_valid.total_samples // self.batch_size

        # start training
        self.model.fit(
            x=dataset,
            batch_size=None,
            steps_per_epoch=train_steps,
            epochs=self.max_epochs,
            verbose=1,
            validation_data=dataset_valid,
            validation_steps=valid_steps,
            callbacks=[checkpoints, reduce_lr, csv_logger, early_stopping],
            max_queue_size=50,
            workers=12,
            use_multiprocessing=True)

        # save model
        self.save_model(save_path + run_name + ".h5", save_path + run_name)

        # clear session
        tf.keras.backend.clear_session()
