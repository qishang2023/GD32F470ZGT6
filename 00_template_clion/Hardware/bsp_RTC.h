//
// Created by pc on 2024/1/3.
//

#ifndef INC_00_TEMPLATE_CLION_BSP_RTC_H
#define INC_00_TEMPLATE_CLION_BSP_RTC_H

#include "gd32f4xx.h"
#include <stdio.h>
#include "lcd.h"

// 十位取出左移4位 + 个位 (得到BCD数)
#define WRITE_BCD(val) 	((val / 10) << 4) + (val % 10)
// 将高4位乘以10 + 低四位 (得到10进制数)
#define READ_BCD(val) 	(val >> 4) * 10 + (val & 0x0F)

void RTC_config();
void RTC_read();

#endif //INC_00_TEMPLATE_CLION_BSP_RTC_H
