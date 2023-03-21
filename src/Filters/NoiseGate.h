#ifndef NOISE_GATE_H
#define NOISE_GATE_H

#include <cmath>

/// @brief The NoiseGate class implements a simple noise gate audio effect. It
/// applies a threshold to an audio signal and sets samples below that threshold
/// to 0. The class takes a threshold value as input during initialization and
/// processes audio frames through its process method.
class NoiseGate
{
  private:
    /// @brief A private member variable that stores the threshold value used to
    /// gate the audio signal.
    float m_threshold;

  public:
    /// @brief The class constructor that takes a threshold value as input in db
    /// and initializes the m_threshold member variable converting it to
    /// amplitude.
    /// @param threshold The threshold amplitude value used to gate the audio
    /// signal.
    NoiseGate(float threshold) { m_threshold = powf(10.0, threshold / 20.0); }

    /// @brief The method loops through the audio frames and applies the gating
    /// effect. If a sample in the input buffer is greater than the threshold
    /// value, it is written to the corresponding sample in the output buffer.
    /// Otherwise, the sample is set to 0.
    /// @param in A pointer to the input audio buffer.
    /// @param out A pointer to the output audio buffer.
    /// @param framesPerBuffer The number of frames in the audio buffer.
    void process(const float* in, float* out, unsigned long framesPerBuffer)
    {
        for (int i = 0; i < framesPerBuffer; ++i) {
            if (std::abs(in[i]) > m_threshold) {
                out[i] = in[i];
            } else {
                out[i] = 0;
            }
        }
    }
};

#endif // NOISE_GATE_H
