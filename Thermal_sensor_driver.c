#include <stdbool.h>
#include <stdint.h>

#include "Thermal_sensor_driver.h"
#include "I2C_functions.h"
#include "nrf_delay.h"
#include "math.h"

// Use these values (in degrees C) to adjust the contrast
#define HOT 30
#define COLD 10

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
    float float_temp = (float)temp *.25;
    //fahrenheit
    //float float_temp = ((float)temp *.0625) * 1.8 + 32;

    return float_temp;

}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void grid_eye(float pixel_table[8][8]) {

    // loop through all 64 pixels on the device and map each float value to a number
    // between 0 and 3 using the HOT and COLD values we set at the top of the sketch
    for (uint8_t i = 0; i < 8; i++){
        for(uint8_t j = 0; i < 8; i++){
            //map scales the given value
            //value, min and max that we could get, min and max that we want
            uint8_t pixel_num = (i * 8) + j;
            float orig_temp = get_pixel_temp(pixel_num);

            /*if ( orig_temp < COLD ) {
                orig_temp = COLD;
            } else if ( orig_temp > HOT ) {
                orig_temp = HOT;
            }*/

            pixel_table[i][j] = orig_temp;
            printf("i: %u j: %u orig: %f\n", i, j, orig_temp);
            //pixel_table[j] = map(orig_temp, COLD, HOT, 0, 5);
            //    printf("i: %u orig: %f mapped: %f\n", pixel_num, orig_temp, pixel_table[pixel_num]);
        }
    }

    printf("\n");

    // loop through the table of mapped values and print a character corresponding to each
    // pixel's temperature. Add a space between each. Start a new line every 8 in order to 
    // create an 8x8 grid
    for (uint8_t j = 0; i < 8; i++) {
        for(uint8_t i = 0; j < 8; j++) {
            printf("%f  ", pixel_table[i][j]);
            /*
            uint8_t pixel_num = (j * 8) + i;
            if (pixel_table[pixel_num] == 0) {printf("0");}
            else if (pixel_table[pixel_num] == 1) {printf("1");}
            else if (pixel_table[pixel_num] == 2) {printf("2");}
            else if (pixel_table[pixel_num] == 3) {printf("3");}
            else if (pixel_table[pixel_num] == 4) {printf("4");}
            else if (pixel_table[pixel_num] == 5) {printf("5");}
            printf(" ");

            */
            if ((j+1) % 8 == 0) {
                printf("\n");
            }
        }
    }
}

void thermal_init(const nrf_twi_mngr_t* i2c){
    i2c_manager = i2c;
    float pixel_table[8][8];
    while(1){
        //printf("Temperature: %f\n", getDeviceTemp());
        grid_eye(pixel_table);
        printf("\n");
        nrf_delay_ms(2000);
    }
}

