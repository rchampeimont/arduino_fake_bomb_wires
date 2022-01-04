#include <LiquidCrystal.h>

// Connections to the circuit: LCD screen
const int LCD_RS_PIN = 12;
const int LCD_ENABLE_PIN = 11;
const int LCD_DATA_PIN_0 = 5;
const int LCD_DATA_PIN_1 = 4;
const int LCD_DATA_PIN_2 = 3;
const int LCD_DATA_PIN_3 = 2;
const int LCD_BACKLIGHT_RED = 8;
const int LCD_BACKLIGHT_GREEN = 9;
const int LCD_BACKLIGHT_BLUE = 10;

LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_DATA_PIN_0, LCD_DATA_PIN_1, LCD_DATA_PIN_2, LCD_DATA_PIN_3);

const int LCD_ROWS = 2;
const int LCD_COLS = 16;

const unsigned long TOTAL_TIME = 10 * 1000;

byte curentColor = 0;

void red() {
  if (curentColor == 1) return;
  analogWrite(LCD_BACKLIGHT_RED, 0);
  analogWrite(LCD_BACKLIGHT_GREEN, 255);
  analogWrite(LCD_BACKLIGHT_BLUE, 255);
  curentColor = 1;
}

void blue() {
  if (curentColor == 0) return;
  analogWrite(LCD_BACKLIGHT_RED, 0);
  analogWrite(LCD_BACKLIGHT_GREEN, 0);
  analogWrite(LCD_BACKLIGHT_BLUE, 0);
  curentColor = 0;
}

void formatTime(unsigned long t, char str[12]) {
  unsigned long totalSeconds = t / 1000;
  unsigned long ms = t % 1000;
  unsigned long h = totalSeconds / 3600;
  unsigned long m = (totalSeconds - 3600*h) / 60;
  unsigned long s = totalSeconds - 3600*h - 60*m;
  snprintf(str, 12, "%02lu:%02lu:%02lu.%03lu", h, m, s, ms);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LCD_BACKLIGHT_RED, OUTPUT);
  pinMode(LCD_BACKLIGHT_GREEN, OUTPUT);
  pinMode(LCD_BACKLIGHT_BLUE, OUTPUT);
  
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
  lcd.print("Ready");

  blue();
}

void loop() {
  char s[12];
  unsigned long time, remainingTime;
  
  time = millis();
  if (time < TOTAL_TIME) {
    remainingTime = TOTAL_TIME - time;
  } else {
    remainingTime = 0;
    red();
  }

  formatTime(remainingTime, s);
  lcd.home();
  lcd.print(s);
}
