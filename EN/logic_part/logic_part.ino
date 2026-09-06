#include <LiquidCrystal.h>
#include <Keypad.h>
#include <avr/pgmspace.h>

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buzzerPin = A0;

const byte MATRIX_ROWS = 4;
const byte MATRIX_COLS = 4;
char matrixKeys[MATRIX_ROWS][MATRIX_COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte matrixRowPins[MATRIX_ROWS] = {A1, A2, A3, A4};
byte matrixColPins[MATRIX_COLS] = {3, 4, 5, 6};
Keypad matrixKeypad = Keypad(makeKeymap(matrixKeys), matrixRowPins, matrixColPins, MATRIX_ROWS, MATRIX_COLS);

const char noteNames_0[] PROGMEM = "C4";
const char noteNames_1[] PROGMEM = "D4";
const char noteNames_2[] PROGMEM = "E4";
const char noteNames_3[] PROGMEM = "F4";
const char noteNames_4[] PROGMEM = "G4";
const char noteNames_5[] PROGMEM = "A4";
const char noteNames_6[] PROGMEM = "B4";
const char noteNames_7[] PROGMEM = "C5";
const char noteNames_8[] PROGMEM = "D5";
const char noteNames_9[] PROGMEM = "E5";
const char noteNames_10[] PROGMEM = "F5";
const char noteNames_11[] PROGMEM = "F#5";
const char* const noteNames[] PROGMEM = {noteNames_0, noteNames_1, noteNames_2, noteNames_3, noteNames_4, noteNames_5, noteNames_6, noteNames_7, noteNames_8, noteNames_9, noteNames_10, noteNames_11};

struct SongNote {
  int keyIndex;
  unsigned long duration;
};

struct Song {
  const char* name;
  SongNote* notes;
  int length;
};

SongNote song1_notes[] = {
  {0, 500}, {2, 500}, {4, 500}, {5, 500},
  {7, 1000}, {5, 500}, {4, 500}, {2, 500},
  {0, 1000}
};

SongNote song2_notes[] = {
  {4, 400}, {4, 400}, {5, 400}, {4, 400},
  {3, 400}, {2, 800},
  {4, 400}, {4, 400}, {5, 400}, {4, 400},
  {2, 400}, {0, 800}
};

SongNote song3_notes[] = {
  {0, 300}, {2, 300}, {4, 300}, {5, 300},
  {7, 300}, {9, 300}, {11, 600},
  {9, 300}, {7, 300}, {5, 300}, {4, 300},
  {2, 300}, {0, 600}
};

SongNote song4_notes[] = {
  {7, 300}, {7, 300}, {7, 300}, {5, 200}, {7, 200}, {8, 400}, {9, 400},
  {7, 300}, {7, 300}, {7, 300}, {5, 200}, {7, 200}, {8, 400}, {9, 400},
  {7, 300}, {7, 300}, {7, 300}, {5, 200}, {7, 200}, {8, 400}, {9, 400},
  {10, 300}, {10, 300}, {9, 300}, {8, 300}, {7, 300}, {5, 200}, {7, 400}, {5, 400}
};

SongNote song5_notes[] = {
  {4, 500}, {4, 500}, {4, 500}, {3, 350}, {5, 150}, {4, 500},
  {3, 350}, {5, 150}, {4, 1000},
  {7, 500}, {7, 500}, {7, 500}, {8, 350}, {10, 150}, {9, 500},
  {8, 350}, {10, 150}, {9, 1000},
  {5, 500}, {5, 500}, {5, 500}, {6, 350}, {8, 150}, {7, 500},
  {6, 350}, {8, 150}, {7, 1000}
};

Song songs[] = {
  {"Mary Lamb", song1_notes, 9},
  {"Twinkle", song2_notes, 12},
  {"Scale Up", song3_notes, 13},
  {"Wolf3D", song4_notes, 28},
  {"STALKER", song5_notes, 27}
};
const int NUM_BUILTIN_SONGS = 5;
const int MAX_CUSTOM_SONGS = 1;
const int MAX_NOTES_PER_SONG = 22;
const int TOTAL_SONGS = NUM_BUILTIN_SONGS + MAX_CUSTOM_SONGS;

SongNote customSongNotes[MAX_CUSTOM_SONGS][MAX_NOTES_PER_SONG];
int customSongLengths[MAX_CUSTOM_SONGS] = {0};
char customSongNames[MAX_CUSTOM_SONGS][16] = {"Custom 1"};

bool receivingSong = false;
int receivingSongIdx = -1;
int receivingNoteCount = 0;

Song* currentSong = &songs[0];
int songPosition = 0;
int correctPresses = 0;
int wrongPresses = 0;
bool waitingForKey = false;
int expectedKey = -1;
unsigned long keyPressStartTime = 0;
bool keyHeld = false;
bool isPaused = false;
unsigned long pauseStartTime = 0;
unsigned long totalPausedTime = 0;

SongNote learningNotes[22];
int learningLength = 0;

bool keyStates[11] = {false};

enum MenuState { MENU_WELCOME, MENU_SELECT_SONG, MENU_CONFIRM_PLAY, MENU_LEARN, MENU_STATS, MENU_PAUSED, MENU_PLAYBACK };
MenuState currentMenu = MENU_WELCOME;
int selectedSong = 0;
int scrollOffset = 0;
unsigned long lastScrollTime = 0;
bool isPlaying = false;
bool playbackPaused = false;
unsigned long playbackStartTime = 0;
unsigned long playbackPausedTime = 0;
unsigned long totalPausedPlaybackTime = 0;
int playbackNoteIndex = 0;
unsigned long menuEnterTime = 0;
bool autoStartPlayback = false;
bool autoPlayEnabled = false;
bool enteringSongNumber = false;
int songNumberBuffer = 0;
float playbackSpeed = 1.0;

void setup() {
  Serial.begin(9600);
  Serial.println("LOGIC READY");

  lcd.begin(16, 2);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  showWelcomeScreen();
}

void showWelcomeScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Music Keyboard");
  lcd.setCursor(0, 1);
  lcd.print("Loading...");
  delay(3000);
  currentMenu = MENU_SELECT_SONG;
  selectedSong = 0;
  menuEnterTime = millis();
  showSongSelect();
}

