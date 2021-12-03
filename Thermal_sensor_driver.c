#include <stdbool.h>
#include <stdint.h>

#include "Thermal_sensor_driver.h"
#include "I2C_functions.h"
#include "nrf_delay.h"
#include "math.h"

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

float getDeviceTemp() {
    // Sample function to familiarize with I2C of device. Not used in our project though
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
    // Helper function for grid_eye,
    // Acquires the temperature of a specified pixel from the GridEye
    
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
    //float float_temp = (float)temp *.25;
    //fahrenheit
    float float_temp = ((float)temp *.25) * 1.8 + 32;

    return float_temp;

}

// long map(long x, long in_min, long in_max, long out_min, long out_max) {
//   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }


 void grid_eye(float pixel_table[8][8]) {
    // Update the global pixel grid with the latest temperatures

    // loop through all 64 pixels on the device and map each float value to a number
    // between 0 and 3 using the HOT and COLD values we set at the top of the sketch
    for (uint8_t i = 0; i < 8; i++){
        for(uint8_t j = 0; j < 8; j++){
            uint8_t pixel_num = (i * 8) + j;
            float orig_temp = get_pixel_temp(pixel_num);
            pixel_table[i][j] = orig_temp;
        }
    }
 }
    
void temp_averages(float averages[8], float pixel_table[8][8]){
    // Calculate the averages of each column and print them out in the respective positions
    // Function will update the passed in averages array
    float holder = 0;
    for (uint8_t i = 0; i < 8; i++){
        for (uint8_t j = 0; j < 8; j++){
            holder += pixel_table[j][i];
        }
        float avg = holder/8.0;
        averages[i] = avg;
        // printf("%.1f  ", avg);
        holder = 0;
    } 
    printf("\n");
}

void temp_maxes(float maxes[8], float pixel_table[8][8]){
    // Calculate the maxes of each column and print them out in the respective positions
    // Function will update the passed in maxes array
    float max = 0;
    for (uint8_t i = 0; i < 8; i++){
        for (uint8_t j = 0; j < 8; j++){
            if (pixel_table[j][i] > max){
                max = pixel_table[j][i];
            }
        }
        maxes[i] = max;
        // printf("%.1f  ", max);
        max = 0;
    }
    // printf("\n");
}

void print_temp_grid(float pixel_table[8][8]){
    for (uint8_t i = 0; i < 8; i++) {
        for(uint8_t j = 0; j < 8; j++) {
            printf("%.1f  ", pixel_table[i][j]);
            if ((j+1) % 8 == 0) {
                printf("\n");
            }
        }
    }
}

void thermal_init(const nrf_twi_mngr_t* i2c){
    i2c_manager = i2c;
    // float averages[8];
    // float maxes[8];
    // float pixel_table[8][8];
    // while(1){
    //     //printf("Temperature: %f\n", getDeviceTemp());
    //     grid_eye(pixel_table);
    //     temp_averages(averages, pixel_table);
    //     // temp_maxes(maxes, pixel_table);
    //     // print_temp_grid(pixel_table);
    //     printf("\n");
    //     nrf_delay_ms(1000);
    // }
}

