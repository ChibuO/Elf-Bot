// LSM303AGR driver for Microbit_v2
//
// Initializes sensor and communicates over I2C
// Capable of reading temperature, acceleration, and magnetic field strength

#include <stdbool.h>
#include <stdint.h>

#include "lsm303agr.h"
#include "nrf_delay.h"

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

// Helper function to perform a 1-byte I2C read of a given register
//
// i2c_addr - address of the device to read from
// reg_addr - address of the register within the device to read
//
// returns 8-bit read value
static uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    //TODO: implement me
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);

  return rx_buf;
}

// Helper function to perform a 1-byte I2C write of a given register
//
// i2c_addr - address of the device to write to
// reg_addr - address of the register within the device to write
static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  //TODO: implement me
  //Note: there should only be a single two-byte transfer to be performed
  uint8_t tx_buf[2] = { reg_addr, data };
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    //TODO: implement me
    NRF_TWI_MNGR_WRITE(i2c_addr, tx_buf, 2, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);
}

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void lsm303agr_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;

  // ---Initialize Accelerometer---

  // Reboot acclerometer
  i2c_reg_write(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_CTRL_REG5, 0x80);
  nrf_delay_ms(100); // needs delay to wait for reboot

  // Enable Block Data Update
  // Only updates sensor data when both halves of the data has been read
  i2c_reg_write(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_CTRL_REG4, 0x80);

  // Configure accelerometer at 100Hz, normal mode (10-bit)
  // Enable x, y and z axes
  i2c_reg_write(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_CTRL_REG1, 0x57);

  // Read WHO AM I register
  // Always returns the same value if working
  uint8_t result = i2c_reg_read(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_WHO_AM_I_REG);
  //TODO: check the result of the Accelerometer WHO AM I register
  printf("WHOAMI_ACC = 0x%x\n", result);

  // ---Initialize Magnetometer---

  // Reboot magnetometer
  i2c_reg_write(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_CFG_REG_A, 0x40);
  nrf_delay_ms(100); // needs delay to wait for reboot

  // Enable Block Data Update
  // Only updates sensor data when both halves of the data has been read
  i2c_reg_write(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_CFG_REG_C, 0x10);

  // Configure magnetometer at 100Hz, continuous mode
  i2c_reg_write(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_CFG_REG_A, 0x0C);

  // Read WHO AM I register
  result = i2c_reg_read(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_WHO_AM_I_REG);
  //TODO: check the result of the Magnetometer WHO AM I register
  printf("WHOAMI_MAG = 0x%x\n", result);

  // ---Initialize Temperature---

  // Enable temperature sensor
  i2c_reg_write(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_TEMP_CFG_REG, 0xC0);
}

// Read the internal temperature sensor
//
// Return measurement as floating point value in degrees C
float lsm303agr_read_temperature(void) {
  //TODO: implement me
  uint8_t temp_h = i2c_reg_read(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_TEMP_H);
  uint8_t temp_l = i2c_reg_read(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_TEMP_L);

  int16_t raw_temp = (temp_h << 8 | temp_l);
  float temp = raw_temp * (1.0/256.0) + 25.0;

  return temp;
}

lsm303agr_measurement_t lsm303agr_read_accelerometer(void) {
  //TODO: implement me
  uint8_t x_h = i2c_reg_read(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_OUT_X_H);
  uint8_t x_l = i2c_reg_read(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_OUT_X_L);
  int16_t raw_x = (x_h << 8 | x_l);
  raw_x = raw_x >> 6;
  
  uint8_t y_h = i2c_reg_read(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_OUT_Y_H);
  uint8_t y_l = i2c_reg_read(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_OUT_Y_L);
  int16_t raw_y = (y_h << 8 | y_l);
  raw_y = raw_y >> 6;

  uint8_t z_h = i2c_reg_read(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_OUT_Z_H);
  uint8_t z_l = i2c_reg_read(LSM303AGR_ACC_ADDRESS, LSM303AGR_ACC_OUT_Z_L);
  int16_t raw_z = (z_h << 8 | z_l);
  raw_z = raw_z >> 6;


  lsm303agr_measurement_t measurement = {
    .x_axis = raw_x * 0.0039,
    .y_axis = raw_y * 0.0039,
    .z_axis = raw_z * 0.0039,
  };
  return measurement;
}

lsm303agr_measurement_t lsm303agr_read_magnetometer(void) {
  //TODO: implement me

  uint8_t x_h = i2c_reg_read(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_OUT_X_H_REG);
  uint8_t x_l = i2c_reg_read(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_OUT_X_L_REG);
  int16_t raw_x = (x_h << 8 | x_l);
  
  uint8_t y_h = i2c_reg_read(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_OUT_Y_H_REG);
  uint8_t y_l = i2c_reg_read(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_OUT_Y_L_REG);
  int16_t raw_y = (y_h << 8 | y_l);

  uint8_t z_h = i2c_reg_read(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_OUT_Z_H_REG);
  uint8_t z_l = i2c_reg_read(LSM303AGR_MAG_ADDRESS, LSM303AGR_MAG_OUT_Z_L_REG);
  int16_t raw_z = (z_h << 8 | z_l);


  lsm303agr_measurement_t measurement = {
    .x_axis = raw_x * 0.15,
    .y_axis = raw_y * 0.15,
    .z_axis = raw_z * 0.15,
  };
  return measurement;
}

