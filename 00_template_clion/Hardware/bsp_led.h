#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "gd32f4xx.h"

//#define LED1 0
//#define LED2 0
//#define LED3 0
//#define LED4 0

//LED��ö��
typedef enum {
	LED1 = 0, 
	LED2,
	LED3,
	LED4
} LED;

//LED�Ƴ�ʼ��
void leds_config(void);

//��ĳһ����
void turn_on_led(LED led);

//�ر�ĳһ����
void turn_off_led(LED led);

#endif

