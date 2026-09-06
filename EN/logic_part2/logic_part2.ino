#include <LiquidCrystal.h>
#include <Keypad.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>

// ============================================================
// LCD PINOUT
// ============================================================
// RS = 13
// EN = 12
// D4 = 11
// D5 = 10
// D6 = 9
// D7 = 8
// ============================================================

const byte LCD_RS = 13;
const byte LCD_EN = 12;
const byte LCD_D4 = 11;
const byte LCD_D5 = 10;
const byte LCD_D6 = 9;
const byte LCD_D7 = 8;

LiquidCrystal lcd(
  LCD_RS,
  LCD_EN,
  LCD_D4,
  LCD_D5,
  LCD_D6,
  LCD_D7
);


// ============================================================
// MATRIX KEYPAD
// ============================================================

const byte MATRIX_ROWS = 4;
const byte MATRIX_COLS = 4;

char matrixKeys[MATRIX_ROWS][MATRIX_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte matrixRowPins[MATRIX_ROWS] = {
  A1, A2, A3, A4
};

byte matrixColPins[MATRIX_COLS] = {
  3, 4, 5, 6
};

Keypad matrixKeypad = Keypad(
  makeKeymap(matrixKeys),
  matrixRowPins,
  matrixColPins,
  MATRIX_ROWS,
  MATRIX_COLS
);


// ============================================================
// NOTE NAMES
// ============================================================

const char noteName0[] PROGMEM = "C4";
const char noteName1[] PROGMEM = "C#4";
const char noteName2[] PROGMEM = "D4";
const char noteName3[] PROGMEM = "D#4";
const char noteName4[] PROGMEM = "E4";
const char noteName5[] PROGMEM = "F4";
const char noteName6[] PROGMEM = "F#4";
const char noteName7[] PROGMEM = "G4";
const char noteName8[] PROGMEM = "G#4";
const char noteName9[] PROGMEM = "A4";

const char* const noteNames[] PROGMEM = {
  noteName0,
  noteName1,
  noteName2,
  noteName3,
  noteName4,
  noteName5,
  noteName6,
  noteName7,
  noteName8,
  noteName9
};

const byte NUM_NOTES = 10;


// ============================================================
// SONG STRUCTURES
// ============================================================

struct SongNote {
  byte keyIndex;
  unsigned int duration;
};


// ============================================================
// BUILT-IN SONGS
// ============================================================

// Mary Had a Little Lamb
const SongNote song1_notes[] PROGMEM = {
  {4, 500},
  {2, 500},
  {0, 500},
  {2, 500},
  {4, 500},
  {4, 500},
  {4, 1000},
  {2, 500},
  {2, 500},
  {2, 1000},
  {4, 500},
  {7, 500},
  {7, 1000}
};


// Twinkle Twinkle
const SongNote song2_notes[] PROGMEM = {
  {0, 400},
  {0, 400},
  {7, 400},
  {7, 400},
  {9, 400},
  {9, 400},
  {7, 800},

  {5, 400},
  {5, 400},
  {4, 400},
  {4, 400},
  {2, 400},
  {2, 400},
  {0, 800}
};


// Scale Up
const SongNote song3_notes[] PROGMEM = {
  {0, 300},
  {2, 300},
  {4, 300},
  {5, 300},
  {7, 300},
  {9, 600},

  {9, 300},
  {7, 300},
  {5, 300},
  {4, 300},
  {2, 300},
  {0, 600}
};


// Wolf3D
const SongNote song4_notes[] PROGMEM = {
  {7, 300},
  {7, 300},
  {7, 300},
  {5, 200},
  {7, 200},
  {8, 400},
  {9, 400},

  {7, 300},
  {7, 300},
  {7, 300},
  {5, 200},
  {7, 200},
  {8, 400},
  {9, 400},

  {7, 300},
  {7, 300},
  {7, 300},
  {5, 200},
  {7, 200},
  {8, 400},
  {9, 400},

  {9, 300},
  {9, 300},
  {8, 300},
  {7, 300},
  {7, 300},
  {5, 200},
  {7, 400},
  {5, 400}
};


// STALKER
const SongNote song5_notes[] PROGMEM = {
  {4, 500},
  {4, 500},
  {4, 500},
  {3, 350},
  {5, 150},
  {4, 500},

  {3, 350},
  {5, 150},
  {4, 1000},

  {7, 500},
  {7, 500},
  {7, 500},
  {8, 350},
  {9, 150},
  {9, 500},

  {8, 350},
  {9, 150},
  {9, 1000},

  {5, 500},
  {5, 500},
  {5, 500},
  {6, 350},
  {8, 150},
  {7, 500},

  {6, 350},
  {8, 150},
  {7, 1000}
};


// ============================================================
// BUILT-IN SONG METADATA
// ============================================================

const char songName0[] PROGMEM = "Mary Lamb";
const char songName1[] PROGMEM = "Twinkle";
const char songName2[] PROGMEM = "Scale Up";
const char songName3[] PROGMEM = "Wolf3D";
const char songName4[] PROGMEM = "STALKER";

const char* const songNames[] PROGMEM = {
  songName0,
  songName1,
  songName2,
  songName3,
  songName4
};

const SongNote* const builtinSongNotes[] PROGMEM = {
  song1_notes,
  song2_notes,
  song3_notes,
  song4_notes,
  song5_notes
};

const byte builtinSongLengths[] PROGMEM = {
  sizeof(song1_notes) / sizeof(song1_notes[0]),
  sizeof(song2_notes) / sizeof(song2_notes[0]),
  sizeof(song3_notes) / sizeof(song3_notes[0]),
  sizeof(song4_notes) / sizeof(song4_notes[0]),
  sizeof(song5_notes) / sizeof(song5_notes[0])
};

const byte NUM_BUILTIN_SONGS = 5;

const byte MAX_CUSTOM_SONGS = 1;
const byte MAX_NOTES_PER_SONG = 22;

const byte TOTAL_SONGS =
  NUM_BUILTIN_SONGS + MAX_CUSTOM_SONGS;


// ============================================================
// CUSTOM SONGS
// ============================================================

SongNote customSongNotes[
  MAX_CUSTOM_SONGS
][
  MAX_NOTES_PER_SONG
];

byte customSongLengths[MAX_CUSTOM_SONGS] = {
  0
};

char customSongNames[MAX_CUSTOM_SONGS][16] = {
  "Custom 1"
};

bool receivingSong = false;
byte receivingSongIdx = 0;
byte receivingNoteCount = 0;


// ============================================================
// CURRENT SONG
// ============================================================

bool currentSongIsCustom = false;
byte currentSongIndex = 0;

byte songPosition = 0;

unsigned int correctPresses = 0;
unsigned int wrongPresses = 0;

bool waitingForKey = false;
int expectedKey = -1;

unsigned long keyPressStartTime = 0;
bool keyHeld = false;

SongNote learningNotes[MAX_NOTES_PER_SONG];
byte learningLength = 0;

bool isPaused = false;
unsigned long pauseStartTime = 0;
unsigned long totalPausedTime = 0;


// ============================================================
// WRONG MESSAGE
// ============================================================

bool showingWrongMessage = false;
unsigned long wrongMessageStartTime = 0;

const unsigned long WRONG_MESSAGE_TIME = 500;


// ============================================================
// MENU
// ============================================================

enum MenuState {
  MENU_WELCOME,
  MENU_SELECT_SONG,
  MENU_CONFIRM_PLAY,
  MENU_LEARN,
  MENU_STATS,
  MENU_PAUSED,
  MENU_PLAYBACK
};

MenuState currentMenu = MENU_WELCOME;

byte selectedSong = 0;

byte scrollOffset = 0;
unsigned long lastScrollTime = 0;

bool autoPlayEnabled = false;
bool autoPlayPending = false;
unsigned long autoPlayStartTime = 0;

bool enteringSongNumber = false;
byte songNumberBuffer = 0;


// ============================================================
// PLAYBACK
// ============================================================

bool isPlaying = false;
bool playbackPaused = false;

unsigned long playbackStartTime = 0;
unsigned long playbackPausedTime = 0;
unsigned long totalPausedPlaybackTime = 0;

byte playbackNoteIndex = 0;

float playbackSpeed = 1.0;


// ============================================================
// SERIAL INPUT
// ============================================================

const byte SERIAL_BUFFER_SIZE = 128;

char serialBuffer[SERIAL_BUFFER_SIZE];
byte serialBufferIndex = 0;


// ============================================================
// FORWARD DECLARATIONS
// ============================================================

void handleSerialInput();
void processSerialCommand(char* line);

void handleMatrixKeypad();

void showWelcomeScreen();
void showSongSelect();
void showConfirmPlay();
void showLearningScreen();
void showPlaybackScreen();
void showStatsScreen();

void loadSong(byte songIdx);
void loadCustomSong(byte idx);

void loadAndPlaySong();
void confirmSongSelection();

void startPlayback();
void stopPlayback();
void togglePlaybackPause();
void handlePlayback();

void sendPlaybackCommand();

void resetLearning();
void pauseSong();
void resumeSong();

void handleCorrectKeyPress(int keyIdx);
void handleWrongKeyPress(int keyIdx);
void advanceToNextNote();

void checkKeyPressTiming();
void handleWrongMessageTimeout();

void handleScrollDisplay();
void updateBottomLine();
void updateTimerDisplay();

void mergeNotesFromRAM(
  SongNote* source,
  byte sourceLen,
  SongNote* dest,
  byte& destLen
);

void mergeNotesFromPROGMEM(
  const SongNote* source,
  byte sourceLen,
  SongNote* dest,
  byte& destLen
);

void printBuiltinSongName(byte idx);
void printCurrentSongName();

byte getCurrentSongLength();

void getCurrentSongNote(
  byte index,
  SongNote& note
);

const char* getNoteName(int idx);


// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(9600);

  lcd.begin(16, 2);

  delay(50);

  showWelcomeScreen();
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop() {

  handleSerialInput();

  handleMatrixKeypad();


  // ----------------------------------------------------------
  // AUTOPLAY
  // ----------------------------------------------------------

  if (
    currentMenu == MENU_SELECT_SONG &&
    autoPlayEnabled &&
    autoPlayPending
  ) {

    if (
      millis() - autoPlayStartTime >= 3000
    ) {

      autoPlayPending = false;

      loadAndPlaySong();
    }
  }


  // ----------------------------------------------------------
  // LEARNING
  // ----------------------------------------------------------

  if (
    !isPaused &&
    !isPlaying
  ) {

    checkKeyPressTiming();
  }


  // ----------------------------------------------------------
  // WRONG MESSAGE
  // ----------------------------------------------------------

  handleWrongMessageTimeout();


  // ----------------------------------------------------------
  // LCD SCROLL
  // ----------------------------------------------------------

  handleScrollDisplay();


  // ----------------------------------------------------------
  // PLAYBACK
  // ----------------------------------------------------------

  handlePlayback();
}


// ============================================================
// WELCOME
// ============================================================

void showWelcomeScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("Music Keyboard"));

  lcd.setCursor(0, 1);
  lcd.print(F("Loading..."));

  delay(1500);

  currentMenu = MENU_SELECT_SONG;

  selectedSong = 0;

  autoPlayPending = false;

  showSongSelect();
}


