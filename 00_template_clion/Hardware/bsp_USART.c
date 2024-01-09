//
// Created by pc on 2024/1/3.
//

#include "bsp_USART.h"

#define USART_RECEIVE_LENGTH  1024
//���ڽ��ջ�������С
uint8_t g_recv_buff[USART_RECEIVE_LENGTH];   // ���ջ�����

uint8_t dat = 0;
//���յ��ַ���ŵ�λ��
int g_recv_length = 0;

static void usart0_dma_config() {
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(DMA1, DMA_CH7);
    dma_single_data_parameter_struct init_struct;
    init_struct.periph_addr = (uint32_t)(&USART_DATA(USART0)); // �����ַ
    init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;//�����ַ�Ƿ�����
    init_struct.memory0_addr = 0U; // �ڴ��ַ
    init_struct.memory_inc = DMA_PERIPH_INCREASE_ENABLE;//�ڴ��ַ�Ƿ�����
    init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT; // �������ݿ��
    init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;// ѭ��ģʽ
    init_struct.direction = DMA_MEMORY_TO_PERIPH; // ���䷽��
    init_struct.number = 0U; // ���ݸ���
    init_struct.priority = DMA_PRIORITY_LOW; // ���ȼ�
    dma_single_data_mode_init(DMA1, DMA_CH7, &init_struct);
    //����ͨ������ѡ��
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);
}

//����һbyte����
void send_data(uint8_t data) {
    //ͨ��USART����
//    usart_data_transmit(USART0, data);
    //�жϻ������Ƿ��Ѿ�����
    dat = data;
    dma_memory_address_config(DMA1,DMA_CH7,DMA_MEMORY_0,(uint32_t)(&dat));
    dma_transfer_number_config(DMA1,DMA_CH7,1);
////    //����DMA
    dma_channel_enable(DMA1, DMA_CH7);
    //FlagStatus state = usart_flag_get(USART_NUM,USART_FLAG_TBE);
//    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    while (RESET == dma_flag_get(DMA1, DMA_CH7,DMA_FLAG_FTF));
    dma_flag_clear(DMA1, DMA_CH7,DMA_FLAG_FTF);
}

//�����ַ���
void send_string(char *data) {
    //����: 1.dataָ�벻Ϊ��  2.���͵����ݲ���\0�������
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
    rcu_periph_clock_enable(RCU_GPIOA);    // ����ʱ��
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    // rx
    rcu_periph_clock_enable(RCU_GPIOA); // ����ʱ��
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);
    //gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /************** usart config **************/
    // ����ʱ��
    rcu_periph_clock_enable(RCU_USART0);
    // USART��λ
    usart_deinit(USART0);

    usart_baudrate_set(USART0, 115200);    // ������
    usart_parity_config(USART0, USART_PM_NONE); // У��λ
    usart_word_length_set(USART0, USART_WL_8BIT); // ����λ��
    usart_stop_bit_set(USART0, USART_STB_1BIT); // ֹͣλ
    usart_data_first_config(USART0, USART_MSBF_LSB); // �ȷ��͸�λ���ǵ�λ

    // ���͹�������
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

	usart_dma_transmit_config(USART0,USART_TRANSMIT_DMA_ENABLE);
    // ���չ�������
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    // �����ж�����
    nvic_irq_enable(USART0_IRQn, 2, 2);
    // usart int rbne
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    usart_interrupt_enable(USART0, USART_INT_IDLE);

    // ʹ�ܴ���
    usart_enable(USART0);
    usart0_dma_config();
}

void USART0_IRQHandler(void) {
    if ((usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) == SET) {
        uint16_t value = usart_data_receive(USART0);
        g_recv_buff[g_recv_length] = (uint8_t) value;
        g_recv_length++;
    }
    if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET) {
        //��ȡ������,��ջ�����
        usart_data_receive(USART0);
        g_recv_buff[g_recv_length] = '\0';
        printf("%s\n", g_recv_buff);
        g_recv_length = 0;
    }
}