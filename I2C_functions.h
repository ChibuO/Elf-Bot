#pragma once

#include "nrf_twi_mngr.h"

// Helper function to perform a 1-byte I2C read of a given register
//
// i2c_addr - address of the device to read from
// reg_addr - address of the register within the device to read
//
// returns 8-bit read value
uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, const nrf_twi_mngr_t* i2c_manager);

// Helper function to perform a 1-byte I2C write of a given register
//
// i2c_addr - address of the device to write to
// reg_addr - address of the register within the device to write
void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data, const nrf_twi_mngr_t* i2c_manager);


void experimental_write(uint8_t i2c_addr, uint8_t data, const nrf_twi_mngr_t* i2c_manager);
uint8_t experimental_read(uint8_t i2c_addr, const nrf_twi_mngr_t* i2c_manager);