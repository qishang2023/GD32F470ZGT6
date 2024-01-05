/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_usart1.h"

void recv_data_callback(uint8_t buffer[],int16_t length){
    buffer[length] = '\0';
    send_string(buffer);
}

int main(void) {
    systick_config();
    usart1_rx_tx_config(115200);
    /* Infinite loop */
    while (1) {
        delay_1ms(1000);
    }
}




