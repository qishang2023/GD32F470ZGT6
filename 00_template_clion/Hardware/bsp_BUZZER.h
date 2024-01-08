//
// Created by pc on 2024/1/8.
//

#ifndef INC_00_TEMPLATE_CLION_BSP_BUZZER_H
#define INC_00_TEMPLATE_CLION_BSP_BUZZER_H

#include "gd32f4xx.h"
#include "systick.h"
void bsp_BUZZER_config();
void bsp_BUZZER_play(uint8_t pos);
void bsp_BUZZER_stop();
void bsp_BUZZER_music();
#endif //INC_00_TEMPLATE_CLION_BSP_BUZZER_H
