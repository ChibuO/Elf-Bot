// LSM303AGR driver for Microbit_v2
//
// Initializes sensor and communicates over I2C
// Capable of reading temperature, acceleration, and magnetic field strength

#include <stdbool.h>
#include <stdint.h>

#include "I2C_sensor_example.h"
#include "nrf_delay.h"
#include "math.h"


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

void invert() {
  i2c_reg_write(0x59, 0x12, 1);
}

void down() {
  //left 8448 : 0x2100
  //i2c address 89 : 0x59
  //right: 8192 : 0x2000
  //forward: 0: 0x8000
  //reverse: 1 : 0x0000
  //power on: 28673 : 0x70
  //power off: 28672 
  //left_invert: 0x13
  //right_invert : 0x12
  //make left motor mowve forward
  uint16_t pwr = (uint16_t)floor(30 * 127 / 100);
  uint16_t pwr2 = (uint16_t)floor(29 * 127 / 100);
  printf("pwr: %u\n", pwr);
  uint16_t dir = pwr | 0x80;
  uint16_t dir2 = pwr2 | 0x80;
  uint16_t rev_dir = 127 - pwr;
  printf("rev_dir: %u\n", rev_dir);
  printf("dir: %u\n", dir);
  i2c_reg_write(0x59, 0x21, dir2); //make left motor move forward
  printf("read: %u\n", i2c_reg_read(0x59, 0x21));
  i2c_reg_write(0x59, 0x20, dir); //make right motor move forward
  invert();
}

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void lsm303agr_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;

  i2c_reg_write(0x59, 0x70, 1); //power on motors
  down();
  nrf_delay_ms(5000);
  i2c_reg_write(0x59, 0x70, 0); //power off motors
}

