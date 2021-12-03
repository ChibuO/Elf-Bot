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

#include "gpio_buttons.h"
#include "virtual_timer.h"

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

  gpio_config(14, 0); //button A
  gpio_config(23, 0); //button B

  // Initialize drivers
  sonic_init();
  thermal_init(&twi_mngr_instance);
  motor_init(&twi_mngr_instance);
  
  // app_timer_init();
  // app_timer_create(&APP_TIM, APP_TIMER_MODE_REPEATED, deactivate_servos);
  // app_timer_start(APP_TIM, 30000, NULL);

  // Intialize arrays used by thermal sensing driver
  float heat_grid[8][8];
  float average_vals[8];

  bool ignition = true;
  bool is_off = false;
  uint32_t stop_distance = 30;

  // Loop forever
  while (1) {
    if(gpio_read(14) == 0) {
      // printf("Button A read, turning car off\n");
      is_off = true;
      ignition = false;
      deactivate_servos();
    } else if (gpio_read(23) == 0) {
      // printf("Buton B read, turning car on\n");
      is_off = false;
    }

    if(getPulse() <= stop_distance && !is_off) {
      ignition = false;
      // actuate_servos(20, 20, false, false);
      // nrf_delay_ms(100);
      // actuate_servos(30, 30, true, false);
      deactivate_servos();
    } else if (getPulse() > stop_distance && !is_off) {
      ignition = true;
      activate_servos();
    }

    if(ignition) follow_heat(heat_grid, average_vals, &twi_mngr_instance);
    printf("%lu cm\n", getPulse());
    //nrf_delay_ms(25);
    // nrf_delay_ms(1000);
  }
}

