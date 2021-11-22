#include "Thermal_sensor_driver.h"
#include "Motor_driver.h"

void follow_heat() {
    // First update the pixel grid
    float heat_grid[8][8];
    grid_eye(heat_grid);

    // Find the averages of the columns
    float average_vals[8];
    temp_averages(average_vals, heat_grid);


}