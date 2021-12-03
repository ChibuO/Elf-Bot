#include "I2C_functions.h"
#include "Sonic_sensor_driver.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "microbit_v2.h"
#include <time.h>
#include "virtual_timer.h"

#define sonic_addr     0
#define trig_pin       EDGE_P15 
#define echo_pin       EDGE_P16

// Pointer to an initialized I2C instance to use for transactions
// static const nrf_twi_mngr_t* i2c_manager = NULL;

int timeout = 26100;

void setupUltrasonicPins()
{
    nrf_gpio_pin_dir_set(trig_pin, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(echo_pin, NRF_GPIO_PIN_DIR_INPUT);
    // nrf_gpio_pin_write(trig_pin, 0);
    // nrf_gpio_pin_write(echo_pin, 0);
}

uint32_t getPulse() {
    nrf_gpio_pin_write(trig_pin, 0);
    nrf_delay_us(2);
    nrf_gpio_pin_write(trig_pin, 1);
    nrf_delay_us(10);
    nrf_gpio_pin_write(trig_pin, 0);
    int width = 0;

    while (nrf_gpio_pin_read(echo_pin) == 0);

    uint32_t start = read_timer();

    while (nrf_gpio_pin_read(echo_pin) == 1) {
        // width++;
        // nrf_delay_us(1);
        // if (width > timeout){
        //     printf("TIMEOUT\n");
        //     return 0;
        // }
    }

    uint32_t diff = read_timer() - start;
    
    return diff/58;
}

uint64_t getCm(uint trigPin, uint echoPin) {
    uint64_t pulseLength = getPulse(trigPin, echoPin);
    return pulseLength / 29 / 2;
}

uint64_t getInch(uint trigPin, uint echoPin) {
    uint64_t pulseLength = getPulse(trigPin, echoPin);
    return (long)pulseLength / 74.f / 2.f;
}

void sonic_init() {
    setupUltrasonicPins(trig_pin,echo_pin);
    virtual_timer_init();
}