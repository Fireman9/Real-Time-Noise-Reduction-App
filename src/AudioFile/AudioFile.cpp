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

void ProcessAudioFile::filter_audio_file(
    std::function<void(const float*, float*, unsigned long, float, float)>
        process_function)
{
    open();
    read();

    process_function(&m_in_audio_data[0], &m_out_audio_data[0], m_total_frames,
                     50, 44100);

    write();
    close();
}

void ProcessAudioFile::filter_audio_file_by_frames(
    std::function<void(const float*, float*, unsigned long, float, float)>
        process_function,
    unsigned long framesPerBuffer)
{
    open();
    read();

    for (int i = 0; i < std::ceil(m_total_frames / framesPerBuffer); i++) {
        if (i * framesPerBuffer > m_total_frames) {
            process_function(&m_in_audio_data[i * framesPerBuffer],
                             &m_out_audio_data[i * framesPerBuffer],
                             i * framesPerBuffer - m_total_frames, 50, 44100);
        } else {
            process_function(&m_in_audio_data[i * framesPerBuffer],
                             &m_out_audio_data[i * framesPerBuffer],
                             framesPerBuffer, 50, 44100);
        }
    }

    write();
    close();
}

void ProcessAudioFile::spectral_noise_gate_audio_file(int fftSize,
                                                      float threshold)
{
    open();
    read();

    SpectralNoiseGating sng(fftSize, threshold);
    sng.process(&m_in_audio_data[0], &m_out_audio_data[0], m_total_frames);

    write();
    close();
}

void ProcessAudioFile::spectral_noise_gate_audio_file_by_frames(
    int fftSize, float threshold, unsigned long framesPerBuffer)
{
    open();
    read();

    SpectralNoiseGating sng(fftSize, threshold);
    for (int i = 0; i < std::ceil(m_total_frames / framesPerBuffer); i++) {
        if (i * framesPerBuffer > m_total_frames) {
            sng.process(&m_in_audio_data[i * framesPerBuffer],
                        &m_out_audio_data[i * framesPerBuffer],
                        i * framesPerBuffer - m_total_frames);
        } else {
            sng.process(&m_in_audio_data[i * framesPerBuffer],
                        &m_out_audio_data[i * framesPerBuffer],
                        framesPerBuffer);
        }
    }

    write();
    close();
}
