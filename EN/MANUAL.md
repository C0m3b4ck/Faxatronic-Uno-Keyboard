# Arduino Learning Keyboard Manual

## Overview
Two Arduinos communicate via serial (TX→RX) to create a musical learning keyboard:
- **Music Arduino**: 11 piano keys + speaker + 6 control buttons
- **Logic Arduino**: LCD 16x2 + 4×4 matrix keypad + buzzer

---

## Music Arduino - Piano Keys

### Key Mapping (10 Keys - Pins 2-11)
| Piano Key | Arduino Pin | Note | Frequency | Key Index |
|-----------|-------------|------|-----------|-----------|
| 1 (Leftmost) | 2 | **C4** | 261 Hz | 0 |
| 2 | 3 | **D4** | 293 Hz | 1 |
| 3 | 4 | **E4** | 329 Hz | 2 |
| 4 | 5 | **F4** | 349 Hz | 3 |
| 5 | 6 | **G4** | 392 Hz | 4 |
| 6 | 7 | **A4** | 440 Hz | 5 |
| 7 | 8 | **B4** | 494 Hz | 6 |
| 8 | 9 | **C5** | 523 Hz | 7 |
| 9 | 10 | **D5** | 587 Hz | 8 |
| 10 (Rightmost) | 11 | **E5** | 659 Hz | 9 |

> **Note:** This setup uses 10 keys (pins 2-11). The code supports 11 keys (pins 2-12, up to F5). If you add an 11th key on pin 12, it will be F5 (698 Hz, index 10).

### Wiring
- Each key: One leg → Arduino pin (2-12), other leg → **GND**
- Uses `INPUT_PULLUP` (no external resistors needed)
- Press = connects pin to GND (reads LOW)

### Visual Layout (10 Keys)
```
Pins:  2  3  4  5  6  7  8  9  10 11
       │  │  │  │  │  │  │  │  │  │
Keys: [C4][D4][E4][F4][G4][A4][B4][C5][D5][E5]
       └──┬──┴──┬──┴──┬──┴──┬──┴──┬──┘
          C     D     E     F     G     A     B     C     D     E
          4     4     4     4     4     4     4     5     5     5
```
**Range:** C4 to E5 (10 keys, one octave + major third)

### Octave Shift (Control Buttons)
| Button | Pin | Function |
|--------|-----|----------|
| Octave + | A3 | Shift up (×2, ×4, ×8) |
| Octave - | A4 | Shift down (÷2, ÷4) |
| Reset | A5 | Back to normal |

- **Scale Offset +1** = ×2 frequency (one octave up)
- **Scale Offset +3** = ×8 max (three octaves up)
- **Scale Offset -1** = ÷2 (one octave down)
- **Scale Offset -2** = ÷4 max (two octaves down)

### Music Arduino (Plays Sound)
| Component | Pins |
|-----------|------|
| 11 Piano Keys | 2-12 (to GND, INPUT_PULLUP) |
| 6 Control Buttons | A0-A5 (to GND, INPUT_PULLUP) |
| Speaker/Buzzer | Pin 13 (+), GND (-) |
| **TX (Pin 1)** | → Logic Arduino RX (Pin 0) |
| GND | → Logic Arduino GND |

### Logic Arduino (Display & Menu)
| Component | Pins |
|-----------|------|
| LCD 16×2 | RS=13, EN=12, D4=11, D5=10, D6=9, D7=8 |
| LCD Contrast | Potentiometer to VO pin |
| Buzzer | A0 (+), GND (-) |
| Matrix Keypad 4×4 | Rows: A1,A2,A3,A4 → Cols: 3,4,5,6 |
| **RX (Pin 0)** | ← Music Arduino TX (Pin 1) |
| GND | → Music Arduino GND |

### Matrix Keypad Layout
```
┌───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ A │  ← Row 1 (A1)
├───┼───┼───┼───┤
│ 4 │ 5 │ 6 │ B │  ← Row 2 (A2)
├───┼───┼───┼───┤
│ 7 │ 8 │ 9 │ C │  ← Row 3 (A3)
├───┼───┼───┼───┤
│ * │ 0 │ # │ D │  ← Row 4 (A4)
└───┴───┴───┴───┘
   │   │   │   │
  C3  C4  C5  C6   ← Cols: 3,4,5,6
```
**Pin mapping:** Rows A1-A4, Cols 3-6

> ⚠️ **Upload with Logic Arduino Pin 0 disconnected**, then reconnect.

### Code Files
- Music Arduino: `EN/ai_garbo/music_player_2/music_player_2.ino`
- Logic Arduino: `EN/ai_garbo/logic_part2/logic_part2.ino`

---

## Startup
1. Power both Arduinos
2. **Welcome screen** (3 sec): "Music Keyboard / Loading..."
3. **Song Select** appears automatically

---

## Song Select Menu

### Display
```
AUTO:OFF  1/6
> Mary Lamb #=P *=N 0=AP
```
- Line 1: Auto-play status + song number / total
- Line 2: `>` current song name + help codes

