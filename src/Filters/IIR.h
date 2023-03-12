#ifndef IIR_H
#define IIR_H

#include <cmath>

class IIR
{
  public:
    static void fo_hpf(const float* in, float* out,
                       unsigned long framesPerBuffer, float cutoff_freq,
                       float sampling_freq);
    static void fo_lpf(const float* in, float* out,
                       unsigned long framesPerBuffer, float cutoff_freq,
                       float sampling_freq);
    static void fo_apf(const float* in, float* out,
                       unsigned long framesPerBuffer, float cutoff_freq,
                       float sampling_freq);
};

#endif // IIR_H
