#include "bsp_lcd_OLED.h"
#include "bsp_lcd_OLED_init.h"
#include "bsp_DHT11.h"
#include "bsp_RTC.h"
#include "bsp_USART.h"
#include "bsp_key.h"
#include "bsp_timer.h"
#include "bsp_BUZZER.h"
//#include "bsp_pic_OLED.h"
#include "bsp_soft_i2c.h"
#include "bsp_ADC.h"
#include "bsp_hard_i2c.h"
#include "pic.h"

int main(void) {
    u8 humidity = 0, ret = 0;
    int j = 1;
    float temperature = 0;
    char str[30];
    // 系统时钟初始化
    systick_config();
    RTC_config();
//    SoftI2C_init();
    bsp_ADC0_C16_Init();
    USART_config();
    bsp_hard_i2c_init();
//    EXTI_config();
//    bsp_timer5_init();
    bsp_hard_spi_config();
    LCD_Init(); // LCD初始化
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
    rtc_clock.year = 2023;
    rtc_clock.month = 12;
    rtc_clock.day = 21;
    rtc_clock.week = 6;
    rtc_clock.hour = 23;
    rtc_clock.minute = 59;
    rtc_clock.second = 55;
    RTC_set_time();
    DHT11_init();
    uint8_t p[7] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    uint8_t r = bsp_hard_I2C_write(0x51, 0x02, p, 7);
    printf("start...\n");
    bsp_dma_spi_config();
    while (1) {
//        float cpu_t = adc_temp_get();
//        bsp_hard_I2C_read(0x51,0x02,p,7);
//        LCD_ShowIntNum(10,220,p[0],2,BLACK,WHITE,24);
//        sprintf(str, "CPU_T:%.1fC", cpu_t);
//        LCD_ShowString(10, 180, (u8 *)str, BLACK, WHITE, 16, 0);
//        RTC_read();
//        sprintf(str, "%d-%d-%d %02d:%02d:%02d", rtc_clock.year, rtc_clock.month, rtc_clock.day,
//                rtc_clock.hour, rtc_clock.minute, rtc_clock.second);
//        LCD_ShowString(10, 40, (u8 *)str, BLACK, WHITE, 16, 0);
//        ret = DHT11_get_temperature(&humidity, &temperature);
//        sprintf(str, "H:%d%% T:%.1fC   \0", (int) humidity, temperature);
//        LCD_ShowString(10, 100, (u8 *)str, RED, WHITE, 24, 0);
//        sprintf(str, "ret = %d", (int) ret);
//        LCD_ShowString(10, 70,(u8 *)str, RED, WHITE, 24, 0);
        LCD_ShowPicture(0, 0, 240, 240, gImage_1);
        delay_1ms(1000);
    }
}
