#pragma once

#include "nrf_twi_mngr.h"

// Function prototypes
void motor_init(const nrf_twi_mngr_t* i2c);
void actuate_servos(int l_speed, int r_speed, bool l_forward, bool r_forward);
void activate_servos();
void deactivate_servos();