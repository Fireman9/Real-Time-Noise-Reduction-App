import os
import random
import time
import librosa
import soundfile as sf
import numpy as np


class AudioGenerator:
    """
    Class for creating audio datasets for training and validation.
    """

    def __init__(self, path_clean, path_noise,
                 path_train_noisy, path_train_clean,
                 path_valid_noisy, path_valid_clean,
                 min_snr_db, max_snr_db,
                 train_proportion=0.9):
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
            train_proportion (float, optional): proportion between train and validation data. Defaults to 0.9.
        """

        self.path_clean = path_clean
        self.path_noise = path_noise

        self.path_train_noisy = path_train_noisy
        self.path_train_clean = path_train_clean

        self.path_valid_noisy = path_valid_noisy
        self.path_valid_clean = path_valid_clean

        self.min_snr_db = min_snr_db
        self.max_snr_db = max_snr_db

        self.train_proportion = train_proportion

    def generate_data(self):
        """
        Method for generating an audio dataset from clean and noise audio. Includes train/validation data proportioning and progress tracking.
        """

        # list all clean audio
        clean_files = os.listdir(self.path_clean)

        # calculate total files to be generated
        num_files = len(clean_files) * len(os.listdir(self.path_noise))
        # calculate files count for training based on proportion
        num_train = int(num_files * self.train_proportion)
        # calculate files count for validation based on proportion
        num_valid = num_files - num_train

        # print info about files to be generated
        print(f"Number of files to generate: {num_files}")
        print(f"Number of train files: {num_train}")
        print(f"Number of validation files: {num_valid}")

        # counter and start time to track progress and execution time
        i = 0
        start_time = time.time()

        # loop through each clean file and add noise
        for clean_file in clean_files:
            # load the clean audio file
            clean_audio, sr = librosa.load(
                os.path.join(self.path_clean, clean_file), sr=16000)

            # loop through each noise file in the noise directory
            for noise_file in os.listdir(self.path_noise):
                # load the noise audio file
                noise_audio, sr = librosa.load(
                    os.path.join(self.path_noise, noise_file), sr=16000)

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

                # randomly assign each example to train or validation set
                if random.random() < self.train_proportion:
                    train_flag = True
                else:
                    train_flag = False

                # save noisy and clean audio files in their directories
                if train_flag:
                    # set train path
                    noisy_save_path = os.path.join(
                        self.path_train_noisy, clean_file[: -4] + "_" +
                        noise_file[: -4] + ".wav")
                    clean_save_path = os.path.join(
                        self.path_train_clean, clean_file[: -4] + "_" +
                        noise_file[: -4] + ".wav")
                else:
                    # set validation path
                    noisy_save_path = os.path.join(
                        self.path_valid_noisy, clean_file[: -4] + "_" +
                        noise_file[: -4] + ".wav")
                    clean_save_path = os.path.join(
                        self.path_valid_clean, clean_file[: -4] + "_" +
                        noise_file[: -4] + ".wav")
                # write audio
                sf.write(noisy_save_path, noisy_audio, sr)
                sf.write(clean_save_path, clean_audio, sr)

                # progress tracking
                i += 1
                progress = i / num_files * 100
                print(
                    f"Progress: {progress:.2f}%, Time Elapsed: {time.time()-start_time:.2f}s",
                    end="\r")

        # avoid stacking
        print("")