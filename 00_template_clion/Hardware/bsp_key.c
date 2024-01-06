//
// Created by pc on 2024/1/6.
//

#include "bsp_key.h"

void EXTI_config() {
    uint32_t extix = EXTI_0; // �ĸ��ж�
    uint32_t extix_irq = EXTI0_IRQn;
    uint32_t extix_trig = EXTI_TRIG_BOTH; // �жϴ�������

    uint32_t extix_src_port = EXTI_SOURCE_GPIOA; // �жϴ���Դ���ĸ�����
    uint32_t extix_src_pin = EXTI_SOURCE_PIN0;

    uint32_t extix_io_rcu = RCU_GPIOA;
    uint32_t extix_io_port = GPIOA;
    uint32_t extix_io_pin = GPIO_PIN_0;

    /********************* GPIO config *********************/
    // ʱ�ӳ�ʼ��
    rcu_periph_clock_enable(extix_io_rcu);
    rcu_periph_clock_enable(RCU_GPIOD);
    // ����GPIOģʽ
    gpio_mode_set(extix_io_port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, extix_io_pin);
    gpio_mode_set(RCU_GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE,
                  GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);

    /********************* EXTI config *********************/
    // ʱ������
    rcu_periph_clock_enable(RCU_SYSCFG);
    // �����ж�Դ
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN0);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN1);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN4);
    syscfg_exti_line_config(EXTI_SOURCE_GPIOD, EXTI_SOURCE_PIN5);
    // �жϳ�ʼ��
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_init(EXTI_1, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_init(EXTI_4, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    // �����ж����ȼ�
    nvic_irq_enable(EXTI0_IRQn, 1, 1);
    nvic_irq_enable(EXTI1_IRQn, 1, 1);
    nvic_irq_enable(EXTI4_IRQn, 1, 1);
    nvic_irq_enable(EXTI5_9_IRQn, 1, 1);
    // ʹ���ж�
    exti_interrupt_enable(EXTI_0);
    exti_interrupt_enable(EXTI_1);
    exti_interrupt_enable(EXTI_4);
    exti_interrupt_enable(EXTI_5);
    // ����жϱ�־λ
    exti_interrupt_flag_clear(EXTI_0);
    exti_interrupt_flag_clear(EXTI_1);
    exti_interrupt_flag_clear(EXTI_4);
    exti_interrupt_flag_clear(EXTI_5);
}

// �жϺ���
void EXTI0_IRQHandler(void) {
    static uint64_t pre;
    static FlagStatus pre_state = RESET;
    // ����жϱ�־λ
    if (SET == exti_interrupt_flag_get(EXTI_0)) {
        uint64_t current_tick = get_tick();
        if (current_tick - pre > 20000) {
            pre = current_tick;
            FlagStatus state = gpio_input_bit_get(GPIOD, GPIO_PIN_0);
            if (SET == state && RESET == pre_state) {
                // �����ǰΪ�ߵ�ƽ����һ��Ϊ�͵�ƽ������
                pre_state = state;
                printf("up \r\n");
            } else if (RESET == state && SET == pre_state) {
                // �����ǰΪ�͵�ƽ����һ��Ϊ�ߵ�ƽ��̧��
                pre_state = state;
                printf("down \r\n");
            }
        }
    }
    exti_interrupt_flag_clear(EXTI_0);
}