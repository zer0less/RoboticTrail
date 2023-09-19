#include <Wire.h>
#include <Servo.h>


Servo servoMotor;
int potPin = A0;
int servoPos = 0;
float ax, ay, az, gx, gy, gz;

void setup() {

  Serial.begin(9600);
  Wire.begin();

  servoMotor.attach(9);
  servoMotor.write(servoPos);
  delay(100);
}

void loop() {
  Serial.print("hi\n");
  Wire.requestFrom(0x68, 6, true);
  ax = Wire.read();
  ay = Wire.read();
  az = Wire.read();

  //int value = analogRead(A4);
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
  delay(20);
}
