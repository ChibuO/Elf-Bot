#include "Thermal_sensor_driver.h"
#include "Motor_driver.h"

static const nrf_twi_mngr_t* i2c_manager = NULL;

uint8_t find_max_index(float average_array[8]) {
    uint8_t holder = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (average_array[i] > holder) {
            holder = i;
        }
    }
    printf("Holder value: %u\n", holder);
    return holder;
}
void follow_heat(float heat_grid[8][8], float average_vals[8], const nrf_twi_mngr_t* i2c) {
    // Function that updates an inputted pixel grid, finds the averages of each column
    // and moves the robot accordingly to follow a heat source
    // i2c_manager = i2c;
    // First update the pixel grid
    // The pixel grid for the GridEye is always an 8x8 array
    grid_eye(heat_grid);

    // Find the averages of the columns
    // The average array will consist of 8 values, each corresponding to the average of each column from the GridEye
    printf("Temp averages:\n");
    temp_averages(average_vals, heat_grid);

    // Two conditions:
    // 1) If max averages are center column, then just drive forward
    // 2) If max averages are left or right of center, adjust to orient until the maxes are in the center
    // Column setup:
    // 0  1  2  3  4  5  6  7  
    // If the max average is in columns 2-5, the robot should keep going forward
    // If the max average is in columns 0-1, the robot should  turn left
    // If the max average is in clumns 6-7, the robot should turn right
    int wheel_speed = 10;
    int turning_wheel_speed = wheel_speed + 10;
    uint8_t max_index = find_max_index(average_vals);
    if (max_index == 0 || max_index == 1) {     
        // Target is to the left    
        printf("Turning left...\n");        
        activate_servos(wheel_speed, turning_wheel_speed, true, true);
    }
    else if (max_index == 6 || max_index == 7) {    
        // Target is to the right        
        printf("Turning right...\n");
        activate_servos(turning_wheel_speed, wheel_speed, true, true);
    }
    else {                                                  
        // Target is in the center
        printf("Going forward...\n");
        activate_servos(wheel_speed, wheel_speed, true, true);
    }
}
