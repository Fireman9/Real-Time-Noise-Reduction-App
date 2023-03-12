#include "IIR.h"

void IIR::fo_hpf(const float* in, float* out, unsigned long framesPerBuffer,
                 float cutoff_freq, float sampling_freq)
{
    float th = 2.0 * M_PI * cutoff_freq / sampling_freq;
    float g = cos(th) / (1.0 + sin(th));
    float a0 = (1.0 + g) / 2.0;
    float a1 = -((1.0 + g) / 2.0);
    float a2 = 0.0;
    float b1 = -g;
    float b2 = 0.0;

    float x1 = 0.0;
    float x2 = 0.0;
    float y1 = 0.0;
    float y2 = 0.0;

    for (int i = 0; i < framesPerBuffer; i++) {
        *out = a0 * (*in) + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;
        x2 = x1;
        x1 = *in++;
        y2 = y1;
        y1 = *out++;
    }
}

void IIR::fo_lpf(const float* in, float* out, unsigned long framesPerBuffer,
                 float cutoff_freq, float sampling_freq)
{
    float th = 2.0 * M_PI * cutoff_freq / sampling_freq;
    float g = cos(th) / (1.0 + sin(th));
    float a0 = (1.0 - g) / 2.0;
    float a1 = (1.0 - g) / 2.0;
    float a2 = 0.0;
    float b1 = -g;
    float b2 = 0.0;

    float x1 = 0.0;
    float x2 = 0.0;
    float y1 = 0.0;
    float y2 = 0.0;

    for (int i = 0; i < framesPerBuffer; i++) {
        *out = a0 * (*in) + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;
        x2 = x1;
        x1 = *in++;
        y2 = y1;
        y1 = *out++;
    }
}

void IIR::fo_apf(const float* in, float* out, unsigned long framesPerBuffer,
                 float cutoff_freq, float sampling_freq)
{
    float a = (tan(M_PI * cutoff_freq / sampling_freq) - 1.0) /
              (tan(M_PI * cutoff_freq / sampling_freq) + 1.0);
    float a0 = a;
    float a1 = 1.0;
    float a2 = 0.0;
    float b1 = a;
    float b2 = 0.0;

    float x1 = 0.0;
    float x2 = 0.0;
    float y1 = 0.0;
    float y2 = 0.0;

    for (int i = 0; i < framesPerBuffer; i++) {
        *out = a0 * (*in) + a1 * x1 + a2 * x2 - b1 * y1 - b2 * y2;
        x2 = x1;
        x1 = *in++;
        y2 = y1;
        y1 = *out++;
    }
}
