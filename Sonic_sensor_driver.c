#include "I2C_functions.h"
#include "Sonic_sensor_driver.h"
#include "nrf_delay.h"


#define sonic_addr     0x00
#define sonic_data_reg 0x005216
#define sonic_status_reg 0x005217
#define beep_reg   0x0050F0

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;


void print_data(){
    i2c_reg_write(0x00, 0x00, 1, i2c_manager);
    uint8_t data = i2c_reg_read(0x00, 0x00, i2c_manager);
    printf("sonic data: %lu\n", data);
}
    
void beep(){
    i2c_reg_write(sonic_addr, beep_reg, 1, i2c_manager);
    nrf_delay_ms(1000);
    i2c_reg_write(sonic_addr, beep_reg, 0, i2c_manager);
}

void sonic_init(const nrf_twi_mngr_t* i2c){
    while(1){
        i2c_manager = i2c;
        printf("sonic_init called!\n");
        print_data();
        nrf_delay_ms(500);
    } 
    //beep();
}