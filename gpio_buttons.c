#include "gpio_buttons.h"
#include "stdio.h"

typedef struct{
  // Register definitions 
  uint32_t _unused_A[64 + 64 + 64 + 64 + 64 + 1];
  uint32_t OUT;
  uint32_t OUTSET;
  uint32_t OUTCLR;
  uint32_t IN;
  uint32_t DIR;
  uint32_t DIRSET;
  uint32_t DIRCLR;
  uint32_t LATCH;
  uint32_t DETECTMODE;
  uint32_t _unused_B[64 + 54];
  uint32_t PIN_CNF[31];
} gpio_reg_t;

volatile gpio_reg_t* something = (gpio_reg_t*)(0x50000000);    // Port 0
volatile gpio_reg_t* somethingtoo = (gpio_reg_t*)(0x50000300); // Port 1

void gpio_print() {
  printf("%p\n", &(something->OUT));
  printf("%p\n", &(something->OUTSET));
  printf("%p\n", &(something->OUTCLR));
  printf("%p\n", &(something->IN));
  printf("%p\n", &(something->DIR));
  printf("%p\n", &(something->DIRSET));
  printf("%p\n", &(something->DIRCLR));
  printf("%p\n", &(something->LATCH));
  printf("%p\n", &(something->DETECTMODE));
  printf("%p\n", &(something->PIN_CNF));
}
// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
  something->PIN_CNF[gpio_num] = dir;

}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_set(uint8_t gpio_num) {
  (something->OUTSET) = 1 << gpio_num;
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_clear(uint8_t gpio_num) {
  // Implement me
  (something->OUTCLR) = 1 << gpio_num;
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
// Output:
//  bool - pin state (true == high)
bool gpio_read(uint8_t gpio_num) {
  // Implement me
  uint32_t binary_pin_num = 1 << gpio_num;
  if(((something->IN) & binary_pin_num) > 0)
  {
    return true;
  }
  else
    {
      return false;
    }
      
}