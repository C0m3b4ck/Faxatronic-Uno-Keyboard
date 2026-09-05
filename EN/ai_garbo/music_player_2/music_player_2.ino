// ============================================================
// MUSIC PLAYER ARDUINO
// ============================================================

// ============================================================
// PHYSICAL PIANO KEYS
// ============================================================

const int buttonPins[] = {
  2, 3, 4, 5, 6,
  7, 8, 9, 10, 11
};

const int NUM_KEYS = 10;

// ============================================================
// CONTROL BUTTONS
// ============================================================

const int controlPins[] = {
  A0, A1, A2, A3, A4, A5
};

const int NUM_CONTROLS = 6;

// ============================================================
// SPEAKER
// ============================================================

const int speakerPin = 13;

// ============================================================
// NOTE NAMES
// ============================================================

const char* buttonNoteNames[] = {
  "C4",
  "C#4",
  "D4",
  "D#4",
  "E4",
  "F4",
  "F#4",
  "G4",
  "G#4",
  "A4"
};

// ============================================================
// FREQUENCIES
// ============================================================

const int baseFreqs[] = {
  261,
  277,
  293,
  311,
  329,
  349,
  370,
  392,
  415,
  440
};

// ============================================================
// SETTINGS
// ============================================================

int volume = 128;

int scaleOffset = 0;

// ============================================================
// BUTTON STATES
// ============================================================

bool lastControlStates[NUM_CONTROLS];

bool lastKeyStates[NUM_KEYS];

// ============================================================
// INTRO
// ============================================================

int introNotes[] = {
  392,
  440,
  494,
  440,
  392,
  349,
  330,
  349
};

int introDurations[] = {
  220,
  180,
  180,
  220,
  220,
  180,
  180,
  260
};

// ============================================================
// PLAYBACK
// ============================================================

enum PlaybackState {
  PB_IDLE,
  PB_PLAYING,
  PB_PAUSED
};

PlaybackState playbackState =
  PB_IDLE;

int playbackNoteIndex = 0;

unsigned long playbackNoteStart = 0;

unsigned long pausedAt = 0;

int playbackKeyIndices[50];

int playbackDurations[50];

int playbackLength = 0;

float playbackSpeed = 1.0;

// ============================================================
// SERIAL RECEIVER
// ============================================================

String serialBuffer = "";

// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(9600);

  Serial.println("KEYBOARD ACTIVATED");

  // ----------------------------------------------------------
  // Piano keys
  // ----------------------------------------------------------

  for (
    int i = 0;
    i < NUM_KEYS;
    i++
  ) {

    pinMode(
      buttonPins[i],
      INPUT_PULLUP
    );

    lastKeyStates[i] = HIGH;
  }

  // ----------------------------------------------------------
  // Controls
  // ----------------------------------------------------------

  for (
    int i = 0;
    i < NUM_CONTROLS;
    i++
  ) {

    pinMode(
      controlPins[i],
      INPUT_PULLUP
    );

    lastControlStates[i] = HIGH;
  }

  // ----------------------------------------------------------
  // Speaker
  // ----------------------------------------------------------

  pinMode(
    speakerPin,
    OUTPUT
  );

  noTone(speakerPin);

  // ----------------------------------------------------------
  // Intro
  // ----------------------------------------------------------

  playIntro();
}

// ============================================================
// INTRO
// ============================================================

void playIntro() {

  for (
    int i = 0;
    i < 8;
    i++
  ) {

    tone(
      speakerPin,
      introNotes[i],
      introDurations[i]
    );

    delay(
      introDurations[i] + 30
    );

    noTone(speakerPin);
  }
}

// ============================================================
// GET FREQUENCY
// ============================================================

int getFrequency(int keyIdx) {

  if (
    keyIdx < 0 ||
    keyIdx >= NUM_KEYS
  ) {

    return 0;
  }

  int freq =
    baseFreqs[keyIdx];

  if (scaleOffset >= 0) {

    int shift =
      min(scaleOffset, 3);

    freq =
      freq * (1 << shift);

  } else {

    int shift =
      min(-scaleOffset, 2);

    freq =
      freq / (1 << shift);

    if (freq < 1) {

      freq =
        baseFreqs[keyIdx];
    }
  }

  return freq;
}

// ============================================================
// LOOP
// ============================================================

void loop() {

  readControls();

  checkKeyChanges();

  handleSerialCommands();

  updatePlayback();

  // ----------------------------------------------------------
  // Manual piano only when playback isn't active
  // ----------------------------------------------------------

  if (
    playbackState ==
    PB_IDLE
  ) {

    bool anyKeyPlaying =
      false;

    int newFreq = 0;

    for (
      int i = 0;
      i < NUM_KEYS;
      i++
    ) {

      if (
        digitalRead(
          buttonPins[i]
        ) == LOW
      ) {

        newFreq =
          getFrequency(i);

        anyKeyPlaying =
          true;

        break;
      }
    }

    if (
      anyKeyPlaying &&
      newFreq > 0
    ) {

      tone(
        speakerPin,
        newFreq
      );

    } else {

      noTone(speakerPin);
    }
  }

  delay(5);
}

