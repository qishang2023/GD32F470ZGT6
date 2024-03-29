#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "gd32f4xx.h"
#include "systick.h"
#include "gd32f4xx_misc.h"
#include "bsp_SPI.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif


//-----------------LCD端口定义---------------- 

#define LCD_SCLK_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_3)//SCL=SCLK
#define LCD_SCLK_Set() gpio_bit_set(GPIOB,GPIO_PIN_3)

#define LCD_MOSI_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_5)//SDA=MOSI
#define LCD_MOSI_Set() gpio_bit_set(GPIOB,GPIO_PIN_5)

#define LCD_RES_Clr()  gpio_bit_reset(GPIOB,GPIO_PIN_8)//RES
#define LCD_RES_Set()  gpio_bit_set(GPIOB,GPIO_PIN_8)

#define LCD_DC_Clr()   gpio_bit_reset(GPIOA,GPIO_PIN_15)//DC
#define LCD_DC_Set()   gpio_bit_set(GPIOA,GPIO_PIN_15)
 		     
#define LCD_CS_Clr()   gpio_bit_reset(GPIOB,GPIO_PIN_4)//CS
#define LCD_CS_Set()   gpio_bit_set(GPIOB,GPIO_PIN_4)

#define LCD_BLK_Clr()  gpio_bit_reset(GPIOA,GPIO_PIN_4)//BLK
#define LCD_BLK_Set()  gpio_bit_set(GPIOA,GPIO_PIN_4)




void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




