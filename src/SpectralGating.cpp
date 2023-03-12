#include "SpectralGating.h"

SpectralNoiseGating::SpectralNoiseGating() :
    m_fftSize(1024), m_threshold(0.5), m_isInitialized(false), m_fftPlan(NULL),
    m_fftInputBuffer(NULL), m_fftOutputBuffer(NULL),
    m_isNoiseProfileValid(false)
{}

SpectralNoiseGating::SpectralNoiseGating(int fftSize, float threshold) :
    m_fftSize(fftSize), m_threshold(threshold), m_isInitialized(false),
    m_fftPlan(NULL), m_fftInputBuffer(NULL), m_fftOutputBuffer(NULL),
    m_isNoiseProfileValid(false)
{}

SpectralNoiseGating::~SpectralNoiseGating()
{
    if (m_isInitialized) {
        fftw_destroy_plan(m_fftPlan);
        fftw_free(m_fftInputBuffer);
        fftw_free(m_fftOutputBuffer);
    }
}

void SpectralNoiseGating::setFFTSize(int fftSize)
{
    m_fftSize = fftSize;
    m_isNoiseProfileValid = false;
}

void SpectralNoiseGating::setThreshold(float threshold)
{
    m_threshold = threshold;
}

void SpectralNoiseGating::process(const float* in, float* out,
                                  unsigned long framesPerBuffer)
{
    if (!m_isInitialized) {
        initialize();
    }
    if (!m_isNoiseProfileValid) {
        calculateNoiseProfile(in, framesPerBuffer);
    }

    for (int i = 0; i < framesPerBuffer; i += m_fftSize) {
        // Copy the input samples into the FFT input buffer
        for (int j = 0; j < m_fftSize; j++) {
            if (i + j < framesPerBuffer) {
                m_fftInputBuffer[j][0] = in[i + j];
                m_fftInputBuffer[j][1] = 0;
            } else {
                m_fftInputBuffer[j][0] = 0;
                m_fftInputBuffer[j][1] = 0;
            }
        }

        // Execute the FFT
        fftw_execute(m_fftPlan);

        // Apply the noise gating to the FFT output
        for (int j = 0; j < m_fftSize; j++) {
            double magnitude =
                sqrt(m_fftOutputBuffer[j][0] * m_fftOutputBuffer[j][0] +
                     m_fftOutputBuffer[j][1] * m_fftOutputBuffer[j][1]);
            if (magnitude < m_noiseProfile[j] * m_threshold) {
                m_fftOutputBuffer[j][0] = 0;
                m_fftOutputBuffer[j][1] = 0;
            }
        }

        // Execute the inverse FFT
        fftw_execute(m_fftPlan);

        // Copy the output samples into the output buffer
        for (int j = 0; j < m_fftSize; j++) {
            if (i + j < framesPerBuffer) {
                out[i + j] = m_fftOutputBuffer[j][0] / m_fftSize;
            }
        }
    }
}

void SpectralNoiseGating::initialize()
{
    m_fftInputBuffer =
        (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_fftSize);
    m_fftOutputBuffer =
        (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_fftSize);
    m_fftPlan = fftw_plan_dft_1d(m_fftSize, m_fftInputBuffer, m_fftOutputBuffer,
                                 FFTW_FORWARD, FFTW_ESTIMATE);
    m_isInitialized = true;
}

void SpectralNoiseGating::calculateNoiseProfile(const float* in,
                                                unsigned long framesPerBuffer)
{
    m_noiseProfile.resize(m_fftSize);
    // Calculate the noise profile by averaging the magnitudes of the FFT of
    // short sections of the input audio
    vector<double> section(m_fftSize);
    for (int i = 0; i < framesPerBuffer; i += m_fftSize) {
        for (int j = 0; j < m_fftSize; j++) {
            if (i + j < framesPerBuffer) {
                section[j] = in[i + j];
            } else {
                section[j] = 0;
            }
        }
        // Execute the FFT
        fftw_execute_dft_r2c(m_fftPlan, section.data(), m_fftOutputBuffer);

        // Add the magnitudes of the FFT output to the noise profile
        for (int j = 0; j < m_fftSize / 2 + 1; j++) {
            double magnitude =
                sqrt(m_fftOutputBuffer[j][0] * m_fftOutputBuffer[j][0] +
                     m_fftOutputBuffer[j][1] * m_fftOutputBuffer[j][1]);
            m_noiseProfile[j] += magnitude;
        }
    }

    // Normalize the noise profile by the number of sections and the FFT size
    double scaleFactor = 1.0 / (framesPerBuffer / m_fftSize);
    for (int i = 0; i < m_noiseProfile.size(); i++) {
        m_noiseProfile[i] *= scaleFactor;
    }

    m_isNoiseProfileValid = true;
}
