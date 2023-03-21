#include "AudioStream.h"

AudioStream::AudioStream() : m_stream(nullptr)
{
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        throw AudioStreamException(err);
    }
}

AudioStream::~AudioStream()
{
    close_stream();
    Pa_Terminate();
}

// TODO: 1 channel -> 2 channels transfer, getting channelCount from device,
//  constants, input/output setup functions
void AudioStream::open_stream(int output_device_id)
{
    if (m_stream) {
        close_stream();
    }

    PaStreamParameters input_params;
    input_params.device = Pa_GetDefaultInputDevice();
    if (input_params.device == paNoDevice) {
        throw AudioStreamException("Error: No default input device.\n");
    }
    input_params.channelCount = 1;
    input_params.sampleFormat = paFloat32; // move to constants
    input_params.suggestedLatency =
        Pa_GetDeviceInfo(input_params.device)->defaultLowInputLatency;
    input_params.hostApiSpecificStreamInfo = nullptr;

    PaStreamParameters output_params;
    output_params.device = output_device_id;
    if (output_params.device == paNoDevice) {
        throw AudioStreamException(paInvalidDevice);
    }
    output_params.channelCount = 1;
    output_params.sampleFormat = paFloat32; // move to constants
    output_params.suggestedLatency =
        Pa_GetDeviceInfo(output_params.device)->defaultLowOutputLatency;
    output_params.hostApiSpecificStreamInfo = nullptr;

    PaError err =
        Pa_OpenStream(&m_stream, &input_params, &output_params, 48000, 64, 0,
                      process_callback, nullptr); // move to constants
    if (err != paNoError) {
        throw AudioStreamException(err);
    }

    err = Pa_StartStream(m_stream);
    if (err != paNoError) {
        throw AudioStreamException(err);
    }
}

// TODO: 1 channel -> 2 channels transfer, getting channelCount from device,
//  constants, input/output setup functions
void AudioStream::open_stream(int input_device_id, int output_device_id)
{
    if (m_stream) {
        close_stream();
    }

    PaStreamParameters input_params;
    input_params.device = input_device_id;
    if (input_params.device == paNoDevice) {
        throw AudioStreamException("Error: No default input device.\n");
    }
    input_params.channelCount = 1;
    input_params.sampleFormat = paFloat32; // move to constants
    input_params.suggestedLatency =
        Pa_GetDeviceInfo(input_params.device)->defaultLowInputLatency;
    input_params.hostApiSpecificStreamInfo = nullptr;

    PaStreamParameters output_params;
    output_params.device = output_device_id;
    if (output_params.device == paNoDevice) {
        throw AudioStreamException(paInvalidDevice);
    }
    output_params.channelCount = 1;
    output_params.sampleFormat = paFloat32; // move to constants
    output_params.suggestedLatency =
        Pa_GetDeviceInfo(output_params.device)->defaultLowOutputLatency;
    output_params.hostApiSpecificStreamInfo = nullptr;

    PaError err =
        Pa_OpenStream(&m_stream, &input_params, &output_params, 48000, 64, 0,
                      process_callback, nullptr); // 48000, 256 to constants
    if (err != paNoError) {
        throw AudioStreamException(err);
    }

    err = Pa_StartStream(m_stream);
    if (err != paNoError) {
        throw AudioStreamException(err);
    }
}

// TODO: rework all
int AudioStream::process_callback(const void* inputBuffer, void* outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo* timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void* userData)
{
    //    const float *inBase = (float *) inputBuffer;
    //    std::vector<float> vectorBuffer(inBase, inBase+framesPerBuffer);

    const float* in = (const float*)inputBuffer;
    float* out = (float*)outputBuffer;

    //    // TODO: size true?
    //    std::vector<float> outTest(framesPerBuffer*2, 0);
    //    for(int i = 0; i < framesPerBuffer*2; i++) {
    //        outTest[i] = *in++;
    //    }

    unsigned int i;
    //    (void) timeInfo;
    //    (void) statusFlags; /* Prevent unused variable warnings. */
    //    (void) userData;

    if (inputBuffer == NULL) {
        for (i = 0; i < framesPerBuffer; i++) {
            *out++ = 0; /* left - silent */
                        //            *out++ = 0;  /* right - silent */
        }
    } else {
        //        for (i = 0; i < framesPerBuffer; i++) {
        ////            *out++ = FUZZ(*in++);  /* left - distorted */
        ////            *out++ = FUZZ(*in++);  /* right - distorted */
        //            *out++ = *in++;          /*  clean */
        //        }
        for (i = 0; i < framesPerBuffer; i++) {
            *out++ = *in++;
        }
    }

    return paContinue;
}

void AudioStream::close_stream()
{
    if (m_stream) {
        PaError err = Pa_StopStream(m_stream);
        if (err != paNoError) {
            throw AudioStreamException(err);
        }

        err = Pa_CloseStream(m_stream);
        if (err != paNoError) {
            throw AudioStreamException(err);
        }

        m_stream = nullptr;
    }
}

int AudioStream::get_device_id_by_name(const std::string& deviceName)
{
    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0) {
        throw AudioStreamException(deviceCount);
    }

    int deviceId = -1;
    for (int i = 0; i < deviceCount; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo != nullptr && deviceName == deviceInfo->name) {
            deviceId = i;
            break;
        }
    }

    if (deviceId < 0) {
        throw AudioStreamException("Error: No such device with given name.\n");
    }

    return deviceId;
}

void AudioStream::debug_print_all_devices()
{
    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0) {
        throw AudioStreamException(deviceCount);
    }

    std::cout << "Available audio devices:" << std::endl;
    for (int i = 0; i < deviceCount; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo != nullptr) {
            std::cout << i << ": " << deviceInfo->name << std::endl;
        }
    }
}

void AudioStream::debug_print_all_input_devices()
{
    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0) {
        throw AudioStreamException(deviceCount);
    }

    std::cout << "Available input audio devices:" << std::endl;
    for (int i = 0; i < deviceCount; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo != nullptr && deviceInfo->maxInputChannels > 0) {
            std::cout << i << ": " << deviceInfo->name << std::endl;
        }
    }
}

void AudioStream::debug_print_all_output_devices()
{
    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0) {
        throw AudioStreamException(deviceCount);
    }

    std::cout << "Available output audio devices:" << std::endl;
    for (int i = 0; i < deviceCount; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo != nullptr && deviceInfo->maxOutputChannels > 0) {
            std::cout << i << ": " << deviceInfo->name << std::endl;
        }
    }
}
