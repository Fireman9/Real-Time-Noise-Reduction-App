import os
from Model.Model import Model

# use GPU 0
os.environ["CUDA_VISIBLE_DEVICES"] = '0'


def main():
    # path to train noised audio
    path_train_noisy = ""
    # path to train clean audio
    path_train_clean = ""
    # path to test noised audio
    path_test_noisy = ""
    # path to test clean audio
    path_test_clean = ""

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
                      path_test_noisy, path_test_clean)


if __name__ == "__main__":
    main()