void loop() {
  handleSerialInput();
  handleMatrixKeypad();
  
  if (currentMenu == MENU_SELECT_SONG && autoPlayEnabled && !autoStartPlayback) {
    if (millis() - menuEnterTime >= 3000) {
      autoStartPlayback = true;
      loadAndPlaySong();
    }
  }
  
  if (!isPaused && !isPlaying) {
    checkKeyPressTiming();
  }
  handleScrollDisplay();
  handlePlayback();
}

void handleSerialInput() {
  while (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    
    if (receivingSong) {
      if (line == "SONG:END") {
        customSongLengths[receivingSongIdx] = receivingNoteCount;
        receivingSong = false;
        Serial.print("SONG:SAVED:");
        Serial.println(receivingSongIdx);
        lcd.clear();
        lcd.print("Song Saved!");
        delay(1000);
        showSongSelect();
      } else if (line.startsWith("NOTE:")) {
        int comma1 = line.indexOf(',', 5);
        int comma2 = line.indexOf(',', comma1 + 1);
        if (comma1 > 0 && comma2 > 0) {
          int keyIdx = line.substring(5, comma1).toInt();
          int duration = line.substring(comma1 + 1, comma2).toInt();
          if (receivingNoteCount < MAX_NOTES_PER_SONG && keyIdx >= 0 && keyIdx < 12) {
            customSongNotes[receivingSongIdx][receivingNoteCount] = {keyIdx, (unsigned long)duration};
            receivingNoteCount++;
          }
        }
      }
      return;
    }
    
    if (line.startsWith("KEY:DOWN:")) {
      int keyIdx = line.substring(9).toInt();
      if (keyIdx >= 0 && keyIdx < 11) {
        keyStates[keyIdx] = true;
        if (!isPaused && waitingForKey && keyIdx == expectedKey) {
          handleCorrectKeyPress(keyIdx);
          //Serial.println("Correct key! \n"); //debug
        } else if (!isPaused && waitingForKey && keyIdx != expectedKey) {
          handleWrongKeyPress(keyIdx);
          //Serial.println("Wrong key! \n"); //debug
        }
      }
    } else if (line.startsWith("KEY:UP:")) {
      //Serial.println("Key up! \n"); //debug
      int keyIdx = line.substring(7).toInt();
      if (keyIdx >= 0 && keyIdx < 11) {
        keyStates[keyIdx] = false;
        if (keyHeld && keyIdx == expectedKey) {
          keyHeld = false;
        }
      }
    } else if (line == "CMD:RESET") {
      resetLearning();
    } else if (line.startsWith("SONG:NEW:")) {
      int idx = line.substring(9).toInt();
      if (idx >= 0 && idx < MAX_CUSTOM_SONGS) {
        receivingSong = true;
        receivingSongIdx = idx;
        receivingNoteCount = 0;
        String name = line.substring(line.indexOf(':') + 1);
        name = name.substring(name.indexOf(':') + 1);
        name.toCharArray(customSongNames[idx], 16);
        Serial.println("SONG:READY");
        lcd.clear();
        lcd.print("Receiving Song");
        lcd.setCursor(0, 1);
        lcd.print(customSongNames[idx]);
      }
    } else if (line == "SONG:LIST") {
      Serial.println("SONG:LIST:START");
      for (int i = 0; i < NUM_BUILTIN_SONGS; i++) {
        Serial.print("SONG:BUILTIN:");
        Serial.print(i);
        Serial.print(":");
        Serial.println(songs[i].name);
      }
      for (int i = 0; i < MAX_CUSTOM_SONGS; i++) {
        if (customSongLengths[i] > 0) {
          Serial.print("SONG:CUSTOM:");
          Serial.print(i);
          Serial.print(":");
          Serial.println(customSongNames[i]);
        }
      }
      Serial.println("SONG:LIST:END");
    } else if (line.startsWith("SONG:LOAD:")) {
      int typeEnd = line.indexOf(':', 10);
      String type = line.substring(10, typeEnd);
      int idx = line.substring(typeEnd + 1).toInt();
      if (type == "BUILTIN" && idx >= 0 && idx < NUM_BUILTIN_SONGS) {
        loadSong(idx);
        currentMenu = MENU_LEARN;
        showLearningScreen();
      } else if (type == "CUSTOM" && idx >= 0 && idx < MAX_CUSTOM_SONGS && customSongLengths[idx] > 0) {
        loadCustomSong(idx);
        currentMenu = MENU_LEARN;
        showLearningScreen();
      }
    }
  }
}

