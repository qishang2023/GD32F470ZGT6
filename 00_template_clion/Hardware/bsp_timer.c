//
// Created by pc on 2024/1/6.
//

#include "bsp_timer.h"
#include "stdio.h"
/**
 * @brief ��ʼ��������ʱ��5
 */
void bsp_timer5_init(){
    // ʱ������
    rcu_periph_clock_enable(RCU_TIMER5);

    // ��λ��ʱ��
    timer_deinit(TIMER5);

    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_parameter_struct tps;
    timer_struct_para_init(&tps);
    tps.prescaler = 10000 - 1; // ��Ƶϵ��  240 000 000
    tps.period = SystemCoreClock / 10000 - 1; // ����

    timer_init(TIMER5, &tps);
    timer_enable(TIMER5);
    nvic_irq_enable(TIMER5_DAC_IRQn, 2, 2);
    timer_interrupt_enable(TIMER5, TIMER_INT_UP);
}
/**
 * @brief ��ʱ��5�жϷ�����
 */
void TIMER5_DAC_IRQHandler(){
    // ����жϱ�־λ
    static uint32_t cnt;
    if(timer_interrupt_flag_get(TIMER5,TIMER_INT_FLAG_UP)== SET){
        timer_interrupt_flag_clear(TIMER5,TIMER_INT_FLAG_UP);
        printf("%lu\n",cnt++);
    }

}