#include "AudioStreamException.h"

AudioStreamException::AudioStreamException(PaError error) :
    mDescription(Pa_GetErrorText(error))
{}

AudioStreamException::AudioStreamException(const std::string& description) :
    mDescription(description)
{}

const char* AudioStreamException::what() const noexcept
{
    return mDescription.c_str();
}
