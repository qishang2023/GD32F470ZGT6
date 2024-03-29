//
// Created by pc on 2024/1/15.
//

#include "bsp_hard_i2c.h"

#define    TIMEOUT    50000

static uint8_t I2C_wait(uint32_t flag) {
    uint16_t cnt = 0;

    while (!i2c_flag_get(I2C0, flag)) {
        cnt++;
        if (cnt > TIMEOUT) return 1;
    }
    return 0;
}

static uint8_t I2C_waitn(uint32_t flag) {
    uint16_t cnt = 0;

    while (i2c_flag_get(I2C0, flag)) {
        cnt++;
        if (cnt > TIMEOUT) return 1;
    }
    return 0;
}

void bsp_hard_i2c_init() {
    //开启GPIO复用模式
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_6);
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    //配置I2C
    rcu_periph_clock_enable(RCU_I2C0);
    i2c_clock_config(I2C0, 100000, I2C_DTCY_2);
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
    i2c_enable(I2C0);
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

uint8_t bsp_hard_I2C_write(uint8_t addr, uint8_t reg, const uint8_t *data, uint32_t len) {
    uint32_t i2cx = I2C0;
    uint8_t address = addr << 1;

    /************* start ***********************/
    // 等待I2C闲置
    if(I2C_waitn(I2C_FLAG_I2CBSY)) return 1;

    // start
    i2c_start_on_bus(i2cx);

    // 等待I2C主设备成功发送起始信号
    if(I2C_wait(I2C_FLAG_SBSEND)) return 2;

    /************* device address **************/
    // 发送设备地址
    i2c_master_addressing(i2cx, address, I2C_TRANSMITTER);
    // 等待地址发送完成
    if(I2C_wait(I2C_FLAG_ADDSEND)) return 3;
    i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);

    /************ register address ************/
    // 寄存器地址
    // 等待发送数据缓冲区为空
    if(I2C_wait(I2C_FLAG_TBE)) return 4;

    // 发送数据
    i2c_data_transmit(i2cx, reg);

    // 等待数据发送完成
    if(I2C_wait(I2C_FLAG_BTC)) return 5;

    /***************** data ******************/
    // 发送数据
    uint32_t i;
    for(i = 0; i < len; i++) {
        uint32_t d = data[i];

        // 等待发送数据缓冲区为空
        if(I2C_wait(I2C_FLAG_TBE)) return 6;

        // 发送数据
        i2c_data_transmit(i2cx, d);

        // 等待数据发送完成
        if(I2C_wait(I2C_FLAG_BTC)) return 7;
    }

    /***************** stop ********************/
    // stop
    i2c_stop_on_bus(i2cx);
    if(I2C_waitn(I2C_CTL0(i2cx)&I2C_CTL0_STOP)) return 8;

    return 0;
}

uint8_t bsp_hard_I2C_read(uint8_t addr, uint8_t reg, uint8_t *data, uint32_t len) {
    uint32_t i2cx = I2C0;
    uint8_t address = addr << 1;

    /************* start ***********************/
    // 等待I2C空闲
    if(I2C_waitn(I2C_FLAG_I2CBSY)) return 1;
    // 发送启动信号
    i2c_start_on_bus(i2cx);
    if(I2C_wait(I2C_FLAG_SBSEND)) return 2;

    /************* device address **************/
    // 发送从设备写地址
    i2c_master_addressing(i2cx, address, I2C_TRANSMITTER);

    if(I2C_wait(I2C_FLAG_ADDSEND)) return 3;
    i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);

    /********** register address **************/
    // 等待发送缓冲区
    if(I2C_wait(I2C_FLAG_TBE)) return 4;

    // 发送寄存器地址
    i2c_data_transmit(i2cx, reg);

    // 等待发送数据完成
    if(I2C_wait(I2C_FLAG_BTC)) return 5;

    /************* start ***********************/
    // 发送再启动信号
    i2c_start_on_bus(i2cx);
    if(I2C_wait(I2C_FLAG_SBSEND)) return 7;

    /************* device address **************/
    // 发送从设备读地址
    i2c_master_addressing(i2cx, address + 1, I2C_RECEIVER);
    if(I2C_wait(I2C_FLAG_ADDSEND)) return 8;
    i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);
    /************* data **************/
    //ack
    i2c_ack_config(i2cx, I2C_ACK_ENABLE);
    i2c_ackpos_config(i2cx, I2C_ACKPOS_CURRENT);
    if(I2C_wait((I2C_CTL0(i2cx) & I2C_CTL0_ACKEN))) return 23;
    // 读取数据
    uint32_t i;
    for (i = 0; i < len; i++) {
        if(i != len - 1) {
            // 等待ACK发送完成
            if(I2C_wait(I2C_FLAG_BTC)) return 9;
        }

        // 等待ACK数据发送完成
        // 等待接收缓冲区
        if(I2C_wait(I2C_FLAG_RBNE)) return 10;
        data[i] = i2c_data_receive(i2cx);

        if (i == len - 2) {
            // 在读取最后一个字节之前，禁用ACK，并发送停止信号
            // 配置自动NACK
            i2c_ack_config(i2cx, I2C_ACK_DISABLE);
        }
    }

    /***************** stop ********************/
    i2c_stop_on_bus(i2cx);
    if(I2C_waitn(I2C_CTL0(I2C0) & I2C_CTL0_STOP)) return 11;
    return 0;
}