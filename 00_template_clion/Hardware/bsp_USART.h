//
// Created by pc on 2024/1/3.
//

#ifndef INC_00_TEMPLATE_CLION_BSP_USART_H
#define INC_00_TEMPLATE_CLION_BSP_USART_H

#include "gd32f4xx.h"
#include "stdio.h"



void USART_config();
void send_string(char *data);
void send_data(uint8_t data);
#endif //INC_00_TEMPLATE_CLION_BSP_USART_H
