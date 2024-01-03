//
// Created by pc on 2024/1/3.
//

#ifndef INC_00_TEMPLATE_CLION_BSP_RTC_H
#define INC_00_TEMPLATE_CLION_BSP_RTC_H

#include "gd32f4xx.h"
#include <stdio.h>
#include "lcd.h"

// ʮλȡ������4λ + ��λ (�õ�BCD��)
#define WRITE_BCD(val) 	((val / 10) << 4) + (val % 10)
// ����4λ����10 + ����λ (�õ�10������)
#define READ_BCD(val) 	(val >> 4) * 10 + (val & 0x0F)

typedef struct{
    uint8_t second, minute, hour, day, month, week, C;
    uint16_t year;
}RTC_clock;

extern RTC_clock rtc_clock;

void RTC_config();
void RTC_read();
void RTC_set_time();

#endif //INC_00_TEMPLATE_CLION_BSP_RTC_H
