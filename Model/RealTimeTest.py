import soundfile as sf
import numpy as np
import tensorflow as tf


# one time domain frame size = 32 ms for 16k sr
block_len = 512
# shift for block_len = 8 ms for 16k sr
block_shift = 128

# load model
model = tf.saved_model.load("")
# inference model
infer = model.signatures["serving_default"]

# load audio
audio, sr = sf.read("")

# check sampling rate
if sr != 16000:
    raise ValueError("Model supports only 16000 sampling rate.")

# output audio init
out_file = np.zeros((len(audio)))

# create input and output buffers
input_buffer = np.zeros((block_len))
output_buffer = np.zeros((block_len))

# calculate number of blocks
num_blocks = (audio.shape[0] - (block_len-block_shift)) // block_shift

# iterate over the number of blocks
for i in range(num_blocks):
    # shift values and write to the buffer
    input_buffer[:-block_shift] = input_buffer[block_shift:]
    input_buffer[-block_shift:] = \
        audio[i * block_shift: (i * block_shift) + block_shift]

    # create a batch dimension of one
    in_block = np.expand_dims(input_buffer, axis=0).astype('float32')

    # process one block
    out_block = infer(tf.constant(in_block))["conv1d_3"]

    # shift values and write to buffer
    output_buffer[:-block_shift] = output_buffer[block_shift:]
    output_buffer[-block_shift:] = np.zeros((block_shift))
    output_buffer += np.squeeze(out_block)
    
    # devide signal values by 2
    output_buffer[:] = [x / 2 for x in output_buffer]

    # write block to output file
    out_file[i * block_shift:
             (i * block_shift) + block_shift] = output_buffer[:block_shift]


# write result to .wav file
sf.write("", out_file, sr)
