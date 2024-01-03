#include "bsp_led.h"

//ÿһ������������Ҫ������
typedef struct{
	rcu_periph_enum periph;
	uint32_t port;
	uint32_t pin;
}LED_CONFIG;

//4��led�Ƶ�����
LED_CONFIG arr[4]={
	{RCU_GPIOE,GPIOE,GPIO_PIN_3},
	{RCU_GPIOD,GPIOD,GPIO_PIN_7},
	{RCU_GPIOG,GPIOG,GPIO_PIN_3},
	{RCU_GPIOA,GPIOA,GPIO_PIN_5}
};

//�Ƶĸ���
#define LED_NUM (sizeof(arr)/sizeof(arr[0]))


//LED�Ƴ�ʼ��
void leds_config(void){
	for(int i=0;i<LED_NUM;i++){
		//1.��ʱ��
		rcu_periph_clock_enable(arr[i].periph);
		//2.����gpio
		gpio_mode_set(arr[i].port,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,arr[i].pin);
		gpio_output_options_set(arr[i].port,GPIO_OTYPE_PP,GPIO_OSPEED_2MHZ,arr[i].pin);
	}
}

//��ĳһ����
void turn_on_led(LED led){
	if(led<0||led>3){
		return;
	}
	gpio_bit_set(arr[led].port,arr[led].pin);
}

//�ر�ĳһ����
void turn_off_led(LED led){
	if(led<0||led>3){
		return;
	}
	gpio_bit_reset(arr[led].port,arr[led].pin);
}