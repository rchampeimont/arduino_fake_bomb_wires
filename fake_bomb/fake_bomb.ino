// Code designed for Arduino Uno R3

#include <LiquidCrystal.h>

// Connections to the circuit: LCD screen
const int LCD_RS_PIN = 12;
const int LCD_ENABLE_PIN = 11;
const int LCD_DATA_PIN_D4 = 5;
const int LCD_DATA_PIN_D5 = 4;
const int LCD_DATA_PIN_D6 = 3;
const int LCD_DATA_PIN_D7 = 2;
const int LCD_BACKLIGHT_RED = 8;
const int LCD_BACKLIGHT_GREEN = 9;
const int LCD_BACKLIGHT_BLUE = 10;

LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_DATA_PIN_D4, LCD_DATA_PIN_D5, LCD_DATA_PIN_D6, LCD_DATA_PIN_D7);

const int LCD_ROWS = 2;
const int LCD_COLS = 16;

const int TOTAL_WIRES = 6;
const int CUTTABLE_WIRES[TOTAL_WIRES] = {A0, A1, A2, A3, A4, A5};
const bool WIRES_TO_CUT[TOTAL_WIRES] = { 0, 1, 1, 0, 1, 0 };

const unsigned long TOTAL_TIME = 3600 * 1000L;

bool wireStates[TOTAL_WIRES];
byte currentColor = 0;
unsigned long elapsedTime, remainingTime;

void setDefaultBG() {
  if (currentColor == 0) return;
  digitalWrite(LCD_BACKLIGHT_RED, 0);
  digitalWrite(LCD_BACKLIGHT_GREEN, 0);
  digitalWrite(LCD_BACKLIGHT_BLUE, 0);
  currentColor = 0;
}

void setRedBG() {
  if (currentColor == 1) return;
  digitalWrite(LCD_BACKLIGHT_RED, 0);
  digitalWrite(LCD_BACKLIGHT_GREEN, 1);
  digitalWrite(LCD_BACKLIGHT_BLUE, 1);
  currentColor = 1;
}

void setGreenBG() {
  if (currentColor == 2) return;
  digitalWrite(LCD_BACKLIGHT_RED, 1);
  digitalWrite(LCD_BACKLIGHT_GREEN, 0);
  digitalWrite(LCD_BACKLIGHT_BLUE, 1);
  currentColor = 2;
}

void formatTime(unsigned long t, char str[13]) {
  unsigned long totalSeconds = t / 1000;
  unsigned long ms = t % 1000;
  unsigned long h = totalSeconds / 3600;
  unsigned long m = (totalSeconds - 3600*h) / 60;
  unsigned long s = totalSeconds - 3600*h - 60*m;
  snprintf(str, 13, "%02lu:%02lu:%02lu.%03lu", h, m, s, ms);
}

void setup() {
  Serial.begin(9600); // for debug
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LCD_BACKLIGHT_RED, OUTPUT);
  pinMode(LCD_BACKLIGHT_GREEN, OUTPUT);
  pinMode(LCD_BACKLIGHT_BLUE, OUTPUT);

  for (int i=0; i<TOTAL_WIRES; i++) {
    pinMode(CUTTABLE_WIRES[i], INPUT_PULLUP);
    wireStates[i] = digitalRead(CUTTABLE_WIRES[i]);
  }

  setDefaultBG();

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
  lcd.print("Ready");

  Serial.println("Ready");
}

int detectWireStateChange() {
  for (int i=0; i<TOTAL_WIRES; i++) {
    int newValue = digitalRead(CUTTABLE_WIRES[i]);
    if (newValue != wireStates[i]) {
      wireStates[i] = newValue;
      return i;
    }
  }
  return -1;
}

void displayCurrentState() {
  lcd.setCursor(5, 1);
  int missingWires = 0;
  for (int i=0; i<TOTAL_WIRES; i++) {
    if (WIRES_TO_CUT[i]) {
      if (wireStates[i]) {
        // Wire was correctly cut
        lcd.print("*");
      } else {
        missingWires++;
      }
    }
  }

  // This is just to erase previously shown asterisks
  for (int i=0; i<missingWires; i++) {
    lcd.print(" ");
  }
}

void displayTimer() {
  char s[13];
  formatTime(remainingTime, s);
  lcd.home();
  lcd.print(s);
}

bool isIncorrectWriteCut() {
  for (int i=0; i<TOTAL_WIRES; i++) {
    if (wireStates[i] == 1 && WIRES_TO_CUT[i] == 0) {
      return true;
    }
  }
  return false;
}

bool areAllCorrectWiresCut() {
  for (int i=0; i<TOTAL_WIRES; i++) {
    if (wireStates[i] == 0 && WIRES_TO_CUT[i] == 1) {
      return false;
    }
  }
  return true;
}

void handleWireStateChange(int wireWithNewState) {
  Serial.print("Wire ");
  Serial.print(wireWithNewState);
  if (wireStates[wireWithNewState]) {
    Serial.print(" was cut");
    if (WIRES_TO_CUT[wireWithNewState]) {
      Serial.println(" => correct");
      setGreenBG();
      delay(200);
    } else {
      Serial.println(" => INCORRECT");
    }
  } else {
    Serial.println(" was reconnected");
  }
  
  if (isIncorrectWriteCut()) {
    setRedBG();
  } else {
    setDefaultBG();
  }
}

void loop() {
  elapsedTime = millis();
  if (elapsedTime < TOTAL_TIME) {
    remainingTime = TOTAL_TIME - elapsedTime;
  } else {
    remainingTime = 0;
    setRedBG();
    lcd.clear();
    lcd.write("*** BOOM ***");
    while (true);
  }

  displayTimer();

  int wireWithNewState = detectWireStateChange();
  if (wireWithNewState >= 0) {
    handleWireStateChange(wireWithNewState);
  }

  displayCurrentState();

  if (areAllCorrectWiresCut() && !isIncorrectWriteCut()) {
    // Win
    setGreenBG();
    lcd.clear();
    lcd.write("BOMB DEFUSED");
    lcd.setCursor(0, 1);
    lcd.write("CODE = 1234");
    while (true);
  }
}
