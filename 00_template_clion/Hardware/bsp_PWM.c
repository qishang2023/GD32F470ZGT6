#include "bsp_PWM.h"

#define PERIOD (SystemCoreClock / 10000 - 1)

/*
 * 通用定时器和基本定时器区别：
 *  1.也需要分频
 *  2.也需要周期
 *  3.多了输出比较寄存器值(占空比计数值)
 *  4.不需要中断
 */
void PWM_config() {
    /*************** GPIO config **************/
    // 配置时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    // 配置GPIO模式
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    // 配置GPIO输出
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
    // 配置复用功能
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_5);

    /*************** Timer config *************/
    // 时钟配置
    rcu_periph_clock_enable(RCU_TIMER1);
    // 复位定时器
    timer_deinit(TIMER1); // 重置定时器外设（类似重启系统）
    // 倍频配置
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);

    // 初始化定时器
    timer_parameter_struct tps;
    tps.prescaler = 10 - 1;
    tps.alignedmode = TIMER_COUNTER_EDGE;
    tps.counterdirection = TIMER_COUNTER_UP;
    tps.period = PERIOD;
    tps.clockdivision = TIMER_CKDIV_DIV1;
    tps.repetitioncounter = 0U;
    timer_init(TIMER1, &tps);

    // 配置输出通道
    timer_oc_parameter_struct tops;
    /*通用定时器通道配置*/
    tops.outputstate = TIMER_CCX_ENABLE;// 开启通道输出
    tops.ocpolarity = TIMER_OC_POLARITY_HIGH;
    tops.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    /*高级定时器互补输出通道配置*/
    tops.outputnstate = TIMER_CCXN_DISABLE;
    tops.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    tops.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER1, TIMER_CH_0, &tops);

    // 输出模式配置
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 0);
    // 初始化
    timer_enable(TIMER1);
}
/********************************************************************************
* @author: Qi Shang
* @brief: 定时器占空比更新
* @param: pulse: 占空比0~100
* @retval: None
********************************************************************************/

void PWM_update(uint8_t pulse) {
    // 配置输出的占空比
    uint32_t i = PERIOD * pulse / 100;
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, i);
}
