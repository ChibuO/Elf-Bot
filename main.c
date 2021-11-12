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
#include "lsm303agr.h"

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

APP_TIMER_DEF(APP_TIM);

float tilt_angle(lsm303agr_measurement_t acc) {
    float tilt = atan(sqrt((acc.x_axis * acc.x_axis) + (acc.y_axis * acc.y_axis))/acc.z_axis);
    tilt = tilt * 360 / (2* M_PI);
    return tilt;
}

static void app_tim_callback(void *args) {
    // printf("Temp: X:%f\tY:%f\tZ:%f\n", acc.x_axis, acc.y_axis, acc.z_axis);
    lsm303agr_measurement_t acc = lsm303agr_read_accelerometer();
    printf("Accelerometer: X:%f\tY:%f\tZ:%f\n", acc.x_axis, acc.y_axis, acc.z_axis);
    // lsm303agr_measurement_t mag = lsm303agr_read_magnetometer();
    // printf("Temp Magnetometer: X:%f\tY:%f\tZ:%f\n", mag.x_axis, mag.y_axis, mag.z_axis);
    printf("Tilt: %f\n\n", tilt_angle(acc));
}

int main(void) {
  printf("Board started!\n");

  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = I2C_SCL;
  i2c_config.sda = I2C_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  // Initialize the LSM303AGR accelerometer/magnetometer sensor
  lsm303agr_init(&twi_mngr_instance);

  //TODO: implement me!
  app_timer_init();
  app_timer_create(&APP_TIM, APP_TIMER_MODE_REPEATED, app_tim_callback);
  app_timer_start(APP_TIM, 10000, NULL);

  // Loop forever
  while (1) {
    // Don't put any code in here. Instead put periodic code in a callback using a timer.
    nrf_delay_ms(1000);
  }
}

