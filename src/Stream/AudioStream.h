#ifndef AUDIO_STREAM_H
#define AUDIO_STREAM_H

#include <QObject>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <cppflow/cppflow.h>
#include <portaudio.h>

#include "../Filters/NoiseGate.h"
#include "AudioStreamException.h"

/// @brief Class representing an audio stream.
class AudioStream : public QObject
{
    Q_OBJECT

  private:
    /// @brief Pointer to the audio stream object.
    PaStream* mStream;

    /// @brief Sample rate.
    int mSR;
    /// @brief One time domain frame size.
    int mBlockLen;

    /// @brief Flag to indicate that the noise reduction is active.
    bool mReduceNoiseStatus;

    /// @brief Trained noise reduction model smart pointer.
    std::unique_ptr<cppflow::model> mModel;

    /// @brief Static function representing the process callback function.
    /// @param inputBuffer Pointer to the input buffer.
    /// @param outputBuffer Pointer to the output buffer.
    /// @param framesPerBuffer Number of frames per buffer.
    /// @param timeInfo Pointer to the time info structure.
    /// @param statusFlags The status flags.
    /// @param userData Pointer to the user data.
    /// @return The port audio stream callback result.
    static int processCallback(const void* inputBuffer, void* outputBuffer,
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo* timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void* userData);

    /// @brief Private function to setup device parameters.
    /// @param params Reference to stream parameters object.
    /// @param deviceId Devide ID. Defaults to default device ID.
    void setupDevice(PaStreamParameters& params,
                     int deviceId = Pa_GetDefaultInputDevice());

  public:
    /// @brief Constructor for the AudioStream class.
    /// @param modelFilepath Path to the noise reduction model directory.
    AudioStream(std::string modelFilepath = "./model");
    /// @brief Destructor for the AudioStream class.
    ~AudioStream();

    /// @brief Function to open the stream from the default input device.
    /// @param outDeviceId The ID of the output device.
    /// @throws AudioStreamException If there is an error opening the stream.
    void openStream(int outDeviceId);
    /// @brief Function to open the stream.
    /// @param inDeviceId The ID of the input device.
    /// @param outDeviceId The ID of the output device.
    /// @throws AudioStreamException If there is an error opening the stream.
    void openStream(int inDeviceId, int outDeviceId);
    /// @brief Function to close the audio stream.
    void closeStream();

    /// @brief Function to get the device ID by name.
    /// @param deviceName The name of the device.
    /// @return The device ID.
    int getDeviceIdByName(const std::string& deviceName);

    void debugPrintAllDevices();
    void debugPrintAllInputDevices();
    void debugPrintAllOutputDevices();

    /// @brief Function to retrieve all input devices available in the system.
    /// @return Vector with the names of the input devices.
    std::vector<std::string> getAllInputDevices();

    /// @brief The class object smart pointer which is responsible for noise
    /// gating.
    std::unique_ptr<NoiseGate> mNoiseGate;

  signals:
    /// @brief The custom signal for passing the maximum amplitude value in dB
    /// on sound input.
    /// @param value The maximum amplitude value in dB.
    void tick(int value);
    /// @brief The custom signal for passing the maximum amplitude value on
    /// sound input.
    /// @param outBuffer The maximum amplitude value.
    void tickGated(float value);

  public slots:
    /// @brief Function to set noise reduction status.
    /// @param status Boolean to set.
    void setReduceNoise(bool status);
};

#endif // AUDIO_STREAM_H
