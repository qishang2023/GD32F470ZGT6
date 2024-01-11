//
// Created by pc on 2024/1/3.
//

#include "bsp_USART.h"

#define USART_RECEIVE_LENGTH  1024
//串口接收缓冲区大小
uint8_t g_recv_buff[USART_RECEIVE_LENGTH];   // 接收缓冲区

uint8_t dat = 0;
//接收到字符存放的位置
//int g_recv_length = 0;
static void usart0_dma_rx_config(){
    //1.打开dma时钟
    rcu_periph_clock_enable(RCU_DMA1);
    //2.重置
    dma_deinit(DMA1,DMA_CH2);
    //3.初始化dma
    //DMA属性
    dma_single_data_parameter_struct init_struct;
    dma_single_data_para_struct_init(&init_struct);
    /**************** 这些属性可以不用配置 *****************/
    //循环模式
    init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;
    //优先级
    init_struct.priority            = DMA_PRIORITY_LOW;
    /**************** 这些都需要手动配置 *****************/
    init_struct.direction           = DMA_PERIPH_TO_MEMORY;//传输方向
    init_struct.periph_addr         = (uint32_t)(&USART_DATA(USART0));//串口外设(注意需要&)  源头
    init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;//设置地址增加(寄存器地址不能增加)
    init_struct.memory0_addr        = (uint32_t)g_recv_buff;//内存  目标
    init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;//设置地址增加
    init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;//每一次传输长度
    init_struct.number              = sizeof(g_recv_buff)/sizeof(g_recv_buff[0]);//传输数据个数
    dma_single_data_mode_init(DMA1,DMA_CH2,&init_struct);//3.初始化DMA
    dma_channel_subperipheral_select(DMA1,DMA_CH2,DMA_SUBPERI4);//4.选择外设
    dma_channel_enable(DMA1,DMA_CH2);//5.开启DMA搬运
    dma_flag_clear(DMA1,DMA_CH2,DMA_FLAG_FTF);//清除标志位
    usart_dma_receive_config(USART0,USART_RECEIVE_DMA_ENABLE);
}
static void usart0_dma_tx_config() {
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1, DMA_CH7);
    dma_single_data_parameter_struct init_struct;
    init_struct.periph_addr = (uint32_t)(&USART_DATA(USART0)); // 外设地址
    init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;//外设地址是否增加
    init_struct.memory0_addr = 0U; // 内存地址
    init_struct.memory_inc = DMA_PERIPH_INCREASE_ENABLE;//内存地址是否增加
    init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT; // 外设数据宽度
    init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;// 循环模式
    init_struct.direction = DMA_MEMORY_TO_PERIPH; // 传输方向
    init_struct.number = 0U; // 数据个数
    init_struct.priority = DMA_PRIORITY_LOW; // 优先级
    dma_single_data_mode_init(DMA1, DMA_CH7, &init_struct);
    //配置通道功能选择
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);
    //串口发送功能开启
    usart_dma_transmit_config(USART0,USART_TRANSMIT_DMA_ENABLE);
}

//发送一byte数据
void send_data(uint8_t data) {
    //通过USART发送
//    usart_data_transmit(USART0, data);
    //判断缓冲区是否已经空了
    dat = data;
    dma_memory_address_config(DMA1,DMA_CH7,DMA_MEMORY_0,(uint32_t)(&dat));
    dma_transfer_number_config(DMA1,DMA_CH7,1);
////    //开启DMA
    dma_channel_enable(DMA1, DMA_CH7);
    //FlagStatus state = usart_flag_get(USART_NUM,USART_FLAG_TBE);
//    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    while (RESET == dma_flag_get(DMA1, DMA_CH7,DMA_FLAG_FTF));
    dma_flag_clear(DMA1, DMA_CH7,DMA_FLAG_FTF);
}

//发送字符串
void send_string(char *data) {
    //满足: 1.data指针不为空  2.发送的数据不是\0结束标记
    while (data && *data) {
        send_data((uint8_t) (*data));
        data++;
    }
}

int __io_putchar(int ch) {
    send_data((uint8_t) ch);
    return ch;
}

void USART_config() {
    /************** gpio config **************/
    // tx
    rcu_periph_clock_enable(RCU_GPIOA);    // 配置时钟
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    // rx
    rcu_periph_clock_enable(RCU_GPIOA); // 配置时钟
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);
    //gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /************** usart config **************/
    // 串口时钟
    rcu_periph_clock_enable(RCU_USART0);
    // USART复位
    usart_deinit(USART0);

    usart_baudrate_set(USART0, 115200);    // 波特率
    usart_parity_config(USART0, USART_PM_NONE); // 校验位
    usart_word_length_set(USART0, USART_WL_8BIT); // 数据位数
    usart_stop_bit_set(USART0, USART_STB_1BIT); // 停止位
    usart_data_first_config(USART0, USART_MSBF_LSB); // 先发送高位还是低位

    // 发送功能配置
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    // 接收功能配置
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    // 接收中断配置
    nvic_irq_enable(USART0_IRQn, 2, 2);
    // usart int rbne
//    usart_interrupt_enable(USART0, USART_INT_RBNE);
    usart_interrupt_enable(USART0, USART_INT_IDLE);

    // 使能串口
    usart_enable(USART0);
    usart0_dma_tx_config();
    usart0_dma_rx_config();
}

void USART0_IRQHandler(void) {
//    if ((usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) == SET) {
//        uint16_t value = usart_data_receive(USART0);
//        g_recv_buff[g_recv_length] = (uint8_t) value;
//        g_recv_length++;
//    }
    if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET) {
        //读取缓冲区,清空缓冲区
        usart_data_receive(USART0);
        dma_channel_disable(DMA1,DMA_CH2);
        uint32_t len = USART_RECEIVE_LENGTH - dma_transfer_number_get(DMA1,DMA_CH2);
        if(len >0){
            g_recv_buff[len] = '\0';
            printf("g_recv_buff:%s\n", g_recv_buff);
            printf("len:%d\n", len);
        }
//        dma_transfer_number_config(DMA1,DMA_CH2,USART_RECEIVE_LENGTH);
//        dma_memory_address_config(DMA1,DMA_CH2,DMA_MEMORY_0,(uint32_t)g_recv_buff);
        dma_flag_clear(DMA1, DMA_CH2,DMA_FLAG_FTF);//清除标记要在开启DMA之前
        dma_channel_enable(DMA1,DMA_CH2);
    }
}