import os
import random
import librosa
import multiprocessing
import math
import soundfile as sf
import numpy as np


class AudioGenerator:
    """
    Class for creating audio datasets for training and validation.
    """

    def __init__(self, path_clean, path_noise,
                 path_train_noisy, path_train_clean,
                 path_valid_noisy, path_valid_clean,
                 min_snr_db, max_snr_db, sr,
                 train_proportion=0.9, num_purts=12):
        """
        Constructor of the AudioGenerator class.

        Args:
            path_clean (str): path to clean audio.
            path_noise (str): path to noise audio.
            path_train_noisy (str): path to store train noisy audio
            path_train_clean (str): path to store train clean audio
            path_valid_noisy (str): path to store validation noisy audio
            path_valid_clean (str): path to store validation clean audio
            min_snr_db (int): min sound-to-noise ratio value in db
            max_snr_db (int): max sound-to-noise ratio value in db
            sr (int): sample rate of audio files.
            train_proportion (float, optional): proportion between train and validation data. Defaults to 0.9.
            num_purts (int, optional): number of processes to use. Defaults to 12.
        """

        self.path_clean = path_clean
        self.path_noise = path_noise

        self.path_train_noisy = path_train_noisy
        self.path_train_clean = path_train_clean

        self.path_valid_noisy = path_valid_noisy
        self.path_valid_clean = path_valid_clean

        self.min_snr_db = min_snr_db
        self.max_snr_db = max_snr_db
        self.sr = sr

        self.train_proportion = train_proportion
        self.num_parts = num_purts

    def normalize_audio(self, audio):
        """
        Method to normalize audio file.

        Args:
            audio (array): Audio data.

        Returns:
            array: Normalized audio data.
        """

        # Calculate the maximum absolute value of the audio samples
        max_audio = np.max(np.abs(audio))

        # Normalize the audio
        audio = audio / max_audio

        return audio

    def add_noise(self, clean_audio, noise_audio):
        """
        Methods for adjusting noise and adding it to clean audio.

        Args:
            clean_audio (array): Audio data of clean audio file.
            noise_audio (array): Audio data of noise audio file.

        Returns:
            array: Audio data of noised audio.
        """

        # randomly select the SNR in the range of min to max
        desired_snr_db = np.random.uniform(
            self.min_snr_db, self.max_snr_db)

        # adjust the noise level based on the desired SNR
        clean_rms = np.sqrt(np.mean(clean_audio**2))
        noise_rms = np.sqrt(np.mean(noise_audio**2))
        target_noise_rms = clean_rms / (10**(desired_snr_db / 20))
        adjusted_noise = noise_audio * (target_noise_rms / noise_rms)

        # resize array to match the length of the clean_audio array
        adjusted_noise = librosa.util.fix_length(
            adjusted_noise, size=len(clean_audio))

        # overlay the clean audio with the adjusted noise
        noisy_audio = clean_audio + adjusted_noise

        return noisy_audio

    def save_audio(self, clean_audio, noisy_audio,
                   clean_filename, noise_filename,
                   train_flag):
        """
        Method to save audio files in train and validation directories.

        Args:
            clean_audio (array): Audio data of clean audio file.
            noisy_audio (array): Audio data of noisy audio file.
            clean_filename (str): Clean audio file filename.
            noise_filename (str): Noise audio file filename.
            train_flag (bool): Train flag, if true - save to train directory, else - save to validation directory.
        """

        # randomly assign each example to train or validation set
        if random.random() < self.train_proportion:
            train_flag = True
        else:
            train_flag = False

        # save noisy and clean audio files in their directories
        if train_flag:
            # set train path
            noisy_save_path = os.path.join(
                self.path_train_noisy, clean_filename + "_" +
                noise_filename + ".wav")
            clean_save_path = os.path.join(
                self.path_train_clean, clean_filename + "_" +
                noise_filename + ".wav")
        else:
            # set validation path
            noisy_save_path = os.path.join(
                self.path_valid_noisy, clean_filename + "_" +
                noise_filename + ".wav")
            clean_save_path = os.path.join(
                self.path_valid_clean, clean_filename + "_" +
                noise_filename + ".wav")

        # write audio
        sf.write(noisy_save_path, noisy_audio, self.sr)
        sf.write(clean_save_path, clean_audio, self.sr)

    def generate_data(self, method="rand"):
        """
        Method for starting audio dataset generation in parallel. Includes train/validation data proportioning.
        """

        # list all clean audio
        clean_files = os.listdir(self.path_clean)

        # divide clean_files into 12 parts
        chunk_size = int(math.ceil(len(clean_files) / self.num_parts))
        chunks = [
            clean_files[i: i + chunk_size]
            for i in range(0, len(clean_files), chunk_size)
        ]

        # calculate total files to be generated
        if method == "rand":
            num_files = len(clean_files)
        elif method == "all_to_all":
            num_files = len(clean_files) * len(os.listdir(self.path_noise))

        # print info about files to be generated
        print(f"Number of files to generate: {num_files}")

        # create a pool of processors
        pool = multiprocessing.Pool(processes=self.num_parts)

        # process each chunk in parallel
        for chunk in chunks:
            if method == "rand":
                pool.apply_async(self.process_random, args=(chunk,))
            elif method == "all_to_all":
                pool.apply_async(self.process_all_to_all, args=(chunk,))

        # close the pool and wait for all processes to complete
        pool.close()
        pool.join()

    def process_all_to_all(self, clean_files):
        """
        Method for generating an audio dataset from clean and noise audio in one process. For clean file generates noisy files with all noises. Includes progress tracking.

        Args:
            clean_files (array): clean files names for one process
        """

        # loop through each clean file and add noise
        for clean_file in clean_files:
            # load the clean audio file
            clean_audio = librosa.load(os.path.join(
                self.path_clean, clean_file), sr=self.sr)

            # Normalize the audio
            clean_audio = self.normalize_audio(clean_audio)

            # loop through each noise file in the noise directory
            for noise_file in os.listdir(self.path_noise):
                # load the noise audio file
                noise_audio = librosa.load(os.path.join(
                    self.path_noise, noise_file), sr=self.sr)

                # overlay the clean audio with the adjusted noise
                noisy_audio = self.add_noise(clean_audio, noise_audio)

                # randomly assign each example to train or validation set
                if random.random() < self.train_proportion:
                    train_flag = True
                else:
                    train_flag = False

                # save audio files in train and validation directories
                self.save_audio(clean_audio, noisy_audio,
                                clean_file[: -4], noise_file[: -4],
                                train_flag)

    def process_random(self, clean_files):
        """
        Method for generating an audio dataset from clean and noise audio in one process. For clean file generates noisy file with random noise file. Includes progress tracking.

        Args:
            clean_files (array): clean files names for one process
        """

        # list all noise audio
        noise_files = os.listdir(self.path_noise)

        # loop through each clean file and add noise
        for clean_file in clean_files:
            # load the clean audio file
            clean_audio = librosa.load(os.path.join(
                self.path_clean, clean_file), sr=self.sr)

            # Normalize the audio
            clean_audio = self.normalize_audio(clean_audio)

            # select random noise
            noise_file = random.choice(noise_files)

            # load the noise audio file
            noise_audio = librosa.load(os.path.join(
                self.path_noise, noise_file), sr=self.sr)

            # overlay the clean audio with the adjusted noise
            noisy_audio = self.add_noise(clean_audio, noise_audio)

            # randomly assign each example to train or validation set
            if random.random() < self.train_proportion:
                train_flag = True
            else:
                train_flag = False

            # save audio files in train and validation directories
            self.save_audio(clean_audio, noisy_audio,
                            clean_file[: -4], noise_file[: -4],
                            train_flag)
