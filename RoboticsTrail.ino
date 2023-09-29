#include <Wire.h>
#include <Servo.h>
Servo servoMotor;
float ax, ay, az;
float pitch;
float angleX, angleY;
float pointTo = 0;
float angle = 0;

void setup() {
  Serial.begin(9600); // To print data in the console
  Wire.begin(); // To initialize the Wire Library

  Wire.beginTransmission(0x68); // 0x68 is the address of the mpu
  Wire.write(0x6B); // Used to send data
  Wire.write(0); // Make a reset
  Wire.endTransmission();

  servoMotor.attach(9); // The pin the data is sent from
  servoMotor.write(pointTo); // Initialize the angle of the rotor (points upward)
}

void loop() {
  angle = Serial.read(); // reads the value given and stores it in angle
  delay(100); // Wait for the response (if one)
  Serial.print(pointTo);
  if (angle != -1.00) // enter if statement only when a value is given
    pointTo = angle; // puts the angle given in pointTo

  Wire.beginTransmission(0x68);
  Wire.write(0x3B); // To say that we need data from Accelerator of mpu
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true); // Request data from mpu
  ax = (Wire.read() << 8 | Wire.read()) / 16384.0; // Divide by 16384.0 to get -+2g range
  ay = (Wire.read() << 8 | Wire.read()) / 16384.0;
  az = (Wire.read() << 8 | Wire.read()) / 16384.0;

  angleX = atan2(-ay, az) * RAD_TO_DEG; // The angle the mpu has rolled
  if (ax < 0) { // Only 0 to 180 degrees
    pitch = atan2(ax, ay) * RAD_TO_DEG; // The angle the mpu has pivoted
    pitch = pitch - pointTo; // change the pitch to point where wanted
    angleY = -pitch; // Make it positive
  }

  Serial.print(" New Data: ");
  Serial.print("X: ");
  Serial.print(angleX);
  Serial.print(", Y: ");
  Serial.print(angleY);
  Serial.print("\n");
  
  servoMotor.write(angleY); // Turn the motor to the angle
}
