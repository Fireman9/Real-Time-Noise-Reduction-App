#ifndef SANDBOXCLION_AUDIOSTREAMEXCEPTION_H
#define SANDBOXCLION_AUDIOSTREAMEXCEPTION_H

#include <stdexcept>
#include <string>

#include "portaudio.h"

// TODO: rework for logging
class AudioStreamException : public std::exception
{
  private:
    std::string m_description;

  public:
    AudioStreamException(PaError error);
    AudioStreamException(const std::string& description);

    const char* what() const noexcept override;
};

#endif // SANDBOXCLION_AUDIOSTREAMEXCEPTION_H
