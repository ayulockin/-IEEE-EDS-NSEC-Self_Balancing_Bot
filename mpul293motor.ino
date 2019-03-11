#include <Wire.h>
#include<I2Cdev.h>
#include <MPU6050.h>
 
#define first_motor_pin1 8
#define first_motor_pin2 9
#define second_motor_pin1 10
#define second_motor_pin2 11

MPU6050 sensor;
int16_t accelerometer_x, accelerometer_y, accelerometer_z; 
int16_t gyro_x, gyro_y, gyro_z; 
 
int first_motor_speed;
int second_motor_speed;

void setup ( )
{
  Wire.begin( );
  Serial.begin (9600);
  Serial.println ("Initializing the sensor");
  sensor.initialize ( );
  Serial.println("Taking Values from the sensor");
  delay(1000);
}

void loop ( )
{
  sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  Serial.print ("Motor1 Speed = ");
  Serial.print (first_motor_speed, DEC);
  Serial.print (" && ");
  Serial.print ("Motor2 Speed = ");
  Serial.println (second_motor_speed, DEC);
    digitalWrite(inputPin1, HIGH);
    digitalWrite(inputPin2, LOW);
    digitalWrite(inputPin3, HIGH);
    digitalWrite(inputPin4, LOW); 
  delay (200);
