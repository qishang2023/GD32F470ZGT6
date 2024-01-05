//
// Created by pc on 2024/1/5.
//

#ifndef INC_00_STM32F407_SPL_TEMP_BSP_DHT11_H
#define INC_00_STM32F407_SPL_TEMP_BSP_DHT11_H
#include "systick.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define  DHT_SET   GPIO_SetBits(GPIOD, GPIO_Pin_3)
#define  DHT_RESET GPIO_ResetBits(GPIOD, GPIO_Pin_3)

#define  DHT      GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)

void DHT11_init();
u8 DHT11_read_data(u8* dat);
u8 DHT11_get_temperature(u8 *humidity, float *temperature);

#endif //INC_00_STM32F407_SPL_TEMP_BSP_DHT11_H
