#include <LiquidCrystal.h>
#include <Keypad.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>

// ============================================================
// LCD
// ============================================================

const int rs = 13;
const int en = 12;
const int d4 = 11;
const int d5 = 10;
const int d6 = 9;
const int d7 = 8;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// ============================================================
// BUZZER
// ============================================================

const int buzzerPin = A0;


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
// Stored in PROGMEM to reduce SRAM usage.
// ============================================================

const char noteNames_0[] PROGMEM = "C4";
const char noteNames_1[] PROGMEM = "C#4";
const char noteNames_2[] PROGMEM = "D4";
const char noteNames_3[] PROGMEM = "D#4";
const char noteNames_4[] PROGMEM = "E4";
const char noteNames_5[] PROGMEM = "F4";
const char noteNames_6[] PROGMEM = "F#4";
const char noteNames_7[] PROGMEM = "G4";
const char noteNames_8[] PROGMEM = "G#4";
const char noteNames_9[] PROGMEM = "A4";

const char* const noteNames[] PROGMEM = {
  noteNames_0,
  noteNames_1,
  noteNames_2,
  noteNames_3,
  noteNames_4,
  noteNames_5,
  noteNames_6,
  noteNames_7,
  noteNames_8,
  noteNames_9
};

const int NUM_NOTES = 10;


// ============================================================
// SONG STRUCTURES
// ============================================================

struct SongNote {
  byte keyIndex;
  unsigned int duration;
};

struct Song {
  const char* name;
  const SongNote* notes;
  byte length;
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
// SONG NAMES
// Stored in PROGMEM.
// ============================================================

const char songName0[] PROGMEM = "Mary Lamb";
const char songName1[] PROGMEM = "Twinkle";
const char songName2[] PROGMEM = "Scale Up";
const char songName3[] PROGMEM = "Wolf3D";
const char songName4[] PROGMEM = "STALKER";


// ============================================================
// SONG TABLE
// ============================================================

const Song songs[] = {
  {songName0, song1_notes, sizeof(song1_notes) / sizeof(song1_notes[0])},
  {songName1, song2_notes, sizeof(song2_notes) / sizeof(song2_notes[0])},
  {songName2, song3_notes, sizeof(song3_notes) / sizeof(song3_notes[0])},
  {songName3, song4_notes, sizeof(song4_notes) / sizeof(song4_notes[0])},
  {songName4, song5_notes, sizeof(song5_notes) / sizeof(song5_notes[0])}
};

const byte NUM_BUILTIN_SONGS = 5;
const byte MAX_CUSTOM_SONGS = 1;
const byte MAX_NOTES_PER_SONG = 22;
const byte TOTAL_SONGS = NUM_BUILTIN_SONGS + MAX_CUSTOM_SONGS;


// ============================================================
// CUSTOM SONGS
// ============================================================

SongNote customSongNotes[MAX_CUSTOM_SONGS][MAX_NOTES_PER_SONG];

byte customSongLengths[MAX_CUSTOM_SONGS] = {0};

char customSongNames[MAX_CUSTOM_SONGS][16] = {
  "Custom 1"
};

bool receivingSong = false;
byte receivingSongIdx = 0;
byte receivingNoteCount = 0;


// ============================================================
// CURRENT LEARNING SONG
// ============================================================

const Song* currentSong = &songs[0];

byte songPosition = 0;
unsigned int correctPresses = 0;
unsigned int wrongPresses = 0;

bool waitingForKey = false;
byte expectedKey = 0;

unsigned long keyPressStartTime = 0;
bool keyHeld = false;

SongNote learningNotes[MAX_NOTES_PER_SONG];
byte learningLength = 0;

bool isPaused = false;
unsigned long pauseStartTime = 0;
unsigned long totalPausedTime = 0;


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

unsigned long menuEnterTime = 0;

bool autoStartPlayback = false;
bool autoPlayEnabled = false;

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
// SERIAL INPUT BUFFER
//
// Non-blocking parser.
// This is important because key messages must not be delayed
// while the Arduino is processing other serial data.
// ============================================================

const byte SERIAL_LINE_SIZE = 96;

char serialLine[SERIAL_LINE_SIZE];
byte serialLinePos = 0;


// ============================================================
// FORWARD DECLARATIONS
// ============================================================

void showWelcomeScreen();
void showSongSelect();
void showConfirmPlay();
void showLearningScreen();
void showStatsScreen();
void showPlaybackScreen();

void handleSerialInput();
void processSerialLine(char* line);

void handleMatrixKeypad();

void handleCorrectKeyPress(byte keyIdx);
void handleWrongKeyPress(byte keyIdx);
void checkKeyPressTiming();
void advanceToNextNote();

void loadSong(byte songIdx);
void loadCustomSong(byte idx);
void resetLearning();

void pauseSong();
void resumeSong();

void updateTimerDisplay();
void updateBottomLine();
void handleScrollDisplay();

void confirmSongSelection();
void loadAndPlaySong();

void startPlayback();
void stopPlayback();
void togglePlaybackPause();
void handlePlayback();

void beep(unsigned int duration);

const char* getNoteName(byte idx);

void mergeNotes(
  const SongNote* source,
  byte sourceLen,
  SongNote* dest,
  byte& destLen
);


// ============================================================
// STARTUP
// ============================================================

void showWelcomeScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(F("Music Keyboard"));

