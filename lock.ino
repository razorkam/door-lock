

#include <Stepper.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>



int stepsPerRevolution = 200;  
const int E1 = 10;// logic control pin 1
const int E2 = 11;// logic control pin 2
const int joystickX = A1;
const int joystickY = A2;
const int joystickMiddle = 2;
const int sensorGreenIn = 3;
const int sensorWhiteOut = 4;

Stepper myStepper(stepsPerRevolution, 9, 8, 13, 12);
SoftwareSerial mySerial(sensorGreenIn,sensorWhiteOut);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int getFingerprintIDez();

void softStop()
{
   digitalWrite(E1, LOW);
   digitalWrite(E2, LOW);
}

void softStart()
{
  digitalWrite(E1, HIGH);
  digitalWrite(E2, HIGH);
}

bool middlePressed()
{
  return digitalRead(joystickMiddle) == LOW ? true : false;
}

void revolution()
{
    softStart();
    myStepper.step(2 * stepsPerRevolution);
    softStop();
    stepsPerRevolution = -stepsPerRevolution; //revert motor 
    delay(1000);  
}



void setup() {
  Serial.begin(9600);
  Serial.println("fingertest");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  myStepper.setSpeed(120);
  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
  pinMode(joystickMiddle, INPUT_PULLUP); //enable pullup resistor on joystick middle
   if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Waiting for valid finger...");
}

void loop() {

    if (middlePressed() || getFingerprintIDez() != -1)
    {
      revolution();
    }
  
   
}


// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

