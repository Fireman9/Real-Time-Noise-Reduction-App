#ifndef SANDBOXCLION_AUDIOSTREAM_H
#define SANDBOXCLION_AUDIOSTREAM_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cppflow/cppflow.h>
#include <portaudio.h>

#include "../Filters/NoiseGate.h"
#include "AudioStreamException.h"

class AudioStream
{
  private:
    PaStream* m_stream;

    /// @brief Sample rate.
    int mSR;
    /// @brief One time domain frame size.
    int mBlockLen;

    /// @brief Trained noise reduction model smart pointer.
    std::unique_ptr<cppflow::model> mModel;

    static int process_callback(const void* inputBuffer, void* outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void* userData);

  public:
    AudioStream(std::string modelFilepath = "./model");
    ~AudioStream();

    void open_stream(int output_device_id);
    void open_stream(int input_device_id, int output_device_id);
    void close_stream();

    int get_device_id_by_name(const std::string& deviceName);

    void debug_print_all_devices();
    void debug_print_all_input_devices();
    void debug_print_all_output_devices();

    /// @brief Function to retrieve all input devices available in the system.
    /// @return Vector with the names of the input devices.
    std::vector<std::string> getAllInputDevices();

    /// @brief The class object that is responsible for noise gating.
    NoiseGate mNoiseGate;
};

#endif // SANDBOXCLION_AUDIOSTREAM_H