// ============================================================
// SERIAL BUFFER CLEAR
// ============================================================

void clearSerialBuffer() {

  while (Serial.available() > 0) {
    Serial.read();
  }

  serialBufferIndex = 0;
  serialBuffer[0] = '\0';
}


// ============================================================
// SERIAL INPUT
// ============================================================

void handleSerialInput() {

  while (Serial.available() > 0) {

    char c = Serial.read();


    // Ignore empty line endings.

    if (
      (c == '\n' || c == '\r') &&
      serialBufferIndex == 0
    ) {
      continue;
    }


    // Complete command.

    if (
      c == '\n' ||
      c == '\r'
    ) {

      serialBuffer[serialBufferIndex] = '\0';

      if (serialBufferIndex > 0) {
        processSerialCommand(serialBuffer);
      }

      serialBufferIndex = 0;
      serialBuffer[0] = '\0';

      continue;
    }


    // Store character.

    if (
      serialBufferIndex <
      SERIAL_BUFFER_SIZE - 1
    ) {

      serialBuffer[serialBufferIndex++] = c;

    } else {

      serialBufferIndex = 0;
      serialBuffer[0] = '\0';
    }
  }
}


// ============================================================
// PROCESS SERIAL COMMAND
// ============================================================

void processSerialCommand(char* line) {

  // ----------------------------------------------------------
  // TRIM LEADING SPACES
  // ----------------------------------------------------------

  char* start = line;

  while (
    *start == ' ' ||
    *start == '\t'
  ) {
    start++;
  }


  // ----------------------------------------------------------
  // TRIM TRAILING SPACES
  // ----------------------------------------------------------

  char* end = start + strlen(start);

  while (
    end > start &&
    (
      *(end - 1) == ' ' ||
      *(end - 1) == '\t'
    )
  ) {
    end--;
  }

  *end = '\0';

  if (*start == '\0') {
    return;
  }

  line = start;


  // ==========================================================
  // RECEIVING CUSTOM SONG
  // ==========================================================

  if (receivingSong) {

    if (
      strcmp(line, "SONG:END") == 0
    ) {

      customSongLengths[receivingSongIdx] =
        receivingNoteCount;

      receivingSong = false;

      Serial.print(F("SONG:SAVED:"));
      Serial.println(receivingSongIdx);

      lcd.clear();
      lcd.print(F("Song Saved!"));

      delay(1000);

      currentMenu = MENU_SELECT_SONG;

      showSongSelect();

      return;
    }


    if (
      strncmp(line, "NOTE:", 5) == 0
    ) {

      char* comma1 =
        strchr(line + 5, ',');

      char* comma2 =
        comma1 ?
        strchr(comma1 + 1, ',') :
        NULL;

      if (
        comma1 &&
        comma2
      ) {

        *comma1 = '\0';
        *comma2 = '\0';

        int keyIdx =
          atoi(line + 5);

        long duration =
          atol(comma1 + 1);

        if (
          receivingNoteCount <
          MAX_NOTES_PER_SONG &&

          keyIdx >= 0 &&
          keyIdx < NUM_NOTES &&

          duration > 0 &&
          duration <= 65535L
        ) {

          customSongNotes[
            receivingSongIdx
          ][
            receivingNoteCount
          ].keyIndex =
            (byte)keyIdx;

          customSongNotes[
            receivingSongIdx
          ][
            receivingNoteCount
          ].duration =
            (unsigned int)duration;

          receivingNoteCount++;
        }
      }

      return;
    }

    return;
  }


  // ==========================================================
  // KEY DOWN
  // ==========================================================

  if (
    strncmp(line, "KEY:DOWN:", 9) == 0
  ) {

    int keyIdx =
      atoi(line + 9);

    if (
      keyIdx >= 0 &&
      keyIdx < NUM_NOTES &&
      !isPaused &&
      !isPlaying &&
      waitingForKey
    ) {

      if (!keyHeld) {

        if (
          keyIdx == expectedKey
        ) {

          handleCorrectKeyPress(keyIdx);

        } else {

          handleWrongKeyPress(keyIdx);
        }
      }
    }

    return;
  }


  // ==========================================================
  // KEY UP
  // ==========================================================

  if (
    strncmp(line, "KEY:UP:", 7) == 0
  ) {

    int keyIdx =
      atoi(line + 7);

    if (
      keyIdx >= 0 &&
      keyIdx < NUM_NOTES
    ) {

      if (
        keyHeld &&
        keyIdx == expectedKey
      ) {

        keyHeld = false;
      }
    }

    return;
  }


  // ==========================================================
  // RESET
  // ==========================================================

  if (
    strcmp(line, "CMD:RESET") == 0
  ) {

    resetLearning();

    return;
  }


  // ==========================================================
  // NEW CUSTOM SONG
  // ==========================================================

  if (
    strncmp(line, "SONG:NEW:", 9) == 0
  ) {

    char* secondColon =
      strchr(line + 9, ':');

    if (secondColon) {

      *secondColon = '\0';

      int idx =
        atoi(line + 9);

      if (
        idx >= 0 &&
        idx < MAX_CUSTOM_SONGS
      ) {

        receivingSong = true;

        receivingSongIdx =
          (byte)idx;

        receivingNoteCount = 0;

        char* nameStart =
          secondColon + 1;

        while (
          *nameStart == ' ' ||
          *nameStart == '\t'
        ) {
          nameStart++;
        }

        if (
          strlen(nameStart) == 0
        ) {

          strcpy(
            nameStart,
            "Custom"
          );
        }

        strncpy(
          customSongNames[idx],
          nameStart,
          15
        );

        customSongNames[idx][15] =
          '\0';

        Serial.println(
          F("SONG:READY")
        );

        lcd.clear();

        lcd.print(
          F("Receiving Song")
        );

        lcd.setCursor(0, 1);

        lcd.print(
          customSongNames[idx]
        );
      }
    }

    return;
  }


  // ==========================================================
  // SONG LIST
  // ==========================================================

  if (
    strcmp(line, "SONG:LIST") == 0
  ) {

    Serial.println(
      F("SONG:LIST:START")
    );

    for (
      byte i = 0;
      i < NUM_BUILTIN_SONGS;
      i++
    ) {

      Serial.print(
        F("SONG:BUILTIN:")
      );

      Serial.print(i);

      Serial.print(':');

      printBuiltinSongName(i);

      Serial.println();
    }

    for (
      byte i = 0;
      i < MAX_CUSTOM_SONGS;
      i++
    ) {

      if (
        customSongLengths[i] > 0
      ) {

        Serial.print(
          F("SONG:CUSTOM:")
        );

        Serial.print(i);

        Serial.print(':');

        Serial.println(
          customSongNames[i]
        );
      }
    }

    Serial.println(
      F("SONG:LIST:END")
    );

    return;
  }


  // ==========================================================
  // LOAD BUILT-IN / CUSTOM SONG
  // ==========================================================

  if (
    strncmp(line, "SONG:LOAD:", 10) == 0
  ) {

    char* colon1 =
      strchr(line + 10, ':');

    if (colon1) {

      *colon1 = '\0';

      char* type =
        line + 10;

      int idx =
        atoi(colon1 + 1);

      if (
        strcmp(type, "BUILTIN") == 0 &&
        idx >= 0 &&
        idx < NUM_BUILTIN_SONGS
      ) {

        loadSong((byte)idx);

        currentMenu =
          MENU_LEARN;

        showLearningScreen();
      }

      else if (
        strcmp(type, "CUSTOM") == 0 &&
        idx >= 0 &&
        idx < MAX_CUSTOM_SONGS &&
        customSongLengths[idx] > 0
      ) {

        loadCustomSong((byte)idx);

        currentMenu =
          MENU_LEARN;

        showLearningScreen();
      }
    }

    return;
  }


  // ==========================================================
  // PLAY ACK
  // ==========================================================

  if (
    strcmp(line, "PLAY:ACK") == 0
  ) {

    Serial.println(
      F("PLAY:ACK:RECEIVED")
    );

    return;
  }


  // ==========================================================
  // PLAY DONE
  // ==========================================================

  if (
    strcmp(line, "PLAY:DONE") == 0
  ) {

    isPlaying = false;

    playbackPaused = false;

    currentMenu =
      MENU_LEARN;

    showLearningScreen();

    return;
  }
}


