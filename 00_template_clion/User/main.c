#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"
#include "bsp_DHT11.h"
#include "bsp_RTC.h"

void keys_config(void){
    //1.打开时钟
    rcu_periph_clock_enable(RCU_GPIOD);
    //2.配置GPIO模式
    gpio_mode_set(GPIOD,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_0|GPIO_PIN_1);
}

int main(void) {
    u8 humidity = 0, ret = 0;
    float temperature = 0;
    u8 str[30], dat[5];
    // 系统时钟初始化
    systick_config();
    RTC_config();
//    keys_config();
//    leds_config();
    // IO引脚初始化
//	 GPIO_config();
    LCD_Init(); // LCD初始化
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    DHT11_init();
    while (1) {
        RTC_read();
        ret = DHT11_get_temperature(&humidity, &temperature);
        sprintf(str, "H:%d%% T:%.1fC   \0", (int) humidity, temperature);
        LCD_ShowString(10, 100, str, RED, WHITE, 24, 0);
        sprintf(str, "ret = %d", (int) ret);
        LCD_ShowString(10, 70, str, RED, WHITE, 24, 0);
        delay_1ms(1000);			//获取按键电平状态
    }
}
