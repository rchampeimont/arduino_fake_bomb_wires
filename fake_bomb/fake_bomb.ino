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

const unsigned long TOTAL_TIME = 3600 * 1000L;

byte currentColor = 0;

void red() {
  if (currentColor == 1) return;
  digitalWrite(LCD_BACKLIGHT_RED, 0);
  digitalWrite(LCD_BACKLIGHT_GREEN, 1);
  digitalWrite(LCD_BACKLIGHT_BLUE, 1);
  currentColor = 1;
}

void blue() {
  if (currentColor == 0) return;
  digitalWrite(LCD_BACKLIGHT_RED, 0);
  digitalWrite(LCD_BACKLIGHT_GREEN, 0);
  digitalWrite(LCD_BACKLIGHT_BLUE, 0);
  currentColor = 0;
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
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LCD_BACKLIGHT_RED, OUTPUT);
  pinMode(LCD_BACKLIGHT_GREEN, OUTPUT);
  pinMode(LCD_BACKLIGHT_BLUE, OUTPUT);

  for (int i=0; i<TOTAL_WIRES; i++) {
    pinMode(CUTTABLE_WIRES[i], INPUT_PULLUP);
  }
  
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
  lcd.print("Ready");

  blue();
}

// Show wire state
void debugWires() {
  lcd.setCursor(LCD_COLS - TOTAL_WIRES, 1);
  for (int i=0; i<TOTAL_WIRES; i++) {
    lcd.write(digitalRead(CUTTABLE_WIRES[i]) ? '1' : '0');
  }
}

void loop() {
  char s[13];
  unsigned long elapsedTime, remainingTime;
  
  elapsedTime = millis();
  if (elapsedTime < TOTAL_TIME) {
    remainingTime = TOTAL_TIME - elapsedTime;
  } else {
    remainingTime = 0;
    red();
  }

  formatTime(remainingTime, s);
  lcd.home();
  lcd.print(s);

  debugWires(); // Show wire state
}