// ============================================================
// MATRIX KEYPAD
// ============================================================

void handleMatrixKeypad() {

  char key =
    matrixKeypad.getKey();

  if (!key) {
    return;
  }


  // ==========================================================
  // SONG SELECT
  // ==========================================================

  if (
    currentMenu ==
    MENU_SELECT_SONG
  ) {


    // --------------------------------------------------------
    // NUMBER ENTRY
    // --------------------------------------------------------

    if (
      key >= '1' &&
      key <= '9'
    ) {

      if (
        enteringSongNumber
      ) {

        byte newNumber =
          songNumberBuffer * 10 +
          (key - '0');

        if (
          newNumber <= TOTAL_SONGS
        ) {

          songNumberBuffer =
            newNumber;

          lcd.setCursor(0, 1);
          lcd.print(F("Number: "));
          lcd.print(songNumberBuffer);
          lcd.print(F("      "));
        }

      } else {

        byte songNum =
          key - '1';

        if (
          songNum < TOTAL_SONGS
        ) {

          selectedSong =
            songNum;

          showConfirmPlay();
        }
      }

      return;
    }


    // --------------------------------------------------------
    // 0 = AUTOPLAY
    // --------------------------------------------------------

    if (key == '0') {

      if (enteringSongNumber) {

        byte newNumber =
          songNumberBuffer * 10;

        if (
          newNumber > 0 &&
          newNumber <= TOTAL_SONGS
        ) {

          songNumberBuffer =
            newNumber;
        }

      } else {

        autoPlayEnabled =
          !autoPlayEnabled;

        if (autoPlayEnabled) {

          autoPlayPending = true;

          autoPlayStartTime =
            millis();

        } else {

          autoPlayPending = false;
        }

        showSongSelect();
      }

      return;
    }


    // --------------------------------------------------------
    // * = ENTER SONG NUMBER
    // --------------------------------------------------------

    if (key == '*') {

      if (
        !enteringSongNumber
      ) {

        enteringSongNumber = true;

        songNumberBuffer = 0;

        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print(F("Enter song #:"));

        lcd.setCursor(0, 1);
        lcd.print(F("1-"));
        lcd.print(TOTAL_SONGS);
        lcd.print(F(" then #"));

      } else {

        enteringSongNumber = false;

        songNumberBuffer = 0;

        showSongSelect();
      }

      return;
    }


    // --------------------------------------------------------
    // # = CONFIRM
    // --------------------------------------------------------

    if (key == '#') {

      if (
        enteringSongNumber
      ) {

        if (
          songNumberBuffer > 0 &&
          songNumberBuffer <= TOTAL_SONGS
        ) {

          selectedSong =
            songNumberBuffer - 1;

          enteringSongNumber =
            false;

          songNumberBuffer = 0;

          showConfirmPlay();
        }

      } else {

        confirmSongSelection();
      }

      return;
    }


    // --------------------------------------------------------
    // A/B/C/D = CONFIRM
    // --------------------------------------------------------

    if (
      key == 'A' ||
      key == 'B' ||
      key == 'C' ||
      key == 'D'
    ) {

      confirmSongSelection();

      return;
    }
  }


  // ==========================================================
  // CONFIRM PLAY
  // ==========================================================

  if (
    currentMenu ==
    MENU_CONFIRM_PLAY
  ) {

    if (
      key == '#' ||
      key == 'D'
    ) {

      loadAndPlaySong();

    } else if (
      key == '*' ||
      key == 'A' ||
      key == 'C'
    ) {

      currentMenu =
        MENU_SELECT_SONG;

      autoPlayPending = false;

      showSongSelect();
    }

    return;
  }


  // ==========================================================
  // PLAYBACK MENU
  // ==========================================================

  if (
    currentMenu ==
    MENU_PLAYBACK
  ) {

    if (key == '*') {

      playbackSpeed += 0.25;

      if (
        playbackSpeed > 2.0
      ) {
        playbackSpeed = 2.0;
      }

      // Re-send the complete playback command so the
      // audio Arduino receives the new speed.
      sendPlaybackCommand();

      showPlaybackScreen();

      return;
    }


    if (key == '#') {

      playbackSpeed -= 0.25;

      if (
        playbackSpeed < 0.25
      ) {
        playbackSpeed = 0.25;
      }

      // Re-send the complete playback command so the
      // audio Arduino receives the new speed.
      sendPlaybackCommand();

      showPlaybackScreen();

      return;
    }


    if (key == 'B') {

      togglePlaybackPause();

      return;
    }
  }


  // ==========================================================
  // GENERAL CONTROLS
  // ==========================================================

  switch (key) {

    case 'A':

      if (
        currentMenu ==
        MENU_PAUSED
      ) {

        resumeSong();

      } else if (
        currentMenu ==
        MENU_PLAYBACK
      ) {

        stopPlayback();

      } else {

        currentMenu =
          MENU_LEARN;

        showLearningScreen();
      }

      break;


    case 'B':

      if (
        currentMenu ==
        MENU_LEARN &&
        !isPaused &&
        !isPlaying
      ) {

        pauseSong();

      } else {

        currentMenu =
          MENU_STATS;

        showStatsScreen();
      }

      break;


    case 'C':

      currentMenu =
        MENU_SELECT_SONG;

      selectedSong = 0;

      enteringSongNumber = false;

      songNumberBuffer = 0;

      showSongSelect();

      if (
        autoPlayEnabled
      ) {

        autoPlayPending = true;

        autoPlayStartTime =
          millis();
      }

      break;


    case 'D':

      if (
        currentMenu ==
        MENU_PLAYBACK
      ) {

        stopPlayback();

      } else {

        resetLearning();
      }

      break;


    case '0':

      if (
        currentMenu ==
        MENU_LEARN &&
        !isPaused &&
        !isPlaying
      ) {

        startPlayback();
      }

      break;
  }
}