void handleMatrixKeypad() {
  char key = matrixKeypad.getKey();
  if (key) {
    beep(50);
    
    if (currentMenu == MENU_SELECT_SONG) {
      if (key >= '1' && key <= '9') {
        if (enteringSongNumber) {
          songNumberBuffer = songNumberBuffer * 10 + (key - '0');
          if (songNumberBuffer > 0 && songNumberBuffer <= TOTAL_SONGS) {
            selectedSong = songNumberBuffer - 1;
            enteringSongNumber = false;
            songNumberBuffer = 0;
            showConfirmPlay();
          }
        } else {
          int songNum = key - '1';
          if (songNum < TOTAL_SONGS) {
            selectedSong = songNum;
            showConfirmPlay();
          }
        }
      } else if (key == '0') {
        if (enteringSongNumber) {
          songNumberBuffer = songNumberBuffer * 10;
          if (songNumberBuffer > 0 && songNumberBuffer <= TOTAL_SONGS) {
            selectedSong = songNumberBuffer - 1;
            enteringSongNumber = false;
            songNumberBuffer = 0;
            showConfirmPlay();
          }
        } else {
          autoPlayEnabled = !autoPlayEnabled;
          autoStartPlayback = false;
          menuEnterTime = millis();
          showSongSelect();
        }
      } else if (key == '*') {
        if (!enteringSongNumber) {
          enteringSongNumber = true;
          songNumberBuffer = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter song #:   ");
          lcd.setCursor(0, 1);
          lcd.print("1-9, 0, #=ok");
        } else {
          if (songNumberBuffer > 0 && songNumberBuffer <= TOTAL_SONGS) {
            selectedSong = songNumberBuffer - 1;
            enteringSongNumber = false;
            songNumberBuffer = 0;
            showConfirmPlay();
          }
        }
        selectedSong = (selectedSong + 1) % TOTAL_SONGS;
        menuEnterTime = millis();
        autoStartPlayback = false;
        showSongSelect();
      } else if (key == '#') {
        confirmSongSelection();
      } else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
        confirmSongSelection();
      }
      return;
    }
    
    if (currentMenu == MENU_CONFIRM_PLAY) {
      if (key == '#' || key == 'D') {
        loadAndPlaySong();
      } else if (key == '*' || key == 'A' || key == 'C') {
        currentMenu = MENU_SELECT_SONG;
        menuEnterTime = millis();
        autoStartPlayback = false;
        showSongSelect();
      }
      return;
    }
    
    if (currentMenu == MENU_PLAYBACK) {
      if (key == '*') {
        playbackSpeed = min(2.0, playbackSpeed + 0.25);
        showPlaybackScreen();
        return;
      } else if (key == '#') {
        playbackSpeed = max(0.25, playbackSpeed - 0.25);
        showPlaybackScreen();
        return;
      } else if (key == 'B') {
        togglePlaybackPause();
        return;
      }
    }
    
    switch (key) {
      case 'A':
        if (currentMenu == MENU_PAUSED) {
          resumeSong();
        } else if (currentMenu == MENU_PLAYBACK) {
          stopPlayback();
        } else {
          currentMenu = MENU_LEARN;
          showLearningScreen();
        }
        break;
      case 'B':
        if (currentMenu == MENU_LEARN && !isPaused && !isPlaying) {
          pauseSong();
        } else if (currentMenu == MENU_PLAYBACK) {
          togglePlaybackPause();
        } else {
          currentMenu = MENU_STATS;
          showStatsScreen();
        }
        break;
      case 'C':
        currentMenu = MENU_SELECT_SONG;
        selectedSong = 0;
        menuEnterTime = millis();
        autoStartPlayback = false;
        showSongSelect();
        break;
      case 'D':
        if (currentMenu == MENU_PLAYBACK) {
          stopPlayback();
        } else {
          resetLearning();
        }
        break;
      case '0':
        if (currentMenu == MENU_LEARN) {
          startPlayback();
        }
        break;
    }
  }
}

