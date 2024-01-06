////
//// Created by pc on 2024/1/3.
////
//
//#include "bsp_RTC.h"
//
//RTC_clock rtc_clock;
//
//void RTC_config() {
//    RTC_InitTypeDef  RTC_InitStructure;
//    RTC_TimeTypeDef  RTC_TimeStruct;
//    /* Enable the PWR clock */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//
//    /* Allow access to RTC */
//    PWR_BackupAccessCmd(ENABLE);
//
//    /* Reset RTC Domain */
//    RCC_BackupResetCmd(ENABLE);
//    RCC_BackupResetCmd(DISABLE);
//
//    /* Enable the LSE OSC */
//    RCC_LSEConfig(RCC_LSE_ON);
//
//    /* Wait till LSE is ready */
//    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
//    {
//    }
//
//    /* Select the RTC Clock Source */
//    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
//
//    /* Configure the RTC data register and RTC prescaler */
//    /* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
//    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
//    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
//    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
//    RTC_Init(&RTC_InitStructure);
//
//    /* Set the time to 00h 00mn 00s AM */
//    RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
//    RTC_TimeStruct.RTC_Hours   = 0;
//    RTC_TimeStruct.RTC_Minutes = 0;
//    RTC_TimeStruct.RTC_Seconds = 0;
//    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
//    // 电池管理加载
//    rcu_periph_clock_enable(RCU_PMU);
//    pmu_backup_write_enable();
//    // 晶振加载
//    rcu_osci_on(RCU_LXTAL);
//    rcu_osci_stab_wait(RCU_LXTAL);
//    rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
//    // RTC功能加载
//    rcu_periph_clock_enable(RCU_RTC);
//    rtc_register_sync_wait();
//
//    rtc_parameter_struct rps;
//    rps.year = WRITE_BCD(23);
//    rps.month = WRITE_BCD(4);
//    rps.date = WRITE_BCD(20);
//    rps.day_of_week = WRITE_BCD(4);
//    rps.hour = WRITE_BCD(23);
//    rps.minute = WRITE_BCD(59);
//    rps.second = WRITE_BCD(55);
//    rps.display_format = RTC_24HOUR;
//    rps.am_pm = RTC_AM;
//    rps.factor_asyn = 0x7F;
//    rps.factor_syn = 0xFF;
//    rtc_init(&rps);
//}
//
//void RTC_set_time(){
//    rtc_parameter_struct rps;
//    rtc_current_time_get(&rps);
//    uint8_t year = rtc_clock.year - 2000;
//    rps.year = WRITE_BCD(year);
//    rps.month = WRITE_BCD(rtc_clock.month);
//    rps.date = WRITE_BCD(rtc_clock.day);
//    rps.day_of_week = WRITE_BCD(rtc_clock.week);
//    rps.hour = WRITE_BCD(rtc_clock.hour);
//    rps.minute = WRITE_BCD(rtc_clock.minute);
//    rps.second = WRITE_BCD(rtc_clock.second);
//    rps.display_format = RTC_24HOUR;
//    rps.am_pm = RTC_AM;
//    rps.factor_asyn = 0x7F;
//    rps.factor_syn = 0xFF;
//    rtc_init(&rps);
//}
//
//void RTC_read() {
//    rtc_parameter_struct rps;
//    rtc_current_time_get(&rps);
//    rtc_clock.year = READ_BCD(rps.year) + 2000;
//    rtc_clock.month = READ_BCD(rps.month);
//    rtc_clock.day = READ_BCD(rps.date);
//    rtc_clock.hour = READ_BCD(rps.hour);
//    rtc_clock.minute = READ_BCD(rps.minute);
//    rtc_clock.second = READ_BCD(rps.second);
//    rtc_clock.week = READ_BCD(rps.day_of_week);
//}