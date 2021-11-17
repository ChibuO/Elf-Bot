#pragma once

#include "nrf_twi_mngr.h"

// Function prototypes

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void motor_init(const nrf_twi_mngr_t* i2c);