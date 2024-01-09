#include "bsp_DMA.h"

/********************************************************************************
* @author: Qi Shang
* @brief: DMA初始化配置
* @param:
* @retval:
********************************************************************************/
void dma1_config(void){
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1, DMA_CH0);
    dma_single_data_parameter_struct init_struct;
    init_struct.periph_addr         = 0U; // 外设地址
    init_struct.periph_inc          = DMA_PERIPH_INCREASE_ENABLE;//外设地址是否增加
    init_struct.memory0_addr        = 0U; // 内存地址
    init_struct.memory_inc          = DMA_PERIPH_INCREASE_ENABLE;//内存地址是否增加
    init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT; // 外设数据宽度
    init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;// 循环模式
    init_struct.direction           = DMA_MEMORY_TO_MEMORY; // 传输方向
    init_struct.number              = 0U; // 数据个数
    init_struct.priority            = DMA_PRIORITY_LOW; // 优先级
    dma_single_data_mode_init(DMA1, DMA_CH0, &init_struct);
    //到NVIC里面注册中断
    nvic_irq_enable(DMA1_Channel0_IRQn, 2, 2);
    //开启DMA中断
    dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_FTF);
    dma_interrupt_enable(DMA1, DMA_CH0, DMA_CHXCTL_FTFIE);
    //开启DMA  需要调用的地方再调用
//    dma_channel_enable(DMA1, DMA_CH0);
}

/********************************************************************************
* @author: Qi Shang
* @brief: DMA中断处理函数
* @param:None
* @retval: None
********************************************************************************/

void DMA1_Channel0_IRQHandler(){
    if(SET == dma_interrupt_flag_get(DMA1, DMA_CH0, DMA_INT_FLAG_FTF)){
        dma_interrupt_flag_clear(DMA1, DMA_CH0, DMA_INT_FLAG_FTF);

    }
}