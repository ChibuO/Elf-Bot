#include "I2C_functions.h"
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"

uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr, const nrf_twi_mngr_t* i2c_manager) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);

  return rx_buf;
}

void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data, const nrf_twi_mngr_t* i2c_manager) {
  //Note: there should only be a single two-byte transfer to be performed
  uint8_t tx_buf[2] = { reg_addr, data };
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, tx_buf, 2, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);
}

void experimental_write(uint8_t i2c_addr, uint8_t data, const nrf_twi_mngr_t* i2c_manager) {
  uint8_t tx_buf = data;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, (int)&tx_buf, 1, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);
}

uint8_t experimental_read(uint8_t i2c_addr, const nrf_twi_mngr_t* i2c_manager) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr,(int)&i2c_addr, 1, 0)
  };
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 2, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);

  return rx_buf;
}