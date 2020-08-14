#include "mpu.hpp"
#include "quaternionFilters.hpp"

MPU9250 IMU;
MPU9250_DMP imud; 

float aBias[3]; float gBias[3]; float mCal[3];
uint32_t lastUpdate = 0;

void initMPU()
{
  byte c = IMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  if (c == 0x71) {
    // IMU.initMPU9250();
    // IMU.initAK8963(IMU.magCalibration);
    // getMagBiasEEPROM(IMU.magbias);
    imud.begin();
    imud.dmpBegin(DMP_FEATURE_PEDOMETER | DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL);
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

void gagewatchRead(float *q) {
  if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01) {
    IMU.readAccelData(IMU.accelCount);  // Read the x/y/z adc values
    IMU.getAres();
    // This depends on scale being set
    IMU.ax = (float)IMU.accelCount[0] * IMU.aRes; // - accelBias[0];
    IMU.ay = (float)IMU.accelCount[1] * IMU.aRes; // - accelBias[1];
    IMU.az = (float)IMU.accelCount[2] * IMU.aRes; // - accelBias[2];
    IMU.readGyroData(IMU.gyroCount);  // Read the x/y/z adc values
    IMU.getGres();
    IMU.gx = (float)IMU.gyroCount[0] * IMU.gRes;
    IMU.gy = (float)IMU.gyroCount[1] * IMU.gRes;
    IMU.gz = (float)IMU.gyroCount[2] * IMU.gRes;
    IMU.readMagData(IMU.magCount);  // Read the x/y/z adc values
    IMU.getMres();
    // User environmental x-axis correction in milliGauss, should be automatically calculated
    IMU.magbias[0] = +470.;
    IMU.magbias[1] = +120.;
    IMU.magbias[2] = +125.;

    IMU.mx = (float)IMU.magCount[0] * IMU.mRes * IMU.magCalibration[0] -
             IMU.magbias[0];
    IMU.my = (float)IMU.magCount[1] * IMU.mRes * IMU.magCalibration[1] -
             IMU.magbias[1];
    IMU.mz = (float)IMU.magCount[2] * IMU.mRes * IMU.magCalibration[2] -
             IMU.magbias[2];
  } // if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)

  IMU.updateTime();
  //  MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f,  my,  mx, mz);
  MahonyQuaternionUpdate(IMU.ax, IMU.ay, IMU.az, IMU.gx * DEG_TO_RAD,
                         IMU.gy * DEG_TO_RAD, IMU.gz * DEG_TO_RAD, IMU.my,
                         IMU.mx, IMU.mz, IMU.deltat);
  // Serial print and/or display at 0.5 s rate independent of data rates
  IMU.delt_t = millis() - IMU.count;

  if (IMU.delt_t > 20) {
    if (false) {
      Serial.print("ax = ");
      Serial.print((int)1000 * IMU.ax);
      Serial.print(" ay = ");
      Serial.print((int)1000 * IMU.ay);
      Serial.print(" az = ");
      Serial.print((int)1000 * IMU.az);
      Serial.println(" mg");

      Serial.print("gx = ");
      Serial.print( IMU.gx, 2);
      Serial.print(" gy = ");
      Serial.print( IMU.gy, 2);
      Serial.print(" gz = ");
      Serial.print( IMU.gz, 2);
      Serial.println(" deg/s");

      Serial.print("mx = ");
      Serial.print( (int)IMU.mx );
      Serial.print(" my = ");
      Serial.print( (int)IMU.my );
      Serial.print(" mz = ");
      Serial.print( (int)IMU.mz );
      Serial.println(" mG");
    }

      q[0] = (*getQ()); // q0
      q[1] = (*(getQ() + 1)); // qx
      q[2] = (*(getQ() + 2)); // qy
      q[3] = (*(getQ() + 3)); // qz

    IMU.yaw   = atan2(2.0f * (*(getQ() + 1) * *(getQ() + 2) + *getQ() *
                              *(getQ() + 3)), *getQ() * *getQ() + * (getQ() + 1) * *(getQ() + 1)
                      - * (getQ() + 2) * *(getQ() + 2) - * (getQ() + 3) * *(getQ() + 3));
    IMU.pitch = -asin(2.0f * (*(getQ() + 1) * *(getQ() + 3) - *getQ() *
                              *(getQ() + 2)));
    IMU.roll  = atan2(2.0f * (*getQ() * *(getQ() + 1) + * (getQ() + 2) *
                              *(getQ() + 3)), *getQ() * *getQ() - * (getQ() + 1) * *(getQ() + 1)
                      - * (getQ() + 2) * *(getQ() + 2) + * (getQ() + 3) * *(getQ() + 3));
    IMU.pitch *= RAD_TO_DEG;
    IMU.yaw   *= RAD_TO_DEG;
    // Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
    //  8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
    // - http://www.ngdc.noaa.gov/geomag-web/#declination
    IMU.yaw   -= 8.5;
    IMU.roll  *= RAD_TO_DEG;

  /*
    q[4] = IMU.yaw;
    q[5] = IMU.pitch;
    q[6] = IMU.roll;
  */
    q[4] = imud.yaw;
    q[5] = imud.pitch;
    q[6] = imud.roll;

    IMU.count = millis();
    IMU.sumCount = 0;
    IMU.sum = 0;

  } // if (IMU.delt_t > 20)
}

void updateDMP() {
    if (imud.fifoAvailable()) {
        if (imud.dmpUpdateFifo() == INV_SUCCESS) {
            imud.computeEulerAngles();
        }
    }
}

void getDMP(float *q) {
    q[4] = imud.yaw;
    q[5] = imud.pitch;
    q[6] = imud.roll;
}