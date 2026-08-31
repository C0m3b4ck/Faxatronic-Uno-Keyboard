const int buttonPins[] = {2,3,4,5,6,7,8,9,10,11};
const int controlPins[] = {A0,A1,A2,A3,A4,A5};
const int speakerPin = 13;

const char* buttonNoteNames[] = {"C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4"};

int volume = 128;
int scaleOffset = 0;
int baseFreqs[] = {261,277,293,311,329,349,370,392,415,440};

bool lastControlStates[6];
bool lastKeyStates[10];

int introNotes[] = {
  392, 440, 494, 440,
  392, 349, 330, 349
};

int introDurations[] = {
  220, 180, 180, 220,
  220, 180, 180, 260
};

enum PlaybackState { PB_IDLE, PB_PLAYING, PB_PAUSED };
PlaybackState playbackState = PB_IDLE;
int playbackNoteIndex = 0;
unsigned long playbackNoteStart = 0;
int playbackKeyIndices[50];
int playbackDurations[50];
int playbackLength = 0;
float playbackSpeed = 1.0;

void playIntro() {
  for (int i = 0; i < 8; i++) {
    tone(speakerPin, introNotes[i], introDurations[i]);
    delay(introDurations[i] + 30);
    noTone(speakerPin);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("KEYBOARD ACTIVATED");

  for (int i = 0; i < 10; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastKeyStates[i] = HIGH;
  }

  for (int i = 0; i < 6; i++) {
    pinMode(controlPins[i], INPUT_PULLUP);
    lastControlStates[i] = HIGH;
  }

  pinMode(speakerPin, OUTPUT);
  noTone(speakerPin);
  playIntro();
}

void readControls() {
  for (int i = 0; i < 6; i++) {
    int state = digitalRead(controlPins[i]);

    if (state == LOW && lastControlStates[i] == HIGH) {
      if (i == 1) volume = min(255, volume + 10);
      if (i == 2) volume = max(0, volume - 10);
      if (i == 3) scaleOffset = min(3, scaleOffset + 1);
      if (i == 4) scaleOffset = max(-2, scaleOffset - 1);
      if (i == 5) {
        volume = 128;
        scaleOffset = 0;
        Serial.println("CMD:RESET");
      }
      delay(10);
    }

    if (state == HIGH) {
      lastControlStates[i] = HIGH;
    } else {
      lastControlStates[i] = LOW;
    }
  }
}

void checkKeyChanges() {
  for (int i = 0; i < 10; i++) {
    int state = digitalRead(buttonPins[i]);
    if (state != lastKeyStates[i]) {
      if (state == LOW) {
        Serial.print("KEY:DOWN:");
        Serial.println(i);
        Serial.print("NOTE_NAME:");
        Serial.println(buttonNoteNames[i]);
      } else {
        Serial.print("KEY:UP:");
        Serial.println(i);
      }
      lastKeyStates[i] = state;
    }
  }
}

const char* getButtonNoteName(int idx) {
  if (idx >= 0 && idx < 10) return buttonNoteNames[idx];
  return "?";
}

void handleSerialCommands() {
  while (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    
    if (line.startsWith("PLAY:START:")) {
      int firstComma = line.indexOf(',', 11);
      if (firstComma > 0) {
        playbackLength = line.substring(11, firstComma).toInt();
        String rest = line.substring(firstComma + 1);
        
        int lastComma = rest.lastIndexOf(',');
        if (lastComma > 0) {
          String notesStr = rest.substring(0, lastComma);
          String speedStr = rest.substring(lastComma + 1);
          playbackSpeed = speedStr.toFloat();
          if (playbackSpeed <= 0) playbackSpeed = 1.0;
          
          parsePlaybackNotes(notesStr);
        } else {
          parsePlaybackNotes(rest);
          playbackSpeed = 1.0;
        }
        playbackState = PB_PLAYING;
        playbackNoteIndex = 0;
        playbackNoteStart = millis();
        tone(speakerPin, 1000, 100);  // Debug beep on receive
      }
    } else if (line == "PLAY:STOP") {
      playbackState = PB_IDLE;
      noTone(speakerPin);
    } else if (line == "PLAY:PAUSE") {
      if (playbackState == PB_PLAYING) {
        playbackState = PB_PAUSED;
        noTone(speakerPin);
      }
    } else if (line == "PLAY:RESUME") {
      if (playbackState == PB_PAUSED) {
        playbackState = PB_PLAYING;
        playbackNoteStart = millis();
      }
    }
  }
}

void parsePlaybackNotes(String notesStr) {
  int idx = 0;
  int start = 0;
  while (idx < playbackLength && start < notesStr.length()) {
    int comma = notesStr.indexOf(',', start);
    if (comma == -1) comma = notesStr.length();
    String pair = notesStr.substring(start, comma);
    int colon = pair.indexOf(':');
    if (colon > 0) {
      playbackKeyIndices[idx] = pair.substring(0, colon).toInt();
      playbackDurations[idx] = pair.substring(colon + 1).toInt();
      idx++;
    }
    start = comma + 1;
  }
  playbackLength = idx;
}

void updatePlayback() {
  if (playbackState != PB_PLAYING) return;
  if (playbackNoteIndex >= playbackLength) {
    playbackState = PB_IDLE;
    noTone(speakerPin);
    return;
  }
  
  unsigned long elapsed = (millis() - playbackNoteStart) * playbackSpeed;
  if (elapsed >= playbackDurations[playbackNoteIndex]) {
    playbackNoteIndex++;
    if (playbackNoteIndex < playbackLength) {
      int keyIdx = playbackKeyIndices[playbackNoteIndex];
      if (keyIdx >= 0 && keyIdx < 11) {
        int freq = baseFreqs[keyIdx];
        if (scaleOffset >= 0) {
          int shift = min(scaleOffset, 3);
          freq = freq * (1 << shift);
        } else {
          int s = min(-scaleOffset, 2);
          freq = freq / (1 << s);
          if (freq < 1) freq = baseFreqs[keyIdx];
        }
        tone(speakerPin, freq);
      }
      playbackNoteStart = millis();
    } else {
      playbackState = PB_IDLE;
      noTone(speakerPin);
    }
  }
}

void loop() {
  readControls();
  checkKeyChanges();
  handleSerialCommands();
  updatePlayback();

  if (playbackState == PB_IDLE) {
    bool anyKeyPlaying = false;
    int newFreq = 0;

    for (int i = 0; i < 10; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        if (scaleOffset >= 0) {
          int shift = min(scaleOffset, 3);
          newFreq = baseFreqs[i] * (1 << shift);
        } else {
          int s = min(-scaleOffset, 2);
          newFreq = baseFreqs[i] / (1 << s);
          if (newFreq < 1) newFreq = baseFreqs[i];
        }
        anyKeyPlaying = true;
        break;
      }
    }

    if (anyKeyPlaying && newFreq > 0) {
      tone(speakerPin, newFreq);
    } else {
      noTone(speakerPin);
    }
  }

  delay(20);
}