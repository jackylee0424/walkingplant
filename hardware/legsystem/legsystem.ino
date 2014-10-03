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

#define SAMPLE_DELAY 25 // in ms, 50ms seems good

uint8_t recordButtonPin = 12;
uint8_t playButtonPin = 7;
uint8_t ledPin = 13;

uint8_t servoPinM1 = 9;
uint8_t feedbackPinM1 = A0;

uint8_t servoPinM2 = 10;
uint8_t feedbackPinM2 = A1;

uint8_t servoPinM3 = 11;
uint8_t feedbackPinM3 = A2;

Servo m1Servo;
Servo m2Servo;
Servo m3Servo;

/*
// Calibration values
int minDegrees;
int maxDegrees;
int minFeedback;
int maxFeedback;
int tolerance = 2; // max feedback measurement error

*/
/*
  This function establishes the feedback values for 2 positions of the servo.
  With this information, we can interpolate feedback values for intermediate positions
*/
/*
void calibrate(Servo servo, int analogPin, int minPos, int maxPos)
{
  // Move to the minimum position and record the feedback value
  servo.write(minPos);
  minDegrees = minPos;
  delay(2000); // make sure it has time to get there and settle
  minFeedback = analogRead(analogPin);
  
  // Move to the maximum position and record the feedback value
  servo.write(maxPos);
  maxDegrees = maxPos;
  delay(2000); // make sure it has time to get there and settle
  maxFeedback = analogRead(analogPin);
}
*/

  
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
  
  // need to do calibration
  //m1Servo.attach(servoM1Pin);
  //calibrate(m1Servo, feedbackM1Pin, 20, 160);  // calibrate for the 20-160 degree range
  //m1Servo.detach();
}

// main loop
void loop() {
  
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
  while (digitalRead(playButtonPin)) {    
    uint8_t x = EEPROM.read(addr);
    if ((addr % 3) == 0){
      Serial.print("Read M1: "); Serial.print(x);
      if (x == 255) break;
      // map to 0-180 degrees
      x = map(x, 0, 254, 0, 180);
      Serial.print(" -> "); Serial.println(x);
      m1Servo.write(x);
    }
    delay(SAMPLE_DELAY);
    
    addr++;
    if (addr == 512) break;
  }
  Serial.println("Done");
  m1Servo.detach();
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
     uint16_t a = analogRead(feedbackPinM1);
     if ((addr % 3) == 0){
       Serial.print("Read M1 analog: "); Serial.print(a);
       if (a < CALIB_MIN_M1) a = CALIB_MIN_M1;
       if (a > CALIB_MAX_M1) a = CALIB_MAX_M1;
       a = map(a, CALIB_MIN_M1, CALIB_MAX_M1, 0, 254);
       Serial.print(" -> "); Serial.println(a);
       EEPROM.write(addr, a);
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

/*
void Seek(Servo servo, int analogPin, int pos)
{
  // Start the move...
  servo.write(pos);
  
  // Calculate the target feedback value for the final position
  int target = map(pos, minDegrees, maxDegrees, minFeedback, maxFeedback); 
  
  // Wait until it reaches the target
  while(abs(analogRead(analogPin) - target) > tolerance){} // wait...
}

int getPos(int analogPin)
{
  return map(analogRead(analogPin), minFeedback, maxFeedback, minDegrees, maxDegrees);
}
*/
