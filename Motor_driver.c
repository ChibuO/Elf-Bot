// LSM303AGR driver for Microbit_v2
//
// Initializes sensor and communicates over I2C
// Capable of reading temperature, acceleration, and magnetic field strength

#include <stdbool.h>
#include <stdint.h>

#include "Motor_driver.h"
#include "I2C_functions.h"
#include "nrf_delay.h"
#include "math.h"

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

void actuate_servos(int l_speed, int r_speed, bool l_forward, bool r_forward) {
  uint16_t l_pwr = (uint16_t)floor(l_speed * 127 / 100);
  uint16_t r_pwr = (uint16_t)floor(r_speed * 127 / 100);
  uint16_t l_dir = l_pwr | mc_fwd;
  uint16_t r_dir = r_pwr | mc_fwd;
  
  i2c_reg_write(mc_i2c_addr, mc_inv_left, !l_forward, i2c_manager); // left = 0 forwards, 1 backwards
  i2c_reg_write(mc_i2c_addr, mc_inv_right, r_forward, i2c_manager); // right = 0 backwards, 1 forwards

  i2c_reg_write(mc_i2c_addr, mc_left, l_dir, i2c_manager); //make left motor move forward
  i2c_reg_write(mc_i2c_addr, mc_right,r_dir, i2c_manager); //make right motor move forward
}

void activate_servos(){
  i2c_reg_write(mc_i2c_addr, mc_pwr_on, 1, i2c_manager);
}

void deactivate_servos(){
  i2c_reg_write(mc_i2c_addr, mc_pwr_on, 0, i2c_manager);
}

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void motor_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;
}