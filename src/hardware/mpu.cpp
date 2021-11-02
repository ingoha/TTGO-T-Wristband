#include "mpu.hpp"
#include "hal.hpp"
#include "pins.hpp"

// cf. https://learn.sparkfun.com/tutorials/9dof-razor-imu-m0-hookup-guide#using-the-mpu-9250-dmp-arduino-library

MPU::MPU(TwoWire* wire)
{
  imu = new Mpu9250(wire, MPU_ADDR);

  if(!imu->Begin())  
  {
    while(true)
    {
      // Failed to initialize MPU-9250, loop forever
      // FIXME add timeout...
      Serial.println("[MPU] MPU9250 init failed!");
    }
  }
  Serial.println("MPU9250 is online...");
}


void MPU::update() {
  imu->Read();
}

void MPU::sleep()
{
  /*
  IMU->writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 5, false);
  IMU->writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 6, false);
  IMU->writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 4, false);
  IMU->writeByte(MPU9250_ADDRESS, PWR_MGMT_2, 0x07);
  IMU->writeByte(MPU9250_ADDRESS, ACCEL_CONFIG2, 0x09);
  IMU->writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x40);
  IMU->writeBit(MPU9250_ADDRESS, MOT_DETECT_CTRL, 7, true);
  IMU->writeBit(MPU9250_ADDRESS, MOT_DETECT_CTRL, 6, true);
  IMU->writeByte(MPU9250_ADDRESS, WOM_THR, 220);
  IMU->writeBit(MPU9250_ADDRESS, INT_PIN_CFG, 5, 0);
  IMU->writeByte(MPU9250_ADDRESS, INT_STATUS, 0);
  IMU->writeByte(MPU9250_ADDRESS, LP_ACCEL_ODR, 0x01);
  IMU->writeBit(MPU9250_ADDRESS, PWR_MGMT_1, 5, true);
  */
}

const int16_t MPU::getBearing()
{
  float magX = imu->mag_x_ut(); // magX is x-axis magnetic field in uT
  float magY = imu->mag_y_ut(); // magY is y-axis magnetic field in uT
  int16_t bearing = atan2(magY, magX);
  return (bearing > 0 ? bearing : (2 * PI + bearing)) * 360 / (2 * PI);
}

int MPU::calibrateBearing()
{
}

void MPU::calibrate() { 
  //IMU->calibrateMPU9250(gBias, aBias);
}

const float MPU::getTemperature()
{
  return imu->die_temperature_c();
}


/*
void MPU::updateDMP() {
    if (imud->fifoAvailable()) {
        if (imud->dmpUpdateFifo() == INV_SUCCESS) {
            imud->computeEulerAngles();
        }
    }
}
*/

void MPU::getDMP(float *q) {
  /*
    q[4] = imud->yaw;
    q[5] = imud->pitch;
    q[6] = imud->roll;
    */
}

const float MPU::getYaw() {
  return imu->gyro_x_radps();;
}

const float MPU::getRoll() {
  return imu->gyro_y_radps();
}

const float MPU::getPitch() {
  return imu->gyro_z_radps();
}

/*
void MPU::sleep() {
    //IMU->setSleepEnabled(true);
}
*/