//
// Created by pc on 2024/1/17.
//

#ifndef INC_00_TEMPLATE_CLION_BSP_SPI_H
#define INC_00_TEMPLATE_CLION_BSP_SPI_H

#include "gd32f4xx.h"

//SCL:PB3
#define SPI_SCL_RCU  RCU_GPIOB
#define SPI_SCL_PORT GPIOB
#define SPI_SCL_PIN  GPIO_PIN_3
//MOSI:PB5
#define SPI_MOSI_RCU  RCU_GPIOB
#define SPI_MOSI_PORT GPIOB
#define SPI_MOSI_PIN  GPIO_PIN_5

//FSO(MISO):PB4
#define SPI_MISO_RCU  RCU_GPIOB
#define SPI_MISO_PORT GPIOB
#define SPI_MISO_PIN  GPIO_PIN_4

//SPI配置
#define SPI_RCU 			RCU_SPI0
#define SPI_NUM				SPI0


//SCL
#define SPI_SCL_L() gpio_bit_reset(SPI_SCL_PORT,SPI_SCL_PIN)
#define SPI_SCL_H() gpio_bit_set(SPI_SCL_PORT,SPI_SCL_PIN)
//MOSI
#define SPI_MOSI_L() gpio_bit_reset(SPI_MOSI_PORT,SPI_MOSI_PIN)
#define SPI_MOSI_H() gpio_bit_set(SPI_MOSI_PORT,SPI_MOSI_PIN)

//FS0(MISO)
#define SPI_MISO_READ()    gpio_input_bit_get(SPI_MISO_PORT,SPI_MISO_PIN)


#ifndef u8
#define u8 uint8_t
#endif
#ifndef u16
#define u16 uint16_t
#endif
#ifndef u32
#define u32 uint32_t
#endif

//初始化spi
void bsp_hard_spi_config(void);

//写入
void bsp_hard_spi_write(u8 dat);

//读取
u8 bsp_hard_spi_read(void);

#endif //INC_00_TEMPLATE_CLION_BSP_SPI_H
