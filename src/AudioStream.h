#ifndef SANDBOXCLION_AUDIOSTREAM_H
#define SANDBOXCLION_AUDIOSTREAM_H

#include <algorithm>
#include <iostream>
#include <string>

#include "portaudio.h"

#include "AudioStreamException.h"
#include "Filters/IIR.h"

class AudioStream
{
  private:
    PaStream* m_stream;

    static int process_callback(const void* inputBuffer, void* outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void* userData);

  public:
    AudioStream();
    ~AudioStream();

    void open_stream(int output_device_id);
    void open_stream(int input_device_id, int output_device_id);
    void close_stream();

    int get_device_id_by_name(const std::string& deviceName);

    void debug_print_all_devices();
    void debug_print_all_input_devices();
    void debug_print_all_output_devices();
};

#endif // SANDBOXCLION_AUDIOSTREAM_H
