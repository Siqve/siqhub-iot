import spotipy
from spotipy.oauth2 import SpotifyOAuth
import requests
import time

# Spotify setup
scope = "user-read-currently-playing user-read-playback-state"
sp = spotipy.Spotify(auth_manager=SpotifyOAuth(client_id="eea11f76c6d244a28dcc97c16c3b1ceb",
                                               client_secret="4d8724d446fa488c820c2f31dcc70351",
                                               redirect_uri="https://localhost:8080",
                                               scope=scope))


def calculate_power(loudness):
    # Define how you convert loudness to the 'power' value
    # This is a placeholder function
    return max(min((loudness + 60) / 60, 2), 0.5)  # example conversion


counter = 0

def send_bounce(beat_power):
    global counter
    counter += 1
    print("test: ", counter)
    requests.get("http://192.168.0.200/update", params={'beat': beat_power})


# Main logic for syncing beats
while True:
    playback = sp.current_playback()
    if playback and playback['is_playing']:
        progress_ms = playback['progress_ms']
        track_id = playback['item']['id']
        analysis = sp.audio_analysis(track_id)

        # Get beats and current segment loudness
        beats = analysis['beats']
        segments = analysis['segments']

        # Find the next beat
        next_beat = next(beat for beat in beats if beat['start'] * 1000 > progress_ms)

        # Calculate the time until the next beat
        time_until_next_beat = (next_beat['start'] * 1000) - progress_ms

        # Wait until right before the next beat to send the request
        time.sleep(time_until_next_beat / 1000)

        # Calculate 'power' based on the loudness of the current segment
        current_segment = next(segment for segment in segments if segment['start'] * 1000 <= progress_ms < (segment['start'] + segment['duration']) * 1000)
        power = 1

        # Send the bounce to the Arduino endpoint
        send_bounce(power)

    # Sleep for a short duration before checking again
    time.sleep(0.1)  # Adjust as needed
