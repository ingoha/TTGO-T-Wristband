#include "mpu.hpp"
#include "quaternionFilters.hpp"

MPU9250 IMU;

float aBias[3]; float gBias[3]; float mCal[3];
uint32_t lastUpdate = 0;

void initMPU()
{
  byte c = IMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  if (c == 0x71)
  {
    IMU.initMPU9250();
    IMU.initAK8963(IMU.magCalibration);
    getMagBiasEEPROM(IMU.magbias);
  }
}

void updateMPU() {
    int16_t aData[3];
    IMU.getAres(); IMU.readAccelData(aData);
    float ax = (float)aData[0] * IMU.aRes - aBias[0];
    float ay = (float)aData[1] * IMU.aRes - aBias[1];
    float az = (float)aData[2] * IMU.aRes - aBias[2];
    int16_t gData[3];
    IMU.getGres(); IMU.readGyroData(gData);
    float gx = (float)gData[0] * IMU.gRes;
    float gy = (float)gData[1] * IMU.gRes;
    float gz = (float)gData[2] * IMU.gRes;
    int16_t mData[3];
    IMU.getMres(); IMU.readMagData(mData);
    float mx = (float)mData[0] * IMU.mRes * IMU.magCalibration[0] - IMU.magbias[0];
    float my = (float)mData[1] * IMU.mRes * IMU.magCalibration[1] - IMU.magbias[1];
    float mz = (float)mData[2] * IMU.mRes * IMU.magCalibration[2] - IMU.magbias[2];
    uint32_t now = micros();
    float deltat = ((now - lastUpdate) / 1000000.0f);
    lastUpdate = now;
    // MadgwickQuaternionUpdate(-ax, ay, az, gx*PI/180.0f, -gy*PI/180.0f, -gz*PI/180.0f,  my,  -mx, mz, deltat);
    MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f,  my,  mx, -mz, deltat);
    //
}

const float *getQuaternion() { return getQ(); }

void mpuSleep()
{
  IMU.writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 5, false);
  IMU.writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 6, false);
  IMU.writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 4, false);
  IMU.writeByte(MPU9250_ADDRESS, PWR_MGMT_2, 0x07);
  IMU.writeByte(MPU9250_ADDRESS, ACCEL_CONFIG2, 0x09);
  IMU.writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x40);
  IMU.writeBit(MPU9250_ADDRESS, MOT_DETECT_CTRL, 7, true);
  IMU.writeBit(MPU9250_ADDRESS, MOT_DETECT_CTRL, 6, true);
  IMU.writeByte(MPU9250_ADDRESS, WOM_THR, 220);
  IMU.writeBit(MPU9250_ADDRESS, INT_PIN_CFG, 5, 0);
  IMU.writeByte(MPU9250_ADDRESS, INT_STATUS, 0);
  IMU.writeByte(MPU9250_ADDRESS, LP_ACCEL_ODR, 0x01);
  IMU.writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 5, true);
}

int16_t getBearing()
{
  IMU.readMagData(IMU.magCount); // Read the x/y/z adc values
  IMU.getMres();
  IMU.mx = (float)IMU.magCount[0] * IMU.mRes * IMU.magCalibration[0] -
           IMU.magbias[0];
  IMU.my = (float)IMU.magCount[1] * IMU.mRes * IMU.magCalibration[1] -
           IMU.magbias[1];
  IMU.mz = (float)IMU.magCount[2] * IMU.mRes * IMU.magCalibration[2] -
           IMU.magbias[2];
  float bearing = atan2(IMU.my, IMU.mx);
  return (bearing > 0 ? bearing : (2 * PI + bearing)) * 360 / (2 * PI);
}

int calibrateBearing()
{
  uint16_t ii = 0, sample_count = 1500;
  int32_t mag_bias[3] = {0, 0, 0};
  int16_t mag_max[3] = {-32767, -32767, -32767}, mag_min[3] = {32767, 32767, 32767}, mag_temp[3] = {0, 0, 0};
  sleep(4);
  for (ii = 0; ii < sample_count; ii++)
  {
    IMU.readMagData(mag_temp); // Read the mag data
    for (int jj = 0; jj < 3; jj++)
    {
      if (mag_temp[jj] > mag_max[jj])
        mag_max[jj] = mag_temp[jj];
      if (mag_temp[jj] < mag_min[jj])
        mag_min[jj] = mag_temp[jj];
    }
    delay(12);                                                              // at 100 Hz ODR, new mag data is available every 10 ms
    mag_bias[0] = (mag_max[0] + mag_min[0]) / 2;                            // get average x mag bias in counts
    mag_bias[1] = (mag_max[1] + mag_min[1]) / 2;                            // get average y mag bias in counts
    mag_bias[2] = (mag_max[2] + mag_min[2]) / 2;                            // get average z mag bias in counts
    IMU.magbias[0] = (float)mag_bias[0] * IMU.mRes * IMU.magCalibration[0]; // save mag biases in G for main program
    IMU.magbias[1] = (float)mag_bias[1] * IMU.mRes * IMU.magCalibration[1];
    IMU.magbias[2] = (float)mag_bias[2] * IMU.mRes * IMU.magCalibration[2];
  }
  storeMagBiasEEPROM(IMU.magbias);
  return 1;
}

void calibrateMPU() { IMU.calibrateMPU9250(gBias, aBias); }

float getTemperature()
{
  int tempCount = IMU.readTempData();
  float temperature = ((float)tempCount) / 333.87 + 21.0;
  return temperature;
}
