import math
import os
import numpy as np
import tensorflow as tf
from keras.layers import Lambda, Input, Conv1D, ReLU, BatchNormalization
from keras.optimizers import Adam
from keras.callbacks import CSVLogger, EarlyStopping, ReduceLROnPlateau, \
    ModelCheckpoint

from DatasetGenerator import DatasetGenerator


# TODO: remove
tf.debugging.set_log_device_placement(True)


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
        self.model
        # sample rate
        self.sr = 16000
        # batches of size - batchsize with samples of size - samples_len
        self.batch_size = 32
        self.samples_len = 15
        # activation function
        self.activation = 'sigmoid'

        # TODO
        # self.numUnits = 128
        # self.numLayer = 2

        # one time domain frame size (1, blockLen) for real-time processing
        self.block_len = 512
        # shift for block_len
        self.block_shift = 128
        self.dropout = 0.25
        self.learning_rate = 1e-3
        self.max_epochs = 200

        # TODO
        # self.encoder_size = 256

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
    def snr_cost(pred_val, true_val):
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
            math.pow(true_val, 2),
            axis=-1,
            keepdims=True
        ) / (
            tf.reduce_mean(
                math.pow(true_val - pred_val, 2),
                axis=-1,
                keepdims=True) + 1e-7
        )

        loss = math.log10(snr)
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

    def stft_lambda_layer(self, x):
        """
        Method for the calculation of the STFT for the lambda layer of the model. Calculates the STFT from the continuous signal amplitudes and returns complex STFT values.

        Args:
            x: time signal layer input

        Returns:
            Tensor: complex STFT values where the unique components of the FFT is fft_length // 2 + 1
        """

        # calculate STFT from the continuous signal amplitudes with
        # frame_legth = block_len and frame_step = block_shift
        stft = tf.signal.stft(x, self.block_len, self.block_shift)

        # return calculated STFT
        return stft

    def istft_lambda_layer(self, x):
        """
        Method for the calculation of the inverse STFT for the model lambda layer. Calculates and returns signals representing the inverse STFT for each input STFT.

        Args:
            x (Tensor): complex STFT values

        Returns:
            [..., samples]: Tensor of float signals representing the inverse STFT for each input STFT in stfts.
        """

        # calculate inverse STFT(istft) from complex STFT values
        istft = tf.signal.inverse_stft(x, self.block_len, self.block_shift)

        # return calculated iSTFT signals
        return istft

    def build_model(self):
        """
        Method to build model.
        """

        # input layer for time signal
        time_signal = Input(shape=(1, self.block_len))

        # building model
        x = Conv1D(filters=16, kernel_size=9, padding='same')(time_signal)
        x = BatchNormalization()(x)
        x = ReLU()(x)
        x = Lambda(self.stft_lambda_layer)(x)
        x = Conv1D(filters=16, kernel_size=9, padding='same')(x)
        x = BatchNormalization()(x)
        x = ReLU()(x)
        x = Lambda(self.istft_lambda_layer)(x)
        outputs = x

        # create model
        self.model = tf.keras.Model(inputs=time_signal, outputs=outputs)

        # print model summary
        print(self.model.summary())

    def compile_model(self):
        """
        Method to compile the model.
        """

        # TODO: test mse loss
        # compile model with snr loss function
        self.model.compile(
            loss=self.loss_wrapper(),
            optimizer=Adam(lr=self.learning_rate)
        )

    def train_model(self, run_name,
                    path_train_noisy, path_train_clean,
                    path_test_noisy, path_test_clean):
        """
        Method to train the model.

        Args:
            run_name (str): name of training run
            path_train_noisy (str): path to train noised audio
            path_train_clean (str): path to train clean audio
            path_test_noisy (str): path to test noised audio
            path_test_clean (str): path to test clean audio
        """

        # create save path for model if not created
        savePath = './model_' + run_name + '/'
        if not os.path.exists(savePath):
            os.makedirs(savePath)

        # logger
        csv_logger = CSVLogger(savePath + 'training_' + run_name + '.log')

        # adaptive learning rate
        reduce_lr = ReduceLROnPlateau(monitor='val_loss', factor=0.5,
                                      patience=3, min_lr=1e-8, cooldown=1)

        # callback for early stopping
        early_stopping = EarlyStopping(monitor='val_loss', patience=10)

        # save best results
        checkpoints = ModelCheckpoint(savePath + run_name + '.h5',
                                      monitor='val_loss',
                                      verbose=1,
                                      save_best_only=True,
                                      save_weights_only=True
                                      )

        # audio chunks length in samples
        audio_chunks_len = int(np.fix(
            self.sr * self.samples_len / self.block_shift
        ) * self.block_shift)

        # dataset generator for training data
        generator_train = DatasetGenerator(path_train_noisy,
                                           path_train_clean,
                                           audio_chunks_len,
                                           self.sr, True)
        dataset = generator_train.tf_dataset
        dataset = dataset.batch(self.batch_size, drop_remainder=True).repeat()

        # calculate training steps in one epoch
        train_steps = generator_train.total_samples // self.batch_size

        # dataset generator for testing data
        generator_test = DatasetGenerator(path_test_noisy,
                                          path_test_clean,
                                          audio_chunks_len,
                                          self.sr)
        dataset_test = generator_test.tf_dataset
        dataset_test = dataset_test.batch(
            self.batch_size, drop_remainder=True).repeat()

        # calculate test steps
        test_steps = generator_test.total_samples // self.batch_size

        # start training
        self.model.fit(
            x=dataset,
            batch_size=None,
            steps_per_epoch=train_steps,
            epochs=self.max_epochs,
            verbose=1,
            validation_data=dataset_test,
            validation_steps=test_steps,
            callbacks=[checkpoints, reduce_lr, csv_logger, early_stopping],
            max_queue_size=50,
            workers=12,
            use_multiprocessing=True)

        # clear session
        tf.keras.backend.clear_session()
