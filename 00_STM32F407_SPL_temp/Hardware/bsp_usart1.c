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
//���黺��
uint8_t buffer[1024]={0};
//���Ԫ������
int16_t next_index = 0;

//uart����
void usart1_rx_tx_config(uint32_t baudval){
    GPIO_InitTypeDef GPIO_InitStructure;
    /**************** �������� ****************/
    //1.����TX��GPIO��
    //1.1 ��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //1.2 ����GPIOģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    gpio_mode_set(TX_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,TX_PIN);
    //���ѡ�����AF����ģʽ,����Ҫ����ѡ���ĸ�����
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
//    gpio_af_set(TX_PORT,TX_ALT,TX_PIN);

    /**************** �������� ****************/
    //1.1 ��ʱ��
//    rcu_periph_clock_enable(RX_RCU);
    //1.2 ����GPIOģʽ
//    gpio_mode_set(RX_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,RX_PIN);
    //���ѡ�����AF����ģʽ,����Ҫ����ѡ���ĸ�����
//    gpio_af_set(RX_PORT,RX_ALT,RX_PIN);

    USART_InitTypeDef USART_InitStruct;
    //2.����UARTͨ���������
    //��usart��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//    rcu_periph_clock_enable(USART_RCU);
    //����uart
    USART_DeInit(USART1);
    USART_StructInit(&USART_InitStruct);
    USART_InitStruct.USART_BaudRate = 115200;
    //����usart
    USART_Init(USART1, &USART_InitStruct);

    // ************* �����ж�����
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);//�ж����ȼ�����
//    nvic_irq_enable(USART_NVIC_IRQ, 2, 2);
    // usart int rbne
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
//    usart_interrupt_enable(USART_NUM, USART_INT_RBNE);
//    usart_interrupt_enable(USART_NUM, USART_INT_IDLE);
    //������
    USART_ClearFlag(USART1,USART_IT_RXNE);
//    usart_flag_clear(USART_NUM,USART_FLAG_RBNE);
    USART_Cmd(USART1, ENABLE);//�򿪴���
}

//abcdefg
//����һbyte����
void send_data(uint8_t data) {
    //ͨ��USART����
    USART_SendData(USART1, data);
    //�жϻ������Ƿ��Ѿ�����
    //FlagStatus state = usart_flag_get(USART_NUM,USART_FLAG_TBE);
    //�ȴ����ݼĴ������͸���λ�Ĵ���֮�� ��
    while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    //TC��־ָ��λ�Ĵ���Ϊ��
//		if(SET == usart_flag_get(USART0, USART_FLAG_TC)){
//			usart_disable(USART0);
//		}
}


//�ַ�������
void send_string(char data[]){
    for (; *data != 0;	data++)  send_data(*data); 	//����ֹͣ��0����
}

//printf�ڲ������������
PUTCHAR_PROTOTYPE
{
    //ͨ��USART����
    send_data((uint8_t)ch);
    return ch;
}

//USART�����жϴ�����(ϵͳ�ڲ�����)
void USART_IRQHandler(void){
    //printf("exec_recv\n");
    //1.RBNE�ж�,������յ���һ���ַ�,�����ȴ洢
    if(SET==USART_GetFlagStatus(USART1,USART_FLAG_RXNE)){
        //printf("");
        //�����־λ
        //��ȡ���ݼĴ����е�����
        uint8_t data = (uint8_t)USART_ReceiveData(USART1);
        buffer[next_index] = data;
        next_index++;
    }
    //2.IDLE�ж�,����һ�����ݽ���,��Ҫ����(��ӡ)
    if(SET==USART_GetFlagStatus(USART_NUM,USART_FLAG_IDLE)){
        //��ȡ���״̬
        USART_ReceiveData(USART_NUM);
        //������\0
//		buffer[next_index] = '\0';
//		printf("recv_data:%s\n",buffer);
        recv_data_callback(buffer,next_index);
        next_index = 0;
    }
}