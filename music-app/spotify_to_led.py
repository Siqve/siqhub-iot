import time

import requests
import spotipy
from spotipy.oauth2 import SpotifyOAuth

SPOTIPY_CLIENT_ID = 'eea11f76c6d244a28dcc97c16c3b1ceb'
SPOTIPY_CLIENT_SECRET = '4d8724d446fa488c820c2f31dcc70351'
SPOTIPY_REDIRECT_URI = 'http://localhost:3000'
SCOPE = 'user-read-playback-state user-read-currently-playing'

spotify = spotipy.Spotify(auth_manager=SpotifyOAuth(client_id=SPOTIPY_CLIENT_ID,
                                                    client_secret=SPOTIPY_CLIENT_SECRET,
                                                    redirect_uri=SPOTIPY_REDIRECT_URI,
                                                    scope=SCOPE))

current_track_id = None
progress_ms = 0 
track_start_time = None

beats_locations = []

mode = "BPM"


def is_playing(current_track):
    return current_track and current_track['is_playing']


def calculate_beats_from_bpm(track_id):
    global beats_locations
    features = spotify.audio_features([track_id])[0]
    duration_sec = features['duration_ms'] / 1000.0
    beat_interval = 60.0 / features['tempo']
    beats_locations = [beat_interval * i for i in range(int(duration_sec / beat_interval))]

def analyze_current_track():
    global progress_ms, track_start_time, current_track_id, beats_locations
    current_track = spotify.current_playback()

    if not is_playing(current_track):
        print("No track is currently playing.")
        track_start_time = None  # Reset the start time when no track is playing
        time.sleep(1)
        return
    # Retrieve track ID and progress in the song
    track_id = current_track['item']['id']
    progress_ms = current_track['progress_ms']

    # If track_start_time is None, it means this is a new track or the track has resumed
    if track_start_time is None or track_id != current_track_id:
        # Calculate the UNIX timestamp when the track started playing
        track_start_time = time.time() - (progress_ms / 1000)
        current_track_id = track_id
        calculate_beats_from_bpm(track_id)
        return

    if mode == "BPM":
        sync_with_bpm(track_start_time, beats_locations)
    else:
        # Perform audio analysis
        analysis = spotify.audio_analysis(track_id)
        sync_with_beats(track_start_time, analysis['beats'])


def sync_with_beats(track_start_time, beats_array):
    # Find the current beat
    current_time_in_track = time.time() - track_start_time
    for i, beat in enumerate(beats_array):
        beat_start = beat['start']
        beat_end = beat_start + beat['duration']

        if beat_start <= current_time_in_track <= beat_end:
            # If this is the last beat, break
            if i == len(beats_array) - 1:
                break

            # Get the start time of the next beat
            next_beat_start = beats_array[i + 1]['start']

            # Calculate the time to wait until the next beat
            time_to_wait = next_beat_start - current_time_in_track

            # Wait until the next beat starts
            time.sleep(time_to_wait)

            # Print beat information at the transition point
            print(f"Transitioning to new beat at {next_beat_start} seconds")
            requests.get("http://192.168.0.200/update", params={'beat': 100})
            break


def sync_with_bpm(track_start_time, beats_array):
    # Find the current beat
    current_time_in_track = time.time() - track_start_time
    for i, beat in enumerate(beats_array):
        if i == len(beats_array) - 1:
            break
        beat_start = beat
        beat_end = beats_array[i+1]

        if beat_start <= current_time_in_track <= beat_end:
            # If this is the last beat, break
            if i == len(beats_array) - 1:
                break

            # Get the start time of the next beat
            next_beat_start = beats_array[i + 1]

            # Calculate the time to wait until the next beat
            time_to_wait = next_beat_start - current_time_in_track

            # Wait until the next beat starts
            time.sleep(time_to_wait)

            # Print beat information at the transition point
            print(f"Transitioning to new beat at {next_beat_start} seconds")
            requests.get("http://192.168.0.200/update", params={'beat': 100})
            break


if __name__ == '__main__':
    try:
        while True:
            analyze_current_track()
            #time.sleep(0.5)
    except KeyboardInterrupt:
        print("Stopping playback analysis.")