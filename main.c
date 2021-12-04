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

// Global variable
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

void turn_around() {
  // Function that tells the robot to turn around if it's been stuck/stopped by an object in front of it for too long
  // This function should be called after the timer has been checked, and after the servos are activated
  activate_servos();
  // First reverse
  actuate_servos(40, 40, false, false);
  nrf_delay_ms(1000);

  // Rotate
  actuate_servos(25, 25, true, false);
}


int main(void) {
  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = EDGE_P19; //Output SCL, different from internal one 
  i2c_config.sda = EDGE_P20; //Outputput SDA
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // Config GPIO
  gpio_config(14, 0); //button A
  gpio_config(23, 0); //button B

  // Initialize drivers
  sonic_init();
  thermal_init(&twi_mngr_instance);
  motor_init(&twi_mngr_instance);
  
  // Intialize arrays used by thermal sensing driver
  float heat_grid[8][8];
  float average_vals[8];

  // Bools used to determine if robot should be moving or not
  bool ignition = false;
  bool is_off = true;
  bool is_stopped = true;

  // Distance (in cm that the robot should stop moving at if it detects anything closer)
  uint32_t stop_distance = 30;

  // Time at which the robot stopped moving due to an object in front of it
  uint32_t stop_time = 0;

  // Loop forever
  while (1) {
    // Check if we actually want the robot to be on and following someone by checking
    // button A and B. 
    if(gpio_read(14) == 0) {
      is_off = true;
      ignition = false;
      deactivate_servos();
    } else if (gpio_read(23) == 0) {
      ignition = true;
      is_off = false;
    }

    // 2) Check if the robot is too close to an object.
    if(getPulse() <= stop_distance && !is_off && !is_stopped) {
      stop_time = read_timer();
      is_stopped = true;
      ignition = false;
      printf("stopped\n");
      deactivate_servos();
    } else if (getPulse() > stop_distance && !is_off) {
      is_stopped = false;
      ignition = true;
      activate_servos();
    }

    // 3) If stopped for over 5 seconds, robot must be stuck, so turn around
    if (getPulse() <= stop_distance && !is_off && is_stopped) {
      if (read_timer() - stop_time > 5000000) {
        turn_around();
        follow_heat(heat_grid, average_vals, &twi_mngr_instance);
      }
    }

    // 4) Move accordingly to follow a heat source
    if(ignition && !is_off) follow_heat(heat_grid, average_vals, &twi_mngr_instance);
  }
}