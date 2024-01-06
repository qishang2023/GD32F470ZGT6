//
// Created by pc on 2024/1/6.
//

#include "bsp_key.h"

void EXTI_config() {
    uint32_t extix = EXTI_0; // 哪个中断
    uint32_t extix_irq = EXTI0_IRQn;
    uint32_t extix_trig = EXTI_TRIG_BOTH; // 中断触发条件

    uint32_t extix_src_port = EXTI_SOURCE_GPIOA; // 中断触发源，哪个引脚
    uint32_t extix_src_pin = EXTI_SOURCE_PIN0;

    uint32_t extix_io_rcu = RCU_GPIOA;
    uint32_t extix_io_port = GPIOA;
    uint32_t extix_io_pin = GPIO_PIN_0;

    /********************* GPIO config *********************/
    // 时钟初始化
    rcu_periph_clock_enable(extix_io_rcu);
    rcu_periph_clock_enable(RCU_GPIOD);
    // 配置GPIO模式
    gpio_mode_set(extix_io_port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, extix_io_pin);
    gpio_mode_set(RCU_GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE,
                  GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);

    /********************* EXTI config *********************/
    // 时钟配置
    rcu_periph_clock_enable(RCU_SYSCFG);
    // 配置中断源
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN0);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN1);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN4);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN5);
    // 中断初始化
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_init(EXTI_1, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_init(EXTI_4, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    // 配置中断优先级
    nvic_irq_enable(EXTI0_IRQn, 1, 1);
    nvic_irq_enable(EXTI1_IRQn, 1, 1);
    nvic_irq_enable(EXTI4_IRQn, 1, 1);
    nvic_irq_enable(EXTI5_9_IRQn, 1, 1);
    // 使能中断
    exti_interrupt_enable(EXTI_0);
    exti_interrupt_enable(EXTI_1);
    exti_interrupt_enable(EXTI_4);
    exti_interrupt_enable(EXTI_5);
    // 清除中断标志位
    exti_interrupt_flag_clear(EXTI_0);
    exti_interrupt_flag_clear(EXTI_1);
    exti_interrupt_flag_clear(EXTI_4);
    exti_interrupt_flag_clear(EXTI_5);
}

// 中断函数
void EXTI0_IRQHandler(void) {
    static uint64_t pre;
    static FlagStatus pre_state = RESET;
    // 清除中断标志位
    if (SET == exti_interrupt_flag_get(EXTI_0)) {
        uint64_t current_tick = get_tick();
        if (current_tick - pre > 20000) {
            pre = current_tick;
            FlagStatus state = gpio_input_bit_get(GPIOD, GPIO_PIN_0);
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
    exti_interrupt_flag_clear(EXTI_0);
}