# Elf-Bot
![Alt text](Media/IMG_1823.JPG?raw=true "Elf Bot!")

The Elf-Bot is a Micro:Bit based robot that follows people using Grid-EYE Infrared sensors and has object detection using an HC-SR04 Ultrasonic Sensor.
The NRF library from Nordic Semiconductors was used due to the project being Micro:Bit based (it is run on a nRF52 processor). As a result much of the code written utilizes functions from the library.

This project was tackled by creating numerous driver files for each component then utilizing them in the main.c file, where the actual sensing is done. Here is an explanation on each file

## Main following script
The main script is responsible for performing all initializations and holds the following actions of the robots. A lot of initializations happen here, but some of note include what distance the robot should stop at for object detection (we set it to 30 cm), global arrays used for the pixel temperature arrays, I2C bus initialization, and all component initializations (motor, IR sensor, and sonic sensor). 

The basic logic of the main following script is as follows:
Make sure we actually want the robot to run by checking what the last button on the Micro:Bit was pressed (the Micro:Bit features two buttons on it that can be mapped to any user defined function)
Determine if there is anything in front of the robot using the sonic sensor (if there is anything is 30 cm or closer, the robot should stop)
If the robot has been stopped for 5 seconds or more, reverse and try finding another heat source. Obviously if the object that stopped it was a human the robot would just go back to its original position, however if it is blocked by a non human object it will find another heat source and move away from the non human object.
Scan the area in front of the robot, and move in the direction where the highest temperature is coming from

## Driver files

#### I2C Functions
This is a driver for executing I2C read and write operations across devices connected to the bus. We communicate with our motors as well as the IR sensor over the I2C bus to simplify the process of sending commands (in the case of the motor) or reading data values (in the case of the IR sensor). 

For I2C communication on the nRF52, transfer objects must be created with commands stored inside. These transfer objects are then passed to a function that executes the transfer, which will result in a read or write.

#### Motor_driver
![Alt text](Media/motors.jpg?raw=true "SparkFun Hobby Gearmotor")

This is a driver used primarily for turning on and controlling the servos. One challenge we faced with this product was a lack of proper documentation for I2C addresses, but we were able to find the addresses through a combination of online searching and trial and error. With the addresses being defined, writing functions to control the servos become much easier. 

There is an activate and deactivate function for the servos because they require initialization and deinitialization processes required for turning on/off the servos. 

The actuate servos function is the main focus of this driver file, as it dictates the speed and direction of each wheel. This is done by doing a conversion of the inputted speed to be on a 0-100 scale, then performing an I2C write operation to the address of the motors to store the desired speed and polarity on them.

Motor init is used in the initialization of the main script in order to establish the I2C manager that the read and write functions require. The IR sensor also features an init function to establish a similar I2C manager. Without these init functions, I2C communication is not possible.

#### Thermal_sensor_driver
![Alt text](Media/IR_sensor.jpg?raw=true "SparkFun Grid-EYE Infrared Array Breakout - AMG8833 (Qwiic)")

This driver is used to extract data from the IR sensor. The IR sensor we are using utilizes a camera that reads in data from a 8x8 (64 pixel) array. In order to get the temperature for a specific pixel, you index the address of the desired pixel, and get values from that register, and one up. Pixel temperature is divided into two arrays (the overall value is 16 bits but each register only stores 8 bits), so we acquire the two 8 bit values and combine them to be 16 bit. Afterwards we check to see if the temperature is negative (if it is we mask out the sign bit making a negative float) and plug our value into an equation that converts the value to fahrenheit.

With the get pixel temp function established, we use this function in our grid eye function that takes in an 8x8 empty array. We then simply loop through the entire array and get the pixel temperature for the appropriate pixel position in the array. One thing to note is that the inputted pixel table is anticipated to be a global array in whichever file this driver function will be used in, so our function doesn’t explicitly return anything, it just updates this pixel array because we assume it will be a global function. 

We also implemented functions that would take maxes and averages of each column. These values would then be stored in an 8 wide array that is to be initialized as a global function. These functions are used to assist us during actual thermal sensing, as we want the robot to move in the direction where the hottest temperature is.

#### Thermal sensing driver
The primary focus of this driver is to combine the temperature sensing as well as motor movements. The main function of this file is the follow heat function. The first thing done is to update our global temperature pixel array to get the latest temperature readings. The averages of each column is then extracted using the average temperature function from our thermal sensor driver file. After acquiring the averages of each column, we run the array through conditionals to determine the direction the robot should move in. The logic we used is as follows:
If the max average is in columns 2-5, the robot should keep going forward
If the max average is in columns 0-1, the robot should turn left
If the max average is in columns 6-7, the robot should turn right
For turning, we decided to set turning wheel speed to be 27%. We initially tried higher values but found that if the wheel speed was too high, it would overturn, causing a back and forth stuttering motion as the robot did its best to center the max temperatures. 27% was fast enough that the robot did not feel too slow, but slow enough that the robot would not turn too fast. For moving forward we chose a wheel speed of 50%. 

#### Sonic sensor driver
![Alt text](Media/sonic_sensor.jpg?raw=true "Ultrasonic Distance Sensor - HC-SR04")

We use a standard HC-SR04 sonic sensor for object detection (so the robot doesn’t run into anything). This driver file essentially provides a setup function for the pins (similar to what had to be done for the motors) and an acquire distance function. Distance is acquired by setting the trigger pin to high for 10 microseconds, timing how long the pulse on the echo pin is held high, then using this time to determine the distance. The time is divided by 58 to get a reading in centimeters. It is entirely possibly to use an I2C enabled sonic sensor to better integrate with our already established I2C bus, however for the one sold on SparkFun, we could not find any I2C addresses so we decided to opt for the I/O version of the sensor instead (which is also much cheaper).

#### Virtual timer and gpio buttons functions
![Alt text](Media/microbit.jpg?raw=true "Microbit buttons")
Virtual timer is used to set up a global timer in the nrF52 and provides a function for reading what the value of the timer is. This timer value is used to help calculate distance for the sonic sensor file, and later on in the main script to determine when the robot should try to navigate around an object.

Gpio buttons is used to set up the A and B buttons found on the LED side of the Micro:Bit. We use these buttons to start and stop the following mode of the robot
