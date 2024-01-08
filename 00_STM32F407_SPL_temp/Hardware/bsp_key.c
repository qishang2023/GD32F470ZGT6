//
// Created by pc on 2024/1/8.
//

#include "bsp_key.h"

void EXTI_config() {
    /********************* GPIO config *********************/
    // 时钟初始化
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    // 配置GPIO模式
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    /********************* EXTI config *********************/
    // 时钟配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    // 配置中断线
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource4);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource5);
    // 中断初始化
    // 配置中断源
    // 中断初始化
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_Init(&NVIC_InitStruct);
    // EXTI初始化
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line4|EXTI_Line5;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    // 配置中断优先级
    // 使能中断
    // 清除中断标志位
    EXTI_ClearITPendingBit(EXTI_Line0);
    EXTI_ClearITPendingBit(EXTI_Line1);
    EXTI_ClearITPendingBit(EXTI_Line4);
    EXTI_ClearITPendingBit(EXTI_Line5);
}

// 中断函数
void EXTI0_IRQHandler(void) {
    static uint64_t pre;
    static FlagStatus pre_state = RESET;
    // 清除中断标志位
    if (SET == EXTI_GetITStatus(EXTI_Line0)) {
        uint64_t current_tick = get_tick();
        if (current_tick - pre > 20000) {
            pre = current_tick;
            FlagStatus state = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0);
            if (SET == state && RESET == pre_state) {
                // 如果当前为高电平，上一次为低电平，按下
                pre_state = state;
                printf("up \r\n");
            } else if (RESET == state && SET == pre_state) {
                // 如果当前为低电平，上一次为高电平，抬起
                pre_state = state;
                printf("down \r\n");
            }
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}