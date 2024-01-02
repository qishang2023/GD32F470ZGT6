#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"

/**
LED硬件对应关系
LED1	PE3
LED2	PD7
LED3	PG3
LED4	PA5

点亮LED1
**/
static void GPIO_config()
{
	// 1. 时钟初始化
	rcu_periph_clock_enable(RCU_GPIOE);
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_GPIOG);
	rcu_periph_clock_enable(RCU_GPIOA);
	// 2. 配置GPIO 输入输出模式
	gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
	gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
	gpio_mode_set(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
	// 3. 配置GPIO 模式的操作方式
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_7);
	gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_5);
}

int main(void)
{
	u8 i;
	float t=0.0;
	u8 j;
	// 系统时钟初始化
	systick_config();
	// IO引脚初始化
	// GPIO_config();
	LCD_Init(); // LCD初始化
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);

	while (1)
	{
		LCD_ShowChinese(0, 0, "中景园电子", RED, WHITE, 32, 0);
		LCD_ShowString(0, 40, "LCD_W:", RED, WHITE, 16, 0);
		LCD_ShowIntNum(48, 40, LCD_W, 3, RED, WHITE, 16);
		LCD_ShowString(80, 40, "LCD_H:", RED, WHITE, 16, 0);
		LCD_ShowIntNum(128, 40, LCD_H, 3, RED, WHITE, 16);
		LCD_ShowString(80, 40, "LCD_H:", RED, WHITE, 16, 0);
		LCD_ShowString(0, 70, "Increaseing Nun:", RED, WHITE, 16, 0);
		LCD_ShowFloatNum1(128, 70, t, 4, RED, WHITE, 16);
		t += 0.11;
		for (j = 0; j < 3; j++)
		{
			for (i = 0; i < 6; i++)
			{
				LCD_ShowPicture(40 * i, 120 + j * 40, 40, 40, gImage_1);
			}
		}
		// gpio_bit_set(GPIOE, GPIO_PIN_3);
		// delay_1ms(200);
		// gpio_bit_reset(GPIOE, GPIO_PIN_3);
		// gpio_bit_set(GPIOD, GPIO_PIN_7);
		// delay_1ms(200);
		// gpio_bit_reset(GPIOD, GPIO_PIN_7);
		// gpio_bit_set(GPIOG, GPIO_PIN_3);
		// delay_1ms(200);
		// gpio_bit_reset(GPIOG, GPIO_PIN_3);
		// gpio_bit_set(GPIOA, GPIO_PIN_5);
		// delay_1ms(200);
		// gpio_bit_reset(GPIOA, GPIO_PIN_5);
		// delay_1ms(1000);
	}
}
