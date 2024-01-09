//
// Created by pc on 2024/1/3.
//

#include "bsp_RTC.h"

RTC_clock rtc_clock;

void RTC_config() {
    RTC_InitTypeDef RTC_InitStructure;
    RTC_TimeTypeDef RTC_TimeStruct;
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);// 开启PWR时钟
    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE); // 允许对备份域寄存器的写访问
    /* Reset RTC Domain */
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);
    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    RTC_DeInit();
    /* Configure the RTC data register and RTC prescaler */
    /* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv = 0xFF;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);
    RTC_EnterInitMode();
    RTC_DateTypeDef RTC_DateStruct;
    RTC_DateStruct.RTC_WeekDay = RTC_Weekday_Monday;
    RTC_DateStruct.RTC_Date = WRITE_BCD(31);
    RTC_DateStruct.RTC_Month = RTC_Month_December;
    RTC_DateStruct.RTC_Year = WRITE_BCD(23);
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStruct);
    /* Set the time to 00h 00mn 00s AM */
    RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
    RTC_TimeStruct.RTC_Hours = WRITE_BCD(23);
    RTC_TimeStruct.RTC_Minutes = WRITE_BCD(59);
    RTC_TimeStruct.RTC_Seconds = WRITE_BCD(50);
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
    RTC_ExitInitMode();
}

void RTC_set_time() {
    RTC_InitTypeDef RTC_InitStructure;
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DeInit();
    /* Configure the RTC data register and RTC prescaler */
    /* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv = 0xFF;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);
    RTC_EnterInitMode();
    RTC_DateTypeDef RTC_DateStruct;
    RTC_DateStruct.RTC_WeekDay = rtc_clock.week;
    RTC_DateStruct.RTC_Date = rtc_clock.day;
    RTC_DateStruct.RTC_Month = rtc_clock.month;
    RTC_DateStruct.RTC_Year = rtc_clock.year - 2000;
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
    /* Set the time to 00h 00mn 00s AM */
    RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
    RTC_TimeStruct.RTC_Hours = rtc_clock.hour;
    RTC_TimeStruct.RTC_Minutes = rtc_clock.minute;
    RTC_TimeStruct.RTC_Seconds = rtc_clock.second;
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
    RTC_ExitInitMode();
}

void RTC_read_time() {
    RTC_DateTypeDef RTC_DateStruct;
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
    rtc_clock.year = RTC_DateStruct.RTC_Year + 2000;
    rtc_clock.month = RTC_DateStruct.RTC_Month;
    rtc_clock.day = RTC_DateStruct.RTC_Date;
    rtc_clock.hour = RTC_TimeStruct.RTC_Hours;
    rtc_clock.minute = RTC_TimeStruct.RTC_Minutes;
    rtc_clock.second = RTC_TimeStruct.RTC_Seconds;
    rtc_clock.week = RTC_DateStruct.RTC_WeekDay;
}
