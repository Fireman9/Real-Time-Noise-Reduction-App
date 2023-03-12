#ifndef SPECTRAL_GATING_H
#define SPECTRAL_GATING_H

#include <cmath>
#include <vector>

#include "fftw3.h"

using std::vector;

class SpectralNoiseGating
{
  private:
    /// @brief The FFT size used by the algorithm.
    int m_fftSize;

    /// @brief The noise threshold used by the algorithm.
    float m_threshold;

    /// @brief Flag that indicates whether the FFTW library has been
    /// initialized.
    bool m_isInitialized;

    /// @brief The FFTW plan used by the algorithm.
    fftw_plan m_fftPlan;

    /// @brief The FFT input buffer used by the algorithm.
    fftw_complex* m_fftInputBuffer;

    /// @brief The FFT output buffer used by the algorithm.
    fftw_complex* m_fftOutputBuffer;

    /// @brief The noise profile calculated by the algorithm.
    vector<double> m_noiseProfile;

    /// @brief Flag that indicates whether the noiseprofile is valid.
    bool m_isNoiseProfileValid;

    /// @brief Method that initializes the FFTW library and creates the FFTW
    /// plan.
    void initialize();

    /// @brief Method that calculates the noise profile from the input audio.
    /// @param in Input buffer.
    /// @param framesPerBuffer Input buffer size.
    void calculateNoiseProfile(const float* in, unsigned long framesPerBuffer);

  public:
    /// @brief Constructor that initializes the class with default values for
    /// FFT size and threshold.
    SpectralNoiseGating();

    /// @brief Constructor that initializes the class with user-specified values
    /// for FFT size and threshold.
    /// @param fftSize The FFT size that the algorithm will use.
    /// @param threshold The noise threshold that the algorithm will use.
    SpectralNoiseGating(int fftSize, float threshold);

    /// @brief Destructor that cleans up memory used by the class.
    ~SpectralNoiseGating();

    /// @brief Method that processes the input audio and outputs the noise -
    /// gated signal.
    /// @param in Input buffer.
    /// @param out Output buffer.
    /// @param framesPerBuffer Input/output buffer size.
    void process(const float* in, float* out, unsigned long framesPerBuffer);

    /// @brief Method that sets the FFT size used by the algorithm.
    /// @param fftSize The FFT size that the algorithm will use.
    void setFFTSize(int fftSize);

    /// @brief Method that sets the noise threshold used by the algorithm.
    /// @param threshold The noise threshold that the algorithm will use.
    void setThreshold(float threshold);
};

#endif // SPECTRAL_GATING_H
