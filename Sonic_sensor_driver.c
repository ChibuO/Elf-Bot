#include "I2C_functions.h"
#include "Sonic_sensor_driver.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "microbit_v2.h"
#include <time.h>
#include "virtual_timer.h"

#define trig_pin       EDGE_P15 
#define echo_pin       EDGE_P16

void setupUltrasonicPins()
{
    // Configure trigger pin and echo pin as output and input respectively
    nrf_gpio_pin_dir_set(trig_pin, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(echo_pin, NRF_GPIO_PIN_DIR_INPUT);
}

uint32_t getPulse() {
    // Triggers a pulse to be recieved on the echo pin. The duration of that pulse directly correlates to distance of an object from the sonic sensor
    nrf_gpio_pin_write(trig_pin, 0);
    nrf_delay_us(2);
    nrf_gpio_pin_write(trig_pin, 1);
    nrf_delay_us(10);
    nrf_gpio_pin_write(trig_pin, 0);

    while (nrf_gpio_pin_read(echo_pin) == 0);

    uint32_t start = read_timer();

    while (nrf_gpio_pin_read(echo_pin) == 1) {
        // Do nothing while the echo pin is high
    }

    // Find the duration the echo pin read a high pulse
    uint32_t diff = read_timer() - start;
    
    return diff/58;
}

void sonic_init() {
    setupUltrasonicPins(trig_pin,echo_pin);
    virtual_timer_init();
}