//
// Created by pc on 2024/1/6.
//

#include "bsp_timer.h"
#include "stdio.h"

void bsp_timer_init(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Period = SystemCoreClock / 10000 - 1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 10000-1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
    TIM_ClearFlag(TIM6,TIM_FLAG_Update);
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM6,ENABLE);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void TIM6_DAC_IRQHandler(void){
    // 清除中断标志位
    static uint32_t cnt;
    if(TIM_GetFlagStatus(TIM6,TIM_FLAG_Update)== SET){
        TIM_ClearFlag(TIM6,TIM_FLAG_Update);
        printf("%lu\n",cnt++);
    }

}
