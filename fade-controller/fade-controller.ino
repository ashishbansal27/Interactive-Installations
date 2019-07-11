
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <math.h>

#define writeDefaults 0

// Button Feedbacks
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Current control modes
#define ctrlNONE      -1
#define ctrlPeriod    0
#define ctrlMaxBright 1
#define ctrlMinBright 2

// EEPROM addresses for values
#define addrMinBright 0
#define addrMaxBright 1
#define addrPeriod    2

// LED strip pins
#define led1  3
#define led2  11

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// minimum and maximum brightness values
// (in percent)
float min_brightness = 0.0;
float max_brightness = 1.0;

// fade-in/fade-out time
// (in seconds)
long fade_time = 600;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("PLEASE WAIT...");
  lcd.setCursor(0, 1);
  lcd.print("READ FRM MEMORY");
  delay(1000);

  int exitLoop = 0;
  int currentControl = ctrlNONE;

  if (!writeDefaults) {
    min_brightness = EEPROM.read(addrMinBright) / 100.0;
    max_brightness = EEPROM.read(addrMaxBright) / 100.0;
    fade_time = EEPROMReadlong(addrPeriod);
  } else {
    exitLoop = 1;
    EEPROMWriteDefaults();
  }

  while (!exitLoop) {
    int btn = getLCDButton();

    if (millis() > 60000)
      exitLoop = 1;

    if (btn != btnNONE) {
      switch (btn) {
        case btnRIGHT:
          switch (currentControl) {
            case ctrlNONE:
              currentControl = ctrlPeriod;
              break;
            case ctrlPeriod:
              currentControl = ctrlMaxBright;
              break;
            case ctrlMaxBright:
              currentControl = ctrlMinBright;
              break;
            case ctrlMinBright:
              currentControl = ctrlPeriod;
              break;
          }
          break;
        case btnLEFT:
          switch (currentControl) {
            case ctrlNONE:
              currentControl = ctrlMinBright;
              break;
            case ctrlPeriod:
              currentControl = ctrlMinBright;
              break;
            case ctrlMaxBright:
              currentControl = ctrlPeriod;
              break;
            case ctrlMinBright:
              currentControl = ctrlMaxBright;
              break;
          }
          break;
        case btnUP:
          switch (currentControl) {
            case ctrlPeriod:
              fade_time = constrain(fade_time + 5, 0, 20000);
              break;
            case ctrlMaxBright:
              max_brightness = constrain(max_brightness + 0.01, min_brightness, 1.0);
              break;
            case ctrlMinBright:
              min_brightness = constrain(min_brightness + 0.01, 0.0, max_brightness);
              break;
          }
          break;
        case btnDOWN:
          switch (currentControl) {
            case ctrlPeriod:
              fade_time = constrain(fade_time - 5, 0, 20000);
              break;
            case ctrlMaxBright:
              max_brightness = constrain(max_brightness - 0.01, min_brightness, 1.0);
              break;
            case ctrlMinBright:
              min_brightness = constrain(min_brightness - 0.01, 0.0, max_brightness);
              break;
          }
          break;
        case btnSELECT:
          exitLoop = 1;
          break;
      }
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    switch (currentControl) {
      case ctrlNONE:
        analogWrite(led1, 0);
        analogWrite(led2, 0);
        lcd.print("SELECT CTRL MODE");
        lcd.setCursor(0, 1);
        lcd.print("   use < or >");
        break;
      case ctrlPeriod:
        analogWrite(led1, 0);
        analogWrite(led2, 0);
        lcd.print("ctrl: DELAY");
        lcd.setCursor(0, 1);
        lcd.print(String(int(fade_time), DEC) + "s");
        break;
      case ctrlMaxBright:
        analogWrite(led1, max_brightness * 255);
        analogWrite(led2, max_brightness * 255);
        lcd.print("ctrl: MAX BRIGHT");
        lcd.setCursor(0, 1);
        lcd.print(String(int(max_brightness * 100), DEC) + "%");
        break;
      case ctrlMinBright:
        analogWrite(led1, min_brightness * 255);
        analogWrite(led2, min_brightness * 255);
        lcd.print("ctrl: MIN BRIGHT");
        lcd.setCursor(0, 1);
        lcd.print(String(int(min_brightness * 100), DEC) + "%");
    }

    delay(200);

  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PLEASE WAIT...");
  lcd.setCursor(0, 1);
  lcd.print("WRITE TO MEMORY");

  EEPROM.update(addrMinBright, int(min_brightness * 100));
  EEPROM.update(addrMaxBright, int(max_brightness * 100));
  EEPROMWritelong(addrPeriod, fade_time);
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DONE!");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FROM " +
            String(int(min_brightness * 100), DEC) +
            "% TO " +
            String(int(max_brightness * 100), DEC) +
            "%");
  lcd.setCursor(0, 1);
  lcd.print("DELAY " +
            String(int(fade_time), DEC) + "s");

}

void loop() {
  for (long i = 0; i < fade_time * 1000; i++) {
    double t = 1.5 * PI + (double(i) / double(fade_time * 1000)) * 2 * PI;
    //    Serial.println(t);
    double out_range = (max_brightness - min_brightness) * 255;
    double out = min_brightness * 255 + pulse(t) * out_range;

    analogWrite(led1, out);
    analogWrite(led2, out);
    delay(1);
  }
}

float breathe(float x) {
  return (exp(sin(x - PI)) - exp(sin(PI/2.0)))/(exp(sin(1.5*PI)) - exp(sin(0.5*PI)));
}

float pulse(float x) {
  return (sin(x) + 1) / 2.0;
}

void EEPROMWriteDefaults() {
  EEPROM.write(addrMinBright, int(min_brightness * 100));
  EEPROM.write(addrMaxBright, int(max_brightness * 100));
  EEPROMWritelong(addrPeriod, fade_time);
}

int getLCDButton() {
  int adc_key_in = analogRead(0);

  /*
     The button values are centered on:
     0000 - right
     0100 - up
     0256 - down
     0410 - left
     0640 - select
     1023 - none
  */

  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;

  return btnNONE;
}

void EEPROMWritelong(int address, long value)
{
  // Decomposition from a long to 4 bytes by using bitshift.
  byte p4 = (value & 0xFF);
  byte p3 = ((value >> 8) & 0xFF);
  byte p2 = ((value >> 16) & 0xFF);
  byte p1 = ((value >> 24) & 0xFF);

  EEPROM.update(address + 0, p4);
  EEPROM.update(address + 1, p3);
  EEPROM.update(address + 2, p2);
  EEPROM.update(address + 3, p1);
}

long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long p4 = EEPROM.read(address);
  long p3 = EEPROM.read(address + 1);
  long p2 = EEPROM.read(address + 2);
  long p1 = EEPROM.read(address + 3);

  // Return the recomposed long by using bitshift.
  return ((p4 << 0) & 0xFF) +
         ((p3 << 8) & 0xFFFF) +
         ((p2 << 16) & 0xFFFFFF) +
         ((p1 << 24) & 0xFFFFFFFF);
}
