#ifndef AUDIO_STREAM_EXCEPTION_H
#define AUDIO_STREAM_EXCEPTION_H

#include <stdexcept>
#include <string>

#include "portaudio.h"

/// @brief Custom exception class for audio stream related errors. This class
/// inherits from std::exception and provides custom error messages for audio
/// stream related errors.
class AudioStreamException : public std::exception
{
  private:
    /// @brief The error description.
    std::string mDescription;

  public:
    /// @brief Constructor for AudioStreamException that takes a PortAudio error
    /// code.
    /// @param error The PortAudio error code.
    AudioStreamException(PaError error);
    /// @brief Constructor for AudioStreamException that takes a custom error
    /// description.
    /// @param description The custom error description.
    AudioStreamException(const std::string& description);

    /// @brief Returns the error message for the exception.
    /// @return A C-style string with the error message.
    const char* what() const noexcept override;
};

#endif // AUDIO_STREAM_EXCEPTION_H
