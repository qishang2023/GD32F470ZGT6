//
// Created by pc on 2024/1/5.
//

#ifndef INC_00_STM32F407_SPL_TEMP_BSP_USART1_H
#define INC_00_STM32F407_SPL_TEMP_BSP_USART1_H

#include "stm32f4xx.h"
#include "systick.h"
#include <stdio.h>

//�������
#define TX_RCU RCU_GPIOA
#define TX_PORT GPIOA
#define TX_PIN GPIO_PIN_9
#define TX_ALT GPIO_AF_7
//�������
#define RX_RCU RCU_GPIOA
#define RX_PORT GPIOA
#define RX_PIN GPIO_PIN_10
#define RX_ALT GPIO_AF_7
//��������
#define USART_RCU RCU_USART0
#define USART_NUM USART1
#define USART_NVIC_IRQ USART0_IRQn
//�жϴ�����
#define USART_IRQHandler USART1_IRQHandler

//1.��ʼ�����÷���
void usart1_rx_tx_config(uint32_t baudval);
//2.���͹���
//���͵����ֽ�
void send_data(uint8_t data);
//�����ַ���
void send_string(char data[]);
//3.����֮������������ṩ��Ӧ�ò�
extern void recv_data_callback(uint8_t buffer[],uint16_t length);
#endif //INC_00_STM32F407_SPL_TEMP_BSP_USART1_H
