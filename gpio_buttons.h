#pragma once

#include "nrf.h"
#include "stdbool.h"

typedef enum {
  GPIO_INPUT = 0,
  GPIO_OUTPUT,
} gpio_direction_t;

typedef struct {
  uint32_t _unused_A[64 + 64 + 64 + 64 + 63];
  uint32_t OUT;
  uint32_t OUTSET;
  uint32_t OUTCLR;
  uint32_t IN;
  uint32_t DIR;
  uint32_t LATCH;
  uint32_t DETECTMODE;
  uint32_t _unused_B[64 + 54];
  uint32_t PIN_CNF[31];
} GPIO_MMIO;

bool gpio_read(uint8_t gpio_num);

void gpio_config(uint8_t gpio_num, gpio_direction_t dir);