STEREO_MIX_NAME = "Stereo Mix (Realtek(R) Audio)"
STEREO_MIX_SAMPLE_RATE = 48000.0


IP = '192.168.0.150' #use param instead

# DISPLAY_FPS = True
# """Whether to display the FPS when running (can reduce performance)"""

MIC_RATE = 44100
"""Sampling frequency of the microphone in Hz"""

FPS = 60

MIN_FREQUENCY = 200
"""Frequencies below this value will be removed during audio processing"""

MAX_FREQUENCY = 12000
"""Frequencies above this value will be removed during audio processing"""

N_FFT_BINS = 24
"""Number of frequency bins to use when transforming audio to frequency domain
Fast Fourier transforms are used to transform time-domain audio data to the
frequency domain. The frequencies present in the audio signal are assigned
to their respective frequency bins. This value indicates the number of
frequency bins to use.
A small number of bins reduces the frequency resolution of the visualization
but improves amplitude resolution. The opposite is true when using a large
number of bins. More bins is not always better!
There is no point using more bins than there are pixels on the LED strip.
"""

N_ROLLING_HISTORY = 2
"""Number of past audio frames to include in the rolling window"""

MIN_VOLUME_THRESHOLD = 1e-7
"""No music visualization displayed if recorded audio volume below threshold"""