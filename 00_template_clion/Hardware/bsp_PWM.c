#include "bsp_PWM.h"

#define PERIOD (SystemCoreClock / 10000 - 1)

/*
 * ͨ�ö�ʱ���ͻ�����ʱ������
 *  1.Ҳ��Ҫ��Ƶ
 *  2.Ҳ��Ҫ����
 *  3.��������ȽϼĴ���ֵ(ռ�ձȼ���ֵ)
 *  4.����Ҫ�ж�
 */
void PWM_config() {
    /*************** GPIO config **************/
    // ����ʱ��
    rcu_periph_clock_enable(RCU_GPIOA);
    // ����GPIOģʽ
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5);
    // ����GPIO���
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_5);
    // ���ø��ù���
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_5);

    /*************** Timer config *************/
    // ʱ������
    rcu_periph_clock_enable(RCU_TIMER1);
    // ��λ��ʱ��
    timer_deinit(TIMER1); // ���ö�ʱ�����裨��������ϵͳ��
    // ��Ƶ����
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL2);

    // ��ʼ����ʱ��
    timer_parameter_struct tps;
    tps.prescaler = 10 - 1;
    tps.alignedmode = TIMER_COUNTER_EDGE;
    tps.counterdirection = TIMER_COUNTER_UP;
    tps.period = PERIOD;
    tps.clockdivision = TIMER_CKDIV_DIV1;
    tps.repetitioncounter = 0U;
    timer_init(TIMER1, &tps);

    // �������ͨ��
    timer_oc_parameter_struct tops;
    /*ͨ�ö�ʱ��ͨ������*/
    tops.outputstate = TIMER_CCX_ENABLE;// ����ͨ�����
    tops.ocpolarity = TIMER_OC_POLARITY_HIGH;
    tops.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    /*�߼���ʱ���������ͨ������*/
    tops.outputnstate = TIMER_CCXN_DISABLE;
    tops.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    tops.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER1, TIMER_CH_0, &tops);

    // ���ģʽ����
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 0);
    // ��ʼ��
    timer_enable(TIMER1);
}
/********************************************************************************
* @author: Qi Shang
* @brief: ��ʱ��ռ�ձȸ���
* @param: pulse: ռ�ձ�0~100
* @retval: None
********************************************************************************/

void PWM_update(uint8_t pulse) {
    // ���������ռ�ձ�
    uint32_t i = PERIOD * pulse / 100;
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, i);
}
