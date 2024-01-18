
#include "main.h"
#include "bsp_usart1.h"
#include "bsp_DHT11.h"
#include "bsp_lcd_OLED.h"
#include "bsp_lcd_OLED_init.h"
#include "bsp_pic_OLED.h"
#include "bsp_timer.h"
#include "bsp_key.h"
#include "bsp_RTC.h"
#include "bsp_PWM.h"
#include "oled.h"
#include "bsp_hard_i2c.h"

void recv_data_callback(uint8_t buffer[],uint16_t length){
    buffer[length] = '\0';
    send_string(buffer);
}

int main(void) {
    char h,str[30];
    float t;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    systick_config();
    usart1_rx_tx_config(115200);
    DHT11_init();
    EXTI_config();
//    bsp_timer_init();
    bsp_i2c_config();
//    OLED_Init();
    LCD_Init();
    LCD_Fill(0,0,240,320,WHITE);
    RTC_config();
    rtc_clock.hour = 23;
    rtc_clock.minute = 59;
    rtc_clock.second = 50;
    rtc_clock.day = 31;
    rtc_clock.month = 12;
    rtc_clock.year = 2024;
    RTC_set_time();
    uint8_t p[7] = {0x11,0x12,0x13,0x14,0x15,0x16,0x17};
    uint8_t r = bsp_hard_I2C_write(0x51,0x02,p,7);
    send_data('a');
    /* Infinite loop */
    while (1) {
        DHT11_get_temperature(&h, &t);
        bsp_hard_I2C_read(0x51,0x02,p,7);
        LCD_ShowIntNum(10,220,p[0],2,BLACK,WHITE,24);
        RTC_read_time();
        sprintf(str,"%d-%2d-%2d ", rtc_clock.year, rtc_clock.month, rtc_clock.day);
        LCD_ShowString(10, 110, str, BLACK, WHITE, 16, 0);
        sprintf(str,"%02d:%02d:%02d", rtc_clock.hour, rtc_clock.minute, rtc_clock.second);
        LCD_ShowString(10, 130, str, BLACK, WHITE, 16, 0);
        LCD_ShowPicture(40, 40, 60, 59, gImage_1);
        sprintf(str,"H:%d%%,t:%.1f   \0",h,t);
        LCD_ShowString(10, 180, (uint8_t*)str, RED, WHITE, 24, 0);
//        OLED_ShowString(8,16,"ZHONGJINGYUAN",16,1);
//        OLED_Refresh();
        delay_1ms(1000);
    }
}




