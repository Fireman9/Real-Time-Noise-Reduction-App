#include "AudioFile.h"

ProcessAudioFile::ProcessAudioFile(string in_filename, string out_filename) :
    m_in_filename(in_filename), m_out_filename(out_filename)
{
    if (m_out_filename == "") {
        m_out_filename = m_in_filename;
        if (m_out_filename.find_last_of("/") != string::npos) {
            m_out_filename.insert(m_out_filename.find_last_of("/") + 1, "p_");
        }
    }
}

void ProcessAudioFile::open()
{
    m_in_file = sf_open(m_in_filename.c_str(), SFM_READ, &m_in_sf_info);
    if (m_in_file == NULL) {
        // TODO: error handling
        printf("Error opening input file %s: %s\n", m_in_filename,
               sf_strerror(m_in_file));
        return;
    }

    m_out_sf_info = m_in_sf_info;
    m_out_file = sf_open(m_out_filename.c_str(), SFM_WRITE, &m_out_sf_info);
    if (m_out_file == NULL) {
        // TODO: error handling
        printf("Error opening output file %s: %s\n", m_out_filename,
               sf_strerror(m_out_file));
        close();
        return;
    }
}

void ProcessAudioFile::read()
{
    m_total_frames = m_in_sf_info.frames * m_in_sf_info.channels;
    m_in_audio_data.resize(m_total_frames);
    m_out_audio_data.resize(m_total_frames);
    sf_count_t num_read =
        sf_read_float(m_in_file, &m_in_audio_data[0], m_total_frames);
    if (num_read != m_total_frames) {
        // TODO: error handling
        printf("Error reading input file: %s\n", sf_strerror(m_in_file));
        close();
        return;
    }
}

void ProcessAudioFile::write()
{
    m_out_sf_info = m_in_sf_info;
    sf_count_t num_written =
        sf_write_float(m_out_file, &m_out_audio_data[0], m_total_frames);
    if (num_written != m_total_frames) {
        // TODO: error handling
        printf("Error writing output file: %s\n", sf_strerror(m_out_file));
    }
}

void ProcessAudioFile::close()
{
    sf_close(m_in_file);
    sf_close(m_out_file);
}

void ProcessAudioFile::kalman(unsigned long framesPerBuffer)
{
    open();
    read();

    double Q = 0.01;
    double R = 0.1;
    Kalman filter(Q, R);

    for (int i = 0; i < std::ceil(m_total_frames / framesPerBuffer); i++) {
        if (i * framesPerBuffer > m_total_frames) {
            for (int j = i * framesPerBuffer; j < m_total_frames; j++) {
                double x_hat = filter.update(m_in_audio_data[j]);
                m_out_audio_data[j] = x_hat;
            }
        } else {
            for (int j = i * framesPerBuffer; j < (i + 1) * framesPerBuffer;
                 j++) {
                double x_hat = filter.update(m_in_audio_data[j]);
                m_out_audio_data[j] = x_hat;
            }
        }
    }

    write();
    close();
}

void ProcessAudioFile::noise_gate(float threshold)
{
    open();
    read();

    NoiseGate ng(threshold);
    ng.process(m_in_audio_data.data(), m_out_audio_data.data(), m_total_frames);

    write();
    close();
}
