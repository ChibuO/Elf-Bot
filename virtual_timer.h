#pragma once

#include "nrf.h"

// Type for the function pointer to call when the timer expires
typedef void (*virtual_timer_callback_t)(void);

// Read the current value of the hardware timer counter
// Returns the counter value
uint32_t read_timer(void);

// Initialize the timer peripheral
void virtual_timer_init(void);