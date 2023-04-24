#include "AudioStreamException.h"

AudioStreamException::AudioStreamException(PaError error) :
    m_description(Pa_GetErrorText(error))
{}

AudioStreamException::AudioStreamException(const std::string& description) :
    m_description(description)
{}

const char* AudioStreamException::what() const noexcept
{
    return m_description.c_str();
}
