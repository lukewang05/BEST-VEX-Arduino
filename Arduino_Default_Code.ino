// Default code for Arduino setup to be run in the Arduino IDE

#include <PS4BT.h>
#include <usbhub.h>
#include <Servo.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
BTD Btd(&Usb); // Create Bluetooth Dongle instance
Servo myservo;

// Motor A connections 
int enA = 8;
int in1 = 28;
int in2 = 29;

// Motor B connections
int enB = 5;
int in3 = 32;
int in4 = 33;

// Small motor connections
int enASmall = 2;
int in1Small = 39;
int in2Small = 41;

// Hold down the PS and Share button at the same time; the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {
  Serial.begin(115200);
  #if !defined(__MIPSEL__)
  while (!Serial);
  #endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));

	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);

  pinMode(enASmall, OUTPUT);
  pinMode(in1Small, OUTPUT);
  pinMode(in2Small, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  digitalWrite(in1Small, LOW);
  digitalWrite(in2Small, LOW);

  // Set servo pin
  myservo.attach(25);

}
void loop() {
  Usb.Task();

  int motorSpeedA = 0;
  int motorSpeedB = 0;
  int motorSpeedSmall = 0;

  if (PS4.connected()) {
    int rightY = PS4.getAnalogHat(RightHatY);
    int rightX = PS4.getAnalogHat(RightHatX);
    int leftY = PS4.getAnalogHat(LeftHatY);
    int leftX = PS4.getAnalogHat(LeftHatX);

    if (rightY > 130) { // Forward driving
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      motorSpeedA = map(rightY, 130, 255, 0, 255);
      motorSpeedB = map(rightY, 130, 255, 0, 255);
    }
    else if (rightY < 124) { // Backwards driving
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      motorSpeedA = map(rightY, 124, 0, 0, 255);
      motorSpeedB = map(rightY, 124, 0, 0, 255);
    }
    else if (rightX > 130) { // Right turn
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      motorSpeedA = map(rightX, 130, 255, 0, 255);
      motorSpeedB = map(rightX, 130, 255, 0, 255);
    }
    else if (rightX < 124) { // Left turn
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      motorSpeedA = map(rightX, 124, 0, 0, 255);
      motorSpeedB = map(rightX, 124, 0, 0, 255);
    }
    else { // Stop motors when joystick returns to center
      motorSpeedA = 0;
      motorSpeedB = 0;
    }

    analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
    analogWrite(enB, motorSpeedB); // Send PWM signal to motor B


    if (leftY > 130) { // Small motor control
      digitalWrite(in1Small, HIGH);
      digitalWrite(in2Small, LOW);
      motorSpeedSmall = map(leftY, 130, 255, 0, 255);
    }
    else if (leftY < 124) {
      digitalWrite(in1Small, LOW);
      digitalWrite(in2Small, HIGH);
      motorSpeedSmall = map(leftY, 124, 0, 0, 255);
    }
    else if (leftX > 130) { // Servo control
      myservo.write(0);
    }
    else if (leftX < 124) {
      myservo.write(180);
    }
    else { // Stop small motor when joystick returns to center
      motorSpeedSmall = 0;
    }

    analogWrite(enASmall, motorSpeedSmall); // Send PWM signal to small motor

  }
}
