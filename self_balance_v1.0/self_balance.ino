// Lib to read data from GY-521
#include <Wire.h>

// motor pinout
#define lm1 3 //left M1 
#define lm2 5 //left M2
#define rm1 6 //right M1
#define rm2 9 //right M2

// I2C address of MPU6050
const int mpu_addr = 0x68;

////////////////////Declaring global variables//////////////////////

// Accelerometer 
long acc_x, acc_y, acc_z, acc_total_vector;
float angle_roll_acc, angle_pitch_acc;

// Gyroscope
int gy_x, gy_y, gy_z; //for gyro data
long gy_x_cal, gy_y_cal, gy_z_cal;
bool set_gyro_angles;

// For angle
float angle_pitch, angle_roll;
int angle_pitch_buffer, angle_roll_buffer;
float angle_pitch_output, angle_roll_output;

long loop_timer;
int temp;

char tmp_str[7]; // temporary variable used in convert function

int Count=0;
int deg;

///////////////////////PID Parameters//////////////////////
int error=0;
int Kp=0;
int Kd=0;


void setup(){
  Serial.begin(115200);

  // This setup is required to wake up MPU6050
  Wire.begin();
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  // Configure Accelerometer (+/- 8g)
  Wire.begin();
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission(true);

  // Configure Gyroscope (+/- 500 dps)
  Wire.begin();
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission(true);

  // Setting motor pinouts as Output
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);

  brake();

  // To calculate offset value
  for(int i=0; i<1000; i++){
    get_mpu_data();
    gy_x_cal += gy_x;
    gy_y_cal += gy_y;
    gy_z_cal += gy_z;
    delay(4);
  }

  gy_x_cal /= 1000;
  gy_y_cal /= 1000;
  gy_z_cal /= 1000;
  
}

void loop(){
  //pid();
  calculate_angle();
  
}

void get_mpu_data(){
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x3B); // starting with register corresponding to ACCEL_XOUT_H
  Wire.endTransmission(false);
  Wire.requestFrom(mpu_addr, 7*2, true);
  
  acc_x = Wire.read()<<8 | Wire.read(); // read: 0x3B & 0x3C (ACCEL_XOUT_H & ACCEL_XOUT_L)  
  acc_y = Wire.read()<<8 | Wire.read(); // read: 0x3D & 0x3E (ACCEL_YOUT_H & ACCEL_YOUT_L)
  acc_z = Wire.read()<<8 | Wire.read(); // read: 0x3F & 0x40 (ACCEL_ZOUT_H & ACCEL_ZOUT_L)

  temp = Wire.read()<<8 | Wire.read(); // read: 0x41 & 0x42 (TEMP_OUT_H & TEMP_OUT_L)

  gy_x = Wire.read()<<8 | Wire.read(); // read: 0x43 & 0x44 (GYRO_XOUT_H & GYRO_XOUT_L)
  gy_y = Wire.read()<<8 | Wire.read(); // read: 0x45 & 0x46 (GYRO_YOUT_H & GYRO_YOUT_L)
  gy_z = Wire.read()<<8 | Wire.read(); // read: 0x47 & 0x48 (GYRO_ZOUT_H & GYRO_ZOUT_L)

  // For Debugging purpose ************************************************//
//  Serial.print("aX = "); Serial.print(convert_int16_to_str(acc_x));
//  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(acc_y));
//  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(acc_z));
//  // from documentation 
//  Serial.print(" | tmp = "); Serial.print(temp/340.00+36.53);
//  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gy_x));
//  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gy_y));
//  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gy_z));
//  Serial.println();
//
//  delay(1000);
  //***********************************************************************//
}

void calculate_angle(){
  get_mpu_data();

  // Correct raw data by subtracting offset data
  gy_x -= gy_x_cal;
  gy_y -= gy_y_cal;
  gy_z -= gy_z_cal;

  // Gyroscope angle calculation. 0.0000611 = 1 / (250Hz x 65.5)
  angle_pitch += gy_x * 0.0000611;
  angle_roll += gy_y * 0.0000611;
  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr)
  angle_pitch += angle_roll * sin(gy_z * 0.000001066);
  angle_roll -= angle_pitch * sin(gy_z * 0.000001066);

  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));  //Calculate the total accelerometer vector
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
  angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;       //Calculate the pitch angle
  angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;       //Calculate the roll angle
  
  angle_pitch_acc -= 0.0;                                              //Accelerometer calibration value for pitch
  angle_roll_acc -= 0.0;                                               //Accelerometer calibration value for roll

  if(set_gyro_angles){                                                 //If the IMU is already started
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
  }
  else{                                                                //At first start
    angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle 
    angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle 
    set_gyro_angles = true;                                            //Set the IMU started flag
  }
  
  //To dampen the pitch and roll angles a complementary filter is used
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
  Serial.print(" | Angle  = "); Serial.print(angle_pitch_output);
  Serial.print(" | Count = "); Serial.print(Count);
  Serial.print(" |loop_timer = ");Serial.println(loop_timer);
  Count++;
}

//void error_map(){
//  calculate_angle();
//  
//  if deg==0{
//    error=0;
//  }
//  else if deg>0 or deg<=5{
//    error = 0.5;
//  }
//  else if deg<0 or deg>=355{
//    error = -0.5;
//  }
//  
//}



//void pid(){
//  error_map();
//
//  pd = Kp*error + Kd*(error-prev_error);
//
//  lms = pd;
//  rms = -pd;
//
//  if(error>=0){
//    analogWrite(lm1, lms);
//    analogWrite(lm2, 0);
//    analogWrite(rm1, rms);
//    analogWrite(rm2, 0);
//  }
//  else{
//    analogWrite(lm1, 0);
//    analogWrite(lm2, lms);
//    analogWrite(rm1, 0);
//    analogWrite(rm2, rms);
//  }
//  
//}

/////////////////////////Helper Function//////////////////////////

void brake(){
  analogWrite(lm1,255);
  analogWrite(lm2,255);
  analogWrite(rm1,255);
  analogWrite(rm2,255); 
}

// converts int16 to string
char* convert_int16_to_str(int16_t i){
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}


//double  atan2 (double __y, double __x)  // arc tangent of y/x
//arx = ( 180/3.141592) * atan(ax / sqrt(square(ay, 2) + square(az, 2))); 
//ary = (180/3.141592) * atan(ay / sqrt(square(ax, 2) + square(az, 2)));
//arz = (180/3.141592) * atan(sqrt(square(ay) + square(ax)) / az);
