const int buttonPins[] = {2,3,4,5,6,7,8,9,10,11,12}; // klawisze (12 nie użyty w tym przypadku)
const int controlPins[] = {A0,A1,A2,A3,A4,A5};       // przyciski opcji (A0 nie użyty w tym przypadku)
const int speakerPin = 13;                            // głośnik

int volume = 128;
int scaleOffset = 0;
int baseFreqs[] = {261,293,329,349,392,440,494,523,587,659,698}; // gama C-dur

bool lastControlStates[6];

int introNotes[] = {
  392, 440, 494, 440,
  392, 349, 330, 349
};

int introDurations[] = {
  220, 180, 180, 220,
  220, 180, 180, 260
};

//wstepne granie
void playIntro() {
  for (int i = 0; i < 8; i++) {
    tone(speakerPin, introNotes[i], introDurations[i]);
    delay(introDurations[i] + 30);
    noTone(speakerPin);
  }
}

// przygotowanie urządzenia
void setup() {
  Serial.begin(9600);
  Serial.println("Uruchomiono klawiature");

  for (int i = 0; i < 11; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  for (int i = 0; i < 6; i++) {
    pinMode(controlPins[i], INPUT_PULLUP);
    lastControlStates[i] = HIGH;
  }

  pinMode(speakerPin, OUTPUT);
  noTone(speakerPin);
  playIntro();
}

// przyciski opcji
void readControls() {
  for (int i = 0; i < 6; i++) {
    int state = digitalRead(controlPins[i]);

    if (state == LOW && lastControlStates[i] == HIGH) {
      if (i == 1) volume = min(255, volume + 10);        // A1 podgłaśnianie
      if (i == 2) volume = max(0, volume - 10);          // A2 ściszanie
      if (i == 3) scaleOffset = min(3, scaleOffset + 1); // A3 wyżej
      if (i == 4) scaleOffset = max(-2, scaleOffset - 1); // A4 niżej
      if (i == 5) {                                      // A5 reset
        volume = 128;
        scaleOffset = 0;
        Serial.println("\n*** DOMYSLNE USTAWIENIA ***");
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

void loop() {
  bool anyKeyPlaying = false;
  int newFreq = 0;

  readControls();

  // sprawdzenie klawiszy
  for (int i = 0; i < 11; i++) {
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

  // gra tylko jeśli faktycznie wciśnięto klawisz
  if (anyKeyPlaying && newFreq > 0) {
    tone(speakerPin, newFreq);
  } else {
    noTone(speakerPin);
  }

  // diagnostyka
  Serial.print("Glosnosc:");
  Serial.print(volume);
  Serial.print(" Skala:");
  Serial.print(scaleOffset);
  Serial.print(" Klawisze:");
  for (int i = 0; i < 11; i++) {
    Serial.print(digitalRead(buttonPins[i]));
    Serial.print(i == 10 ? "" : ",");
  }
  Serial.println();

  delay(20);
}
