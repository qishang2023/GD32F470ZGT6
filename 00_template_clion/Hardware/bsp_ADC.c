//
// Created by pc on 2024/1/15.
//

#include "bsp_ADC.h"


/********************************************************************************
* @author: Qi Shang
* @brief: 芯片内部温度采集ADC初始化
* @param:
* @retval:
********************************************************************************/
void bsp_ADC0_C16_Init(void) {
    /* 重置 */
    adc_deinit();
/* 使能时钟 */
    rcu_periph_clock_enable(RCU_ADC0);
/* 设置分频系数 */
    adc_clock_config(ADC_ADCCK_HCLK_DIV6);
/* 设置同步模式(独立模式) */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
/* 设置单次模式还是连续转换(单次转换) */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
/* 设置扫描还是非扫描模式(非扫描模式) */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
/* 设置是否打开插入通道(不打开) */
    adc_special_function_config(ADC0, ADC_INSERTED_CHANNEL_AUTO, DISABLE);
/* 设置分辨率 */
    adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
/* 设置数据对齐 */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
/* 设置转换通道个数(包括常规通道组和插入通道组) */
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);
/* 设置转换哪一个通道以及所处序列位置 */
    adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_16, ADC_SAMPLETIME_28);// 采样时间为28+12个时钟周期
    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH,ENABLE);
    /* 使能ADC */
    adc_enable(ADC0);
/* 内部校准(需要delay等待) */
    adc_calibration_enable(ADC0);
    delay_1ms(1);
}

/********************************************************************************
* @author: Qi Shang
* @brief: ADC采集内部温度
* @param: None
* @retval: 温度，float形式
********************************************************************************/
float adc_temp_get(void) {
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
    while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET);
    uint16_t adc_data = adc_routine_data_read(ADC0);
    /* 转换成温度 */
    float v = ((float) adc_data * 3300.0f / 4096.0f);
    float temperature = ((1400.0f - v) / 4.4) + 25.0f;
    return temperature;
}