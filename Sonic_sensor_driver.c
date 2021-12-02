#include "I2C_functions.h"
#include "Sonic_sensor_driver.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "microbit_v2.h"
#include <time.h>

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

uint64_t getPulse()
{

    nrf_gpio_pin_write(trig_pin, 0);
    nrf_delay_us(2);
    // printf("Triggered!\n");
    nrf_gpio_pin_write(trig_pin, 1);
    nrf_delay_us(10);
    // printf("Not triggered anymore!\n");
    nrf_gpio_pin_write(trig_pin, 0);
    int width = 0;

    // nrf_gpio_pin_set(trig_pin);
    // nrf_delay_us(2);
    // printf("Triggered!\n");
    // nrf_gpio_pin_clear(trig_pin);
    // nrf_delay_us(10);
    // printf("Not triggered anymore!\n");
    // nrf_gpio_pin_set(trig_pin);
    // int width = 0;


    // printf("reading echo\n");
    while (nrf_gpio_pin_read(echo_pin) == 0){
        // printf("schlorp\n");
    }
    // printf("first clock\n");
    clock_t start = clock(), diff;
    // printf("first clock value: %lu\n", start);

    while (nrf_gpio_pin_read(echo_pin) == 1) 
    {
        // printf("echo pin high!\n");
        width++;
        nrf_delay_us(1);
        if (width > timeout){
            // printf("TIMEOUT\n");
            return 0;
        }
    }
    // printf("second clock\n");
    diff = clock() - start;
    int msec = diff * 1000;
    // printf("diff: %d\n", msec);
    
    return (uint64_t)msec;
}

uint64_t getCm(uint trigPin, uint echoPin)
{
    uint64_t pulseLength = getPulse(trigPin, echoPin);
    return pulseLength / 29 / 2;
}

uint64_t getInch(uint trigPin, uint echoPin)
{
    uint64_t pulseLength = getPulse(trigPin, echoPin);
    return (long)pulseLength / 74.f / 2.f;
}
// double sonic_gpio() {
//     // unsigned long t1;
//     // unsigned long t2;
//     clock_t t1, t2;
    
//     // nrf_gpio_cfg_output(trig_pin);
//     // nrf_gpio_cfg_input(echo_pin);
//     nrf_gpio_pin_write(trig_pin, 0);
//     nrf_delay_ms(0.002);
//     printf("Triggered!\n");
//     nrf_gpio_pin_write(trig_pin, 1);
//     nrf_delay_ms(0.01);
//     printf("Not triggered anymore!\n");
//     nrf_gpio_pin_write(trig_pin, 0);
//     // printf("trig_pin before: %lu\n",nrf_gpio_pin_read(trig_pin));
//     // printf("Triggered!\n");
//     // printf("Not triggered anymore!\n");
//     // printf("trig_pin after: %lu\n",nrf_gpio_pin_read(trig_pin));


//     printf("checking pin low\n");
//     while(!nrf_gpio_pin_read(echo_pin)) {
//         // printf("echo_pin: %lu\n",nrf_gpio_pin_read(echo_pin));
//     };
//     printf("Recording first clock\n");
//     t1 = clock();
//     printf("checking pin high\n");
//     while(nrf_gpio_pin_read(echo_pin)) {
//         // printf("echo_pin: %lu\n",nrf_gpio_pin_read(echo_pin));
//     };
//     printf("Recording second clock\n");
//     t2 = clock();

//     printf("t1: %lf\n", (double)(t1));
//     printf("t2: %lf\n", (double)(t2));
//     double pulse_width = ((double)(t2 - t1));

//     double inches = (pulse_width / 148.0);
//     return inches;
// }

void sonic_init(const nrf_twi_mngr_t* i2c){
    // double distance;
    setupUltrasonicPins(trig_pin,echo_pin);
    while(1){
        printf("%llu cm\n", getPulse(trig_pin,echo_pin));
        nrf_delay_ms(100);
    }
    // while(1){
    //     i2c_manager = i2c;
    //     printf("\n");
    //     // printf("sonic_init called!\n");
    //     distance = sonic_gpio();
    //     printf("Distance: %f in.\n", distance);
    //     nrf_delay_ms(1000);
    // } 
    /*
    i2c_manager = i2c;
    print_data();
    */
    //beep();
}