// ============================================================
// LOAD + PLAY
// ============================================================

void loadAndPlaySong() {

  confirmSongSelection();

  delay(10);

  if (
    currentMenu ==
    MENU_LEARN
  ) {

    startPlayback();
  }
}


// ============================================================
// CONFIRM SONG SELECTION
// ============================================================

void confirmSongSelection() {

  if (
    selectedSong <
    NUM_BUILTIN_SONGS
  ) {

    loadSong(selectedSong);

  } else {

    byte customIdx =
      selectedSong -
      NUM_BUILTIN_SONGS;

    if (
      customIdx < MAX_CUSTOM_SONGS &&
      customSongLengths[customIdx] > 0
    ) {

      loadCustomSong(customIdx);

    } else {

      lcd.clear();

      lcd.print(
        F("Empty Song")
      );

      delay(1000);

      currentMenu =
        MENU_SELECT_SONG;

      showSongSelect();

      return;
    }
  }

  currentMenu =
    MENU_LEARN;

  showLearningScreen();
}


// ============================================================
// GET CURRENT SONG LENGTH
// ============================================================

byte getCurrentSongLength() {

  if (
    currentSongIsCustom
  ) {

    return customSongLengths[
      currentSongIndex
    ];
  }

  return pgm_read_byte(
    &builtinSongLengths[
      currentSongIndex
    ]
  );
}