void loadAndPlaySong() {
  confirmSongSelection();
  delay(10);
  startPlayback();
}

void confirmSongSelection() {
  if (selectedSong < NUM_BUILTIN_SONGS) {
    loadSong(selectedSong);
  } else {
    int customIdx = selectedSong - NUM_BUILTIN_SONGS;
    if (customSongLengths[customIdx] > 0) {
      loadCustomSong(customIdx);
    }
  }
  currentMenu = MENU_LEARN;
  showLearningScreen();
}

void checkKeyPressTiming() {
  if (waitingForKey && keyHeld) {
    unsigned long heldTime = millis() - keyPressStartTime - totalPausedTime;
    SongNote note = learningNotes[songPosition];
    if (heldTime >= note.duration) {
      advanceToNextNote();
    }
    updateTimerDisplay();
  }
}

void handleCorrectKeyPress(int keyIdx) {
  keyHeld = true;
  keyPressStartTime = millis();
  totalPausedTime = 0;
  beep(100);
  lcd.setCursor(0, 1);
  lcd.print("Correct!       ");
  correctPresses++;
}

const char* getNoteName(int idx) {
  static char buf[5];
  if (idx < 0 || idx > 11) return "?";
  strcpy_P(buf, (char*)pgm_read_ptr(&noteNames[idx]));
  return buf;
}

void handleWrongKeyPress(int keyIdx) {
  beep(200);
  lcd.setCursor(0, 1);
  lcd.print("Wrong: ");
  lcd.print(getNoteName(keyIdx));
  lcd.print("           ");
  wrongPresses++;
  delay(500);
  showLearningScreen();
}

void advanceToNextNote() {
  songPosition++;
  if (songPosition >= learningLength) {
    songPosition = 0;
    lcd.setCursor(0, 1);
    lcd.print("Song Complete! ");
    delay(2000);
    showStatsScreen();
    delay(3000);
    showLearningScreen();
  } else {
    waitingForKey = true;
    expectedKey = learningNotes[songPosition].keyIndex;
    keyHeld = false;
    totalPausedTime = 0;
    scrollOffset = 0;
    showLearningScreen();
  }
}

// ============================================================
// MERGE CONSECUTIVE NOTES
// ============================================================

void mergeNotes(SongNote* source, int sourceLen, SongNote* dest, int& destLen) {
  destLen = 0;
  if (sourceLen <= 0) return;
  int currentKey = source[0].keyIndex;
  unsigned long currentDur = source[0].duration;
  for (int i = 1; i < sourceLen; i++) {
    if (source[i].keyIndex == currentKey) {
      currentDur += source[i].duration;
    } else {
      dest[destLen].keyIndex = currentKey;
      dest[destLen].duration = currentDur;
      destLen++;
      currentKey = source[i].keyIndex;
      currentDur = source[i].duration;
    }
  }
  dest[destLen].keyIndex = currentKey;
  dest[destLen].duration = currentDur;
  destLen++;
}

