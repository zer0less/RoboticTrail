#include <Wire.h>
#include <Servo.h>


Servo servoMotor;
float ax, ay, az;
float pitch;
float angleX, angleY = 0, angleZ;

void setup() {

  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0); // Make a reset
  Wire.endTransmission();

  servoMotor.attach(9);
  servoMotor.write(0);
  delay(10);
}

void loop() {

  Wire.beginTransmission(0x68);
  Wire.write(0x3B); // Accelerator Data
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);
  ax = (Wire.read() << 8 | Wire.read()) / 16384.0; // Divide by 16384.0 to get -+2g range
  ay = (Wire.read() << 8 | Wire.read()) / 16384.0;
  az = (Wire.read() << 8 | Wire.read()) / 16384.0;

  angleX = atan2(-ay, az) * RAD_TO_DEG;
  // Quaters are inversed
  if (ax < 0) { // Q1 & Q2 works
    pitch = atan2(ax, ay) * RAD_TO_DEG; //sqrt(ay * ay + az * az)
    angleY = -pitch;
  } else if (ax > 0) { // 
    pitch = atan2(ax, ay) * RAD_TO_DEG; //sqrt(ay * ay + az * az)
    pitch = map(pitch, 0, 180, -180, 0); // Angle from -180 to 0 instead of 0 to 180
    angleY = - 180 - pitch;
  }

  Serial.print(" New Data: ");
  Serial.print("X: ");
  Serial.print(angleX);
  Serial.print(", Y: ");
  Serial.print(angleY);
  Serial.print("\n");
  
  servoMotor.write(angleY);
  

  // Gyroscope
  /*
  elapseTime = millis() - startTime;
  Wire.beginTransmission(0x68);
  Wire.write(0x43); // Gyroscope data
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);
  gx = (Wire.read() << 8 | Wire.read()) / 131.0; // Divide by 131.0 to get a 250deg/s range
  gy = (Wire.read() << 8 | Wire.read()) / 131.0; 
  gz = (Wire.read() << 8 | Wire.read()) / 131.0;

  gx = gx * elapseTime;
  gy = gy * elapseTime;
  gz = gz * elapseTime;
  */
  
  delay(1000);
}
