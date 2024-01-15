//
// Created by pc on 2024/1/15.
//

#ifndef INC_00_TEMPLATE_CLION_BSP_HARD_I2C_H
#define INC_00_TEMPLATE_CLION_BSP_HARD_I2C_H

#include "gd32f4xx.h"
void bsp_hard_i2c_init();
uint8_t bsp_hard_I2C_write(uint8_t addr, uint8_t reg, const uint8_t *data, uint32_t len);
uint8_t bsp_hard_I2C_read(uint8_t addr, uint8_t reg, uint8_t *data, uint32_t len);
#endif //INC_00_TEMPLATE_CLION_BSP_HARD_I2C_H
