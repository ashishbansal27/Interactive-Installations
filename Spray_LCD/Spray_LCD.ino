 #include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Servo.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //for LCD keypad
Servo myservo;
int pos = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

char rangeholder[6];



int sensitivityValueAdd = 0;
int sensitivityValue = EEPROM.read(sensitivityValueAdd);


int count = 0;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(11);
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.setCursor(1, 0);

  sprintf(rangeholder, "%02d", sensitivityValue);

  lcd.print("Range");
  delay(1000);

  lcd.setCursor(1, 1);
  lcd.print(rangeholder);
  lcd.print(" ");
  lcd.print("CM");



  lcd.blink();
  delay(3000);
  lcd.clear();

}

void loop() {


  if (count == 0) {

    sprintf(rangeholder, "%02d", sensitivityValue);


    lcd.setCursor(0, 0);
    lcd.print("SensorRange:");
    lcd.print(rangeholder);
    lcd.print("CM");
    lcd.setCursor(0, 1);
    lcd.print("SELECT to CHANGE");   //change this portion to display desired text, value of saved distance is stored in variable : "sensitivityValue"
    int sensorValue = analogRead(A1);
    Serial.println(sensorValue);   //sensorValue holds the live values from Sharp Sensor

    int sensitivityValueinAnalog = get_analogValue(sensitivityValue);

    if (sensorValue > sensitivityValueinAnalog)  //earlier it was >360
    { //spray
      Serial.println("  SPRAY!!  ");

      for (pos = 0; pos <= 70; pos += 1) {
        myservo.write(pos);
        delay(2);
      }
      delay (100);


      for (pos = 70; pos >= 0; pos -= 1) {
        // in steps of 1 degree
        myservo.write(pos);
        delay(3);
      }
      delay (1000);
    }



  }

  if (count == 1) {
    // put your main code here, to run repeatedly:
    if (getLCDButton() == btnUP) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("New Range");
      lcd.setCursor(0, 1);


      sensitivityValue = sensitivityValue + 1;

      if (sensitivityValue > 20) {
        sensitivityValue = 20;       //change this to change value capped at 20
      }

      sprintf(rangeholder, "%02d", sensitivityValue);
      lcd.print(rangeholder);
      lcd.print(" ");
      lcd.print("CM");
      while (getLCDButton() == btnUP);    //replace while with delay(500); to make the functionality of longpress to increment or decrement. Right now one press to increase/decrease by 1.
    }


    if (getLCDButton() == btnDOWN) {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("New Range");
      lcd.setCursor(0, 1);


      sensitivityValue = sensitivityValue - 1;

      if (sensitivityValue < 5) {
        sensitivityValue = 5;   //change this to change value capped at 5
      }
      sprintf(rangeholder, "%02d", sensitivityValue);
      lcd.print(rangeholder);
      lcd.print(" ");
      lcd.print("CM");
      while (getLCDButton() == btnDOWN);
    }
  }




  if (getLCDButton() == btnSELECT && count == 0) {
    lcd.clear();
    lcd.setCursor(1, 0);

    count = 1;



    lcd.print("Set Range");
    lcd.setCursor(0, 1);
    lcd.print("Press UP or DOWN");

    while (getLCDButton() == btnSELECT);
  }


  if (getLCDButton() == btnSELECT && count == 1) {
    lcd.clear();
    lcd.setCursor(1, 0);

    EEPROM.update(sensitivityValueAdd, sensitivityValue);


    lcd.print("Range Saved!");  //This is not coming in one line. Can be split into two lines. by changing lcd.setCursor();
    delay(1000);
    count = 0;
    lcd.clear();
    while (getLCDButton() == btnSELECT);
  }
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

int get_analogValue (uint16_t Distancevalue) {

  return round((2076.0 / Distancevalue) + 11);   //found this formula on Sharp data sheet corresponding to our sensor model. Verified by keeping a ruler. occasional slight error is there but is acceptable.
}

