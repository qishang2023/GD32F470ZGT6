//
// Created by pc on 2024/1/17.
//

#include "bsp_SPI.h"

u8 temp = 0;

void bsp_dma_spi_config(void){
    dma_single_data_parameter_struct dma_init_struct;

    /* configure SPI1 transmit DMA */
    dma_deinit(DMA1, DMA_CH3);
    dma_init_struct.periph_addr         = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory0_addr        = 0;
    dma_init_struct.direction           = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.priority            = DMA_PRIORITY_LOW;
    dma_init_struct.number              = 0;
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;
    dma_single_data_mode_init(DMA1, DMA_CH3, &dma_init_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH3, DMA_SUBPERI3);
    dma_init_struct.memory0_addr        = (uint32_t)&temp;
    dma_init_struct.direction           = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_DISABLE;
    dma_single_data_mode_init(DMA1, DMA_CH2, &dma_init_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH2, DMA_SUBPERI3);
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI0, SPI_DMA_RECEIVE);
}

void bsp_dma_spi_send(const u8 *p,u32 len){
    dma_channel_disable(DMA1, DMA_CH3);
    dma_memory_address_config(DMA1, DMA_CH3,DMA_MEMORY_0, (uint32_t)p);
    dma_transfer_number_config(DMA1, DMA_CH3, len);
    dma_channel_enable(DMA1, DMA_CH3);
    dma_channel_enable(DMA1, DMA_CH2);
    while (RESET == dma_flag_get(DMA1, DMA_CH3,DMA_FLAG_FTF));
    dma_flag_clear(DMA1, DMA_CH3,DMA_FLAG_FTF);
}

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
//    rcu_periph_clock_enable(SPI_MISO_RCU);
//    gpio_mode_set(SPI_MISO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_MISO_PIN);
//    gpio_output_options_set(SPI_MISO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI_MISO_PIN);
//    gpio_af_set(SPI_MISO_PORT,GPIO_AF_5,SPI_MISO_PIN);
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
    sps.clock_polarity_phase = SPI_CK_PL_HIGH_PH_1EDGE;//���ü��Ժ���λ 0 0
    sps.nss = SPI_NSS_SOFT;//���Ƭѡ(�ֶ����ͺ�����Ƭѡͨ��)
    sps.prescale = SPI_PSC_16;//Ԥ��Ƶ ����APB2����(120M)  ��Ƶ֮��Ƶ��:60M 30M
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