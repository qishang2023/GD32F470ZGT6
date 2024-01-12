//
// Created by pc on 2024/1/12.
//

#include "bsp_soft_i2c.h"


#define SCL_RCU        RCU_GPIOB
#define SCL_PORT    GPIOB
#define SCL_PIN        GPIO_PIN_6
#define SCL_AF        GPIO_AF_4

#define SDA_RCU        RCU_GPIOB
#define SDA_PORT    GPIOB
#define SDA_PIN        GPIO_PIN_7
#define SDA_AF        GPIO_AF_4

/************** io ***************/
#define SCL(BIT)        gpio_bit_write(SCL_PORT, SCL_PIN, BIT?SET:RESET)
#define SDA(BIT)        gpio_bit_write(SDA_PORT, SDA_PIN, BIT?SET:RESET)
#define SDA_STATE()    gpio_input_bit_get(SDA_PORT, SDA_PIN)

#define SDA_IN()    gpio_mode_set(SDA_PORT, GPIO_MODE_INPUT,  GPIO_PUPD_NONE, SDA_PIN)
#define SDA_OUT()    gpio_mode_set(SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SDA_PIN)

void SoftI2C_init() {
    // ʱ������
    rcu_periph_clock_enable(SCL_RCU);
    // �������ģʽ
    gpio_mode_set(SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SCL_PIN);
    gpio_output_options_set(SCL_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, SCL_PIN);

    // ʱ������
    rcu_periph_clock_enable(SDA_RCU);
    // �������ģʽ
    gpio_mode_set(SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SDA_PIN);
    gpio_output_options_set(SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, SDA_PIN);
}

static void start() {
    SDA_OUT();

    SDA(1);
    delay_1us(5);
    SCL(1);
    delay_1us(5);

    SDA(0);
    delay_1us(5);
    SCL(0);
    delay_1us(5);
}

static void stop() {
    SDA_OUT();

    SCL(0);
    SDA(0);

    SCL(1);
    delay_1us(5);
    SDA(1);
    delay_1us(5);
}

static void send(uint8_t data) {
    uint8_t i;
    SDA_OUT();

    for (i = 0; i < 8; i++) {
        if (data & 0x80) {
            SDA(1);
        } else {
            SDA(0);
        }
        SCL(1);
        delay_1us(5);
        SCL(0);
        delay_1us(5);
        data <<= 1;
    }
}

static uint8_t wait_ack() {
    int8_t retry = 10;

    SCL(0);
    SDA(1);
    SDA_IN();
    delay_1us(5);
    SCL(1);
    delay_1us(5);

    while (SDA_STATE() == 1 && retry > 0) {
        retry--;
        delay_1us(5);
    }

    if (retry <= 0) {
        stop();
        return 1;
    } else {
        SCL(0);
        SDA_OUT();
    }
    return 0;
}

static uint8_t recv() {
    uint8_t i, data;
    SDA_IN();
    data = 0;
    for (i = 0; i < 8; i++) {
        SCL(0);
        delay_1us(5);
        SCL(1);
        delay_1us(5);

        data <<= 1;

        data |= SDA_STATE();

        delay_1us(5);
    }
    SCL(0);
    return data;
}

static void send_ack() {
    // ��������ACK��Ӧ

    // ������ȡSDA����Ȩ���������ģʽ
    SDA_OUT();
    // ����SDA
    SDA(0);
    delay_1us(5);

    // ����SCL
    SCL(1);
    delay_1us(5);

    // ����SCL
    SCL(0);
    delay_1us(5);
}

static void send_nack() {
    // ��������NACK��Ӧ
    // ������ȡSDA����Ȩ���������ģʽ
    SDA_OUT();
    // ����SDA
    SDA(1);
    delay_1us(5);

    // ����SCL
    SCL(1);
    delay_1us(5);

    // ����SCL
    SCL(0);
    delay_1us(5);
}

uint8_t SoftI2C_write(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len) {
    start();

    send(addr << 1);			//�����豸д��ַ
    if(wait_ack()) return 1;	//�ȴ���Ӧ

    send(reg);					//���ͼĴ�����ַ
    if(wait_ack()) return 2;	//�ȴ���Ӧ

    do {
        send(*data++);
        if(wait_ack()) return 3;
    } while(--len);

    stop();
    return 0;
}

uint8_t SoftI2C_read(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len) {
    start();

    send(addr << 1);				//�����豸д��ַ
    if(wait_ack()) return 1;		//�ȴ���Ӧ

    send(reg);						//���ͼĴ�����ַ
    if(wait_ack()) return 2;		//�ȴ���Ӧ

    start();
    send((addr << 1) | 0x01);		//�����豸����ַ
    if(wait_ack()) return 3;		//�ȴ���Ӧ

    do {
        *data = recv();
        data++;
        if(len != 1) send_ack();	// ���� ACK
    } while(--len);
    send_nack();					// ���� NACK
    stop();

    return 0;
}