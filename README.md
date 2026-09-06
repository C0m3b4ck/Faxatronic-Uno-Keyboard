# Arduino Learning Musical Keyboard

A dual-Arduino musical learning keyboard project. Two Arduinos communicate via serial to create an interactive piano learning system:

- **Music Arduino** (Uno/Nano): 10 piano keys + speaker + 6 control buttons
- **Logic Arduino** (Uno/Nano): LCD 16×2 + 4×4 matrix keypad + buzzer

## Quick Start

1. **Wire the hardware** - See [Wiring Guide](EN/MANUAL.md#wiring)
2. **Upload code**:
   - `EN/ai_garbo/logic_part2/logic_part2.ino` → Logic Arduino (disconnect Pin 0 first!)
   - `EN/ai_garbo/music_player_2/music_player_2.ino` → Music Arduino
3. **Power both** - Welcome screen → Song Select menu appears

## Documentation

- 📖 **[Full Instruction Manual](EN/MANUAL.md)** - Complete wiring, controls, menus, and troubleshooting
- 🛠️ **Tools** (in `EN/tools/`):
  - `send_song.py` - Upload custom songs via USB
  - `midi_converter.py` - Convert MIDI files to Arduino format
  - `example_song.json` - JSON song format reference

## Features

- 10-key piano (C4–E5) with octave shifting (±3 octaves)
- 5 built-in songs + custom song upload via USB
- **Learning mode** with long key press support and real-time countdown timer
- Auto-play mode with adjustable speed
- LCD menu navigation via 4×4 keypad

### Long Key Press

Consecutive identical notes (e.g., Wolf3D's G4×3) are automatically merged into a single long press. The LCD shows a countdown timer (`900ms`) while holding the key, turning to `DONE!` when the duration is met.

## Hardware Requirements

| Component | Pins |
|-----------|------|
| 2× Arduino Uno/Nano | - |
| 10 push buttons (keys) | Music: 2–11 |
| 6 push buttons (controls) | Music: A0–A5 |
| Speaker/buzzer | Music: 13 |
| LCD 16×2 | Logic: 8–13 |
| 4×4 matrix keypad | Logic: Rows A1–A4, Cols 3–6 |
| Buzzer | Logic: A0 |
| Potentiometer (LCD contrast) | Logic: VO pin |

## License

APL 2.0
