#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 46

int toucanPin = 49;

char track1[5] = "1";
char track2[5] = "2";
char track3[5] = "3";
char track4[5] = "4";
char track5[5] = "5";
char track6[5] = "6";
char track7[5] = "7";
char track8[5] = "8";
char track9[5] = "9";
char trackStop[5] = "s";
unsigned long musiccurrentMillis = millis();
unsigned long musicpreviousMillis = 0;
int musicCount = 1;

int waittime = 10; //change this value to change duration of prominent scene.
int ledr = 4;
int ledb = 6;
int ledg = 5;
int ledr1 = 7;
int ledb1 = 9;
int ledg1 = 8;

int sunr = 11;
int sunr1 = 13;
int sung = 12;
int sung1 = 10;

int fan1 = 53; //fan2 is also connected to this pin
int fan3 = 40; //fan4 is also connected to this pin

int fish1 = 45;
int fish3 = 3;
int fish2 = 2;
int fish4 = 42;
int count = 0;
int fishRotationCount;
unsigned long previousMillis = 0;
static unsigned long fishtimer = millis();
static unsigned long toucanOntime = 0;
static unsigned long fish0to1time = 0;

int i = 2500;
int u = 10000;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

int color = 0;
int suncolor = 0;
static unsigned long timer = millis();
static unsigned long sun = millis();
int b = 0;
int r = 255;
int g = 0;
int q = 2000;

int z = 0; //sun red value
int x = 0; //sun green value

int k = 0; //delay time for ambient light
int m = 30000; //time for sun rise and set


void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