// ============================================================
// GET CURRENT SONG NOTE
// ============================================================

void getCurrentSongNote(
  byte index,
  SongNote& note
) {

  if (
    currentSongIsCustom
  ) {

    note =
      customSongNotes[
        currentSongIndex
      ][index];

  } else {

    const SongNote* notes =
      (const SongNote*)pgm_read_ptr(
        &builtinSongNotes[
          currentSongIndex
        ]
      );

    memcpy_P(
      &note,
      &notes[index],
      sizeof(SongNote)
    );
  }
}


// ============================================================
// PRINT BUILT-IN SONG NAME
// ============================================================

void printBuiltinSongName(
  byte idx
) {

  if (
    idx >= NUM_BUILTIN_SONGS
  ) {
    return;
  }

  const char* ptr =
    (const char*)pgm_read_ptr(
      &songNames[idx]
    );

  char c;

  while (
    (c = pgm_read_byte(ptr++))
  ) {

    Serial.write(c);
  }
}


// ============================================================
// PRINT CURRENT SONG NAME
// ============================================================

void printCurrentSongName() {

  if (
    currentSongIsCustom
  ) {

    Serial.print(
      customSongNames[
        currentSongIndex
      ]
    );

  } else {

    const char* ptr =
      (const char*)pgm_read_ptr(
        &songNames[
          currentSongIndex
        ]
      );

    char c;

    while (
      (c = pgm_read_byte(ptr++))
    ) {

      Serial.write(c);
    }
  }
}


// ============================================================
// NOTE NAME
// ============================================================

const char* getNoteName(
  int idx
) {

  static char buf[6];

  if (
    idx < 0 ||
    idx >= NUM_NOTES
  ) {

    strcpy(
      buf,
      "?"
    );

    return buf;
  }

  const char* ptr =
    (const char*)pgm_read_ptr(
      &noteNames[idx]
    );

  strcpy_P(
    buf,
    ptr
  );

  return buf;
}


// ============================================================
// MERGE RAM NOTES
// ============================================================

void mergeNotesFromRAM(
  SongNote* source,
  byte sourceLen,
  SongNote* dest,
  byte& destLen
) {

  destLen = 0;

  if (
    sourceLen == 0
  ) {
    return;
  }

  byte currentKey =
    source[0].keyIndex;

  unsigned long currentDur =
    source[0].duration;

  for (
    byte i = 1;
    i < sourceLen;
    i++
  ) {

    if (
      source[i].keyIndex ==
      currentKey
    ) {

      currentDur +=
        source[i].duration;

    } else {

      if (
        destLen <
        MAX_NOTES_PER_SONG
      ) {

        dest[destLen].keyIndex =
          currentKey;

        if (
          currentDur > 65535UL
        ) {
          currentDur = 65535UL;
        }

        dest[destLen].duration =
          (unsigned int)currentDur;

        destLen++;
      }

      currentKey =
        source[i].keyIndex;

      currentDur =
        source[i].duration;
    }
  }

  if (
    destLen <
    MAX_NOTES_PER_SONG
  ) {

    dest[destLen].keyIndex =
      currentKey;

    if (
      currentDur > 65535UL
    ) {
      currentDur = 65535UL;
    }

    dest[destLen].duration =
      (unsigned int)currentDur;

    destLen++;
  }
}


// ============================================================
// MERGE PROGMEM NOTES
// ============================================================

void mergeNotesFromPROGMEM(
  const SongNote* source,
  byte sourceLen,
  SongNote* dest,
  byte& destLen
) {

  destLen = 0;

  if (
    sourceLen == 0
  ) {
    return;
  }

  SongNote temp;

  memcpy_P(
    &temp,
    source,
    sizeof(SongNote)
  );

  byte currentKey =
    temp.keyIndex;

  unsigned long currentDur =
    temp.duration;

  for (
    byte i = 1;
    i < sourceLen;
    i++
  ) {

    memcpy_P(
      &temp,
      &source[i],
      sizeof(SongNote)
    );

    if (
      temp.keyIndex ==
      currentKey
    ) {

      currentDur +=
        temp.duration;

    } else {

      if (
        destLen <
        MAX_NOTES_PER_SONG
      ) {

        dest[destLen].keyIndex =
          currentKey;

        if (
          currentDur > 65535UL
        ) {
          currentDur = 65535UL;
        }

        dest[destLen].duration =
          (unsigned int)currentDur;

        destLen++;
      }

      currentKey =
        temp.keyIndex;

      currentDur =
        temp.duration;
    }
  }

  if (
    destLen <
    MAX_NOTES_PER_SONG
  ) {

    dest[destLen].keyIndex =
      currentKey;

    if (
      currentDur > 65535UL
    ) {
      currentDur = 65535UL;
    }

    dest[destLen].duration =
      (unsigned int)currentDur;

    destLen++;
  }
}


// ============================================================
// LOAD BUILT-IN SONG
// ============================================================

