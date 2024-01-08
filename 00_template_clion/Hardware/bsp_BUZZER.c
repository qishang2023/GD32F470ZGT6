#include "bsp_BUZZER.h"
#define PRESCALER  (240-1)
#define PERIOD(hz) (SystemCoreClock/(PRESCALER+1)/hz-1)

uint16_t FREQS[] = { 1047, 1175, 1319,  1397, 1568, 1760, 1976, 2093 };
uint8_t notes[] = {
1, 2, 3, 1,					1, 2, 3, 1,					3, 4, 5,		3, 4, 5,
5, 6, 5, 4, 3, 1, 	5, 6, 5, 4, 3, 1, 	1, 5, 1,		1, 5, 1,
};

// 延时时长
uint8_t durations[] = {
4, 4, 4, 4,					4, 4, 4, 4,					4, 4, 8,		4, 4, 8,
3, 1, 3, 1, 4, 4,		3, 1, 3, 1, 4, 4,		4, 4, 8,		4, 4, 8,
};

void bsp_BUZZER_config() {
    /*************** GPIO config **************/
    // 配置时钟
    rcu_periph_clock_enable(RCU_GPIOB);
    // 配置GPIO模式
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
    // 配置复用功能
    gpio_af_set(GPIOB, GPIO_AF_2, GPIO_PIN_0);

    /*************** Timer config *************/
    // 时钟配置
    rcu_periph_clock_enable(RCU_TIMER2);
    // 复位定时器
    timer_deinit(TIMER2); // 重置定时器外设（类似重启系统）
    // 倍频配置
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    // 初始化定时器
    timer_parameter_struct tps;
    tps.prescaler = PRESCALER;
    tps.alignedmode = TIMER_COUNTER_EDGE;
    tps.counterdirection = TIMER_COUNTER_UP;
    tps.period = 0;
    tps.clockdivision = TIMER_CKDIV_DIV1;
    tps.repetitioncounter = 0U;
    timer_init(TIMER2, &tps);

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
    timer_channel_output_config(TIMER2, TIMER_CH_2, &tops);

    // 输出模式配置
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