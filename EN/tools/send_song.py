#!/usr/bin/env python3
"""
Send songs to Arduino Learning Keyboard via USB Serial
Usage: python3 send_song.py /dev/ttyUSB0 song_data.json
       python3 send_song.py /dev/ttyUSB0 --list
       python3 send_song.py /dev/ttyUSB0 --midi input.mid [track_num]
"""

import sys
import json
import time
import serial
import mido
from midi_converter import midi_to_song, NOTE_MAP

def send_command(ser, cmd, wait_response=True):
    ser.write((cmd + '\n').encode())
    ser.flush()
    if wait_response:
        start = time.time()
        while time.time() - start < 5:
            if ser.in_waiting:
                line = ser.readline().decode().strip()
                print(f"  Arduino: {line}")
                if line.startswith("SONG:SAVED") or line == "SONG:READY":
                    return True
            time.sleep(0.01)
    return False

def list_songs(ser):
    send_command(ser, "SONG:LIST")
    time.sleep(0.5)
    while ser.in_waiting:
        print(f"  {ser.readline().decode().strip()}")

def send_midi(ser, midi_path, slot, track_num=0, song_name=None):
    notes = midi_to_song(midi_path, track_num)
    if notes is None:
        return False
    
    if song_name is None:
        song_name = midi_path.split('/')[-1].replace('.mid', '').replace('.midi', '')
    
    print(f"Sending {len(notes)} notes to slot {slot}...")
    
    if not send_command(ser, f"SONG:NEW:{slot}:{song_name}"):
        print("Failed to start song transfer")
        return False
    
    for i, (key_idx, duration) in enumerate(notes):
        cmd = f"NOTE:{key_idx},{duration},0"
        if not send_command(ser, cmd, wait_response=False):
            print(f"Failed at note {i}")
            return False
        if i % 20 == 0:
            print(f"  Sent {i+1}/{len(notes)} notes")
        time.sleep(0.005)
    
    if not send_command(ser, "SONG:END"):
        print("Failed to end song transfer")
        return False
    
    print("Song sent successfully!")
    return True

def send_json(ser, json_path, slot):
    with open(json_path) as f:
        data = json.load(f)
    
    song_name = data.get('name', f'Custom {slot}')
    notes = data.get('notes', [])
    
    print(f"Sending {len(notes)} notes to slot {slot}...")
    
    if not send_command(ser, f"SONG:NEW:{slot}:{song_name}"):
        return False
    
    for i, note in enumerate(notes):
        key_idx = note['key']
        duration = note['duration']
        cmd = f"NOTE:{key_idx},{duration},0"
        if not send_command(ser, cmd, wait_response=False):
            print(f"Failed at note {i}")
            return False
        time.sleep(0.005)
    
    if not send_command(ser, "SONG:END"):
        return False
    
    print("Song sent successfully!")
    return True

def main():
    if len(sys.argv) < 3:
        print("Usage:")
        print("  python3 send_song.py <port> --list")
        print("  python3 send_song.py <port> --midi <file.mid> <slot> [track]")
        print("  python3 send_song.py <port> --json <file.json> <slot>")
        sys.exit(1)
    
    port = sys.argv[1]
    action = sys.argv[2]
    
    try:
        ser = serial.Serial(port, 9600, timeout=1)
        time.sleep(2)
        print(f"Connected to {port}")
        
        if action == '--list':
            list_songs(ser)
        
        elif action == '--midi':
            if len(sys.argv) < 5:
                print("Usage: --midi <file.mid> <slot> [track]")
                sys.exit(1)
            midi_file = sys.argv[3]
            slot = int(sys.argv[4])
            track = int(sys.argv[5]) if len(sys.argv) > 5 else 0
            send_midi(ser, midi_file, slot, track)
        
        elif action == '--json':
            if len(sys.argv) < 5:
                print("Usage: --json <file.json> <slot>")
                sys.exit(1)
            json_file = sys.argv[3]
            slot = int(sys.argv[4])
            send_json(ser, json_file, slot)
        
        else:
            print(f"Unknown action: {action}")
    
    except serial.SerialException as e:
        print(f"Serial error: {e}")
    finally:
        ser.close()

if __name__ == "__main__":
    main()