#! /usr/bin/env python

""" A simple demo using aubio and pyaudio to play beats in real time
Note you will need to have pyaudio installed: `pip install pyaudio`.
Examples:
    ./demo_tapthebeat.py ~/Music/track1.ogg
When compiled with ffmpeg/libav, you should be able to open remote streams. For
instance using youtube-dl (`pip install youtube-dl`):
    ./demo_tapthebeat.py `youtube-dl -xg https://youtu.be/zZbM9n9j3_g`
"""

import sys
import time
import struct
import pyaudio
import aubio
import numpy as np
import matplotlib
import matplotlib.pyplot as plt

matplotlib.use("TkAgg")

win_s = 1024  # fft size
hop_s = win_s // 2  # hop size

# parse command line arguments
if len(sys.argv) < 2:
    print("Usage: %s <filename> [samplerate]" % sys.argv[0])
    sys.exit(1)

filename = sys.argv[1]

samplerate = 0
if len(sys.argv) > 2: samplerate = int(sys.argv[2])

# create aubio source
a_source = aubio.source(filename, samplerate, hop_s)
samplerate = a_source.samplerate

# create aubio tempo detection
a_tempo = aubio.tempo("default", win_s, hop_s, samplerate)

# create a simple click sound
click = 0.7 * np.sin(2. * np.pi * np.arange(hop_s) / hop_s * samplerate / 3000.)

data_2 = None
def pyaudio_callback(_in_data, _frame_count, _time_info, _status):
    global data_2
    samples, read = a_source()
    is_beat = a_tempo(samples)
    if is_beat:
        # samples += click
        print(samples.shape)
    audiobuf = samples.tobytes()
    data_2 = audiobuf
    if read < hop_s:
        return (audiobuf, pyaudio.paComplete)
    return (audiobuf, pyaudio.paContinue)


# create pyaudio stream with frames_per_buffer=hop_s and format=paFloat32
p = pyaudio.PyAudio()
pyaudio_format = pyaudio.paFloat32
frames_per_buffer = hop_s
n_channels = 1
stream = p.open(
    format=pyaudio_format,
    channels=n_channels,
    rate=samplerate,
    output=True,
    frames_per_buffer=frames_per_buffer,
    stream_callback=pyaudio_callback)

fig, ax = plt.subplots()

x = np.arange(0, 8 * frames_per_buffer, 2)
line, = ax.plot(x, np.random.rand(4* frames_per_buffer))
ax.set_ylim(0, 255)
ax.set_xlim(0, 4 * frames_per_buffer)

# start pyaudio stream
stream.start_stream()

# wait for stream to finish
while stream.is_active():
    data = data_2
    data_int = (np.array(struct.unpack(str(4 * frames_per_buffer) + 'B', data))+128)%256
    line.set_ydata(data_int)
    fig.canvas.draw()
    fig.canvas.flush_events()
    fig.show()

# stop pyaudio stream
stream.stop_stream()
stream.close()
# close pyaudio
p.terminate()
