#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"
#include "bsp_DHT11.h"
#include "bsp_led.h"

void keys_config(void){
    //1.打开时钟
    rcu_periph_clock_enable(RCU_GPIOD);
    //2.配置GPIO模式
    gpio_mode_set(GPIOD,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_0|GPIO_PIN_1);

    //2.配置GPIO模式
    //gpio_mode_set(GPIOD,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_1);
}

int main(void) {
    u8 humidity = 0, ret = 0;
    float temperature = 0;
    u8 str[30], dat[5];
    // 系统时钟初始化
    systick_config();
    keys_config();
    leds_config();
    // IO引脚初始化
//	 GPIO_config();
    LCD_Init(); // LCD初始化
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    DHT11_init();
    FlagStatus key1_pre_state = SET;
    FlagStatus key2_pre_state = SET;
    while (1) {
        ret = DHT11_get_temperature(&humidity, &temperature);
        sprintf(str, "H:%d%% T:%.1fC   \0", (int) humidity, temperature);
        LCD_ShowString(10, 100, str, RED, WHITE, 24, 0);
        sprintf(str, "ret = %d", (int) ret);
        LCD_ShowString(10, 70, str, RED, WHITE, 24, 0);
        delay_1ms(50);			//获取按键电平状态
        FlagStatus state = gpio_input_bit_get(GPIOD,GPIO_PIN_0);
        //按键1 触发开始充电
        if(key1_pre_state==SET&&state==RESET){//按下
            //电量LED1
            //gpio_bit_set(GPIOE,GPIO_PIN_3);
            turn_on_led(1);
            //开始流水灯
            key1_pre_state = RESET;
        }else if(key1_pre_state==RESET&&state==SET){//松开
            key1_pre_state=SET;
        }

        //按键2 触发停止充电
        state = gpio_input_bit_get(GPIOD,GPIO_PIN_1);
        if(key2_pre_state==SET&&state==RESET){//按下
            //停止充电
            //gpio_bit_set(GPIOD,GPIO_PIN_7);
            //gpio_bit_toggle(GPIOD,GPIO_PIN_7);
            turn_off_led(1);
            key2_pre_state = RESET;
        }else if(key2_pre_state==RESET&&state==SET){//松开
            key2_pre_state=SET;
        }
    }
}