### Controls
| Key | Action |
|-----|--------|
| **1-9** | Select song 1-9 directly → confirm screen |
| **0** | Song 10 / Toggle Auto-play (see below) |
| **\*** | **Next song** / **Enter song number** (hold/press twice for 10+) |
| **#** | Select current song for **Learning Mode** |
| **0** (short) | Toggle Auto-play ON/OFF |
| **A/B/C/D** | Select current song for Learning Mode |

### Selecting Songs 10-99
1. Press **\*** once → "Enter song #:" screen appears
2. Press digits **1-9**, **0** to type number (e.g., **1**, **2** = song 12)
3. Press **\*** again or **#** to confirm
4. Confirm screen appears → press **#** to play

> Example: Song 15 = `*` → `1` → `5` → `*` (or `#`)

---

## Confirm Play Screen
```
Play this song?
Mary Lamb #=Y *=N
```
| Key | Action |
|-----|--------|
| **#** or **D** | **Yes, play it** |
| **\*** or **A** or **C** | **No, go back** |

---

## Auto-Play Mode
- Press **0** to toggle: `AUTO:OFF` ↔ `AUTO:ON`
- When **ON**: After 3 seconds of no input, plays highlighted song
- Any key press resets the 3-second timer

---

## Playback Mode

### Display
```
PLAYING: Mary Lamb
Speed: 1.0x C4 3/9
```
or when paused:
```
PAUSED: Mary Lamb
Speed: 1.0x C4 3/9
```

### Controls (During Playback)
| Key | Action |
|-----|--------|
| **\*** | **Faster** (+0.25x, max 2.0x) |
| **#** | **Slower** (-0.25x, min 0.25x) |
| **B** | **Pause / Resume** |
| **A** or **D** | **Stop** → Learning Mode |
| **C** | Song Select Menu |

---

## Learning Mode

### Display
```
Press: C4
Mary Lamb 1/9
```
Alternates every 1.5s with:
```
Press: C4
Next: D4 E4
```

### Long Key Press & Timer

When a song contains consecutive identical notes (e.g., Wolf3D's G4×3 at 300ms each), they are automatically merged into a single long press. The LCD shows a countdown timer during the hold:

```
Press: G4
Wolf3D 1/26  900ms
```

While holding the correct key, the timer counts down. When it reaches 0:
```
Press: G4
Wolf3D 2/26  DONE!
```

The screen then advances to the next note automatically.

### Controls
| Key | Action |
|-----|--------|
| **Piano Keys** | Play notes (speaker sounds) |
| **C** | Open Song Select |
| **B** | Pause song / Stats screen |
| **D** | Reset current song progress |
| **A** | Return to Learning (from pause/stats) |

### Control Buttons (Music Arduino)
| Button | Pin | Function |
|--------|-----|----------|
| Vol + | A1 | Increase volume |
| Vol - | A2 | Decrease volume |
| Octave + | A3 | Higher pitch |
| Octave - | A4 | Lower pitch |
| Reset | A5 | Reset volume/pitch |

---

## Other Screens

### Stats Screen (Press B in Learning)
```
Correct: 5
Wrong: 1  Acc: 83%
```

### Paused Screen (Press B during playback)
```
PAUSED
A:Resume B:Menu
```

---

## Song List (Built-in)
1. **Mary Lamb** - Mary Had a Little Lamb
2. **Twinkle** - Twinkle Twinkle Little Star
3. **Scale Up** - C Major Scale
4. **Wolf3D** - Wolfenstein 3D Intro (*consecutive G4 notes merged into long press*)
5. **STALKER** - S.T.A.L.K.E.R. Theme
6. **Custom 1** - User uploaded via USB

---

## USB Song Upload
```bash
# List songs on device
python3 tools/send_song.py /dev/ttyUSB0 --list

# Send MIDI file to slot 0
python3 tools/send_song.py /dev/ttyUSB0 --midi song.mid 0

# Send JSON song to slot 0
python3 tools/send_song.py /dev/ttyUSB0 --json song.json 0
```

### JSON Format
```json
{
  "name": "My Song",
  "notes": [
    {"key": 0, "duration": 500},
    {"key": 2, "duration": 500}
  ]
}
```
Keys: 0=C4, 1=D4, 2=E4, 3=F4, 4=G4, 5=A4, 6=B4, 7=C5, 8=D5, 9=E5, 10=F5, 11=F#5

---

## MIDI to Arduino Converter
```bash
# Convert MIDI to C++ array (for hardcoding)
python3 tools/midi_converter.py song.mid

# Or send directly via USB (see above)
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| No sound on auto-play | Check serial wire (Logic TX→Music RX), common GND |
| Upload fails | Disconnect Logic Pin 0 during upload |
| LCD shows garbage | Check contrast pot, shorten song names |
| Keypad unresponsive | Verify row/col wiring: Rows A1-A4, Cols 3-6 |
| Song doesn't play | Press # on confirm screen, check AUTO:OFF |

---

## Serial Protocol (for debugging)
**Logic → Music:**
```
PLAY:START:9,0:500,2:500,4:500,5:500,7:1000,5:500,4:500,2:500,0:1000,1.00
PLAY:STOP
PLAY:PAUSE
PLAY:RESUME
```

**Music → Logic:**
```
KEY:DOWN:0
KEY:UP:0
CMD:RESET
```