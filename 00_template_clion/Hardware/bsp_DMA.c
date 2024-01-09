#include "bsp_DMA.h"

/********************************************************************************
* @author: Qi Shang
* @brief: DMA��ʼ������
* @param:
* @retval:
********************************************************************************/
void dma1_config(void){
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1, DMA_CH0);
    dma_single_data_parameter_struct init_struct;
    init_struct.periph_addr         = 0U; // �����ַ
    init_struct.periph_inc          = DMA_PERIPH_INCREASE_ENABLE;//�����ַ�Ƿ�����
    init_struct.memory0_addr        = 0U; // �ڴ��ַ
    init_struct.memory_inc          = DMA_PERIPH_INCREASE_ENABLE;//�ڴ��ַ�Ƿ�����
    init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT; // �������ݿ��
    init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;// ѭ��ģʽ
    init_struct.direction           = DMA_MEMORY_TO_MEMORY; // ���䷽��
    init_struct.number              = 0U; // ���ݸ���
    init_struct.priority            = DMA_PRIORITY_LOW; // ���ȼ�
    dma_single_data_mode_init(DMA1, DMA_CH0, &init_struct);
    //��NVIC����ע���ж�
    nvic_irq_enable(DMA1_Channel0_IRQn, 2, 2);
    //����DMA�ж�
    dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_FTF);
    dma_interrupt_enable(DMA1, DMA_CH0, DMA_CHXCTL_FTFIE);
    //����DMA  ��Ҫ���õĵط��ٵ���
//    dma_channel_enable(DMA1, DMA_CH0);
}

/********************************************************************************
* @author: Qi Shang
* @brief: DMA�жϴ�����
* @param:None
* @retval: None
********************************************************************************/

void DMA1_Channel0_IRQHandler(){
    if(SET == dma_interrupt_flag_get(DMA1, DMA_CH0, DMA_INT_FLAG_FTF)){
        dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_FTF);

    }
}