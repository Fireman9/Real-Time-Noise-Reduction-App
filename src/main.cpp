#include <QApplication>

#include "AudioFile/AudioFile.h"
#include "AudioStream.h"
#include "GUI/MainWidget.h"

void stream()
{
    AudioStream audioStream;
    audioStream.open_stream(5);
    //    audioStream.open_stream(1, 5);

    printf("Hit ENTER to stop.\n");
    getchar();

    audioStream.close_stream();

    //    std::cout<< audioStream.get_device_id_by_name("VAC Output (Virtual
    //    Audio Cable")
    //    <<std::endl; audioStream.debug_print_all_devices();
    //    audioStream.debug_print_all_output_devices();
    //    audioStream.debug_print_all_input_devices();
}

void audio_file_test()
{
    // ProcessAudioFile audio_file("C:/Users/Admin/Downloads/mix.wav",
    //                             "C:/Users/Admin/Downloads/test_mix.wav");
    ProcessAudioFile audio_file(
        "C:/Users/Admin/Downloads/noise_train_AirportAnnouncements_8.wav",
        "C:/Users/Admin/Downloads/done.wav");
    // audio_file.kalman(64);
    audio_file.adaptive_kalman(64);
    // TODO: check params: frames per buffer everywhere, fft_size, threshold in
    // sng
    // audio_file.filter_audio_file(IIR::fo_hpf);
    // audio_file.filter_audio_file_by_frames(IIR::fo_hpf, 64);

    // audio_file.spectral_noise_gate_audio_file(1024, 16);
    // audio_file.spectral_noise_gate_audio_file_by_frames(2048, 16, 64);
    // audio_file.noise_gate(-60);
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    MainWidget widget;
    widget.show();

    return QApplication::exec();
}
