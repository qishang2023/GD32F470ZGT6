//
// Created by pc on 2024/1/15.
//

#include "bsp_ADC.h"


/********************************************************************************
* @author: Qi Shang
* @brief: оƬ�ڲ��¶Ȳɼ�ADC��ʼ��
* @param:
* @retval:
********************************************************************************/
void bsp_ADC0_C16_Init(void) {
    /* ���� */
    adc_deinit();
/* ʹ��ʱ�� */
    rcu_periph_clock_enable(RCU_ADC0);
/* ���÷�Ƶϵ�� */
    adc_clock_config(ADC_ADCCK_HCLK_DIV6);
/* ����ͬ��ģʽ(����ģʽ) */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
/* ���õ���ģʽ��������ת��(����ת��) */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
/* ����ɨ�軹�Ƿ�ɨ��ģʽ(��ɨ��ģʽ) */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
/* �����Ƿ�򿪲���ͨ��(����) */
    adc_special_function_config(ADC0, ADC_INSERTED_CHANNEL_AUTO, DISABLE);
/* ���÷ֱ��� */
    adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
/* �������ݶ��� */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
/* ����ת��ͨ������(��������ͨ����Ͳ���ͨ����) */
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);
/* ����ת����һ��ͨ���Լ���������λ�� */
    adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_16, ADC_SAMPLETIME_28);// ����ʱ��Ϊ28+12��ʱ������
    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH,ENABLE);
    /* ʹ��ADC */
    adc_enable(ADC0);
/* �ڲ�У׼(��Ҫdelay�ȴ�) */
    adc_calibration_enable(ADC0);
    delay_1ms(1);
}

/********************************************************************************
* @author: Qi Shang
* @brief: ADC�ɼ��ڲ��¶�
* @param: None
* @retval: �¶ȣ�float��ʽ
********************************************************************************/
float adc_temp_get(void) {
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
    while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET);
    uint16_t adc_data = adc_routine_data_read(ADC0);
    /* ת�����¶� */
    float v = ((float) adc_data * 3300.0f / 4096.0f);
    float temperature = ((1400.0f - v) / 4.4) + 25.0f;
    return temperature;
}