// ============================================================
// CONTROL BUTTONS
// ============================================================

void readControls() {

  for (
    int i = 0;
    i < NUM_CONTROLS;
    i++
  ) {

    int state =
      digitalRead(
        controlPins[i]
      );

    // Detect button press
    if (
      state == LOW &&
      lastControlStates[i] == HIGH
    ) {

      // ------------------------------------------------------
      // A0 = unused / reserved
      // ------------------------------------------------------

      if (i == 0) {

        // Reserved
      }

      // ------------------------------------------------------
      // A1 = volume up
      // ------------------------------------------------------

      if (i == 1) {

        volume =
          min(
            255,
            volume + 10
          );
      }

      // ------------------------------------------------------
      // A2 = volume down
      // ------------------------------------------------------

      if (i == 2) {

        volume =
          max(
            0,
            volume - 10
          );
      }

      // ------------------------------------------------------
      // A3 = octave up
      // ------------------------------------------------------

      if (i == 3) {

        scaleOffset =
          min(
            3,
            scaleOffset + 1
          );
      }

      // ------------------------------------------------------
      // A4 = octave down
      // ------------------------------------------------------

      if (i == 4) {

        scaleOffset =
          max(
            -2,
            scaleOffset - 1
          );
      }

      // ------------------------------------------------------
      // A5 = reset
      // ------------------------------------------------------

      if (i == 5) {

        volume = 128;

        scaleOffset = 0;

        Serial.println(
          "CMD:RESET"
        );
      }

      delay(10);
    }

    lastControlStates[i] =
      state;
  }
}

// ============================================================
// KEY CHANGE DETECTION
// ============================================================

void checkKeyChanges() {

  for (
    int i = 0;
    i < NUM_KEYS;
    i++
  ) {

    int state =
      digitalRead(
        buttonPins[i]
      );

    if (
      state !=
      lastKeyStates[i]
    ) {

      if (state == LOW) {

        Serial.print(
          "KEY:DOWN:"
        );

        Serial.println(i);

        Serial.print(
          "NOTE_NAME:"
        );

        Serial.println(
          buttonNoteNames[i]
        );

      } else {

        Serial.print(
          "KEY:UP:"
        );

        Serial.println(i);
      }

      lastKeyStates[i] =
        state;
    }
  }
}

// ============================================================
// NON-BLOCKING SERIAL INPUT
// ============================================================

void handleSerialCommands() {

  while (Serial.available()) {

    char c =
      Serial.read();

    if (c == '\n') {

      serialBuffer.trim();

      if (
        serialBuffer.length() > 0
      ) {

        processSerialCommand(
          serialBuffer
        );
      }

      serialBuffer = "";

    } else if (c != '\r') {

      serialBuffer += c;

      // Prevent memory problems
      if (
        serialBuffer.length() > 250
      ) {

        serialBuffer = "";
      }
    }
  }
}

// ============================================================
// PROCESS SERIAL COMMAND
// ============================================================

