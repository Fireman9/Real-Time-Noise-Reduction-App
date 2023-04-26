#include "NoiseGate.h"

NoiseGate::NoiseGate(float threshold) : mThreshold(threshold) {}

void NoiseGate::process(const float* in, float* out,
                        unsigned long framesPerBuffer)
{
    for (int i = 0; i < framesPerBuffer; ++i) {
        if (20 * log10(in[i]) > mThreshold) {
            out[i] = in[i];
        } else {
            out[i] = 0;
        }
    }
}

void NoiseGate::process(std::vector<float>& buffer)
{
    for (auto value : buffer) {
        if (20 * log10(value) < mThreshold) {
            value = 0;
        }
    }
}

int NoiseGate::getThreshold()
{
    return mThreshold;
}

void NoiseGate::setThreshold(int threshold)
{
    mThreshold = threshold;
}
