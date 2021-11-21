#include "I2C_functions.h"
#include "Sonic_sensor_driver.h"
#include "nrf_delay.h"


#define sonic_addr     2
#define sonic_reg      0x57
#define trig           0
#define echo           0
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


void print_data(){
    
    
    for (int i = 0; i <= 255; i++)
    {
        printf("i: %u\n", i);
        for (int j = 0; j <= 255; j++)
        {
            for (int k = 0; k<= 255; k++)
            {
                i2c_reg_write(i, j, 1, i2c_manager);
                //nrf_delay_ms(6);
                uint8_t data_msb = i2c_reg_read(i, k, i2c_manager);
                if (data_msb != 0)
                {
                    // Upon a data hit, print out that we found data, find the LSB of data, and combine the two bytes to get the full data
                    printf("hit\n");
                    uint8_t data_lsb = i2c_reg_read(i, k+1, i2c_manager);
                    uint16_t most_shifted = data_msb << 8;
                    uint16_t distance = most_shifted | data_lsb;
                    printf("j: %u\n", j);
                    printf("k: %u\n", k);
                    printf("Distance: %u mm\n", distance);
                }   
            }
        }
    }
    printf("Function done\n");
    
    
    /*
    i2c_reg_write(sonic_addr, trig, 1, i2c_manager);
    nrf_delay_ms(1000);
    uint8_t distance_msb = i2c_reg_read(sonic_addr, echo, i2c_manager);
    uint8_t distance_lsb = i2c_reg_read(sonic_addr, echo+1, i2c_manager);
    printf("Dist_MSB: %u\n", distance_msb);
    printf("Dist_LSB: %u\n", distance_lsb);
    uint16_t most_shifted = distance_msb << 8;
    uint16_t distance = most_shifted | distance_lsb;
    
    printf("sonic data: %u\n", distance);
    */
    
}

/*  
void beep(){
    i2c_reg_write(sonic_addr, beep_reg, 1, i2c_manager);
    nrf_delay_ms(1000);
    i2c_reg_write(sonic_addr, beep_reg, 0, i2c_manager);
}
*/

void sonic_init(const nrf_twi_mngr_t* i2c){
    
    while(1){
        i2c_manager = i2c;
        printf("\n");
        printf("sonic_init called!\n");
        print_data();
        nrf_delay_ms(2000);
    } 
    /*
    i2c_manager = i2c;
    print_data();
    */
    //beep();
}