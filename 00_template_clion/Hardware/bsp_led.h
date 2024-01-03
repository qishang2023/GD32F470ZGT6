#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "gd32f4xx.h"

//#define LED1 0
//#define LED2 0
//#define LED3 0
//#define LED4 0

//LED的枚举
typedef enum {
	LED1 = 0, 
	LED2,
	LED3,
	LED4
} LED;

//LED灯初始化
void leds_config(void);

//打开某一个灯
void turn_on_led(LED led);

//关闭某一个灯
void turn_off_led(LED led);

#endif

