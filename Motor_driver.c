// LSM303AGR driver for Microbit_v2
//
// Initializes sensor and communicates over I2C
// Capable of reading temperature, acceleration, and magnetic field strength

#include <stdbool.h>
#include <stdint.h>

#include "Motor_driver.h"
#include "nrf_delay.h"
#include "math.h"

  //left 8448 : 0x2100
  //i2c address 89 : 0x59
  //right: 8192 : 0x2000
  //forward: 0: 0x8000
  //reverse: 1 : 0x0000
  //power on: 28673 : 0x70
  //power off: 28672 
  //left_invert: 0x13
  //right_invert : 0x12
#define mc_i2c_addr   0x59
#define mc_left       0x21
#define mc_right      0x20
#define mc_fwd        0x80
#define mc_rev        0x00
#define mc_pwr_on     0x70
#define mc_inv_left   0x13
#define mc_inv_right  0x12

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
  //Note: there should only be a single two-byte transfer to be performed
  uint8_t tx_buf[2] = { reg_addr, data };
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, tx_buf, 2, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);
}


void actuate_servos(int l_speed, int r_speed, bool l_forward, bool r_forward) {
  uint16_t l_pwr = (uint16_t)floor(l_speed * 127 / 100);
  uint16_t r_pwr = (uint16_t)floor(r_speed * 127 / 100);
  uint16_t l_dir = l_pwr | mc_fwd;
  uint16_t r_dir = r_pwr | mc_fwd;
  
  i2c_reg_write(mc_i2c_addr, mc_inv_left, !l_forward); // left = 0 forwards, 1 backwards
  i2c_reg_write(mc_i2c_addr, mc_inv_right, r_forward); // right = 0 backwards, 1 forwards

  i2c_reg_write(mc_i2c_addr, mc_left, l_dir); //make left motor move forward
  i2c_reg_write(mc_i2c_addr, mc_right,r_dir); //make right motor move forward
}

void activate_servos(){
  i2c_reg_write(mc_i2c_addr, mc_pwr_on, 1);
}

void deactivate_servos(){
  i2c_reg_write(mc_i2c_addr, mc_pwr_on, 0);
}

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void motor_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;
  activate_servos();
  actuate_servos(30, 30, false, false);
  nrf_delay_ms(500);
  actuate_servos(30, 30, true, true);
  nrf_delay_ms(500);
  actuate_servos(30, 30, true, false);
  nrf_delay_ms(500);
  actuate_servos(30, 30, false, true);
  nrf_delay_ms(500);
  deactivate_servos();
}

