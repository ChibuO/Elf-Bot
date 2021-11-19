#include <stdbool.h>
#include <stdint.h>

#include "Thermal_sensor_driver.h"
#include "I2C_functions.h"
#include "nrf_delay.h"
#include "math.h"

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

float getDeviceTemp() {
    uint8_t temp_lsb = i2c_reg_read(THERM_ADDR, THERMISTOR_REGISTER_LSB, i2c_manager);
    uint8_t temp_msb = i2c_reg_read(THERM_ADDR, THERMISTOR_REGISTER_MSB, i2c_manager);

    int16_t most_shifted = temp_msb << 8;
    int16_t temp = most_shifted | temp_lsb;

    if( temp & (1 << 11)) {
        // if temperature is negative, mask out the sign byte and 
        // make the float negative
        temp &= ~(1 << 11);
        temp = temp * -1;
    }

    //celsius
    //float float_temp = (float)temp *.0625;
    //fahrenheit
    float float_temp = ((float)temp *.0625) * 1.8 + 32;

    return float_temp;
}

float get_pixel_temp(uint8_t pixelAddr) {
    // Temperature registers are numbered 128-255
    // Each pixel has a lower and higher register
    uint8_t pixelLowRegister = TEMPERATURE_REGISTER_START + (2 * pixelAddr);
    uint8_t temp_lsb = i2c_reg_read(THERM_ADDR, pixelLowRegister, i2c_manager);
    uint8_t temp_msb = i2c_reg_read(THERM_ADDR, pixelLowRegister + 1, i2c_manager);

    int16_t most_shifted = temp_msb << 8;
    int16_t temp = most_shifted | temp_lsb;

    // temperature is reported as 12-bit twos complement
    // check if temperature is negative
    if( temp & (1 << 11)) {
        // if temperature is negative, mask out the sign byte and 
        // make the float negative
        temp &= ~(1 << 11);
        temp = temp * -1;
    }

    //celsius
    //float float_temp = (float)temp *.0625;
    //fahrenheit
    float float_temp = ((float)temp *.0625) * 1.8 + 32;

    return float_temp;

}

void grid_eye(uint8_t pixel_table) {

    // loop through all 64 pixels on the device and map each float value to a number
    // between 0 and 3 using the HOT and COLD values we set at the top of the sketch
    for(uint8_t i = 0; i < 64; i++){
        pixelTable[i] = map(grideye.getPixelTemperature(i), COLD, HOT, 0, 3);
    }


    // loop through the table of mapped values and print a character corresponding to each
    // pixel's temperature. Add a space between each. Start a new line every 8 in order to 
    // create an 8x8 grid
    for(uint8_t i = 0; i < 64; i++){
        if(pixelTable[i]==0){Serial.print(".");}
        else if(pixelTable[i]==1){Serial.print("o");}
        else if(pixelTable[i]==2){Serial.print("0");}
        else if(pixelTable[i]==3){Serial.print("O");}
        Serial.print(" ");
        if((i+1)%8==0){
        Serial.println();
        }
    }

    // in between updates, throw a few linefeeds to visually separate the grids. If you're using
    // a serial terminal outside the Arduino IDE, you can replace these linefeeds with a clearscreen
    // command
    printf("\n");
    printf("\n");

    // toss in a delay because we don't need to run all out
    delay(100);

}

void thermal_init(const nrf_twi_mngr_t* i2c){
    i2c_manager = i2c;
    uint8_t pixelTable[64];
    while(1){
        printf("Temperature: %f\n", getDeviceTemp());
        nrf_delay_ms(1000);
    }
    //getDeviceTemp();
}

