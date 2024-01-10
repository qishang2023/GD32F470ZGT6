
#include "main.h"
#include "bsp_usart1.h"
#include "bsp_DHT11.h"
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"
#include "bsp_timer.h"
#include "bsp_key.h"
#include "bsp_RTC.h"

void recv_data_callback(uint8_t buffer[],int16_t length){
    buffer[length] = '\0';
    send_string(buffer);
}

int main(void) {
    uint8_t h,str[30];
    float t;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    systick_config();
    usart1_rx_tx_config(115200);
    DHT11_init();
    EXTI_config();
//    bsp_timer_init();
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
    send_data('a');
    /* Infinite loop */
    while (1) {
        DHT11_get_temperature(&h, &t);
        RTC_read_time();
        sprintf(str,"%d-%d-%d ", rtc_clock.year, rtc_clock.month, rtc_clock.day);
        LCD_ShowString(10, 110, str, BLACK, WHITE, 16, 0);
        sprintf(str,"%02d:%02d:%02d", rtc_clock.hour, rtc_clock.minute, rtc_clock.second);
        LCD_ShowString(10, 130, str, BLACK, WHITE, 16, 0);
        LCD_ShowPicture(40, 40, 60, 59, gImage_1);
        delay_1ms(200);
        LCD_ShowPicture(40, 40, 60, 58, gImage_2);
        delay_1ms(200);
        LCD_ShowPicture(40, 40, 60, 58, gImage_3);
        delay_1ms(200);
        LCD_ShowPicture(40, 40, 60, 58, gImage_4);
        delay_1ms(200);
        LCD_ShowPicture(40, 40, 60, 58, gImage_5);
        sprintf(str,"H:%d%%,t:%.1f   \0",h,t);
        LCD_ShowString(10, 180, str, BLACK, WHITE, 24, 0);
//        printf("ret: %d\n", ret);
//        printf("h: %d\n", h);
//        printf("t: %.1f\n", t);
        delay_1ms(180);
    }
}




