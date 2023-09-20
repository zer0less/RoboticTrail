#include <Wire.h>
#include <Servo.h>


Servo servoMotor;
int potPin = A0;
int servoPos = 0;
int pos = 0;
float ax, ay, az, gx, gy, gz;
float axAngle, ayAngle, azAngle, gxAngle, gyAngle, gzAngle;
float elapseTime, startTime;
float roll, pitch;

void setup() {

  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0); // Make a reset
  Wire.endTransmission();

  servoMotor.attach(9);
  servoMotor.write(servoPos);
  delay(100);
}

void loop() {
  startTime = millis();
  // Accelerator
  Wire.beginTransmission(0x68);
  Wire.write(0x3B); // Accelerator Data
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);
  ax = (Wire.read() << 8 | Wire.read()) / 16384.0; // Divide by 16384.0 to get -+2g range
  ay = (Wire.read() << 8 | Wire.read()) / 16384.0;
  az = (Wire.read() << 8 | Wire.read()) / 16384.0;

  roll = atan2(-ay, az) * RAD_TO_DEG;

  if (ay > 0) {
    pitch = atan2(-ax, sqrt(ay * ay + az * az)) * RAD_TO_DEG;
    servoPos = map(pitch, -90, 90, 0, 180); // Angle from 0 to 180 instead of -90 to 90
    servoPos = constrain(servoPos, 0, 180);
  } else if (ay < 0) {
    pitch = atan2(ax, sqrt(ay * ay + az * az)) * RAD_TO_DEG;
    servoPos = map(pitch, -90, 90, 0, -180); // Angle from 0 to -180 instead of -90 to 90
    servoPos = constrain(servoPos, 180, 0);
  }
  
  servoMotor.write(servoPos);

  // Gyroscope
  /*
  elapseTime = millis() - firstTime;
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyroscope data
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);
  gx = (Wire.read() << 8 | Wire.read()) / 131.0; // Divide by 131.0 to get a 250deg/s range
  gy = (Wire.read() << 8 | Wire.read()) / 131.0; 
  gz = (Wire.read() << 8 | Wire.read()) / 131.0;

  gxAngle = gx * elapseTime;
  gyAngle = gy * elapseTime;
  gzAngle = gz * elapseTime;
  */

  // Print out
  Serial.print("New Data: ");
  Serial.print(ax);
  Serial.print(" / ");
  Serial.print(ay);
  Serial.print(" / ");
  Serial.print(az);
  Serial.print("\n");
  
  /*
  // Calculate the servo position based on the tilt angle
  servoPos = map(value, 0, 1023, 0, 180);
  
  // Limit the servo position to prevent extreme angles
  servoPos = constrain(servoPos, 0, 180);
  
  // Set the servo position
  servoMotor.write(servoPos);
  Serial.print("Done.\n");
  
  // Delay for a short time to avoid rapid servo movements
  */
  delay(100);
}
