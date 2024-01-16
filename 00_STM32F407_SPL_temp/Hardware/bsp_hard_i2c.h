#ifndef INC_00_STM32F407_SPL_TEMP_BSP_HARD_I2C_H
#define INC_00_STM32F407_SPL_TEMP_BSP_HARD_I2C_H

#include "stm32f4xx.h"
#include "systick.h"

void bsp_i2c_config();
uint8_t bsp_hard_I2C_write(uint8_t addr, uint8_t reg, const uint8_t *data, uint32_t len);
uint8_t bsp_hard_I2C_read(uint8_t addr, uint8_t reg, uint8_t *data, uint32_t len);

#endif //INC_00_STM32F407_SPL_TEMP_BSP_HARD_I2C_H
