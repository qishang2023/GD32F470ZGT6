
#ifndef INC_00_TEMPLATE_CLION_BSP_SOFT_I2C_H
#define INC_00_TEMPLATE_CLION_BSP_SOFT_I2C_H

#include "systick.h"
#include "gd32f4xx.h"

void SoftI2C_init();
uint8_t SoftI2C_write(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len);
uint8_t SoftI2C_read(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len);
#endif //INC_00_TEMPLATE_CLION_BSP_SOFT_I2C_H
