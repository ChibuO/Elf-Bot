// I2C sensors app
//
// Read from I2C accelerometer/magnetometer on the Microbit

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_twi_mngr.h"

#include "microbit_v2.h"
#include "Motor_driver.h"
#include "Sonic_sensor_driver.h"
#include "Thermal_sensor_driver.h"
#include "Thermal_sensing_driver.h"

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

// APP_TIMER_DEF(APP_TIM);

int main(void) {
  printf("\nBoard started!\n");

  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = EDGE_P19; //Output SCL, different from internal one 
  i2c_config.sda = EDGE_P20; //Outputput SDA
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  // Initialize drivers
  // sonic_init(&twi_mngr_instance);
  thermal_init(&twi_mngr_instance);
  motor_init(&twi_mngr_instance);
  // deactivate_servos();

  
  // app_timer_init();
  // app_timer_create(&APP_TIM, APP_TIMER_MODE_REPEATED, deactivate_servos);
  // app_timer_start(APP_TIM, 30000, NULL);
  

  // Intialize arrays used by thermal sensing driver
  float heat_grid[8][8];
  float average_vals[8];
  // follow_heat(heat_grid, average_vals, &twi_mngr_instance);
  // nrf_delay_ms(5000);
  // deactivate_servos();
  // Loop forever
  while (1) {
    follow_heat(heat_grid, average_vals, &twi_mngr_instance);
    nrf_delay_ms(5000);
  }
}

