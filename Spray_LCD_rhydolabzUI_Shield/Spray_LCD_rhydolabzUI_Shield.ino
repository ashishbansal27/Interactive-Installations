#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Servo.h>

LiquidCrystal lcd(8, 9, 13, 12, 11, 10); //for rhydolabs ui shield
Servo myservo;
int pos = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

char rangeholder[6];

int SW1 = 7, SW2 = 6, SW3 = 5, SW4 = 4, SW5 = 3;

int sensitivityValueAdd = 0;
int sensitivityValue = EEPROM.read(sensitivityValueAdd);


int count = 0;

void setup() {
  pinMode(SW1, INPUT);                  /* Initialize pin D7 as an output     */
  pinMode(SW2, INPUT);                  /* Initialize pin D6 as an output     */
  pinMode(SW3, INPUT);                  /* Initialize pin D5 as an output     */
  pinMode(SW4, INPUT);                  /* Initialize pin D4 as an output     */
  pinMode(SW5, INPUT);
  pinMode(2, OUTPUT);
  pinMode(A5, INPUT);
  
  
  // put your setup code here, to run once:
  myservo.attach(2);
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

  Serial.print(digitalRead(SW1));
  
  Serial.print(digitalRead(SW2));
  
  Serial.print(digitalRead(SW3));
  
  Serial.print(digitalRead(SW4));
  
  Serial.print(digitalRead(SW5));

  delay(1000);

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
    int sensorValue = analogRead(A5);
    Serial.println(sensorValue);   //sensorValue holds the live values from Sharp Sensor

    int sensitivityValueinAnalog = get_analogValue(sensitivityValue);

    if (sensorValue > sensitivityValueinAnalog)  //earlier it was >360
    { //spray
      Serial.println("  SPRAY!!  ");

      for (pos = 0; pos <= 50; pos += 1) {
        myservo.write(pos);
        delay(2);
      }
      delay (100);


      for (pos = 50; pos >= 0; pos -= 1) {
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

  Serial.println(getLCDButton());
}



int getLCDButton() {


  /*
     The button values are centered on:
     0000 - right
     0100 - up
     0256 - down
     0410 - left
     0640 - select
     1023 - none
  */

  // if (adc_key_in > 1000) return btnNONE;
  if (digitalRead(SW1) == LOW)    return btnRIGHT;
  if (digitalRead(SW2) == LOW)    return btnUP;
  if (digitalRead(SW4) == LOW)    return btnDOWN;
  if (digitalRead(SW5) == LOW)    return btnLEFT;
  if (digitalRead(SW3) == LOW)    return btnSELECT;

  else return btnNONE;
}

int get_analogValue (uint16_t Distancevalue) {

  return round((2076.0 / Distancevalue) + 11);   //found this formula on Sharp data sheet corresponding to our sensor model. Verified by keeping a ruler. occasional slight error is there but is acceptable.
}

