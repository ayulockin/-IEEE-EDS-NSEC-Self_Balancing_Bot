// MPU6050_read
// Read raw values from 14 registers

int MPU6050_read(int start, uint8_t *buffer, int size)
{
  int i, n, err;
  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
    return (-10);
    
  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
    return (n);

  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);

  return (0);  // no err
}

// MPU6050_write
// Parameters:
//   start : Start address, use a define for the register
//   pData : A pointer to the data to write.
//   size  : The number of bytes to write.

int MPU6050_write(int start, const uint8_t *pData, int size)
{
  int n, err;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);

  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);

  err = Wire.endTransmission(true); // release the I2C-bus
  if (err != 0)
    return (err);

  return (0);         // return : no err
}

// MPU6050_write_reg 

int MPU6050_write_reg(int reg, uint8_t data)
{
  int err;

  err = MPU6050_write(reg, &data, 1);

  return (err);
}
