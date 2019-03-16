#include <Wire.h>
#include <MPU6050.h>
 
#define M1pin1 8
#define M1pin2 9
#define M2pin1 10
#define M2pin2 11

MPU6050 sensor;
int16_t ax, ay, az;
int16_t gx, gy, gz;
 
int M1speed;
int M2speed;

void setup ( )
{
  Wire.begin( );
  Serial.begin (9600);
  Serial.println ("Initializing the sensor");
  sensor.initialize ( );
  Serial.println(sensor.testConnection( ) ? "Successfully Connected" : "Connection failed");
  delay(1000);
  Serial.println("Taking Values from the sensor");
  delay(1000);
}

void loop ( )
{
  sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax = map(ax, -17000, 17000, 0,255 );
  M1speed = ax;
  M2speed = ax;
  
  Serial.print ("Motor1 Speed = ");
  Serial.print (M1speed, DEC);
  Serial.print (" && ");
  Serial.print ("Motor2 Speed = ");
  Serial.println (M2speed, DEC);
   
  if(ax>128)
  {analogWrite (M1pin1, M1speed);
  analogWrite (M2pin1, M2speed);
  }
  else
  {
    analogWrite (M1pin2,M1speed);
    analogWrite (M2pin2,M2speed);
  }
}