// ============================================================
// LOAD BUILT-IN SONG
// ============================================================

void loadSong(int songIdx) {
  currentSong = &songs[songIdx];
  songPosition = 0;
  correctPresses = 0;
  wrongPresses = 0;
  mergeNotes(currentSong->notes, currentSong->length, learningNotes, learningLength);
  waitingForKey = true;
  expectedKey = learningNotes[0].keyIndex;
  keyHeld = false;
  isPaused = false;
  totalPausedTime = 0;
  scrollOffset = 0;
}

void loadCustomSong(int idx) {
  static Song customSong;
  customSong.name = customSongNames[idx];
  customSong.notes = customSongNotes[idx];
  customSong.length = customSongLengths[idx];
  currentSong = &customSong;
  songPosition = 0;
  correctPresses = 0;
  wrongPresses = 0;
  mergeNotes(currentSong->notes, currentSong->length, learningNotes, learningLength);
  waitingForKey = true;
  expectedKey = learningNotes[0].keyIndex;
  keyHeld = false;
  isPaused = false;
  totalPausedTime = 0;
  scrollOffset = 0;
}

void resetLearning() {
  songPosition = 0;
  correctPresses = 0;
  wrongPresses = 0;
  mergeNotes(currentSong->notes, currentSong->length, learningNotes, learningLength);
  waitingForKey = true;
  expectedKey = learningNotes[0].keyIndex;
  keyHeld = false;
  isPaused = false;
  totalPausedTime = 0;
  scrollOffset = 0;
  showLearningScreen();
}

void pauseSong() {
  isPaused = true;
  pauseStartTime = millis();
  currentMenu = MENU_PAUSED;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PAUSED");
  lcd.setCursor(0, 1);
  lcd.print("A:Resume B:Menu");
}

void resumeSong() {
  if (isPaused) {
    totalPausedTime += millis() - pauseStartTime;
    isPaused = false;
    currentMenu = MENU_LEARN;
    showLearningScreen();
  }
}

void showLearningScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press: ");
  lcd.print(getNoteName(expectedKey));
  lcd.print("  ");
  lcd.setCursor(0, 1);
  lcd.print(currentSong->name);
  lcd.print(" ");
  lcd.print(songPosition + 1);
  lcd.print("/");
  lcd.print(learningLength);
  lcd.print("    ");
}

void updateTimerDisplay() {
  if (keyHeld && !isPaused && !isPlaying && waitingForKey) {
    unsigned long heldTime = millis() - keyPressStartTime - totalPausedTime;
    SongNote note = learningNotes[songPosition];
    long remaining = (long)note.duration - (long)heldTime;
    if (remaining < 0) remaining = 0;
    lcd.setCursor(11, 1);
    if (remaining > 0) {
      if (remaining < 1000) {
        lcd.print("  ");
      } else if (remaining < 10000) {
        lcd.print(" ");
      }
      lcd.print(remaining);
      lcd.print("ms");
    } else {
      lcd.print("   DONE!");
    }
  }
}

void handleScrollDisplay() {
  if (currentMenu == MENU_LEARN && !isPaused) {
    unsigned long now = millis();
    if (now - lastScrollTime > 1500) {
      lastScrollTime = now;
      scrollOffset = (scrollOffset + 1) % 2;
      updateBottomLine();
    }
  }
}

void updateBottomLine() {
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  if (scrollOffset == 0) {
    lcd.print(currentSong->name);
    lcd.print(" ");
    lcd.print(songPosition + 1);
    lcd.print("/");
    lcd.print(learningLength);
    lcd.print("     ");
  } else {
    lcd.print("Next: ");
    int nextIdx = (songPosition + 1) % learningLength;
    lcd.print(getNoteName(learningNotes[nextIdx].keyIndex));
    if (nextIdx + 1 < learningLength) {
      lcd.print(" ");
      lcd.print(getNoteName(learningNotes[nextIdx + 1].keyIndex));
    }
    lcd.print("     ");
  }
}

void showStatsScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Correct: ");
  lcd.print(correctPresses);
  lcd.print("            ");
  lcd.setCursor(0, 1);
  lcd.print("Wrong: ");
  lcd.print(wrongPresses);
  lcd.print("  Acc: ");
  int total = correctPresses + wrongPresses;
  if (total > 0) {
    lcd.print((correctPresses * 100) / total);
    lcd.print("%");
  } else {
    lcd.print("0%");
  }
}

