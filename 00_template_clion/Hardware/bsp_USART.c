//
// Created by pc on 2024/1/3.
//

#include "bsp_USART.h"

#define USART_RECEIVE_LENGTH  1024
//���ڽ��ջ�������С
uint8_t g_recv_buff[USART_RECEIVE_LENGTH];   // ���ջ�����
//���յ��ַ���ŵ�λ��
int g_recv_length = 0;

//����һbyte����
static void send_data(uint8_t data) {
    //ͨ��USART����
    usart_data_transmit(USART0, data);
    //�жϻ������Ƿ��Ѿ�����
    //FlagStatus state = usart_flag_get(USART_NUM,USART_FLAG_TBE);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE));
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
    uint32_t usartx_tx_rcu = RCU_GPIOA;
    uint32_t usartx_tx_port = GPIOA;
    uint32_t usartx_tx_pin = GPIO_PIN_9;
    uint32_t usartx_tx_af = GPIO_AF_7;

    uint32_t usartx_rx_rcu = RCU_GPIOA;
    uint32_t usartx_rx_port = GPIOA;
    uint32_t usartx_rx_pin = GPIO_PIN_10;
    uint32_t usartx_rx_af = GPIO_AF_7;

    uint32_t usartx = USART0;
    uint32_t usartx_rcu = RCU_USART0;
    uint32_t usartx_irqn = USART0_IRQn;

    uint32_t usartx_p_baudrate = 115200;
    uint32_t usartx_p_parity = USART_PM_NONE;
    uint32_t usartx_p_wl = USART_WL_8BIT;
    uint32_t usartx_p_stop_bit = USART_STB_1BIT;
    uint32_t usartx_p_data_first = USART_MSBF_LSB;

    /************** gpio config **************/
    // tx
    rcu_periph_clock_enable(usartx_tx_rcu);	// ����ʱ��
    gpio_mode_set(usartx_tx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, usartx_tx_pin);
    gpio_af_set(usartx_tx_port, usartx_tx_af, usartx_tx_pin);
    gpio_output_options_set(usartx_tx_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, usartx_tx_pin);
    // rx
    rcu_periph_clock_enable(usartx_rx_rcu); // ����ʱ��
    gpio_mode_set(usartx_rx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, usartx_rx_pin);
    gpio_af_set(usartx_rx_port, usartx_rx_af, usartx_rx_pin);
    //gpio_output_options_set(usartx_rx_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, usartx_rx_pin);

    /************** usart config **************/
    // ����ʱ��
    rcu_periph_clock_enable(usartx_rcu);
    // USART��λ
    usart_deinit(usartx);

    usart_baudrate_set(usartx, usartx_p_baudrate);	// ������
    usart_parity_config(usartx, usartx_p_parity); // У��λ
    usart_word_length_set(usartx, usartx_p_wl); // ����λ��
    usart_stop_bit_set(usartx, usartx_p_stop_bit); // ֹͣλ
    usart_data_first_config(usartx, usartx_p_data_first); // �ȷ��͸�λ���ǵ�λ

    // ���͹�������
    usart_transmit_config(usartx, USART_TRANSMIT_ENABLE);

    // ���չ�������
    usart_receive_config(usartx, USART_RECEIVE_ENABLE);
    // �����ж�����
    nvic_irq_enable(usartx_irqn, 2, 2);
    // usart int rbne
    usart_interrupt_enable(usartx, USART_INT_RBNE);
    usart_interrupt_enable(usartx, USART_INT_IDLE);

    // ʹ�ܴ���
    usart_enable(usartx);
}

void USART1_IRQHandler(void) {
    if ((usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) == SET) {
        uint16_t value = usart_data_receive(USART0);
        g_recv_buff[g_recv_length] = (uint8_t)value;
        g_recv_length++;
    }
    if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET) {
        //��ȡ������,��ջ�����
        usart_data_receive(USART0);
        g_recv_buff[g_recv_length] = '\0';
        printf("%s\n",g_recv_buff);
        g_recv_length = 0;
    }
}