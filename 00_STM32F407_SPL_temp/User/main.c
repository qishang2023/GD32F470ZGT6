
#include "main.h"
#include "bsp_usart1.h"
#include "bsp_DHT11.h"
#include "lcd.h"
#include "lcd_init.h"
#include "pic.h"
#include "bsp_timer.h"

void recv_data_callback(uint8_t buffer[],int16_t length){
    buffer[length] = '\0';
    send_string(buffer);
}

int main(void) {
    uint8_t h,ret;
    float t;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    systick_config();
    usart1_rx_tx_config(115200);
    DHT11_init();
    bsp_timer_init();
    LCD_Init();
    LCD_Fill(0,0,240,320,WHITE);
    /* Infinite loop */
    while (1) {
//        LCD_ShowPicture(0, 0, 240, 240, gImage_1);
//        delay_1ms(1000);

        LCD_ShowPicture(40, 40, 60, 59, gImage_1);
        delay_1ms(120);
        LCD_ShowPicture(40, 40, 60, 58, gImage_2);
        delay_1ms(120);
        LCD_ShowPicture(40, 40, 60, 58, gImage_3);
        delay_1ms(120);
        LCD_ShowPicture(40, 40, 60, 58, gImage_4);
        delay_1ms(120);
        LCD_ShowPicture(40, 40, 60, 58, gImage_5);
//        ret = DHT11_get_temperature(&h, &t);
//        printf("ret: %d\n", ret);
//        printf("h: %d\n", h);
//        printf("t: %.1f\n", t);
        delay_1ms(120);
    }
}




