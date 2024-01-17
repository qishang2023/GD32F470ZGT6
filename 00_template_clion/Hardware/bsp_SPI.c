//
// Created by pc on 2024/1/17.
//

#include "bsp_SPI.h"


void bsp_hard_spi_config(void){
    /********************** GPIO初始化 ************************/
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
    //拉高所有的输出引脚
    //SPI_SCL_H();
    SPI_MOSI_H();

    /************************** 外设初始化 **************************/
    rcu_periph_clock_enable(SPI_RCU);
    //重置
    spi_i2s_deinit(SPI_NUM);
    //配置结构体
    spi_parameter_struct sps;
    spi_struct_para_init(&sps);
    sps.device_mode = SPI_MASTER; //主机模式
    sps.frame_size = SPI_FRAMESIZE_8BIT;//每次发送8bit
    sps.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;//设置极性和相位 0 0
    sps.nss = SPI_NSS_SOFT;//软件片选(手动拉低和拉高片选通信)
    sps.prescale = SPI_PSC_32;//预分频 基于APB2总线(120M)  分频之后频率:60M 30M
    sps.endian = SPI_ENDIAN_MSB;//先发高位
    spi_init(SPI_NUM, &sps);
    //开启
    spi_enable(SPI_NUM);

}

//写入
void bsp_hard_spi_write(uint8_t dat){
    //等待发送缓冲区为空
    while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE)) {};
    spi_i2s_data_transmit(SPI0, dat);
    //需要处理另外读取的线路
    while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE)) {};
    spi_i2s_data_receive(SPI0);
}

//读取
uint8_t bsp_hard_spi_read(void){
    //读取也需要写入(生成时钟信号)
    while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE)) {};
    spi_i2s_data_transmit(SPI0, 0xFF);
    //需要处理另外读取的线路
    while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE)) {};
    return spi_i2s_data_receive(SPI0);
}