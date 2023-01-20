import time

import pyaudio
import aubio
import config as cfg
import numpy as np

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

stream_data_buffer = None
def stream_callback(in_data, frame_count, time_info, flag):
    global stream_data_buffer, counter
    stream_data_buffer = in_data
    u_array2 = np.ndarray((BUFFER_SIZE//2,), np.float32, in_data)
    is_beat = a_tempo(u_array2)
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
    stream.start_stream()

    while stream.is_active():
        time.sleep(0.05)


if __name__ == '__main__':
    start_listener()