void processSerialCommand(
  String line
) {

  // ==========================================================
  // PLAY START
  //
  // PLAY:START:length,
  // 0:500,2:500,4:500,
  // 1.00
  // ==========================================================

  if (
    line.startsWith(
      "PLAY:START:"
    )
  ) {

    int firstComma =
      line.indexOf(',', 11);

    if (
      firstComma < 0
    ) {

      Serial.println(
        "PLAY:ERROR:FORMAT"
      );

      return;
    }

    playbackLength =
      line.substring(
        11,
        firstComma
      ).toInt();

    if (
      playbackLength < 0
    ) {

      playbackLength = 0;
    }

    if (
      playbackLength > 50
    ) {

      playbackLength = 50;
    }

    String rest =
      line.substring(
        firstComma + 1
      );

    // --------------------------------------------------------
    // Last comma separates speed
    // --------------------------------------------------------

    int lastComma =
      rest.lastIndexOf(',');

    String notesStr;
    String speedStr;

    if (
      lastComma >= 0
    ) {

      notesStr =
        rest.substring(
          0,
          lastComma
        );

      speedStr =
        rest.substring(
          lastComma + 1
        );

      playbackSpeed =
        speedStr.toFloat();

    } else {

      notesStr = rest;

      playbackSpeed =
        1.0;
    }

    if (
      playbackSpeed <= 0.0
    ) {

      playbackSpeed =
        1.0;
    }

    // --------------------------------------------------------
    // Parse notes
    // --------------------------------------------------------

    parsePlaybackNotes(
      notesStr
    );

    // --------------------------------------------------------
    // Start playback
    // --------------------------------------------------------

    if (
      playbackLength <= 0
    ) {

      playbackState =
        PB_IDLE;

      noTone(speakerPin);

      Serial.println(
        "PLAY:ERROR:EMPTY"
      );

      return;
    }

    playbackState =
      PB_PLAYING;

    playbackNoteIndex = 0;

    playbackNoteStart =
      millis();

    // --------------------------------------------------------
    // IMPORTANT:
    // Play FIRST note immediately.
    // --------------------------------------------------------

    int keyIdx =
      playbackKeyIndices[0];

    int freq =
      getFrequency(keyIdx);

    if (freq > 0) {

      tone(
        speakerPin,
        freq
      );
    }

    Serial.println(
      "PLAY:ACK"
    );

    return;
  }

  // ==========================================================
  // STOP
  // ==========================================================

  if (
    line == "PLAY:STOP"
  ) {

    playbackState =
      PB_IDLE;

    playbackNoteIndex = 0;

    noTone(
      speakerPin
    );

    Serial.println(
      "PLAY:STOP:ACK"
    );

    return;
  }

  // ==========================================================
  // PAUSE
  // ==========================================================

  if (
    line == "PLAY:PAUSE"
  ) {

    if (
      playbackState ==
      PB_PLAYING
    ) {

      playbackState =
        PB_PAUSED;

      pausedAt =
        millis();

      noTone(
        speakerPin
      );

      Serial.println(
        "PLAY:PAUSE:ACK"
      );
    }

    return;
  }

  // ==========================================================
  // RESUME
  // ==========================================================

  if (
    line == "PLAY:RESUME"
  ) {

    if (
      playbackState ==
      PB_PAUSED
    ) {

      unsigned long pauseDuration =
        millis() -
        pausedAt;

      playbackNoteStart +=
        pauseDuration;

      playbackState =
        PB_PLAYING;

      int keyIdx =
        playbackKeyIndices[
          playbackNoteIndex
        ];

      int freq =
        getFrequency(keyIdx);

      if (freq > 0) {

        tone(
          speakerPin,
          freq
        );
      }

      Serial.println(
        "PLAY:RESUME:ACK"
      );
    }

    return;
  }

  // ==========================================================
  // UNKNOWN COMMAND
  // ==========================================================

  Serial.print(
    "RX:UNKNOWN:"
  );

  Serial.println(
    line
  );
}

// ============================================================
// PARSE PLAYBACK NOTES
// ============================================================

void parsePlaybackNotes(
  String notesStr
) {

  int idx = 0;

  int start = 0;

  int expectedLength =
    playbackLength;

  while (
    idx < expectedLength &&
    idx < 50 &&
    start < notesStr.length()
  ) {

    int comma =
      notesStr.indexOf(
        ',',
        start
      );

    if (
      comma == -1
    ) {

      comma =
        notesStr.length();
    }

    String pair =
      notesStr.substring(
        start,
        comma
      );

    int colon =
      pair.indexOf(':');

    if (
      colon > 0
    ) {

      int keyIdx =
        pair.substring(
          0,
          colon
        ).toInt();

      int duration =
        pair.substring(
          colon + 1
        ).toInt();

      // Only accept real physical keys
      if (
        keyIdx >= 0 &&
        keyIdx < NUM_KEYS &&
        duration > 0
      ) {

        playbackKeyIndices[idx] =
          keyIdx;

        playbackDurations[idx] =
          duration;

        idx++;
      }
    }

    start =
      comma + 1;
  }

  playbackLength =
    idx;
}

// ============================================================
// UPDATE PLAYBACK
// ============================================================

void updatePlayback() {

  if (
    playbackState !=
    PB_PLAYING
  ) {

    return;
  }

  if (
    playbackNoteIndex >=
    playbackLength
  ) {

    playbackState =
      PB_IDLE;

    noTone(
      speakerPin
    );

    Serial.println(
      "PLAY:DONE"
    );

    return;
  }

  unsigned long elapsed =
    millis() -
    playbackNoteStart;

  unsigned long adjustedDuration =
    (unsigned long)(
      (float)
      playbackDurations[
        playbackNoteIndex
      ] /
      playbackSpeed
    );

  if (
    elapsed >=
    adjustedDuration
  ) {

    playbackNoteIndex++;

    // --------------------------------------------------------
    // Next note
    // --------------------------------------------------------

    if (
      playbackNoteIndex <
      playbackLength
    ) {

      int keyIdx =
        playbackKeyIndices[
          playbackNoteIndex
        ];

      int freq =
        getFrequency(
          keyIdx
        );

      if (freq > 0) {

        tone(
          speakerPin,
          freq
        );

      } else {

        noTone(
          speakerPin
        );
      }

      playbackNoteStart =
        millis();

    } else {

      // ------------------------------------------------------
      // Finished
      // ------------------------------------------------------

      playbackState =
        PB_IDLE;

      noTone(
        speakerPin
      );

      Serial.println(
        "PLAY:DONE"
      );
    }
  }
}
