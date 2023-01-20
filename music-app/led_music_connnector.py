import time

import pyaudio
import aubio
import array
import config as cfg


# ONLY FOR PRINTING WAVEFORM
import sys
import struct
import matplotlib
import numpy as np

matplotlib.use("TkAgg")
import matplotlib.pyplot as plt

np.set_printoptions(threshold=sys.maxsize)

BUFFER_SIZE = 256  #Lower is better, got "accurate" results with 512 and lower
FORMAT = pyaudio.paFloat32
CHANNELS = 1
RATE = int(cfg.STEREO_MIX_SAMPLE_RATE)

a_tempo = aubio.tempo("default", BUFFER_SIZE, BUFFER_SIZE//2, RATE)

def find_stereo_mix_index():
    p = pyaudio.PyAudio()
    for i in range(p.get_device_count()):
        device_info = p.get_device_info_by_index(i)
        if device_info.get('name') == cfg.STEREO_MIX_NAME and \
                device_info.get('defaultSampleRate') == cfg.STEREO_MIX_SAMPLE_RATE:
            return i

counter = 0
stream_data_buffer = None
def stream_callback(in_data, frame_count, time_info, flag):
    global stream_data_buffer, counter
    stream_data_buffer = in_data
    # arr = array.array('f', in_data)
    # arrList = np.array(arr.tolist(), dtype=np.float32)
    # U = struct.unpack(str(BUFFER_SIZE//2) + 'f', in_data)
    # u_array = np.array(U, dtype=np.float32)
    u_array2 = np.ndarray((BUFFER_SIZE//2,), np.float32, in_data)
    is_beat = a_tempo(u_array2)
    # print(u_array2)
    if is_beat:
        counter += 1
        print("CLIIIICK " + str(counter))
        print("BPM: " + str(a_tempo.get_bpm()) + ", confidence: " + str(a_tempo.get_confidence()))
    return in_data, pyaudio.paContinue

def start_listener():
    p = pyaudio.PyAudio()
    stereo_mix_index = find_stereo_mix_index()
    stream = p.open(
        format=FORMAT,
        channels=CHANNELS,
        rate=RATE,
        input=True,
        input_device_index=stereo_mix_index,
        frames_per_buffer=BUFFER_SIZE//2,
        stream_callback=stream_callback
    )
    fig, ax = plt.subplots()

    x_axis = np.arange(0, BUFFER_SIZE * 4, 2)  # Buffer times two since we're stepping ever other
    line, = ax.plot(x_axis, np.random.rand(2*BUFFER_SIZE))
    ax.set_ylim(0, 255)
    ax.set_xlim(0, 2*BUFFER_SIZE)
    stream.start_stream()

    while stream.is_active():
        time.sleep(0.1)
        # data = stream.read(BUFFER)
        # data = stream_data_buffer
        # data_int = (np.array(struct.unpack(str(4 * BUFFER_SIZE) + 'B', data))[::2] + 128) % 256
        # line.set_ydata(data_int)
        # fig.canvas.draw()
        # fig.canvas.flush_events()
        # fig.show()


if __name__ == '__main__':
    start_listener()