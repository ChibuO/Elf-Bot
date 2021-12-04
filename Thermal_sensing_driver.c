#include "Thermal_sensor_driver.h"
#include "Motor_driver.h"
#include "nrf_delay.h"

uint8_t find_max_index(float average_array[8]) {
    uint8_t holder = 0;
    float max = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (average_array[i] > max) {
            holder = i;
            max = average_array[i];
        }
    }
    return holder;
}
void follow_heat(float heat_grid[8][8], float average_vals[8], const nrf_twi_mngr_t* i2c) {
    // Function that updates an inputted pixel grid, finds the averages of each column
    // and moves the robot accordingly to follow a heat source
    // First update the pixel grid
    // The pixel grid for the GridEye is always an 8x8 array
    grid_eye(heat_grid);

    // Find the averages of the columns
    // The average array will consist of 8 values, each corresponding to the average of each column from the GridEye
    temp_averages(average_vals, heat_grid);

    // Two conditions:
    // 1) If max averages are center column, then just drive forward
    // 2) If max averages are left or right of center, adjust to orient until the maxes are in the center
    // Column setup:
    // 0  1  2  3  4  5  6  7  
    // If the max average is in columns 2-5, the robot should keep going forward
    // If the max average is in columns 0-1, the robot should  turn left
    // If the max average is in clumns 6-7, the robot should turn right
    uint8_t max_index = find_max_index(average_vals);
    if (max_index == 0 || max_index == 1) {     
        // Target is to the left     
        actuate_servos(27, 27, false, true);
    } else if (max_index == 6 || max_index == 7) {    
        // Target is to the right    
        actuate_servos(27, 27, true, false);
    } else {                                                  
        // Target is in the center
        actuate_servos(50, 50, true, true);
    }
}