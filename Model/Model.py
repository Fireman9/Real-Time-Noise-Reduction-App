import math
import tensorflow as tf


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

    def lossWrapper(self):
        """
        Wrapper for loss function.
        """

        def lossFunction(y_true, y_pred):
            # calculate loss, squeeze it: remove all size 1 dimensions and returns a tensor of the same type
            loss = tf.squeeze(self.cost_function(y_pred, y_true))

            # mean over batches
            loss = tf.reduce_mean(loss)

            return loss

        return lossFunction
