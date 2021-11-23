#include "I2C_functions.h"
#include "Sonic_sensor_driver.h"
#include "nrf_delay.h"
#include "microbit_v2.h"
#include <time.h>

#define sonic_addr     0
#define trig_pin       8
#define echo_pin       12
/*
#define sonic_data_reg 0x005216
#define sonic_status_reg 0x005217
#define beep_reg   0x0050F0
*/

// Log of possible hits:
// - sonic_addr = 0; trig = 0; echo = 1
// - sonic_addr = 0; trig = 0; echo = 1
// - sonic_addr = 2; trig = 0; echo = 0

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;


// void print_data(){
    
//     // CODE FOR TRYING TO FIND THE REG ADDRESS //
//     // for (int i = 0; i <= 255; i++)
//     // {
//     //     printf("i: %u\n", i);
//     //     for (int j = 0; j <= 255; j++)
//     //     {
//     //         for (int k = 0; k<= 255; k++)
//     //         {
//     //             i2c_reg_write(i, j, 1, i2c_manager);
//     //             //nrf_delay_ms(6);
//     //             uint8_t data_msb = i2c_reg_read(i, k, i2c_manager);
//     //             if (data_msb != 0)
//     //             {
//     //                 // Upon a data hit, print out that we found data, find the LSB of data, and combine the two bytes to get the full data
//     //                 printf("hit\n");
//     //                 uint8_t data_lsb = i2c_reg_read(i, k+1, i2c_manager);
//     //                 uint16_t most_shifted = data_msb << 8;
//     //                 uint16_t distance = most_shifted | data_lsb;
//     //                 printf("j: %u\n", j);
//     //                 printf("k: %u\n", k);
//     //                 printf("Distance: %u mm\n", distance);
//     //             }   
//     //         }
//     //     }
//     // }
//     // printf("Function done\n");
    
//     // DATA FINDING USING EXPERIMENTAL READ AND WRITES //
//     experimental_write(sonic_addr, 1, i2c_manager);
//     // nrf_delay_ms(5);
//     uint8_t distance_msb = experimental_read(0, i2c_manager);
//     uint8_t distance_lsb = experimental_read(0, i2c_manager);
//     printf("MSB: %u\n", distance_msb);
//     printf("LSB: %u\n", distance_lsb);
//     uint16_t most_shifted = distance_msb << 8;
//     uint16_t distance = most_shifted | distance_lsb;
//     printf("sonic data: %u\n", distance);
    

//     // DEFAULT DATA FINDING //
//     // i2c_reg_write(sonic_addr, trig, 1, i2c_manager);
//     // uint8_t distance_msb = i2c_reg_read(sonic_addr, echo, i2c_manager);
//     // uint8_t distance_lsb = i2c_reg_read(sonic_addr, echo+1, i2c_manager);
//     // printf("Dist_MSB: %u\n", distance_msb);
//     // printf("Dist_LSB: %u\n", distance_lsb);
//     // uint16_t most_shifted = distance_msb << 8;
//     // uint16_t distance = most_shifted | distance_lsb;
//     // printf("sonic data: %u\n", distance);
// }

double sonic_gpio() {
    // unsigned long t1;
    // unsigned long t2;
    clock_t t1, t2;
    nrf_gpio_pin_dir_set(trig_pin, NRF_GPIO_PIN_DIR_OUTPUT);
    nrf_gpio_pin_dir_set(echo_pin, NRF_GPIO_PIN_DIR_INPUT);
    // nrf_gpio_cfg_output(trig_pin);
    // nrf_gpio_cfg_input(echo_pin);
    nrf_gpio_pin_write(trig_pin, 1);
    // printf("trig_pin before: %lu\n",nrf_gpio_pin_read(trig_pin));
    printf("Triggered!\n");
    nrf_delay_ms(5000);
    nrf_gpio_pin_write(trig_pin, 0);
    printf("Not triggered anymore!\n");
    // printf("trig_pin after: %lu\n",nrf_gpio_pin_read(trig_pin));

    while(!nrf_gpio_pin_read(echo_pin)) {
        printf("echo_pin: %lu\n",nrf_gpio_pin_read(echo_pin));
    };
    t1 = clock();
    while(nrf_gpio_pin_read(echo_pin)) {
        printf("echo_pin: %lu\n",nrf_gpio_pin_read(echo_pin));
    };
    t2 = clock();

    printf("t1: %lf\n", (double)(t1));
    printf("t2: %lf\n", (double)(t2));
    double pulse_width = ((double)(t2 - t1));

    double inches = (pulse_width / 148.0);
    return inches;
}

void sonic_init(const nrf_twi_mngr_t* i2c){
    double distance;
    while(1){
        i2c_manager = i2c;
        printf("\n");
        // printf("sonic_init called!\n");
        distance = sonic_gpio();
        printf("Distance: %f in.\n", distance);
        nrf_delay_ms(1000);
    } 
    /*
    i2c_manager = i2c;
    print_data();
    */
    //beep();
}