void setup() {




  pinMode(fan1, OUTPUT);
  pinMode(fan3, OUTPUT);

  pinMode(fish1, OUTPUT);
  pinMode(fish3, OUTPUT);
  pinMode(fish2, OUTPUT);
  pinMode(fish4, OUTPUT);

  digitalWrite(fish1, LOW);
  digitalWrite(fish2, LOW);
  digitalWrite(fish3, LOW);
  digitalWrite(fish4, LOW);

  Serial.begin(115200);
  Serial3.begin(115200);
  for (int j = 0; j <= 13; j++)
  {
    pinMode(j, OUTPUT);
  }

  digitalWrite(sunr, LOW);
  digitalWrite(sunr1, LOW);
  digitalWrite(sung, LOW);
  digitalWrite(sung1, LOW);


#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  b = abs(b);
  r = abs(r);
  g = abs(g);
  z = abs(z);
  x = abs(x);

  analogWrite(ledb, b);
  analogWrite(ledb1, b);
  analogWrite(ledr, r);
  analogWrite(ledr1, r);
  analogWrite(ledg, g);
  analogWrite(ledg1, g);

  analogWrite(sunr, z);
  analogWrite(sunr1, z);
  analogWrite(sung, x);
  analogWrite(sung1, x);


  Serial.print("sunColor:");
  Serial.print(suncolor);



  if (millis() >= sun + q) {
    suncolor++;
    sun = millis();
  }


  if (suncolor == 0)
  { if (z < 75)
      z++;
    if (x < 10)
      x++;
  }

  if (suncolor == 2)
  { if (z < 150)
      z++;
    if (x < 20)
      x++;

  }

  if (suncolor == 3)
  { if (z < 255)
      z++;

    if (x < 100)
      x++;
  }


  if (suncolor == 33)
  { if (z > 75) {
      z--;
    }

    if (x > 10) {
      x--;
    }
  }


  if (suncolor == 34) {
    if (z < 150) {
      z++;
    }

    if (x < 20) {
      x++;
    }
  }

  if (suncolor == 35) {
    if (z < 255) {
      z++;
    }

    if (x > 0) {
      x--;
    }
  }

  if (suncolor == 36) {
    if (z > 0) {
      z--;
    }

    if (x > 0) {
      x--;
    }
  }



  unsigned long currentMillis = millis();
  int sensorvalue = analogRead(A4);
  int sensorvalue1 = analogRead(A3);


  if (fishRotationCount == 10) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 2000 ) {

      previousMillis = currentMillis;

      fishRotationCount = 1;

    }

  }


  if (fishRotationCount == 1) {
    digitalWrite(fish1, HIGH);
    digitalWrite(fish2, HIGH);
    digitalWrite(fish4, LOW);
    digitalWrite(fish3, LOW);

    currentMillis = millis();
    if (currentMillis - previousMillis >= 300) {

      previousMillis = currentMillis;

      fishRotationCount++;

    }
  }





  if (fishRotationCount == 2)
  {
    if (sensorvalue > 300)
    { digitalWrite(fish1, LOW);
      digitalWrite(fish2, LOW);
      digitalWrite(fish3, LOW);
      digitalWrite(fish4, LOW);
      fishRotationCount++;
    }

    currentMillis = millis();
    previousMillis = currentMillis;
  }






  if (fishRotationCount == 3)
  { digitalWrite(fish3, HIGH);
    digitalWrite(fish4, HIGH);
    digitalWrite(fish1, LOW);
    digitalWrite(fish2, LOW);

    currentMillis = millis();
    if (currentMillis - previousMillis >= 300) {
      previousMillis = currentMillis;
      fishRotationCount++;
    }



  }


  if (fishRotationCount == 4) {
    if (sensorvalue1 > 300)
    { digitalWrite(fish1, LOW);
      digitalWrite(fish2, LOW);
      digitalWrite(fish3, LOW);
      digitalWrite(fish4, LOW);

      fishRotationCount++;
    }

    currentMillis = millis();
    previousMillis = currentMillis;
  }

  if (fishRotationCount == 5) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= 300) {

      previousMillis = currentMillis;

      //fishRotationCount=0;
    }
  }

  Serial.print("fishRotationCount");
  Serial.print(fishRotationCount);


  //}

  if (millis() >= timer + 2000) {
    color++;
    timer = millis();
  }

  if (color == 31 + waittime)
  {
    theaterChase(strip.Color(0, 0, 0), 200);
  }

  if (color == 0) {
    r = 255;
    g = 0;
    b = 0;
    delay(k);
  }

  if (color == 1) {
    r = 255;
    g = 0;
    b = 0;
    delay(k);
  }

  if (color == 2) {
    if (r > 200) {
      r--;
    }
    if (g < 30) {
      g++;
    }
    delay(k);
  }

  if (color == 3) {

    if (r >= 115)
    {
      r--;
    }

    if (g <= 115)
    {
      g++;
    }

    if (b <= 115)
    {
      b++;
    }
    Serial.println("coral");
    delay(k);
    // coral

  }


  if (color == 6) {
    digitalWrite(toucanPin, HIGH);
  }

  if (color == 14) {
    digitalWrite(toucanPin, LOW);
  }

  if (color == 20) {
    fishRotationCount = 10;
    color = 21;
  }

  if (color == 25) {
    digitalWrite(fan1, HIGH);
    digitalWrite(fan3, HIGH);
  }




  if (color == 31) {
    digitalWrite(fan1, LOW);
    digitalWrite(fan3, LOW);
  }

  if (color == 23 + waittime) {
    //dark red
    if (r > 50)
    {
      r--;
    }

    if (g > 0)
    {
      g--;
    }

    if (b > 0)
    {
      b--;
    }

    Serial.println("dark red");
    delay(k);
  }


  if (color == 24 + waittime) {
    //light red
    if (r < 255)
    {
      r++;
    }

    if (g > 0)
    {
      g--;
    }

    if (b > 0)
    {
      b--;
    }

    Serial.println("light red");
    delay(k);
  }


  if (color == 25 + waittime) {
    //purple
    if (r <= 128)
    {
      r++;
    }

    if (g > 0)
    {
      g--;
    }

    if (b <= 128)
    {
      b++;
    }

    Serial.println("purple");
    delay(k);
  }

  if (color == 26 + waittime) {
    //fuchsia
    if (r < 255)
    {
      r++;
    }

    if (g > 0)
    {
      g--;
    }

    if (b < 255)
    {
      b++;
    }

    Serial.println("fuchsia");
    delay(k);
  }

  if (color == 27 + waittime) {
    //indigo
    if (r >= 75)
    {
      r--;
    }

    if (g > 0)
    {
      g--;
    }

    if (b >= 130)
    {
      b--;
    }

    Serial.println("indigo");
    delay(k);
  }

  if (color == 28 + waittime) {
    //light indigo
    if (r >= 51)
    {
      r--;
    }

    if (g > 0)
    {
      g--;
    }

    if (b >= 100)
    {
      b--;
    }

    Serial.println("indigo");
    delay(k);
  }
  if (color == 29 + waittime) {
    if (r > 0)
    {
      r--;
    }

    g = 0;

    if (b > 0)
    {
      b--;
    }
    //dark
    strip.show();
    Serial.println("dark");
    delay(k);
  }

  if (color == 30 + waittime) {
    theaterChase(strip.Color(150, 255, 0), 200); // Blue
  }

  //      if (color==31+waittime){
  //   theaterChase(strip.Color(150, 255, 0), 200); // Blue
  //    }






  Serial.println(r);
  Serial.println(g);
  Serial.println(b);



  if (musicCount == 1) {
    Serial3.write(track8, 1);
    musicCount++;
  }






  if (musicCount == 2) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 12000) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 3) {
    Serial3.write(trackStop, 1);
    musicCount++;
  }



  if (musicCount == 4) {
    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 300) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }


  }

  if (musicCount == 5) {
    Serial3.write(track5, 1);
    musicCount++;
  }
  if (musicCount == 6) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 16000) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 7) {
    Serial3.write(trackStop, 1);
    musicCount++;
  }

  if (musicCount == 8) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 300) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 9) {
    Serial3.write(track4, 1);
    musicCount++;
  }


  if (musicCount == 10) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 9000) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 11) {
    Serial3.write(trackStop, 1);
    musicCount++;
  }


  if (musicCount == 12) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 300) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 13) {
    Serial3.write(track2, 1);
    musicCount++;
  }


  if (musicCount == 14) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 5000) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 15) {
    Serial3.write(trackStop, 1);
    musicCount++;
  }


  if (musicCount == 16) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 300) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 17) {
    Serial3.write(track7, 1);
    musicCount++;
  }


  if (musicCount == 18) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 20000) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 19) {
    Serial3.write(trackStop, 1);
    musicCount++;
  }

  if (musicCount == 20) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 300) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 21) {
    Serial3.write(track9, 1);
    musicCount++;
  }


  if (musicCount == 22) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 12000) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 23) {
    Serial3.write(trackStop, 1);
    musicCount++;
  }

  if (musicCount == 24) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 300) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 25) {
    Serial3.write(track6, 1);
    musicCount++;
  }

  if (musicCount == 26) {

    musiccurrentMillis = millis();
    if (musiccurrentMillis - musicpreviousMillis >= 10000) {
      // put your main code here, to run repeatedly:
      musicpreviousMillis = musiccurrentMillis;
      musicCount++;
    }



  }

  if (musicCount == 27) {
    Serial3.write(trackStop, 1);
    musicCount++;
  }





}

