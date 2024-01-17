//
// Created by pc on 2024/1/17.
//

#include "bsp_SPI.h"


void bsp_hard_spi_config(void){
    /********************** GPIO��ʼ�� ************************/
    //SCL:PB3
    rcu_periph_clock_enable(SPI_SCL_RCU);
    gpio_mode_set(SPI_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_SCL_PIN);
    gpio_output_options_set(SPI_SCL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI_SCL_PIN);
    gpio_af_set(SPI_SCL_PORT,GPIO_AF_5,SPI_SCL_PIN);

    //MOSI:PB5
    rcu_periph_clock_enable(SPI_MOSI_RCU);
    gpio_mode_set(SPI_MOSI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_MOSI_PIN);
    gpio_output_options_set(SPI_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI_MOSI_PIN);
    gpio_af_set(SPI_MOSI_PORT,GPIO_AF_5,SPI_MOSI_PIN);

    //FSO(MISO):PB4
    rcu_periph_clock_enable(SPI_MISO_RCU);
    gpio_mode_set(SPI_MISO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_MISO_PIN);
    gpio_output_options_set(SPI_MISO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI_MISO_PIN);
    gpio_af_set(SPI_MISO_PORT,GPIO_AF_5,SPI_MISO_PIN);
    //GPIO_HBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
    //�������е��������
    //SPI_SCL_H();
    SPI_MOSI_H();

    /************************** �����ʼ�� **************************/
    rcu_periph_clock_enable(SPI_RCU);
    //����
    spi_i2s_deinit(SPI_NUM);
    //���ýṹ��
    spi_parameter_struct sps;
    spi_struct_para_init(&sps);
    sps.device_mode = SPI_MASTER; //����ģʽ
    sps.frame_size = SPI_FRAMESIZE_8BIT;//ÿ�η���8bit
    sps.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;//���ü��Ժ���λ 0 0
    sps.nss = SPI_NSS_SOFT;//���Ƭѡ(�ֶ����ͺ�����Ƭѡͨ��)
    sps.prescale = SPI_PSC_32;//Ԥ��Ƶ ����APB2����(120M)  ��Ƶ֮��Ƶ��:60M 30M
    sps.endian = SPI_ENDIAN_MSB;//�ȷ���λ
    spi_init(SPI_NUM, &sps);
    //����
    spi_enable(SPI_NUM);

}

//д��
void bsp_hard_spi_write(uint8_t dat){
    //�ȴ����ͻ�����Ϊ��
    while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE)) {};
    spi_i2s_data_transmit(SPI0, dat);
    //��Ҫ���������ȡ����·
    while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE)) {};
    spi_i2s_data_receive(SPI0);
}

//��ȡ
uint8_t bsp_hard_spi_read(void){
    //��ȡҲ��Ҫд��(����ʱ���ź�)
    while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE)) {};
    spi_i2s_data_transmit(SPI0, 0xFF);
    //��Ҫ���������ȡ����·
    while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE)) {};
    return spi_i2s_data_receive(SPI0);
}