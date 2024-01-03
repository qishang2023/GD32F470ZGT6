//
// Created by pc on 2024/1/3.
//

#include "bsp_RTC.h"


 void RTC_config() {
    // 电池管理加载
    rcu_periph_clock_enable(RCU_PMU);
    pmu_backup_write_enable();
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

 void RTC_read() {
    rtc_parameter_struct rps;
    uint8_t buf[30] = {0};
    rtc_current_time_get(&rps);

    uint16_t year = READ_BCD(rps.year) + 2000;
    uint8_t month = READ_BCD(rps.month);
    uint8_t date = READ_BCD(rps.date);
    uint8_t hour = READ_BCD(rps.hour);
    uint8_t minute = READ_BCD(rps.minute);
    uint8_t second = READ_BCD(rps.second);
    sprintf(buf,"%d-%d-%d %02d:%02d:%02d", year, month, date, hour, minute, second);
    LCD_ShowString(10, 130, buf, BLACK, WHITE, 16,0);
}