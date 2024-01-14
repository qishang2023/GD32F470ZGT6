//
// Created by pc on 2024/1/5.
//

#include "bsp_DHT11.h"

void DHT11_init(){
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//    rcu_periph_clock_enable(RCU_GPIOD);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
//    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
//    gpio_output_options_set(GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
//    gpio_bit_set(GPIOD, GPIO_PIN_3);
    DHT_SET;
}

u8 DHT11_read_data(u8* dat) {
    u16 count;
    u8 i, j;
    // ����: ������������18ms
    // ��������
    // DHT = 0;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
//    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
//    gpio_output_options_set(GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_2MHZ, GPIO_PIN_3);
    DHT_RESET;
    delay_1ms(19);
    // DHT = 1;
    DHT_SET;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
//    gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
    count = 0;
    while(DHT == SET && count < 25) {
        count++;
        delay_1us(1);
    }
    if(count > 25) return 1;

    count = 0;
    while(DHT == RESET && count < 83) {
        count++;
        delay_1us(1);
    }
    if(count > 83) return 2;
    delay_1us(2);
    count = 0;
    while(DHT == SET && count < 87) {
        count++;
        delay_1us(1);
    }
    if(count > 87) return 3;
    delay_1us(2);
    // ��ʼ��������
    for(i = 0; i < 5; i++) {
        for(j = 0; j < 8; j++) {
            dat[i] <<= 1;
            // 54us�� 23-27�ߵ�ƽ ��ʾ0
            // 54us�� 68-74�ߵ�ƽ ��ʾ1
            count = 0;
            //�ȴ���Ϊ�ߵ�ƽ
            while(DHT == RESET && count <= 60) {
                count++;
                delay_1us(1);
            }
            if(count > 60) return 4;
            count = 0;
            // �ȴ���Ϊ�͵�ƽ
            while(DHT == SET && count <= 80) {
                count++;
                delay_1us(1);
            }
            if(count > 33) {
                dat[i] |= 1;
            }
        }
    }
    if(dat[0] + dat[1] + dat[2] + dat[3] != dat[4]) {
        return 5;
    }
    return 0;
}

u8 DHT11_get_temperature(u8 *humidity, float *temperature) {
    u8 dat[5]={0};
    u8 ret;
    u8 retry = 0;
    while((ret = DHT11_read_data(dat)) && retry < 10) {
        retry++;
    }
//     ret = DHT11_read_data(dat);
    *humidity = dat[0];// �����������

    *temperature = dat[2] & 0x7F;// �����������
    *temperature += (dat[3] * 0.1);//����С������
    if(dat[2] & 0x80) {
        //��ʾ����
        *temperature *= -1;
    }
    return ret;
}