void loadSong(
  byte songIdx
) {

  if (
    songIdx >= NUM_BUILTIN_SONGS
  ) {
    return;
  }

  currentSongIsCustom =
    false;

  currentSongIndex =
    songIdx;

  songPosition = 0;

  correctPresses = 0;
  wrongPresses = 0;

  byte length =
    pgm_read_byte(
      &builtinSongLengths[
        songIdx
      ]
    );

  const SongNote* notes =
    (const SongNote*)pgm_read_ptr(
      &builtinSongNotes[
        songIdx
      ]
    );

  mergeNotesFromPROGMEM(
    notes,
    length,
    learningNotes,
    learningLength
  );

  waitingForKey =
    learningLength > 0;

  if (
    learningLength > 0
  ) {

    expectedKey =
      learningNotes[0].keyIndex;

  } else {

    expectedKey = -1;
  }

  keyHeld = false;

  isPaused = false;

  totalPausedTime = 0;

  showingWrongMessage = false;

  scrollOffset = 0;
}


// ============================================================
// LOAD CUSTOM SONG
// ============================================================

void loadCustomSong(
  byte idx
) {

  if (
    idx >= MAX_CUSTOM_SONGS
  ) {
    return;
  }

  currentSongIsCustom =
    true;

  currentSongIndex =
    idx;

  songPosition = 0;

  correctPresses = 0;
  wrongPresses = 0;

  mergeNotesFromRAM(
    customSongNotes[idx],
    customSongLengths[idx],
    learningNotes,
    learningLength
  );

  waitingForKey =
    learningLength > 0;

  if (
    learningLength > 0
  ) {

    expectedKey =
      learningNotes[0].keyIndex;

  } else {

    expectedKey = -1;
  }

  keyHeld = false;

  isPaused = false;

  totalPausedTime = 0;

  showingWrongMessage = false;

  scrollOffset = 0;
}


// ============================================================
// RESET LEARNING
// ============================================================

void resetLearning() {

  songPosition = 0;

  correctPresses = 0;
  wrongPresses = 0;

  if (
    currentSongIsCustom
  ) {

    mergeNotesFromRAM(
      customSongNotes[
        currentSongIndex
      ],
      customSongLengths[
        currentSongIndex
      ],
      learningNotes,
      learningLength
    );

  } else {

    byte length =
      pgm_read_byte(
        &builtinSongLengths[
          currentSongIndex
        ]
      );

    const SongNote* notes =
      (const SongNote*)pgm_read_ptr(
        &builtinSongNotes[
          currentSongIndex
        ]
      );

    mergeNotesFromPROGMEM(
      notes,
      length,
      learningNotes,
      learningLength
    );
  }

  waitingForKey =
    learningLength > 0;

  if (
    learningLength > 0
  ) {

    expectedKey =
      learningNotes[0].keyIndex;

  } else {

    expectedKey = -1;
  }

  keyHeld = false;

  isPaused = false;

  totalPausedTime = 0;

  showingWrongMessage = false;

  scrollOffset = 0;

  currentMenu =
    MENU_LEARN;

  showLearningScreen();
}


// ============================================================
// PAUSE LEARNING
// ============================================================

void pauseSong() {

  if (isPlaying) {
    return;
  }

  isPaused = true;

  pauseStartTime =
    millis();

  currentMenu =
    MENU_PAUSED;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("PAUSED"));

  lcd.setCursor(0, 1);
  lcd.print(F("A:Resume B:Menu"));
}


// ============================================================
// RESUME LEARNING
// ============================================================

void resumeSong() {

  if (!isPaused) {
    return;
  }

  totalPausedTime +=
    millis() -
    pauseStartTime;

  isPaused = false;

  currentMenu =
    MENU_LEARN;

  showLearningScreen();
}


// ============================================================
// CORRECT KEY
// ============================================================

void handleCorrectKeyPress(
  int keyIdx
) {

  if (keyHeld) {
    return;
  }

  keyHeld = true;

  keyPressStartTime =
    millis();

  totalPausedTime = 0;

  showingWrongMessage =
    false;

  lcd.setCursor(0, 1);

  lcd.print(F("Correct!       "));

  correctPresses++;
}


// ============================================================
// WRONG KEY
// ============================================================

void handleWrongKeyPress(
  int keyIdx
) {

  if (
    showingWrongMessage
  ) {
    return;
  }

  lcd.setCursor(0, 1);

  lcd.print(F("Wrong: "));

  lcd.print(
    getNoteName(keyIdx)
  );

  lcd.print(F("       "));

  wrongPresses++;

  showingWrongMessage =
    true;

  wrongMessageStartTime =
    millis();
}


// ============================================================
// WRONG MESSAGE TIMEOUT
// ============================================================

void handleWrongMessageTimeout() {

  if (
    !showingWrongMessage
  ) {
    return;
  }

  if (
    millis() -
    wrongMessageStartTime >=
    WRONG_MESSAGE_TIME
  ) {

    showingWrongMessage =
      false;

    showLearningScreen();
  }
}


// ============================================================
// KEY TIMING
// ============================================================

void checkKeyPressTiming() {

  if (
    waitingForKey &&
    keyHeld &&
    !isPaused &&
    !isPlaying
  ) {

    unsigned long heldTime =
      millis() -
      keyPressStartTime -
      totalPausedTime;

    SongNote note =
      learningNotes[
        songPosition
      ];

    if (
      heldTime >=
      note.duration
    ) {

      advanceToNextNote();

    } else {

      updateTimerDisplay();
    }
  }
}


// ============================================================
// NEXT NOTE
// ============================================================

void advanceToNextNote() {

  showingWrongMessage =
    false;

  songPosition++;

  if (
    songPosition >=
    learningLength
  ) {

    songPosition = 0;

    waitingForKey = false;

    keyHeld = false;

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(F("Song Complete!"));

    delay(1500);

    currentMenu =
      MENU_STATS;

    showStatsScreen();

    delay(2500);

    currentMenu =
      MENU_LEARN;

    resetLearning();

    return;
  }

  waitingForKey = true;

  expectedKey =
    learningNotes[
      songPosition
    ].keyIndex;

  keyHeld = false;

  totalPausedTime = 0;

  scrollOffset = 0;

  showLearningScreen();
}


// ============================================================
// LEARNING SCREEN
// ============================================================