  lcd.setCursor(0, 1);
  lcd.print(F("Loading..."));

  // ----------------------------------------------------------
  // Clear any stale serial data.
  // ----------------------------------------------------------

  while (Serial.available() > 0) {
    Serial.read();
  }

  serialLinePos = 0;

  delay(3000);

  currentMenu = MENU_SELECT_SONG;
  selectedSong = 0;
  menuEnterTime = millis();
  autoStartPlayback = false;

  showSongSelect();
}


// ============================================================
// SETUP
// ============================================================

void setup() {

  Serial.begin(9600);

  lcd.begin(16, 2);

  // Required startup delay after LCD initialization.
  delay(50);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  serialLinePos = 0;

  Serial.println(F("LOGIC READY"));

  showWelcomeScreen();
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop() {

  // ----------------------------------------------------------
  // Process only a small amount of serial data per loop.
  // This prevents serial handling from starving the keypad.
  // ----------------------------------------------------------

  handleSerialInput();

  // ----------------------------------------------------------
  // Physical matrix keypad
  // ----------------------------------------------------------

  handleMatrixKeypad();

  // ----------------------------------------------------------
  // Automatic playback
  // ----------------------------------------------------------

  if (
    currentMenu == MENU_SELECT_SONG &&
    autoPlayEnabled &&
    !autoStartPlayback
  ) {

    if (millis() - menuEnterTime >= 3000) {

      autoStartPlayback = true;

      loadAndPlaySong();
    }
  }

  // ----------------------------------------------------------
  // Learning timing
  // ----------------------------------------------------------

  if (!isPaused && !isPlaying) {
    checkKeyPressTiming();
  }

  // ----------------------------------------------------------
  // LCD scrolling
  // ----------------------------------------------------------

  handleScrollDisplay();

  // ----------------------------------------------------------
  // Playback
  // ----------------------------------------------------------

  handlePlayback();
}


// ============================================================
// SERIAL INPUT
// ============================================================

void handleSerialInput() {

  // Process at most 32 characters per call.
  // This prevents a large serial burst from blocking the loop.

  byte processed = 0;

  while (
    Serial.available() > 0 &&
    processed < 32
  ) {

    char c = Serial.read();

    processed++;

    // --------------------------------------------------------
    // End of line
    // --------------------------------------------------------

    if (c == '\n' || c == '\r') {

      if (serialLinePos > 0) {

        serialLine[serialLinePos] = '\0';

        processSerialLine(serialLine);

        serialLinePos = 0;
      }

      continue;
    }

    // --------------------------------------------------------
    // Store character
    // --------------------------------------------------------

    if (serialLinePos < SERIAL_LINE_SIZE - 1) {

      serialLine[serialLinePos++] = c;

    } else {

      // Overflow protection.
      serialLinePos = 0;
    }
  }
}


// ============================================================
// PROCESS ONE SERIAL LINE
// ============================================================

void processSerialLine(char* line) {

  // ----------------------------------------------------------
  // Trim leading spaces
  // ----------------------------------------------------------

  while (
    *line == ' ' ||
    *line == '\t'
  ) {
    line++;
  }

  // ----------------------------------------------------------
  // Trim trailing spaces
  // ----------------------------------------------------------

  int len = strlen(line);

  while (
    len > 0 &&
    (
      line[len - 1] == ' ' ||
      line[len - 1] == '\t'
    )
  ) {

    line[len - 1] = '\0';
    len--;
  }

  if (len == 0) {
    return;
  }


  // ==========================================================
  // CUSTOM SONG RECEIVING
  // ==========================================================

  if (receivingSong) {

    // --------------------------------------------------------
    // SONG END
    // --------------------------------------------------------

    if (strcmp(line, "SONG:END") == 0) {

      customSongLengths[receivingSongIdx] =
        receivingNoteCount;

      receivingSong = false;

      Serial.print(F("SONG:SAVED:"));
      Serial.println(receivingSongIdx);

      lcd.clear();

      lcd.print(F("Song Saved!"));

      delay(1000);

      showSongSelect();

      return;
    }


    // --------------------------------------------------------
    // NOTE:key,duration
    // --------------------------------------------------------

    if (strncmp(line, "NOTE:", 5) == 0) {

      char* comma1 = strchr(line + 5, ',');

      if (comma1 != NULL) {

        *comma1 = '\0';

        int keyIdx =
          atoi(line + 5);

        int duration =
          atoi(comma1 + 1);

        if (
          receivingNoteCount < MAX_NOTES_PER_SONG &&
          keyIdx >= 0 &&
          keyIdx < NUM_NOTES &&
          duration > 0
        ) {

          customSongNotes[
            receivingSongIdx
          ][receivingNoteCount].keyIndex =
            (byte)keyIdx;

          customSongNotes[
            receivingSongIdx
          ][receivingNoteCount].duration =
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

  if (strncmp(line, "KEY:DOWN:", 9) == 0) {

    int keyIdx =
      atoi(line + 9);

    if (
      keyIdx >= 0 &&
      keyIdx < NUM_NOTES
    ) {

      // ------------------------------------------------------
      // Only accept learning key presses while waiting for
      // a key. This prevents playback/menu activity from
      // interfering with learning.
      // ------------------------------------------------------

      if (
        !isPaused &&
        !isPlaying &&
        waitingForKey
      ) {

        if (
          keyIdx ==
          expectedKey
        ) {

          handleCorrectKeyPress(
            (byte)keyIdx
          );

        } else {

          handleWrongKeyPress(
            (byte)keyIdx
          );
        }
      }
    }

    return;
  }


  // ==========================================================
  // KEY UP
  // ==========================================================

  if (strncmp(line, "KEY:UP:", 7) == 0) {

    int keyIdx =
      atoi(line + 7);

    if (
      keyIdx >= 0 &&
      keyIdx < NUM_NOTES &&
      keyHeld &&
      keyIdx == expectedKey
    ) {

      keyHeld = false;
    }

    return;
  }


  // ==========================================================
  // RESET
  // ==========================================================

  if (strcmp(line, "CMD:RESET") == 0) {

    resetLearning();

    return;
  }


  // ==========================================================
  // NEW SONG
  //
  // SONG:NEW:0:SongName
  // ==========================================================

  if (strncmp(line, "SONG:NEW:", 9) == 0) {

    char* secondColon =
      strchr(line + 9, ':');

    if (secondColon != NULL) {

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

        if (strlen(nameStart) == 0) {

          strcpy(
            customSongNames[idx],
            "Custom"
          );

        } else {

          strncpy(
            customSongNames[idx],
            nameStart,
            15
          );

          customSongNames[idx][15] =
            '\0';
        }

        Serial.println(F("SONG:READY"));

        lcd.clear();

        lcd.print(F("Receiving Song"));

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

  if (strcmp(line, "SONG:LIST") == 0) {

    Serial.println(F("SONG:LIST:START"));

    for (
      byte i = 0;
      i < NUM_BUILTIN_SONGS;
      i++
    ) {

      Serial.print(F("SONG:BUILTIN:"));
      Serial.print(i);
      Serial.print(':');

      printProgmemString(
        songs[i].name
      );

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

        Serial.print(F("SONG:CUSTOM:"));
        Serial.print(i);
        Serial.print(':');

        Serial.println(
          customSongNames[i]
        );
      }
    }

    Serial.println(F("SONG:LIST:END"));

    return;
  }


  // ==========================================================
  // LOAD SONG
  //
  // SONG:LOAD:BUILTIN:0
  // SONG:LOAD:CUSTOM:0
  // ==========================================================

  if (
    strncmp(line, "SONG:LOAD:", 10) == 0
  ) {

    char* colon1 =
      strchr(line + 10, ':');

    if (colon1 != NULL) {

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

  if (strcmp(line, "PLAY:ACK") == 0) {

    Serial.println(F("PLAY:ACK:RECEIVED"));

    return;
  }


  // ==========================================================
  // PLAY DONE
  // ==========================================================

  if (strcmp(line, "PLAY:DONE") == 0) {

    isPlaying = false;

    playbackPaused = false;

    currentMenu =
      MENU_LEARN;

    showLearningScreen();

    return;
  }
}


// ============================================================
// PRINT PROGMEM STRING
// ============================================================

void printProgmemString(
  const char* ptr
) {

  char c;

  while (
    (c = pgm_read_byte(ptr++)) != '\0'
  ) {

    Serial.write(c);
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

  beep(30);


  // ==========================================================
  // SONG SELECT
  // ==========================================================

  if (
    currentMenu ==
    MENU_SELECT_SONG
  ) {

    // --------------------------------------------------------
    // 1-9
    // --------------------------------------------------------

    if (
      key >= '1' &&
      key <= '9'
    ) {

      if (enteringSongNumber) {

        songNumberBuffer =
          songNumberBuffer * 10 +
          (key - '0');

        if (
          songNumberBuffer > 0 &&
          songNumberBuffer <= TOTAL_SONGS
        ) {

          selectedSong =
            songNumberBuffer - 1;

          enteringSongNumber = false;

          songNumberBuffer = 0;

          showConfirmPlay();
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
    // 0
    // --------------------------------------------------------

    if (key == '0') {

      if (enteringSongNumber) {

        songNumberBuffer =
          songNumberBuffer * 10;

        if (
          songNumberBuffer > 0 &&
          songNumberBuffer <= TOTAL_SONGS
        ) {

          selectedSong =
            songNumberBuffer - 1;

          enteringSongNumber = false;

          songNumberBuffer = 0;

          showConfirmPlay();
        }

      } else {

        autoPlayEnabled =
          !autoPlayEnabled;

        autoStartPlayback = false;

        menuEnterTime =
          millis();

        showSongSelect();
      }

      return;
    }


    // --------------------------------------------------------
    // *
    // --------------------------------------------------------

    if (key == '*') {

      if (!enteringSongNumber) {

        enteringSongNumber = true;

        songNumberBuffer = 0;

        lcd.clear();

        lcd.setCursor(0, 0);

        lcd.print(F("Enter song #:"));

        lcd.setCursor(0, 1);

        lcd.print(F("1-6 then #"));

      } else {

        if (
          songNumberBuffer > 0 &&
          songNumberBuffer <= TOTAL_SONGS
        ) {

          selectedSong =
            songNumberBuffer - 1;

          enteringSongNumber = false;

          songNumberBuffer = 0;

          showConfirmPlay();
        }
      }

      return;
    }


    // --------------------------------------------------------
    // #
    // --------------------------------------------------------

    if (key == '#') {

      if (enteringSongNumber) {

        if (
          songNumberBuffer > 0 &&
          songNumberBuffer <= TOTAL_SONGS
        ) {

          selectedSong =
            songNumberBuffer - 1;

          enteringSongNumber = false;

          songNumberBuffer = 0;

          showConfirmPlay();
        }

      } else {

        confirmSongSelection();
      }

      return;
    }


    // --------------------------------------------------------
    // A / B / C / D = confirm
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

      menuEnterTime =
        millis();

      autoStartPlayback = false;

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

      } else {

        currentMenu =
          MENU_LEARN;

        showLearningScreen();
      }

      break;


    case 'B':

      if (
        currentMenu == MENU_LEARN &&
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

      menuEnterTime =
        millis();

      autoStartPlayback = false;

      showSongSelect();

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
        currentMenu == MENU_LEARN &&
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

  startPlayback();
}


// ============================================================
// CONFIRM SONG
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

      lcd.print(F("Empty Song"));

      delay(1000);

      showSongSelect();

      return;
    }
  }

  currentMenu =
    MENU_LEARN;

  showLearningScreen();
}


// ============================================================
// GET SONG NAME
// ============================================================

void getSongName(
  const Song* song,
  char* buffer,
  byte bufferSize
) {

  if (
    buffer == NULL ||
    bufferSize == 0
  ) {
    return;
  }

  strncpy_P(
    buffer,
    song->name,
    bufferSize - 1
  );

  buffer[bufferSize - 1] =
    '\0';
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
      learningNotes[songPosition];

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
// CORRECT KEY
// ============================================================

void handleCorrectKeyPress(
  byte keyIdx
) {

  // Prevent repeated KEY:DOWN messages
  // from incrementing the score repeatedly.
  if (keyHeld) {
    return;
  }

  keyHeld = true;

  keyPressStartTime =
    millis();

  totalPausedTime = 0;

  beep(50);

  lcd.setCursor(0, 1);

  lcd.print(F("Correct!        "));

  correctPresses++;
}


// ============================================================
// NOTE NAME
// ============================================================

const char* getNoteName(
  byte idx
) {

  static char buf[6];

  if (
    idx >= NUM_NOTES
  ) {

    strcpy(buf, "?");

    return buf;
  }

  strcpy_P(
    buf,
    (PGM_P)pgm_read_ptr(
      &noteNames[idx]
    )
  );

  return buf;
}


// ============================================================
// WRONG KEY
// ============================================================

void handleWrongKeyPress(
  byte keyIdx
) {

  beep(80);

  lcd.setCursor(0, 1);

  lcd.print(F("Wrong: "));

  lcd.print(
    getNoteName(keyIdx)
  );

  lcd.print(F("       "));

  wrongPresses++;

  // Short delay only.
  // Serial reception is still buffered by hardware,
  // and the next loop will process it.
  delay(100);

  showLearningScreen();
}


// ============================================================
// NEXT NOTE
// ============================================================

void advanceToNextNote() {

  songPosition++;

  if (
    songPosition >=
    learningLength
  ) {

    songPosition = 0;

    waitingForKey = false;

    keyHeld = false;

    lcd.setCursor(0, 1);

    lcd.print(F("Song Complete!"));

    delay(1000);

    currentMenu =
      MENU_STATS;

    showStatsScreen();

    delay(1500);

    currentMenu =
      MENU_LEARN;

    showLearningScreen();

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
// MERGE CONSECUTIVE NOTES
// ============================================================

void mergeNotes(
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

  SongNote current =
    readSongNote(
      source,
      0
    );

  for (
    byte i = 1;
    i < sourceLen;
    i++
  ) {

    SongNote next =
      readSongNote(
        source,
        i
      );

    if (
      next.keyIndex ==
      current.keyIndex
    ) {

      unsigned long newDuration =
        (unsigned long)current.duration +
        next.duration;

      if (
        newDuration > 65535UL
      ) {
        newDuration = 65535UL;
      }

      current.duration =
        (unsigned int)newDuration;

    } else {

      if (
        destLen <
        MAX_NOTES_PER_SONG
      ) {

        dest[destLen++] =
          current;
      }

      current =
        next;
    }
  }

  if (
    destLen <
    MAX_NOTES_PER_SONG
  ) {

    dest[destLen++] =
      current;
  }
}


// ============================================================
// READ SONG NOTE
// Handles PROGMEM and normal RAM notes.
// ============================================================

SongNote readSongNote(
  const SongNote* source,
  byte index
) {

  SongNote result;

  // Built-in songs are stored in PROGMEM.
  // Custom songs are stored in SRAM.

  if (
    source == song1_notes ||
    source == song2_notes ||
    source == song3_notes ||
    source == song4_notes ||
    source == song5_notes
  ) {

    memcpy_P(
      &result,
      &source[index],
      sizeof(SongNote)
    );

  } else {

    result =
      source[index];
  }

  return result;
}


// ============================================================
// LOAD BUILT-IN SONG
// ============================================================

void loadSong(
  byte songIdx
) {

  if (
    songIdx >=
    NUM_BUILTIN_SONGS
  ) {

    return;
  }

  currentSong =
    &songs[songIdx];

  songPosition = 0;

  correctPresses = 0;

  wrongPresses = 0;

  mergeNotes(
    currentSong->notes,
    currentSong->length,
    learningNotes,
    learningLength
  );

  if (
    learningLength == 0
  ) {

    waitingForKey = false;

    return;
  }

  waitingForKey = true;

  expectedKey =
    learningNotes[0].keyIndex;

  keyHeld = false;

  isPaused = false;

  totalPausedTime = 0;

  scrollOffset = 0;
}


// ============================================================
// LOAD CUSTOM SONG
// ============================================================

void loadCustomSong(
  byte idx
) {

  static Song customSong;

  customSong.name =
    customSongNames[idx];

  customSong.notes =
    customSongNotes[idx];

  customSong.length =
    customSongLengths[idx];

  currentSong =
    &customSong;

  songPosition = 0;

  correctPresses = 0;

  wrongPresses = 0;

  mergeNotes(
    currentSong->notes,
    currentSong->length,
    learningNotes,
    learningLength
  );

  if (
    learningLength == 0
  ) {

    waitingForKey = false;

    return;
  }

  waitingForKey = true;

  expectedKey =
    learningNotes[0].keyIndex;

  keyHeld = false;

  isPaused = false;

  totalPausedTime = 0;

  scrollOffset = 0;
}


// ============================================================
// RESET LEARNING
// ============================================================

void resetLearning() {

  songPosition = 0;

  correctPresses = 0;

  wrongPresses = 0;

  mergeNotes(
    currentSong->notes,
    currentSong->length,
    learningNotes,
    learningLength
  );

  if (
    learningLength == 0
  ) {

    waitingForKey = false;

    return;
  }

  waitingForKey = true;

  expectedKey =
    learningNotes[0].keyIndex;

  keyHeld = false;

  isPaused = false;

  totalPausedTime = 0;

  scrollOffset = 0;

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
// LEARNING SCREEN
// ============================================================

void showLearningScreen() {

  lcd.clear();

  if (
    learningLength == 0
  ) {

    lcd.print(F("No notes"));

    return;
  }

  lcd.setCursor(0, 0);

  lcd.print(F("Press: "));

  lcd.print(
    getNoteName(expectedKey)
  );

  lcd.print(F("     "));

  lcd.setCursor(0, 1);

  char nameBuffer[17];

  getSongName(
    currentSong,
    nameBuffer,
    sizeof(nameBuffer)
  );

  lcd.print(nameBuffer);

  lcd.print(' ');

  lcd.print(songPosition + 1);

  lcd.print('/');

  lcd.print(learningLength);
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
      learningNotes[songPosition];

    long remaining =
      (long)note.duration -
      (long)heldTime;

    if (
      remaining < 0
    ) {
      remaining = 0;
    }

    lcd.setCursor(11, 1);

    if (
      remaining > 0
    ) {

      lcd.print(remaining);

      lcd.print(F("ms "));

    } else {

      lcd.print(F("DONE!"));
    }
  }
}


// ============================================================
// SCROLL
// ============================================================

void handleScrollDisplay() {

  if (
    currentMenu == MENU_LEARN &&
    !isPaused &&
    !isPlaying
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
// UPDATE BOTTOM LINE
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

  if (
    scrollOffset == 0
  ) {

    char nameBuffer[17];

    getSongName(
      currentSong,
      nameBuffer,
      sizeof(nameBuffer)
    );

    lcd.print(nameBuffer);

    lcd.print(' ');

    lcd.print(songPosition + 1);

    lcd.print('/');

    lcd.print(learningLength);

  } else {

    lcd.print(F("Next: "));

    byte nextIdx =
      (songPosition + 1) %
      learningLength;

    lcd.print(
      getNoteName(
        learningNotes[nextIdx].keyIndex
      )
    );

    if (
      nextIdx + 1 <
      learningLength
    ) {

      lcd.print(' ');

      lcd.print(
        getNoteName(
          learningNotes[nextIdx + 1].keyIndex
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

  lcd.print(correctPresses);

  lcd.setCursor(0, 1);

  lcd.print(F("Wrong: "));

  lcd.print(wrongPresses);

  lcd.print(F(" Acc:"));

  unsigned int total =
    correctPresses +
    wrongPresses;

  unsigned int accuracy = 0;

  if (
    total > 0
  ) {

    accuracy =
      (
        (unsigned long)correctPresses *
        100UL
      ) /
      total;
  }

  lcd.print(accuracy);

  lcd.print('%');
}


// ============================================================
// SONG SELECT
// ============================================================

void showSongSelect() {

  lcd.clear();

  lcd.setCursor(0, 0);

  if (autoPlayEnabled) {

    lcd.print(F("AUTO:ON "));

  } else {

    lcd.print(F("AUTO:OFF"));
  }

  lcd.print(' ');

  lcd.print(selectedSong + 1);

  lcd.print('/');

  lcd.print(TOTAL_SONGS);

  lcd.setCursor(0, 1);

  lcd.print(F("> "));

  if (
    selectedSong <
    NUM_BUILTIN_SONGS
  ) {

    char nameBuffer[17];

    getSongName(
      &songs[selectedSong],
      nameBuffer,
      sizeof(nameBuffer)
    );

    lcd.print(nameBuffer);

  } else {

    byte customIdx =
      selectedSong -
      NUM_BUILTIN_SONGS;

    if (
      customSongLengths[customIdx] > 0
    ) {

      lcd.print(
        customSongNames[customIdx]
      );

    } else {

      lcd.print(F("(empty)"));
    }
  }
}


// ============================================================
// CONFIRM SCREEN
// ============================================================

void showConfirmPlay() {

  currentMenu =
    MENU_CONFIRM_PLAY;

  lcd.clear();

  lcd.setCursor(0, 0);

  lcd.print(F("Play this song?"));

  lcd.setCursor(0, 1);

  if (
    selectedSong <
    NUM_BUILTIN_SONGS
  ) {

    char nameBuffer[17];

    getSongName(
      &songs[selectedSong],
      nameBuffer,
      sizeof(nameBuffer)
    );

    lcd.print(nameBuffer);

  } else {

    byte customIdx =
      selectedSong -
      NUM_BUILTIN_SONGS;

    if (
      customSongLengths[customIdx] > 0
    ) {

      lcd.print(
        customSongNames[customIdx]
      );

    } else {

      lcd.print(F("(empty)"));
    }
  }
}


// ============================================================
// BEEP
// ============================================================

void beep(
  unsigned int duration
) {

  digitalWrite(
    buzzerPin,
    HIGH
  );

  delay(duration);

  digitalWrite(
    buzzerPin,
    LOW
  );
}


// ============================================================
// START PLAYBACK
// ============================================================

void startPlayback() {

  if (
    currentSong == NULL ||
    currentSong->length == 0
  ) {

    return;
  }

  isPlaying = true;

  playbackPaused = false;

  currentMenu =
    MENU_PLAYBACK;

  playbackNoteIndex = 0;

  playbackStartTime =
    millis();

  totalPausedPlaybackTime = 0;

  // ----------------------------------------------------------
  // Send song to music player
  // ----------------------------------------------------------

  Serial.print(F("PLAY:START:"));

  Serial.print(
    currentSong->length
  );

  Serial.print(',');

  for (
    byte i = 0;
    i < currentSong->length;
    i++
  ) {

    SongNote note =
      readSongNote(
        currentSong->notes,
        i
      );

    Serial.print(
      note.keyIndex
    );

    Serial.print(':');

    Serial.print(
      note.duration
    );

    if (
      i <
      currentSong->length - 1
    ) {

      Serial.print(',');
    }
  }

  Serial.print(',');

  Serial.println(
    playbackSpeed,
    2
  );

  Serial.flush();

  // ----------------------------------------------------------
  // Local confirmation beep
  // ----------------------------------------------------------

  digitalWrite(
    buzzerPin,
    HIGH
  );

  delay(30);

  digitalWrite(
    buzzerPin,
    LOW
  );

  showPlaybackScreen();
}


// ============================================================
// STOP PLAYBACK
// ============================================================

void stopPlayback() {

  if (!isPlaying) {
    return;
  }

  Serial.println(F("PLAY:STOP"));

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

  if (!isPlaying) {
    return;
  }

  if (
    playbackPaused
  ) {

    totalPausedPlaybackTime +=
      millis() -
      playbackPausedTime;

    playbackPaused = false;

    Serial.println(F("PLAY:RESUME"));

    Serial.flush();

  } else {

    playbackPaused = true;

    playbackPausedTime =
      millis();

    Serial.println(F("PLAY:PAUSE"));

    Serial.flush();
  }

  showPlaybackScreen();
}


// ============================================================
// PLAYBACK UPDATE
// ============================================================

void handlePlayback() {

  if (
    !isPlaying ||
    playbackPaused
  ) {

    return;
  }

  if (
    playbackNoteIndex >=
    currentSong->length
  ) {

    stopPlayback();

    return;
  }

  unsigned long elapsed =
    millis() -
    playbackStartTime -
    totalPausedPlaybackTime;

  unsigned long noteStart = 0;

  for (
    byte i = 0;
    i < playbackNoteIndex;
    i++
  ) {

    SongNote note =
      readSongNote(
        currentSong->notes,
        i
      );

    noteStart +=
      note.duration /
      playbackSpeed;
  }

  SongNote currentNote =
    readSongNote(
      currentSong->notes,
      playbackNoteIndex
    );

  unsigned long adjustedDuration =
    (
      (float)currentNote.duration /
      playbackSpeed
    );

  if (
    elapsed >=
    noteStart +
    adjustedDuration
  ) {

    playbackNoteIndex++;

    showPlaybackScreen();

    if (
      playbackNoteIndex >=
      currentSong->length
    ) {

      delay(100);

      stopPlayback();
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

  char nameBuffer[17];

  getSongName(
    currentSong,
    nameBuffer,
    sizeof(nameBuffer)
  );

  lcd.print(nameBuffer);

  lcd.setCursor(0, 1);

  if (
    playbackNoteIndex <
    currentSong->length
  ) {

    lcd.print(F("Spd:"));

    lcd.print(
      playbackSpeed,
      1
    );

    lcd.print('x');

    lcd.print(' ');

    SongNote note =
      readSongNote(
        currentSong->notes,
        playbackNoteIndex
      );

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
      currentSong->length
    );

  } else {

    lcd.print(F("Complete!"));
  }
}
