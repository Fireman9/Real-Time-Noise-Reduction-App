#include "AudioStream.h"

AudioStream::AudioStream(std::string modelFilepath) : mStream(nullptr)
{
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        throw AudioStreamException(err);
    }

    mSR = 48000;
    // 1536 = 32 ms for 48k sr
    mBlockLen = 1536;

    mModel = std::make_unique<cppflow::model>(modelFilepath);

    mNoiseGate = std::make_unique<NoiseGate>(-100);
}

AudioStream::~AudioStream()
{
    closeStream();
    Pa_Terminate();
}

void AudioStream::openStream(int outDeviceId)
{
    if (mStream) {
        closeStream();
    }

    // setup input device parameters
    PaStreamParameters inParams;
    setupDevice(inParams);

    // setup output device parameters
    PaStreamParameters outParams;
    setupDevice(outParams, outDeviceId);

    PaError err = Pa_OpenStream(&mStream, &inParams, &outParams, mSR, mBlockLen,
                                0, processCallback, this);
    if (err != paNoError) {
        throw AudioStreamException(err);
    }

    err = Pa_StartStream(mStream);
    if (err != paNoError) {
        throw AudioStreamException(err);
    }
}

void AudioStream::openStream(int inDeviceId, int outDeviceId)
{
    if (mStream) {
        closeStream();
    }

    // setup input device parameters
    PaStreamParameters inParams;
    setupDevice(inParams, inDeviceId);

    // setup output device parameters
    PaStreamParameters outParams;
    setupDevice(outParams, outDeviceId);

    PaError err = Pa_OpenStream(&mStream, &inParams, &outParams, mSR, mBlockLen,
                                0, processCallback, &mModel);
    if (err != paNoError) {
        throw AudioStreamException(err);
    }

    err = Pa_StartStream(mStream);
    if (err != paNoError) {
        throw AudioStreamException(err);
    }
}

void AudioStream::setupDevice(PaStreamParameters& params, int deviceId)
{
    params.device = deviceId;
    if (params.device == paNoDevice) {
        throw AudioStreamException("Error: No default input device.\n");
    }
    params.channelCount = 1;
    params.sampleFormat = paFloat32;
    params.suggestedLatency =
        Pa_GetDeviceInfo(params.device)->defaultLowInputLatency;
    params.hostApiSpecificStreamInfo = nullptr;
}

int AudioStream::processCallback(const void* inputBuffer, void* outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void* userData)
{
    // init and cast inputBuffet void* to float*
    const float* in = (float*)inputBuffer;
    // init and cast outputBuffet void* to float*
    float* out = (float*)outputBuffer;
    // getting(casting) this class from userData
    auto stream = static_cast<AudioStream*>(userData);

    // create vector from input values
    std::vector<float> inputBufferVector(in, in + framesPerBuffer);
    // create tensor from input values
    cppflow::tensor inputTensor(inputBufferVector, {framesPerBuffer});
    // expand dimension to match model input
    inputTensor = cppflow::expand_dims(inputTensor, 0);

    // predict results using model
    cppflow::tensor outputTensor = stream->mModel->operator()(
        {{"serving_default_main_input:0", inputTensor}},
        {"StatefulPartitionedCall:0"}
    )[0];

    // define the axes along which to remove dimensions of size 1
    std::vector<long long int> axes = {0, 1};
    // squeeze to get only output values
    outputTensor = cppflow::squeeze(outputTensor, axes);
    // get output results vector from tensor
    std::vector<float> outputBufferVector = outputTensor.get_data<float>();

    // get max amplitude value from output buffer amplitudes
    float maxAmplitude =
        *std::max_element(outputBufferVector.begin(), outputBufferVector.end());
    // calculate max amplitude in dB
    int valueDb = 20 * log10(maxAmplitude);
    // emit signal with max output value
    emit stream->tick(valueDb);

    // use noise gate
    stream->mNoiseGate.get()->process(outputBufferVector);

    // emit signal with gated output values
    emit stream->tickGated(maxAmplitude);

    if (inputBuffer == NULL) {
        for (int i = 0; i < framesPerBuffer; i++) {
            *out++ = 0;
        }
    } else {
        for (int i = 0; i < framesPerBuffer; i++) {
            // write all values to output
            *out++ = outputBufferVector[i];
        }
    }

    return paContinue;
}

void AudioStream::closeStream()
{
    if (mStream) {
        PaError err = Pa_StopStream(mStream);
        if (err != paNoError) {
            throw AudioStreamException(err);
        }

        err = Pa_CloseStream(mStream);
        if (err != paNoError) {
            throw AudioStreamException(err);
        }

        mStream = nullptr;
    }
}

int AudioStream::getDeviceIdByName(const std::string& deviceName)
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

void AudioStream::debugPrintAllDevices()
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

void AudioStream::debugPrintAllInputDevices()
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

void AudioStream::debugPrintAllOutputDevices()
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

std::vector<std::string> AudioStream::getAllInputDevices()
{
    std::vector<std::string> devices;
    // get all devices count
    int deviceCount = Pa_GetDeviceCount();
    if (deviceCount < 0) {
        throw AudioStreamException(deviceCount);
    }
    // through all devices
    for (int i = 0; i < deviceCount; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        // if device info is available and it is an input device
        if (deviceInfo != nullptr && deviceInfo->maxInputChannels > 0) {
            // add device info name to vector
            devices.push_back(deviceInfo->name);
        }
    }

    return devices;
}
