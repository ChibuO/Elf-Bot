#include <stdbool.h>
#include <stdint.h>

#include "Thermal_sensor_driver.h"
#include "I2C_functions.h"
#include "nrf_delay.h"
#include "math.h"

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

void getDeviceTemp()
{
    float temperature = (float)i2c_reg_read(THERM_ADDR, THERMISTOR_REGISTER_LSB, i2c_manager);
    // float realtemp = temperature * .0625;
    printf("Temperature: %f\n", temperature);
}

void thermal_init(const nrf_twi_mngr_t* i2c){
    i2c_manager = i2c;
    while(1){
        getDeviceTemp();
        nrf_delay_ms(1000);
    }
    //getDeviceTemp();
}

