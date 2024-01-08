#include "bsp_BUZZER.h"
#define PRESCALER  (240-1)
#define PERIOD(hz) (SystemCoreClock/(PRESCALER+1)/hz-1)

uint16_t FREQS[] = { 1047, 1175, 1319,  1397, 1568, 1760, 1976, 2093 };
uint8_t notes[] = {
1, 2, 3, 1,					1, 2, 3, 1,					3, 4, 5,		3, 4, 5,
5, 6, 5, 4, 3, 1, 	5, 6, 5, 4, 3, 1, 	1, 5, 1,		1, 5, 1,
};

// ��ʱʱ��
uint8_t durations[] = {
4, 4, 4, 4,					4, 4, 4, 4,					4, 4, 8,		4, 4, 8,
3, 1, 3, 1, 4, 4,		3, 1, 3, 1, 4, 4,		4, 4, 8,		4, 4, 8,
};

void bsp_BUZZER_config() {
    /*************** GPIO config **************/
    // ����ʱ��
    rcu_periph_clock_enable(RCU_GPIOB);
    // ����GPIOģʽ
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
    // ���ø��ù���
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_0);

    /*************** Timer config *************/
    // ʱ������
    rcu_periph_clock_enable(RCU_TIMER2);
    // ��λ��ʱ��
    timer_deinit(TIMER2); // ���ö�ʱ�����裨��������ϵͳ��
    // ��Ƶ����
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    // ��ʼ����ʱ��
    timer_parameter_struct tps;
    tps.prescaler = PRESCALER;
    tps.alignedmode = TIMER_COUNTER_EDGE;
    tps.counterdirection = TIMER_COUNTER_UP;
    tps.period = 0;
    tps.clockdivision = TIMER_CKDIV_DIV1;
    tps.repetitioncounter = 0U;
    timer_init(TIMER2, &tps);

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
    timer_channel_output_config(TIMER2, TIMER_CH_2, &tops);

    // ���ģʽ����
    timer_channel_output_mode_config(TIMER2, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 0);
    timer_enable(TIMER2);
}

void bsp_BUZZER_play(uint8_t pos){
    uint32_t period = (uint32_t)PERIOD(FREQS[pos]);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, period/20);
    timer_autoreload_value_config(TIMER2, period);
}

void bsp_BUZZER_stop(){
    timer_disable(TIMER2);
}

void bsp_BUZZER_music(){
    uint8_t i,len;
    uint16_t d;
    len = sizeof(notes)/sizeof(notes[0]);
    for(i = 0; i < len; i++){
        bsp_BUZZER_play(notes[i]);
        d = durations[i] * 100;
        while(d--) delay_1ms(1);
    }
    bsp_BUZZER_stop();
}