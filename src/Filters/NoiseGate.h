#ifndef NOISE_GATE_H
#define NOISE_GATE_H

#include <cmath>
#include <vector>

/// @brief The NoiseGate class implements a simple noise gate audio effect. It
/// applies a threshold dB to an audio signal and sets samples below that
/// threshold to 0. The class takes a threshold value as input during
/// initialization and processes audio frames through its process method.
class NoiseGate
{
  private:
    /// @brief A private member variable that stores the threshold value in dB
    /// used to gate the audio signal.
    float mThreshold;

  public:
    /// @brief The class constructor that takes a threshold value as input in db
    /// and initializes the mThreshold member variable.
    /// @param threshold The threshold value in dB used to gate the audio
    /// signal. Defaults to -80.
    NoiseGate(float threshold = -80);

    /// @brief The method loops through the audio frames and applies the gating
    /// effect. If a sample value in dB from the input buffer is greater than
    /// the thresholdvalue, it is written to the corresponding sample in the
    /// output buffer.Otherwise, the sample is set to 0.
    /// @param in A pointer to the input audio buffer.
    /// @param out A pointer to the output audio buffer.
    /// @param framesPerBuffer The number of frames in the audio buffer.
    void process(const float* in, float* out, unsigned long framesPerBuffer);

    /// @brief The method loops through the audio frames and applies the gating
    /// effect. If a sample value in dB is greater than the threshold value, it
    /// is not changed. Otherwise, the sample value is set to 0.
    /// @param buffer A vector of floats with amplitude values.
    void process(std::vector<float>& buffer);

    /// @brief Returns the threshold value used by the class.
    /// @return An integer representing the threshold value.
    int getThreshold();
    /// @brief Sets the threshold value to be used by the class.
    /// @param threshold An integer representing the new threshold value to be
    /// set.
    void setThreshold(int threshold);
};

#endif // NOISE_GATE_H