void showSongSelect() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (autoPlayEnabled) {
    lcd.print("AUTO:ON  ");
  } else {
    lcd.print("AUTO:OFF ");
  }
  lcd.print(selectedSong + 1);
  lcd.print("/");
  lcd.print(TOTAL_SONGS);
  lcd.setCursor(0, 1);
  lcd.print("> ");
  if (selectedSong < NUM_BUILTIN_SONGS) {
    lcd.print(songs[selectedSong].name);
  } else {
    int customIdx = selectedSong - NUM_BUILTIN_SONGS;
    if (customSongLengths[customIdx] > 0) {
      lcd.print(customSongNames[customIdx]);
    } else {
      lcd.print("(empty)");
    }
  }
  lcd.setCursor(10, 1);
  lcd.print("#=P *=N 0=AP");
}

void showConfirmPlay() {
  currentMenu = MENU_CONFIRM_PLAY;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Play this song?");
  lcd.setCursor(0, 1);
  if (selectedSong < NUM_BUILTIN_SONGS) {
    lcd.print(songs[selectedSong].name);
  } else {
    int customIdx = selectedSong - NUM_BUILTIN_SONGS;
    if (customSongLengths[customIdx] > 0) {
      lcd.print(customSongNames[customIdx]);
    } else {
      lcd.print("(empty)");
    }
  }
  lcd.setCursor(10, 1);
  lcd.print("#=Y *=N");
}

void beep(int duration) {
  digitalWrite(buzzerPin, HIGH);
  delay(duration);
  digitalWrite(buzzerPin, LOW);
}

void startPlayback() {
  if (currentSong->length == 0) return;
  isPlaying = true;
  currentMenu = MENU_PLAYBACK;
  playbackNoteIndex = 0;
  playbackStartTime = millis();
  totalPausedPlaybackTime = 0;
  
  Serial.print("PLAY:START:");
  Serial.print(currentSong->length);
  Serial.print(",");
  for (int i = 0; i < currentSong->length; i++) {
    Serial.print(currentSong->notes[i].keyIndex);
    Serial.print(":");
    Serial.print(currentSong->notes[i].duration);
    if (i < currentSong->length - 1) Serial.print(",");
  }
  Serial.print(",");
  Serial.println(playbackSpeed, 2);
  Serial.flush();
  
  digitalWrite(buzzerPin, HIGH);
  delay(50);
  digitalWrite(buzzerPin, LOW);
  
  showPlaybackScreen();
}

void stopPlayback() {
  if (isPlaying) {
    Serial.println("PLAY:STOP");
    Serial.flush();
    isPlaying = false;
    playbackPaused = false;
    currentMenu = MENU_LEARN;
    showLearningScreen();
  }
}

void togglePlaybackPause() {
  if (!isPlaying) return;
  
  if (playbackPaused) {
    totalPausedPlaybackTime += millis() - playbackPausedTime;
    playbackPaused = false;
    Serial.println("PLAY:RESUME");
    Serial.flush();
  } else {
    playbackPaused = true;
    playbackPausedTime = millis();
    Serial.println("PLAY:PAUSE");
    Serial.flush();
  }
  showPlaybackScreen();
}

void handlePlayback() {
  if (!isPlaying || playbackPaused) return;
  
  unsigned long elapsed = ((millis() - playbackStartTime) - totalPausedPlaybackTime) * playbackSpeed;
  if (playbackNoteIndex < currentSong->length) {
    unsigned long noteStart = 0;
    for (int i = 0; i < playbackNoteIndex; i++) {
      noteStart += currentSong->notes[i].duration;
    }
    if (elapsed >= noteStart + currentSong->notes[playbackNoteIndex].duration) {
      playbackNoteIndex++;
      showPlaybackScreen();
    }
  } else {
    stopPlayback();
  }
}

void showPlaybackScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (playbackPaused) {
    lcd.print("PAUSED: ");
  } else {
    lcd.print("PLAYING: ");
  }
  lcd.print(currentSong->name);
  lcd.setCursor(0, 1);
  if (playbackNoteIndex < currentSong->length) {
    lcd.print("Speed: ");
    lcd.print(playbackSpeed, 1);
    lcd.print("x ");
    lcd.print(getNoteName(currentSong->notes[playbackNoteIndex].keyIndex));
    lcd.print(" ");
    lcd.print(playbackNoteIndex + 1);
    lcd.print("/");
    lcd.print(currentSong->length);
  } else {
    lcd.print("Complete!       ");
  }
}
