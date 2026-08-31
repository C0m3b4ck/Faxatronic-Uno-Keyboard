#!/usr/bin/env python3
"""
MIDI to Arduino Song Converter
Converts MIDI files to C++ arrays for the learning keyboard.
Usage: python3 midi_converter.py input.mid [output.h]
"""

import sys
import mido
from mido import MidiFile, MidiTrack, Message

NOTE_MAP = {
    60: 0,   # C4
    62: 1,   # D4
    64: 2,   # E4
    65: 3,   # F4
    67: 4,   # G4
    69: 5,   # A4
    71: 6,   # B4
    72: 7,   # C5
    74: 8,   # D5
    76: 9,   # E5
    77: 10,  # F5
    78: 11,  # F#5
}

def midi_to_song(midi_path, track_num=0, max_notes=200):
    mid = MidiFile(midi_path)
    
    if track_num >= len(mid.tracks):
        print(f"Track {track_num} not found. Available tracks: {len(mid.tracks)}")
        for i, track in enumerate(mid.tracks):
            print(f"  Track {i}: {track.name}")
        return None
    
    track = mid.tracks[track_num]
    ticks_per_beat = mid.ticks_per_beat
    
    notes = []
    current_time = 0
    active_notes = {}
    
    for msg in track:
        current_time += msg.time
        
        if msg.type == 'note_on' and msg.velocity > 0:
            if msg.note in NOTE_MAP:
                active_notes[msg.note] = current_time
        
        elif msg.type == 'note_off' or (msg.type == 'note_on' and msg.velocity == 0):
            if msg.note in active_notes:
                start_tick = active_notes.pop(msg.note)
                duration_ticks = current_time - start_tick
                duration_ms = int((duration_ticks / ticks_per_beat) * 500)
                
                key_idx = NOTE_MAP[msg.note]
                notes.append((key_idx, max(50, duration_ms)))
                
                if len(notes) >= max_notes:
                    break
    
    return notes

def generate_cpp(notes, song_name, var_name):
    output = []
    output.append(f"// {song_name} - Auto-generated from MIDI")
    output.append(f"SongNote {var_name}[] = {{")
    
    for i, (key_idx, duration) in enumerate(notes):
        output.append(f"  {{{key_idx}, {duration}}},")
    
    output.append("};")
    output.append(f"const int {var_name}_length = {len(notes)};")
    return "\n".join(output)

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 midi_converter.py input.mid [output.h] [track_num]")
        sys.exit(1)
    
    midi_path = sys.argv[1]
    output_path = sys.argv[2] if len(sys.argv) > 2 else None
    track_num = int(sys.argv[3]) if len(sys.argv) > 3 else 0
    
    notes = midi_to_song(midi_path, track_num)
    
    if notes is None:
        sys.exit(1)
    
    song_name = midi_path.split('/')[-1].replace('.mid', '').replace('.midi', '')
    var_name = song_name.replace(' ', '_').replace('-', '_')
    
    cpp_code = generate_cpp(notes, song_name, var_name)
    
    if output_path:
        with open(output_path, 'w') as f:
            f.write(cpp_code)
        print(f"Written to {output_path}")
    else:
        print(cpp_code)
    
    print(f"\nNotes: {len(notes)}")
    print(f"Duration range: {min(d for _, d in notes)}-{max(d for _, d in notes)}ms")

if __name__ == "__main__":
    main()