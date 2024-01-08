//
// Created by pc on 2024/1/6.
//

#include "bsp_timer.h"
#include "stdio.h"
/**
 * @brief 初始化基本定时器5
 */
void bsp_timer5_init(){
    // 时钟配置
    rcu_periph_clock_enable(RCU_TIMER5);

    // 复位定时器
    timer_deinit(TIMER5);

    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_parameter_struct tps;
    timer_struct_para_init(&tps);
    tps.prescaler = 10000 - 1; // 分频系数  240 000 000
    tps.period = SystemCoreClock / 10000 - 1; // 周期

    timer_init(TIMER5, &tps);
    timer_enable(TIMER5);
    nvic_irq_enable(TIMER5_DAC_IRQn, 2, 2);
    timer_interrupt_enable(TIMER5, TIMER_INT_UP);
}
/**
 * @brief 定时器5中断服务函数
 */
void TIMER5_DAC_IRQHandler(){
    // 清除中断标志位
    static uint32_t cnt;
    if(timer_interrupt_flag_get(TIMER5,TIMER_INT_FLAG_UP)== SET){
        timer_interrupt_flag_clear(TIMER5,TIMER_INT_FLAG_UP);
        printf("%lu\n",cnt++);
    }

}