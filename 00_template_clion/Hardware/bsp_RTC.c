//
// Created by pc on 2024/1/3.
//

#include "bsp_RTC.h"

RTC_clock rtc_clock;

void RTC_config() {
    // 电池管理加载
    rcu_periph_clock_enable(RCU_PMU);
    pmu_backup_write_enable();// 允许对备份域寄存器的写访问
    // 晶振加载
    rcu_osci_on(RCU_LXTAL);
    rcu_osci_stab_wait(RCU_LXTAL);
    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
    // RTC功能加载
    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();

    rtc_parameter_struct rps;
    rps.year = WRITE_BCD(23);
    rps.month = WRITE_BCD(4);
    rps.date = WRITE_BCD(20);
    rps.day_of_week = WRITE_BCD(4);
    rps.hour = WRITE_BCD(23);
    rps.minute = WRITE_BCD(59);
    rps.second = WRITE_BCD(55);
    rps.display_format = RTC_24HOUR;
    rps.am_pm = RTC_AM;
    rps.factor_asyn = 0x7F;
    rps.factor_syn = 0xFF;

    rtc_init(&rps);
}

void RTC_set_time(){
    rtc_parameter_struct rps;
    rtc_current_time_get(&rps);
    uint8_t year = rtc_clock.year - 2000;
    rps.year = WRITE_BCD(year);
    rps.month = WRITE_BCD(rtc_clock.month);
    rps.date = WRITE_BCD(rtc_clock.day);
    rps.day_of_week = WRITE_BCD(rtc_clock.week);
    rps.hour = WRITE_BCD(rtc_clock.hour);
    rps.minute = WRITE_BCD(rtc_clock.minute);
    rps.second = WRITE_BCD(rtc_clock.second);
    rps.display_format = RTC_24HOUR;
    rps.am_pm = RTC_AM;
    rps.factor_asyn = 0x7F;
    rps.factor_syn = 0xFF;
    rtc_init(&rps);
}

void RTC_read() {
    rtc_parameter_struct rps;
    rtc_current_time_get(&rps);
    rtc_clock.year = READ_BCD(rps.year) + 2000;
    rtc_clock.month = READ_BCD(rps.month);
    rtc_clock.day = READ_BCD(rps.date);
    rtc_clock.hour = READ_BCD(rps.hour);
    rtc_clock.minute = READ_BCD(rps.minute);
    rtc_clock.second = READ_BCD(rps.second);
    rtc_clock.week = READ_BCD(rps.day_of_week);
}

void ALARM_config() {
    // 闹钟外部中断
    exti_flag_clear(EXTI_17);
    exti_init(EXTI_17,EXTI_INTERRUPT,EXTI_TRIG_RISING);

    // 重置闹钟
    rtc_alarm_disable(RTC_ALARM0);

    rtc_alarm_struct ras;
    ras.alarm_mask = RTC_ALARM_HOUR_MASK | RTC_ALARM_MINUTE_MASK | RTC_ALARM_DATE_MASK;
    ras.weekday_or_date = RTC_ALARM_DATE_SELECTED;
    ras.alarm_day = 0x21;
    ras.alarm_hour = WRITE_BCD(23);
    ras.alarm_minute = WRITE_BCD(59);
    ras.alarm_second = WRITE_BCD(59);
    ras.am_pm = RTC_AM;
    rtc_alarm_config(RTC_ALARM0, &ras);

    // 中断配置
    nvic_irq_enable(RTC_Alarm_IRQn, 2, 2);
    rtc_interrupt_enable(RTC_INT_ALARM0);
    rtc_flag_clear(RTC_FLAG_ALRM0);

    rtc_alarm_enable(RTC_ALARM0);
}

void RTC_Alarm_IRQHandler() {
    if (SET == rtc_flag_get(RTC_FLAG_ALRM0)) {
        // 处理RTC闹钟中断
        printf("alarm \r\n");
    }
    rtc_flag_clear(RTC_FLAG_ALRM0);
    exti_flag_clear(EXTI_17);
}