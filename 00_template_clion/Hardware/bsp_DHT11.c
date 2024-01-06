#include "bsp_DHT11.h"

void DHT11_init(){    
	rcu_periph_clock_enable(RCU_GPIOD);
	gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
    gpio_bit_set(GPIOD, GPIO_PIN_3);
}

u8 DHT11_read_data(u8* dat) {
	u16 count; 
	u8 i, j;
	// 主机: 总线拉低至少18ms
	// 主机发起
	// DHT = 0;
	gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
	gpio_output_options_set(GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
    DHT_RESET;
	delay_1ms(19);
	// DHT = 1;
    DHT_SET;
	count = 0;
    gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
	while(DHT == SET && count < 25) {
		count++;
		delay_1us(1);
	}
	if(count > 25) return 1;
	
	count = 0;
	while(DHT == RESET && count < 83) {
		count++;
		delay_1us(1);
	}
	if(count > 83) return 2;
	delay_1us(5);
	count = 0;
	while(DHT == SET && count < 87) {
		count++;
		delay_1us(1);
	}
	if(count > 87) return 3;
	delay_1us(5);
	// 开始接收数据
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 8; j++) {
			dat[i] <<= 1;
			// 54us低 23-27高电平 表示0
			// 54us低 68-74高电平 表示1
			count = 0;
			//等待变为高电平
			while(DHT == RESET && count <= 60) {
				count++;
				delay_1us(1);
			}
			if(count > 60) return 4;
			count = 0;
			// 等待变为低电平
			while(DHT == SET && count <= 80) {
				count++;
				delay_1us(1);
			}
			if(count > 33) {
				dat[i] |= 1;
			}
		}
	}
	if(dat[0] + dat[1] + dat[2] + dat[3] != dat[4]) {
		return 5;
	}
	return 0;
}
/**
 * @brief 温度读取转换函数
 * @param humidity
 * @param temperature
 * @return
 */
u8 DHT11_get_temperature(u8 *humidity, float *temperature) {
	u8 dat[5]={0};
	u8 ret;
	u8 retry = 0; 
	while((ret = DHT11_read_data(dat)) && retry < 10) {
		retry++;
	}
	// ret = DHT11_read_data(dat);
	*humidity = dat[0];// 获得正数部分
    
	*temperature = dat[2] & 0x7F;// 获得正数部分
	*temperature += (dat[3] * 0.1);//加上小数部分
	if(dat[2] & 0x80) {
		//表示负数
		*temperature *= -1;
	}
	return ret;
}