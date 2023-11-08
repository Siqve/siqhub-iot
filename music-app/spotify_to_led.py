import time
import spotipy
from spotipy.oauth2 import SpotifyOAuth

SPOTIPY_CLIENT_ID = '64553d87f5604475b731c64d2b8437c9'
SPOTIPY_CLIENT_SECRET = 'd02514bfd6334ac897bc0a8f18a9a63d'
SPOTIPY_REDIRECT_URI = 'http://localhost:3000'
SCOPE = 'user-read-playback-state user-read-currently-playing'

spotify = spotipy.Spotify(auth_manager=SpotifyOAuth(client_id=SPOTIPY_CLIENT_ID,
                                                    client_secret=SPOTIPY_CLIENT_SECRET,
                                                    redirect_uri=SPOTIPY_REDIRECT_URI,
                                                    scope=SCOPE))

progress_ms = 0 
track_start_time = None

def analyze_current_track():
    global progress_ms, track_start_time
    current_track = spotify.current_playback()
    
    if not current_track or not current_track['is_playing']:
        print("No track is currently playing.")
        track_start_time = None  # Reset the start time when no track is playing
        return
    
    # Retrieve track ID and progress in the song
    track_id = current_track['item']['id']
    progress_ms = current_track['progress_ms']
    
    # If track_start_time is None, it means this is a new track or the track has resumed
    if track_start_time is None:
        # Calculate the UNIX timestamp when the track started playing
        track_start_time = time.time() - (progress_ms / 1000)
    
    # Perform audio analysis
    analysis = spotify.audio_analysis(track_id)
    
    # Find the current beat
    current_time_in_track = time.time() - track_start_time
    for i, beat in enumerate(analysis['beats']):
        beat_start = beat['start']
        beat_end = beat_start + beat['duration']
        
        if beat_start <= current_time_in_track <= beat_end:
            # If this is the last beat, break
            if i == len(analysis['beats']) - 1:
                break
            
            # Get the start time of the next beat
            next_beat_start = analysis['beats'][i + 1]['start']
            
            # Calculate the time to wait until the next beat
            time_to_wait = next_beat_start - current_time_in_track
            
            # Wait until the next beat starts
            time.sleep(time_to_wait)
            
            # Print beat information at the transition point
            print(f"Transitioning to new beat at {next_beat_start} seconds")
            break

if __name__ == '__main__':
    try:
        while True:
            analyze_current_track()
            #time.sleep(0.5)
    except KeyboardInterrupt:
        print("Stopping playback analysis.")