import os
import librosa
import numpy as np
import soundfile as sf
from random import shuffle
from tensorflow import TensorShape, float32, data


class ModelDatasetGenerator():
    """
    Class for creating a Tensorflow dataset based on an iterator from an audio dataset. Single-channel audio files only.
    """

    def __init__(self, path_target_noisy, path_target_clean,
                 samples_len, sr, train_flag=False):
        """
        Constructor of the ModelDatasetGenerator class.

        Args:
            path_target_noisy (str): path to the noisy audio
            path_target_clean (str): path to the target clean audio
            samples_len (int): length of audio snippets in samples
            sr (int): sampling rate
            train_flag (bool, optional): flag to enable shuffling of files. Defaults to False.
        """

        # initialising internal class members
        self.path_target_noisy = path_target_noisy
        self.path_target_clean = path_target_clean
        self.samples_len = samples_len
        self.sr = sr
        self.train_flag = train_flag

        # count the total number of samples in the dataset
        self.count_samples()

        # create iterable tf.data.Dataset object
        self.create_tf_data_obj()

    def count_samples(self):
        """
        Method to list the data of the dataset and count the number of samples. 
        """

        # list all wav files in the directory
        self.filenames = [f for f in os.listdir(
            self.path_target_noisy) if f.endswith('.wav')]

        # count the total number of samples in the dataset
        self.total_samples = 0
        for file in self.filenames:
            data, sr = librosa.load(os.path.join(
                self.path_target_noisy, file), sr=None)
            num_samples = int(len(data) / self.samples_len)
            self.total_samples += num_samples

    def create_iterator(self):
        """
        Method to create the iterator.

        Raises:
            ValueError: sampling rate of audio files does not match the specification
            ValueError: more than one audio channel

        Yields:
            float32: yield the noisy and clean chunks as float32
        """

        # check either training or validation
        if self.train_flag:
            shuffle(self.filenames)

        # iterate over the files
        for file in self.filenames:
            # read the audio files
            noisy, sr_noisy = sf.read(
                os.path.join(self.path_target_noisy, file))
            clean, sr_clean = sf.read(
                os.path.join(self.path_target_clean, file))

            # check that the sampling rates match the specifications
            if sr_noisy != self.sr or sr_clean != self.sr:
                raise ValueError(
                    "Sampling rates don't match the specifications.")
            if noisy.ndim != 1 or clean.ndim != 1:
                raise ValueError("Only single channel audio data.")

            # count the number of samples in a file
            num_samples = int(np.fix(noisy.shape[0] / self.samples_len))

            # iterate file by samples
            for i in range(num_samples):
                # cut the audio file in chunks
                noisy_chunks = noisy[i * self.samples_len:
                                     (i + 1) * self.samples_len]
                clean_chunks = clean[i * self.samples_len:
                                     (i + 1) * self.samples_len]

                # yield the noisy and clean chunks as float32
                yield noisy_chunks.astype('float32'), clean_chunks.astype('float32')

    def create_tf_data_obj(self):
        """
        Method to create the tf.data.Dataset.
        """

        self.tf_dataset = data.Dataset.from_generator(
            self.create_iterator,
            (float32, float32),
            (TensorShape([self.samples_len]), TensorShape([self.samples_len]))
        )
