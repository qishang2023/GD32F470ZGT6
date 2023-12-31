#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"

/**
LEDӲ����Ӧ��ϵ
LED1	PE3
LED2	PD7
LED3	PG3
LED4	PA5

����LED1
**/
static void GPIO_config() {
	// 1. ʱ�ӳ�ʼ��
	rcu_periph_clock_enable(RCU_GPIOE);
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_GPIOG);
	rcu_periph_clock_enable(RCU_GPIOA);
	// 2. ����GPIO �������ģʽ
	gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
	gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
	gpio_mode_set(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
	// 3. ����GPIO ģʽ�Ĳ�����ʽ
	gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_7);
	gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO_PIN_5);
}

int main(void)
{
	// ϵͳʱ�ӳ�ʼ��
    systick_config();
	// IO���ų�ʼ��
	GPIO_config();

    while(1) {
		gpio_bit_set(GPIOE, GPIO_PIN_3);
		delay_1ms(200);
		gpio_bit_reset(GPIOE, GPIO_PIN_3);
		gpio_bit_set(GPIOD, GPIO_PIN_7);
		delay_1ms(200);
		gpio_bit_reset(GPIOD, GPIO_PIN_7);
		gpio_bit_set(GPIOG, GPIO_PIN_3);
		delay_1ms(200);
		gpio_bit_reset(GPIOG, GPIO_PIN_3);
		gpio_bit_set(GPIOA, GPIO_PIN_5);
		delay_1ms(200);
		gpio_bit_reset(GPIOA, GPIO_PIN_5);
		delay_1ms(1000);
    }
}
