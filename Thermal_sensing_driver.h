#pragma once

#include "nrf_twi_mngr.h"

// Prototypes
void follow_heat(float heat_grid[8][8], float average_vals[8], const nrf_twi_mngr_t* i2c); 