void showLearningScreen() {

  lcd.clear();

  if (
    !waitingForKey ||
    learningLength == 0
  ) {

    lcd.setCursor(0, 0);
    lcd.print(F("No notes"));

    lcd.setCursor(0, 1);

    if (
      currentSongIsCustom
    ) {

      lcd.print(
        customSongNames[
          currentSongIndex
        ]
      );

    } else {

      const char* ptr =
        (const char*)pgm_read_ptr(
          &songNames[
            currentSongIndex
          ]
        );

      char c;

      while (
        (c = pgm_read_byte(ptr++))
      ) {

        lcd.write(c);
      }
    }

    return;
  }

  lcd.setCursor(0, 0);

  lcd.print(F("Press: "));

  lcd.print(
    getNoteName(expectedKey)
  );

  lcd.print(F("      "));

  lcd.setCursor(0, 1);

  updateBottomLine();
}


// ============================================================
// TIMER DISPLAY
// ============================================================

void updateTimerDisplay() {

  if (
    keyHeld &&
    !isPaused &&
    !isPlaying &&
    waitingForKey
  ) {

    unsigned long heldTime =
      millis() -
      keyPressStartTime -
      totalPausedTime;

    SongNote note =
      learningNotes[
        songPosition
      ];

    long remaining =
      (long)note.duration -
      (long)heldTime;

    if (
      remaining < 0
    ) {
      remaining = 0;
    }

    lcd.setCursor(10, 1);

    lcd.print(F("      "));

    lcd.setCursor(10, 1);

    if (
      remaining > 0
    ) {

      lcd.print(
        remaining
      );

      lcd.print(F("ms"));

    } else {

      lcd.print(F("DONE"));
    }
  }
}


// ============================================================
// SCROLL
// ============================================================

void handleScrollDisplay() {

  if (
    currentMenu ==
    MENU_LEARN &&
    !isPaused &&
    !isPlaying &&
    !showingWrongMessage
  ) {

    unsigned long now =
      millis();

    if (
      now - lastScrollTime >
      1500
    ) {

      lastScrollTime =
        now;

      scrollOffset =
        (scrollOffset + 1) % 2;

      updateBottomLine();
    }
  }
}


// ============================================================
// BOTTOM LINE
// ============================================================

void updateBottomLine() {

  if (
    learningLength == 0
  ) {
    return;
  }

  lcd.setCursor(0, 1);

  lcd.print(F("                "));

  lcd.setCursor(0, 1);


  // ----------------------------------------------------------
  // SONG NAME / POSITION
  // ----------------------------------------------------------

  if (
    scrollOffset == 0
  ) {

    if (
      currentSongIsCustom
    ) {

      lcd.print(
        customSongNames[
          currentSongIndex
        ]
      );

    } else {

      const char* ptr =
        (const char*)pgm_read_ptr(
          &songNames[
            currentSongIndex
          ]
        );

      char c;

      while (
        (c = pgm_read_byte(ptr++))
      ) {

        lcd.write(c);
      }
    }

    lcd.print(' ');

    lcd.print(
      songPosition + 1
    );

    lcd.print('/');

    lcd.print(
      learningLength
    );

  }


  // ----------------------------------------------------------
  // NEXT NOTES
  // ----------------------------------------------------------

  else {

    lcd.print(F("Next: "));

    byte nextIdx =
      (songPosition + 1) %
      learningLength;

    lcd.print(
      getNoteName(
        learningNotes[
          nextIdx
        ].keyIndex
      )
    );

    if (
      nextIdx + 1 <
      learningLength
    ) {

      lcd.print(' ');

      lcd.print(
        getNoteName(
          learningNotes[
            nextIdx + 1
          ].keyIndex
        )
      );
    }
  }
}


// ============================================================
// STATS
// ============================================================

void showStatsScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print(F("Correct: "));

  lcd.print(
    correctPresses
  );

  lcd.print(F("      "));

  lcd.setCursor(0, 1);

  lcd.print(F("Wrong: "));

  lcd.print(
    wrongPresses
  );

  lcd.print(F(" Acc:"));

  unsigned int total =
    correctPresses +
    wrongPresses;

  unsigned int accuracy =
    0;

  if (
    total > 0
  ) {

    accuracy =
      (
        (unsigned long)
        correctPresses *
        100UL
      ) / total;
  }

  lcd.print(
    accuracy
  );

  lcd.print('%');
}


// ============================================================
// SONG SELECT
// ============================================================

void showSongSelect() {

  lcd.clear();

  lcd.setCursor(0, 0);

  if (
    autoPlayEnabled
  ) {

    lcd.print(F("AUTO:ON "));

  } else {

    lcd.print(F("AUTO:OFF"));
  }

  lcd.print(' ');

  lcd.print(
    selectedSong + 1
  );

  lcd.print('/');

  lcd.print(
    TOTAL_SONGS
  );

  lcd.setCursor(0, 1);

  lcd.print(F("> "));

  if (
    selectedSong <
    NUM_BUILTIN_SONGS
  ) {

    const char* ptr =
      (const char*)pgm_read_ptr(
        &songNames[
          selectedSong
        ]
      );

    char c;

    while (
      (c = pgm_read_byte(ptr++))
    ) {

      lcd.write(c);
    }

  } else {

    byte customIdx =
      selectedSong -
      NUM_BUILTIN_SONGS;

    if (
      customSongLengths[
        customIdx
      ] > 0
    ) {

      lcd.print(
        customSongNames[
          customIdx
        ]
      );

    } else {

      lcd.print(
        F("(empty)")
      );
    }
  }
}


// ============================================================
// CONFIRM SCREEN
// ============================================================

void showConfirmPlay() {

  autoPlayPending =
    false;

  currentMenu =
    MENU_CONFIRM_PLAY;

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print(
    F("Play this song?")
  );

  lcd.setCursor(0, 1);

  if (
    selectedSong <
    NUM_BUILTIN_SONGS
  ) {

    const char* ptr =
      (const char*)pgm_read_ptr(
        &songNames[
          selectedSong
        ]
      );

    char c;

    while (
      (c = pgm_read_byte(ptr++))
    ) {

      lcd.write(c);
    }

  } else {

    byte customIdx =
      selectedSong -
      NUM_BUILTIN_SONGS;

    if (
      customIdx <
      MAX_CUSTOM_SONGS &&
      customSongLengths[
        customIdx
      ] > 0
    ) {

      lcd.print(
        customSongNames[
          customIdx
        ]
      );

    } else {

      lcd.print(
        F("(empty)")
      );
    }
  }
}


