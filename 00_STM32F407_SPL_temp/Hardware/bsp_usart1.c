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
uint8_t buffer[1024]={0};
//添加元素索引
int16_t next_index = 0;

//uart配置
void usart1_rx_tx_config(uint32_t baudval){
    GPIO_InitTypeDef GPIO_InitStructure;
    /**************** 发送引脚 ****************/
    //1.配置TX的GPIO口
    //1.1 打开时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //1.2 配置GPIO模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    gpio_mode_set(TX_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,TX_PIN);
    //如果选择的是AF复用模式,还需要配置选择哪个复用
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
//    gpio_af_set(TX_PORT,TX_ALT,TX_PIN);

    /**************** 接收引脚 ****************/
    //1.1 打开时钟
//    rcu_periph_clock_enable(RX_RCU);
    //1.2 配置GPIO模式
//    gpio_mode_set(RX_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,RX_PIN);
    //如果选择的是AF复用模式,还需要配置选择哪个复用
//    gpio_af_set(RX_PORT,RX_ALT,RX_PIN);

    USART_InitTypeDef USART_InitStruct;
    //2.配置UART通信相关内容
    //打开usart的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//    rcu_periph_clock_enable(USART_RCU);
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
//    nvic_irq_enable(USART_NVIC_IRQ, 2, 2);
    // usart int rbne
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
//    usart_interrupt_enable(USART_NUM, USART_INT_RBNE);
//    usart_interrupt_enable(USART_NUM, USART_INT_IDLE);
    //清理标记
    USART_ClearFlag(USART1,USART_IT_RXNE);
//    usart_flag_clear(USART_NUM,USART_FLAG_RBNE);
    USART_Cmd(USART1, ENABLE);//打开串口
}

//abcdefg
//发送一byte数据
void send_data(uint8_t data) {
    //通过USART发送
    USART_SendData(USART1, data);
    //判断缓冲区是否已经空了
    //FlagStatus state = usart_flag_get(USART_NUM,USART_FLAG_TBE);
    //等待数据寄存器发送给移位寄存器之后 空
    while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    //TC标志指移位寄存器为空
//		if(SET == usart_flag_get(USART0, USART_FLAG_TC)){
//			usart_disable(USART0);
//		}
}


//字符串发送
void send_string(char data[]){
    for (; *data != 0;	data++)  send_data(*data); 	//遇到停止符0结束
}

//printf内部调用这个方法
PUTCHAR_PROTOTYPE
{
    //通过USART发送
    send_data((uint8_t)ch);
    return ch;
}

//USART串口中断处理函数(系统内部调用)
void USART_IRQHandler(void){
    //printf("exec_recv\n");
    //1.RBNE中断,代表接收到了一个字符,数组先存储
    if(SET==USART_GetFlagStatus(USART1,USART_FLAG_RXNE)){
        //printf("");
        //清理标志位
        //读取数据寄存器中的数据
        uint8_t data = (uint8_t)USART_ReceiveData(USART1);
        buffer[next_index] = data;
        next_index++;
    }
    //2.IDLE中断,代表一包数据结束,需要处理(打印)
    if(SET==USART_GetFlagStatus(USART_NUM,USART_FLAG_IDLE)){
        //读取清除状态
        USART_ReceiveData(USART_NUM);
        //最后加上\0
//		buffer[next_index] = '\0';
//		printf("recv_data:%s\n",buffer);
        recv_data_callback(buffer,next_index);
        next_index = 0;
    }
}