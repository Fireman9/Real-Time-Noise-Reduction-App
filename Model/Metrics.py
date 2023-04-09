import librosa
import numpy as np
from pesq import pesq
from pystoi import stoi
import matplotlib.pyplot as plt


# Load the audio files
audio_clean, sr = librosa.load("", sr=None)
audio_noisy, sr = librosa.load("", sr=None)

# Calculate the metrics
pesq_score = pesq(sr, audio_clean, audio_noisy, "wb")
stoi_score = stoi(audio_noisy, audio_clean, sr)

# Print the results
print("STOI: {:.2f}".format(stoi_score))
print("PESQ: {:.2f}".format(pesq_score))


# plot noisy and clean waveforms
plt.figure(figsize=(12, 3))
librosa.display.waveshow(audio_noisy, sr=sr)
librosa.display.waveshow(audio_clean, sr=sr)
plt.title("Audio waveform")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.savefig("plot.png")

# plot noisy spectrogram
noisy = np.abs(librosa.stft(audio_noisy))
plt.figure(figsize=(12, 5))
librosa.display.specshow(
    librosa.amplitude_to_db(noisy, ref=np.max),
    y_axis="log", x_axis="time", sr=sr)
plt.colorbar(format="%+2.0f dB")
plt.title("Spectrogram")
plt.savefig("spec_noisy.png")

# plot clean spectrogram
clean = np.abs(librosa.stft(audio_clean))
librosa.display.specshow(
    librosa.amplitude_to_db(clean, ref=np.max),
    y_axis="log", x_axis="time", sr=sr)
plt.savefig("spec_clean.png")
