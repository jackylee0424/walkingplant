// Walking Plant
// is a robotic prosthesis that amplify plant's survival instincts and 
// add social emotions when interacting with people.
// firmware 0.0.1
// ref: http://www.adafruit.com/products/1404
// This sketch is a learning system for a single leg mechanism (M1 toe, M2 ankle, M3)

#include <Servo.h>
#include <EEPROM.h>

#define CALIB_MAX_M1 512
#define CALIB_MIN_M1 100

#define CALIB_MAX_M2 512
#define CALIB_MIN_M2 100

#define CALIB_MAX_M3 512
#define CALIB_MIN_M3 100

#define CALIB_MAX_M4 512
#define CALIB_MIN_M4 100

#define SAMPLE_DELAY 25 // in ms, 50ms seems good

uint8_t recordButtonPin = 12;
uint8_t playButtonPin = 7;
uint8_t ledPin = 13;

uint8_t servoPinM1 = 5;
uint8_t feedbackPinM1 = A0;

uint8_t servoPinM2 = 6;
uint8_t feedbackPinM2 = A1;

uint8_t servoPinM3 = 10;
uint8_t feedbackPinM3 = A2;

uint8_t servoPinM4 = 11;
uint8_t feedbackPinM4 = A3;

Servo m1Servo;
Servo m2Servo;
Servo m3Servo;
Servo m4Servo;

  
void setup() {
  Serial.begin(9600);
  
  // record button
  pinMode(recordButtonPin, INPUT);
  digitalWrite(recordButtonPin, HIGH);
  
  // play button
  pinMode(playButtonPin, INPUT);
  digitalWrite(playButtonPin, HIGH);
  
  // LED
  pinMode(ledPin, OUTPUT);
  
  // feedback pins
  pinMode(feedbackPinM1, INPUT); 
  pinMode(feedbackPinM2, INPUT); 
  pinMode(feedbackPinM3, INPUT); 
  
  Serial.println("Servo RecordPlay");
  
  // need to do manual calibration
}

// main loop
void loop() {
  
  //Serial.println(analogRead(A5));
  //delay(10);
  
   // record new sequence
   if (! digitalRead(recordButtonPin)) {
     delay(10);
     // wait for released
     while (! digitalRead(recordButtonPin));
     delay(20);
     // OK released!
     recordServo();
   }
 
   // play recorded sequence
    if (! digitalRead(playButtonPin)) {
     delay(10);
     // wait for released
     while (! digitalRead(playButtonPin));
     delay(20);
     // OK released!
     playServo();
   }
}

void playServo() {
  uint16_t addr = 0;
  Serial.println("Playing");

  m1Servo.attach(servoPinM1);
  m2Servo.attach(servoPinM2);
  m3Servo.attach(servoPinM3);
  m4Servo.attach(servoPinM4);
  
  while (digitalRead(playButtonPin)) {    
    uint8_t x = EEPROM.read(addr);
    if ((addr % 4) == 0){
      Serial.print("Read M1: "); Serial.print(x);
      if (x == 255) break;
      // map to 0-180 degrees
      x = map(x, 0, 254, 0, 180);
      Serial.print(" -> "); Serial.println(x);
      m1Servo.write(x);
    }
    if ((addr % 4) == 1){
      Serial.print("Read M2: "); Serial.print(x);
      if (x == 255) break;
      // map to 0-180 degrees
      x = map(x, 0, 254, 0, 180);
      Serial.print(" -> "); Serial.println(x);
      m2Servo.write(x);
    }
    if ((addr % 4) == 2){
      Serial.print("Read M3: "); Serial.print(x);
      if (x == 255) break;
      // map to 0-180 degrees
      x = map(x, 0, 254, 0, 180);
      Serial.print(" -> "); Serial.println(x);
      m3Servo.write(x);
    }
    if ((addr % 4) == 3){
      Serial.print("Read M4: "); Serial.print(x);
      if (x == 255) break;
      // map to 0-180 degrees
      x = map(x, 0, 254, 0, 180);
      Serial.print(" -> "); Serial.println(x);
      m4Servo.write(x);
    }
    delay(SAMPLE_DELAY);
    
    addr++;
    if (addr == 512) break;
  }
  Serial.println("Done");
  m1Servo.detach();
  m2Servo.detach();
  m3Servo.detach();
  m4Servo.detach();
  delay(250);  
}

// record from analog inputs
void recordServo() {
  uint16_t addr = 0;
  
  // turn on LED
  Serial.println("Recording");
  digitalWrite(ledPin, HIGH);
  
  // check if button pressed (as stop recording)
  while (digitalRead(recordButtonPin)) {
     uint16_t a1 = analogRead(feedbackPinM1);
     if ((addr % 4) == 0){
       Serial.print("Read M1 analog: "); Serial.print(a1);
       if (a1 < CALIB_MIN_M1) a1 = CALIB_MIN_M1;
       if (a1 > CALIB_MAX_M1) a1 = CALIB_MAX_M1;
       a1 = map(a1, CALIB_MIN_M1, CALIB_MAX_M1, 0, 254);
       Serial.print(" -> "); Serial.println(a1);
       EEPROM.write(addr, a1);
     }
     uint16_t a2 = analogRead(feedbackPinM2);
     if ((addr % 4) == 1){
       Serial.print("Read M2 analog: "); Serial.print(a2);
       if (a2 < CALIB_MIN_M2) a2 = CALIB_MIN_M2;
       if (a2 > CALIB_MAX_M2) a2 = CALIB_MAX_M2;
       a2 = map(a2, CALIB_MIN_M2, CALIB_MAX_M2, 0, 254);
       Serial.print(" -> "); Serial.println(a2);
       EEPROM.write(addr, a2);
     }
     uint16_t a3 = analogRead(feedbackPinM3);
     if ((addr % 4) == 2){
       Serial.print("Read M3 analog: "); Serial.print(a2);
       if (a3 < CALIB_MIN_M3) a3 = CALIB_MIN_M3;
       if (a3 > CALIB_MAX_M3) a3 = CALIB_MAX_M3;
       a3 = map(a3, CALIB_MIN_M3, CALIB_MAX_M3, 0, 254);
       Serial.print(" -> "); Serial.println(a3);
       EEPROM.write(addr, a3);
     }
     uint16_t a4 = analogRead(feedbackPinM4);
     if ((addr % 4) == 3){
       Serial.print("Read M4 analog: "); Serial.print(a2);
       if (a4 < CALIB_MIN_M4) a4 = CALIB_MIN_M4;
       if (a4 > CALIB_MAX_M4) a4 = CALIB_MAX_M4;
       a4 = map(a4, CALIB_MIN_M4, CALIB_MAX_M4, 0, 254);
       Serial.print(" -> "); Serial.println(a4);
       EEPROM.write(addr, a4);
     }
     
     addr++;
     if (addr == 512) break;
     delay(SAMPLE_DELAY);
  }
  if (addr != 512) EEPROM.write(addr, 255);

  digitalWrite(ledPin, LOW);

  Serial.println("Done");
  delay(250);
  
  // note: think about a way to store the motion data externally
  //       make a buffer where it can be loaded dynamically from host computer (Raspi)
}


