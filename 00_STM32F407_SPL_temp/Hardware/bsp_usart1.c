//
// Created by pc on 2024/1/5.
//

#include "bsp_usart1.h"


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
//数组缓冲
uint8_t buffer[1024] = {0};
//添加元素索引
//int16_t next_index = 0;

void usart1_dma_tx_config() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_Channel = DMA_Channel_4;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) (&(USART1->DR));
    DMA_InitStruct.DMA_Memory0BaseAddr = 0;
    DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct.DMA_BufferSize = 0;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream7, &DMA_InitStruct);
}

void usart1_dma_rx_config() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_Channel = DMA_Channel_4;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) (&(USART1->DR));
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) buffer;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_BufferSize = 1024;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream2, &DMA_InitStruct);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
    DMA_Cmd(DMA2_Stream2, ENABLE);
}

//uart配置
void usart1_rx_tx_config(uint32_t baudval) {
    GPIO_InitTypeDef GPIO_InitStructure;
    /**************** 发送引脚 ****************/
    //1.配置TX的GPIO口
    //1.1 打开时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //1.2 配置GPIO模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    //2.配置UART通信相关内容
    USART_InitTypeDef USART_InitStruct;
    //打开usart的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    //重置uart
    USART_DeInit(USART1);
    USART_StructInit(&USART_InitStruct);
    USART_InitStruct.USART_BaudRate = 115200;
    //配置usart
    USART_Init(USART1, &USART_InitStruct);
    // ************* 接收中断配置
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);//中断优先级配置
//    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    //清理标记
//    USART_ClearFlag(USART1,USART_IT_RXNE);
    usart1_dma_tx_config();
//    usart_flag_clear(USART_NUM,USART_FLAG_RBNE);
    USART_Cmd(USART1, ENABLE);//打开串口
    usart1_dma_rx_config();
}

//abcdefg
//发送一byte数据
void send_data(uint8_t data) {
    //通过USART发送
    DMA_MemoryTargetConfig(DMA2_Stream7, (uint32_t) (&data), DMA_Memory_0);
    DMA_SetCurrDataCounter(DMA2_Stream7, 1);
    DMA_Cmd(DMA2_Stream7, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    while (RESET == DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7));
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
}


//字符串发送
void send_string(char data[]) {
    for (; *data != 0; data++) send_data(*data);    //遇到停止符0结束
}

//printf内部调用这个方法
PUTCHAR_PROTOTYPE {
    //通过USART发送
    send_data((uint8_t) ch);
    return ch;
}

//USART串口中断处理函数(系统内部调用)
void USART_IRQHandler(void) {
//    if(SET==USART_GetFlagStatus(USART1,USART_FLAG_RXNE)){
//        uint8_t data = (uint8_t)USART_ReceiveData(USART1);
//        buffer[next_index] = data;
//        next_index++;
//    }
    //2.IDLE中断,代表一包数据结束,需要处理(打印)
    if (SET == USART_GetFlagStatus(USART_NUM, USART_FLAG_IDLE)) {
        USART_ReceiveData(USART_NUM);//读取清除状态
        DMA_Cmd(DMA2_Stream2, DISABLE);
        uint16_t a = DMA_GetCurrDataCounter(DMA2_Stream2);
        uint16_t len = 1024 - a;
        if (len > 0) {
            DMA_SetCurrDataCounter(DMA2_Stream2, 1024);
            recv_data_callback(buffer, len);
        }
        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
        DMA_Cmd(DMA2_Stream2, ENABLE);
    }
}