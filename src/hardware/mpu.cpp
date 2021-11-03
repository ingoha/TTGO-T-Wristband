#include "mpu.hpp"
#include "hal.hpp"
#include "pins.hpp"

//
// FIXME:
//  * calibrate magnetometer and store offsets in EEPROM (cf. https://github.com/asukiaaa/MPU9250_asukiaaa/blob/master/examples/GetMagOffset/GetMagOffset.ino)
//  * sleep (incl. INT wakeup)

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

  setupFilter();
}

void MPU::setupFilter()
{
  filter = new Madgwick();
  filter->begin(sampleRateHz);
  // configuration for AHRS filter
  imu->ConfigAccelRange(Mpu9250::ACCEL_RANGE_2G);
  imu->ConfigGyroRange(Mpu9250::GYRO_RANGE_250DPS);

  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / sampleRateHz;
  microsPrevious = micros();
}

void MPU::updateFilter()
{
  if(!filterEnabled)
    return;

  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  unsigned long microsNow;

  // check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {

    // read data from MP9250
    update();

    ax = imu->accel_x_mps2();
    ay = imu->accel_y_mps2();
    az = imu->accel_z_mps2();
    gx = imu->gyro_x_radps();
    gy = imu->gyro_y_radps();
    gz = imu->gyro_z_radps();

    // update the filter, which computes orientation
    filter->updateIMU(gx, gy, gz, ax, ay, az);

    // print the heading, pitch and roll
    roll = filter->getRoll();
    pitch = filter->getPitch();
    heading = filter->getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);

    // increment previous time, so we keep proper pace
   
    microsPrevious = microsPrevious + microsPerReading;
  }
}

void MPU::startFilter() { filterEnabled = true; }

void MPU::stopFilter() { filterEnabled = false; }

const bool MPU::filterRunning() { return filterEnabled; }

void MPU::update() { imu->Read(); }

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

const float MPU::getTemperature() { return imu->die_temperature_c(); }

const float MPU::getYaw() { return filter->getYaw(); }

const float MPU::getRoll() { return filter->getRoll(); }

const float MPU::getPitch() { return filter->getPitch(); }

const MPU::xyz MPU::getMag() {
  xyz mag;
  mag.x = imu->mag_x_ut();
  mag.y = imu->mag_y_ut();
  mag.z = imu->mag_z_ut();
  return mag;
}

const MPU::xyz MPU::getGyro() {
  xyz gyro;
  gyro.x = imu->gyro_x_radps();
  gyro.y = imu->gyro_y_radps();
  gyro.z = imu->gyro_z_radps();
  return gyro;
}

const MPU::xyz MPU::getAccel() {
  xyz accel;
  accel.x = imu->accel_x_mps2();
  accel.y = imu->accel_y_mps2();
  accel.z = imu->accel_z_mps2();
  return accel;
}

/*
void MPU::sleep() {
    //IMU->setSleepEnabled(true);
}
*/