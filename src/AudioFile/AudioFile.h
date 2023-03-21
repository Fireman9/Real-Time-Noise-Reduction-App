#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include <string>
#include <vector>

#include "sndfile.h"

#include "../Filters/Kalman.h"
#include "../Filters/NoiseGate.h"

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

    void kalman(unsigned long framesPerBuffer);
    void noise_gate(float threshold);
};

#endif // AUDIO_FILE_H