// ============================================================
// SEND PLAYBACK COMMAND
// ============================================================
//
// IMPORTANT:
//
// The logic Arduino does NOT make sound.
//
// This function sends the complete song to the OTHER Arduino.
// That Arduino is responsible for generating the actual audio.
//
// Protocol:
//
// PLAY:START:length,key:duration,key:duration,...,speed
//
// Example:
//
// PLAY:START:13,4:500,2:500,...,1.00
//
// ============================================================

void sendPlaybackCommand() {

  byte songLength =
    getCurrentSongLength();

  if (
    songLength == 0
  ) {
    return;
  }

  Serial.print(
    F("PLAY:START:")
  );

  Serial.print(
    songLength
  );

  for (
    byte i = 0;
    i < songLength;
    i++
  ) {

    SongNote note;

    getCurrentSongNote(
      i,
      note
    );

    Serial.print(',');

    Serial.print(
      note.keyIndex
    );

    Serial.print(':');

    Serial.print(
      note.duration
    );
  }

  Serial.print(',');

  Serial.println(
    playbackSpeed,
    2
  );

  Serial.flush();
}


// ============================================================
// START PLAYBACK
// ============================================================

void startPlayback() {

  byte songLength =
    getCurrentSongLength();

  if (
    songLength == 0
  ) {

    return;
  }

  isPlaying = true;

  playbackPaused =
    false;

  currentMenu =
    MENU_PLAYBACK;

  playbackNoteIndex = 0;

  playbackStartTime =
    millis();

  playbackPausedTime = 0;

  totalPausedPlaybackTime =
    0;


  // ----------------------------------------------------------
  // SEND SONG TO AUDIO ARDUINO
  // ----------------------------------------------------------

  sendPlaybackCommand();

  showPlaybackScreen();
}


// ============================================================
// STOP PLAYBACK
// ============================================================

void stopPlayback() {

  if (
    !isPlaying
  ) {
    return;
  }

  Serial.println(
    F("PLAY:STOP")
  );

  Serial.flush();

  isPlaying = false;

  playbackPaused = false;

  currentMenu =
    MENU_LEARN;

  showLearningScreen();
}


// ============================================================
// PAUSE / RESUME PLAYBACK
// ============================================================

void togglePlaybackPause() {

  if (
    !isPlaying
  ) {
    return;
  }

  if (
    playbackPaused
  ) {

    totalPausedPlaybackTime +=
      millis() -
      playbackPausedTime;

    playbackPaused = false;

    Serial.println(
      F("PLAY:RESUME")
    );

    Serial.flush();

  } else {

    playbackPaused = true;

    playbackPausedTime =
      millis();

    Serial.println(
      F("PLAY:PAUSE")
    );

    Serial.flush();
  }

  showPlaybackScreen();
}


// ============================================================
// PLAYBACK
// ============================================================
//
// This Arduino only tracks playback position for the LCD.
// It does NOT generate sound.
//
// The actual sound is produced by the second Arduino.
//
// ============================================================

void handlePlayback() {

  if (
    !isPlaying ||
    playbackPaused
  ) {
    return;
  }

  byte songLength =
    getCurrentSongLength();

  if (
    songLength == 0
  ) {

    stopPlayback();

    return;
  }

  if (
    playbackNoteIndex >=
    songLength
  ) {

    stopPlayback();

    return;
  }


  // ----------------------------------------------------------
  // ELAPSED PLAYBACK TIME
  // ----------------------------------------------------------

  unsigned long elapsed =
    millis() -
    playbackStartTime -
    totalPausedPlaybackTime;


  // ----------------------------------------------------------
  // CALCULATE START OF CURRENT NOTE
  // ----------------------------------------------------------

  unsigned long noteStart = 0;

  unsigned int speed100 =
    (unsigned int)(
      playbackSpeed * 100.0
    );

  if (
    speed100 < 1
  ) {
    speed100 = 1;
  }

  for (
    byte i = 0;
    i < playbackNoteIndex;
    i++
  ) {

    SongNote previousNote;

    getCurrentSongNote(
      i,
      previousNote
    );

    noteStart +=
      (
        (unsigned long)
        previousNote.duration *
        100UL
      ) /
      speed100;
  }


  // ----------------------------------------------------------
  // CURRENT NOTE DURATION
  // ----------------------------------------------------------

  SongNote currentNote;

  getCurrentSongNote(
    playbackNoteIndex,
    currentNote
  );

  unsigned long adjustedDuration =
    (
      (unsigned long)
      currentNote.duration *
      100UL
    ) /
    speed100;


  // ----------------------------------------------------------
  // NOTE FINISHED
  // ----------------------------------------------------------

  if (
    elapsed >=
    noteStart + adjustedDuration
  ) {

    playbackNoteIndex++;

    if (
      playbackNoteIndex >=
      songLength
    ) {

      showPlaybackScreen();

      // Give the second Arduino a moment to finish its
      // final note before sending STOP.
      delay(100);

      stopPlayback();

    } else {

      showPlaybackScreen();
    }
  }
}


// ============================================================
// PLAYBACK SCREEN
// ============================================================

void showPlaybackScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);

  if (
    playbackPaused
  ) {

    lcd.print(F("PAUSED: "));

  } else {

    lcd.print(F("PLAYING: "));
  }

  if (
    currentSongIsCustom
  ) {

    lcd.print(
      customSongNames[
        currentSongIndex
      ]
    );

  } else {

    const char* ptr =
      (const char*)pgm_read_ptr(
        &songNames[
          currentSongIndex
        ]
      );

    char c;

    while (
      (c = pgm_read_byte(ptr++))
    ) {

      lcd.write(c);
    }
  }

  lcd.setCursor(0, 1);

  byte songLength =
    getCurrentSongLength();

  if (
    playbackNoteIndex <
    songLength
  ) {

    SongNote note;

    getCurrentSongNote(
      playbackNoteIndex,
      note
    );

    lcd.print(F("Spd:"));

    lcd.print(
      playbackSpeed,
      1
    );

    lcd.print('x');

    lcd.print(' ');

    lcd.print(
      getNoteName(
        note.keyIndex
      )
    );

    lcd.print(' ');

    lcd.print(
      playbackNoteIndex + 1
    );

    lcd.print('/');

    lcd.print(
      songLength
    );

  } else {

    lcd.print(
      F("Complete!       ")
    );
  }
}
