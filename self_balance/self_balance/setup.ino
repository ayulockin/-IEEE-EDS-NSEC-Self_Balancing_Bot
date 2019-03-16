void setup()
{      
  int err;
  uint8_t c;

  Serial.begin(19200);
  // Initialize the 'Wire' class for the I2C-bus.
  Wire.begin();
  
  // default at power-up:
  err = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);

  // According to the datasheet, the 'sleep' bit
  // should read a '1'. But I read a '0'.
  // That bit has to be cleared, since the sensor
  // is in sleep mode at power-up. Even if the
  // bit reads '0'.
  err = MPU6050_read (MPU6050_PWR_MGMT_2, &c, 1);
  
  // Clear the 'sleep' bit to start the sensor.
  MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);
  
  //Initialize the angles
  calibrate_sensors();  
  set_last_read_angle_data(millis(), 0, 0, 0, 0, 0, 0);

  // Setting motor pinouts as Output
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);
}

