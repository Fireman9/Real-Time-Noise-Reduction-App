#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include <functional>
#include <string>
#include <vector>
#include <cmath>

#include "sndfile.h"

#include "../Filters/IIR.h"
#include "../SpectralGating.h"

using std::string;
using std::vector;

class ProcessAudioFile
{
  private:
    string m_in_filename;
    string m_out_filename;

    SF_INFO m_in_sf_info;
    SF_INFO m_out_sf_info;

    SNDFILE* m_in_file;
    SNDFILE* m_out_file;

    sf_count_t m_total_frames;

    vector<float> m_in_audio_data;
    vector<float> m_out_audio_data;

    void open();
    void read();
    void write();
    void close();

  public:
    ProcessAudioFile(string in_filename, string out_filename);

    void filter_audio_file(
        std::function<void(const float*, float*, unsigned long, float, float)>
            process_function);
    void filter_audio_file_by_frames(
        std::function<void(const float*, float*, unsigned long, float, float)>
            process_function,
        unsigned long framesPerBuffer);

    // TODO: shorter function names
    void spectral_noise_gate_audio_file(int fftSize, float threshold);
    void
    spectral_noise_gate_audio_file_by_frames(int fftSize, float threshold,
                                             unsigned long framesPerBuffer);
};

#endif // AUDIO_FILE_H