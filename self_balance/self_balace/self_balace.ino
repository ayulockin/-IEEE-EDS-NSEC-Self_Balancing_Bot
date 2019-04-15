#include <Wire.h>

// Motor Pinouts
#define lm1 3 //left M1 
#define lm2 5 //left M2
#define rm1 6 //right M1
#define rm2 9 //right M2

// Register map
#define MPU6050_ACCEL_XOUT_H       0x3B   // R  
#define MPU6050_PWR_MGMT_1         0x6B   // R/W
#define MPU6050_PWR_MGMT_2         0x6C   // R/W
#define MPU6050_WHO_AM_I           0x75   // R

#define MPU6050_I2C_ADDRESS 0x68
//#define MPU6050_I2C_ADDRESS 0x69      // AD0 -> High

typedef union accel_t_gyro_union
{
  struct
  {
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  } reg;
  struct 
  {
    int x_accel;
    int y_accel;
    int z_accel;
    int temperature;
    int x_gyro;
    int y_gyro;
    int z_gyro;
  } value;
};

// Rotation angles
unsigned long last_read_time;
float         last_x_angle;  // filtered angle from x_axis
float         last_y_angle;  // filtered angle from x_axis
float         last_z_angle;  // filtered angle from x_axis

// Store gyro angles to compare drift
float         last_gyro_x_angle; 
float         last_gyro_y_angle;
float         last_gyro_z_angle;

void set_last_read_angle_data(unsigned long time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) {
  last_read_time = time;
  last_x_angle = x;
  last_y_angle = y;
  last_z_angle = z;
  last_gyro_x_angle = x_gyro;
  last_gyro_y_angle = y_gyro;
  last_gyro_z_angle = z_gyro;
}

inline unsigned long get_last_time() {return last_read_time;}
inline float get_last_x_angle() {return last_x_angle;}
inline float get_last_y_angle() {return last_y_angle;}
inline float get_last_z_angle() {return last_z_angle;}
inline float get_last_gyro_x_angle() {return last_gyro_x_angle;}
inline float get_last_gyro_y_angle() {return last_gyro_y_angle;}
inline float get_last_gyro_z_angle() {return last_gyro_z_angle;}

// Used by calibrate_sensor() to store offset value
float base_x_accel;
float base_y_accel;
float base_z_accel;
float base_x_gyro;
float base_y_gyro;
float base_z_gyro;

// Degrees

float angle_x;
float angle_y;
float angle_z;

// pid parameters
float error=0.0;
float prev_error=0.0;

float Kp=70.0;
float Kd=70.0;
float pd;

float lms,rms;
int bs=0;

int left_max = 250;
int right_max = 250;
int left_min = 0;
int right_min = 0;
