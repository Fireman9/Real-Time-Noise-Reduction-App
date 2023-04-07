import os
from Model import Model
from AudioGenerator import AudioGenerator

# use GPU 0
os.environ["CUDA_VISIBLE_DEVICES"] = "0"


def main():
    # path to clean audio
    path_clean = ""
    # path to noise audio
    path_noise = ""

    # path to train noised audio
    path_train_noisy = ""
    # path to train clean audio
    path_train_clean = ""
    # path to validation noised audio
    path_valid_noisy = ""
    # path to validation clean audio
    path_valid_clean = ""

    audio_generator = AudioGenerator(path_clean, path_noise,
                                     path_train_noisy, path_train_clean,
                                     path_valid_noisy, path_valid_clean,
                                     2, 30, 0.8)
    audio_generator.generate_data()

    # training run name
    run_name = "TestRun"

    # model instance
    model = Model()

    # build model
    model.build_model()

    # compile model
    model.compile_model()

    # train model
    model.train_model(run_name,
                      path_train_noisy, path_train_clean,
                      path_valid_noisy, path_valid_clean)


if __name__ == "__main__":
    main()
