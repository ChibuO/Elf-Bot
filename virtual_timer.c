// Virtual timer implementation

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "nrf.h"

#include "virtual_timer.h"


// Read the current value of the timer counter
uint32_t read_timer(void) {
  NRF_TIMER4-> TASKS_CAPTURE[1] = 1;
  
  // Should return the value of the internal counter for TIMER4. Value is in microseconds
  return NRF_TIMER4->CC[1];
}

// Initialize TIMER4 as a free running timer. 
// 1) Set to be a 32 bit timer
// 2) Set to count at 1MHz
// 3) Enable the timer peripheral interrupt (look carefully at the INTENSET register!)
//    Skip this for the first part of the lab.
// 4) Clear the timer
// 5) Start the timer
void virtual_timer_init(void) {
  NRF_TIMER4->BITMODE = 3;
  NRF_TIMER4->PRESCALER =4;
  NRF_TIMER4->INTENSET =1<<18; //cc =2 
  NVIC_EnableIRQ(TIMER4_IRQn);
  NVIC_SetPriority(TIMER4_IRQn, 0);
  NRF_TIMER4->TASKS_CLEAR =1;
  NRF_TIMER4->TASKS_START =1; 
  }