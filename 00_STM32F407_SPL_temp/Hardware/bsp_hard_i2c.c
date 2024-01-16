#include "bsp_hard_i2c.h"

#define    TIMEOUT    50000

static uint8_t I2C_wait(uint32_t flag) {
    uint16_t cnt = 0;

    while (!I2C_GetFlagStatus(I2C1, flag)) {
        cnt++;
        if (cnt > TIMEOUT) return 1;
    }
    return 0;
}

static uint8_t I2C_waitn(uint32_t flag) {
    uint16_t cnt = 0;

    while (I2C_GetFlagStatus(I2C1, flag)) {
        cnt++;
        if (cnt > TIMEOUT) return 1;
    }
    return 0;
}

void bsp_i2c_config(){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_I2C1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

    I2C_InitTypeDef I2C_InitStruct;
    I2C_InitStruct.I2C_ClockSpeed = 400000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1,&I2C_InitStruct);
    I2C_Cmd(I2C1,ENABLE);
}

uint8_t bsp_hard_I2C_write(uint8_t addr, uint8_t reg, const uint8_t *data, uint32_t len) {
    uint8_t address = addr << 1;
    if(I2C_waitn(I2C_FLAG_BUSY)) return 1;
    I2C_GenerateSTART(I2C1,ENABLE);
    if(I2C_wait(I2C_FLAG_SB)) return 2;
    I2C_Send7bitAddress(I2C1,address,I2C_Direction_Transmitter);
    if(I2C_wait(I2C_FLAG_ADDR)) return 3;
    (void)(I2C1->SR2);//读取SR2寄存器，将ADDR标志位清零
    if(I2C_wait(I2C_FLAG_TXE)) return 4;
    I2C_SendData(I2C1,reg);
    if(I2C_wait(I2C_FLAG_BTF)) return 5;
    uint32_t i;
    for(i = 0; i < len; i++) {
        uint32_t d = data[i];

        // 等待发送数据缓冲区为空
        if(I2C_wait(I2C_FLAG_TXE)) return 6;

        // 发送数据
        I2C_SendData(I2C1, d);

        // 等待数据发送完成
        if(I2C_wait(I2C_FLAG_BTF)) return 7;
    }
    I2C_GenerateSTOP(I2C1,ENABLE);
    return 0;
}

uint8_t bsp_hard_I2C_read(uint8_t addr, uint8_t reg, uint8_t *data, uint32_t len) {
    uint8_t address = addr << 1;
    if(I2C_waitn(I2C_FLAG_BUSY)) return 1;
    I2C_GenerateSTART(I2C1,ENABLE);
    if(I2C_wait(I2C_FLAG_SB)) return 2;
    I2C_Send7bitAddress(I2C1,address,I2C_Direction_Transmitter);
    if(I2C_wait(I2C_FLAG_ADDR)) return 3;
    (void)(I2C1->SR2);//读取SR2寄存器，将ADDR标志位清零
    if(I2C_wait(I2C_FLAG_TXE)) return 4;
    I2C_SendData(I2C1,reg);
    if(I2C_wait(I2C_FLAG_BTF)) return 5;
    I2C_GenerateSTART(I2C1,ENABLE);
    if(I2C_wait(I2C_FLAG_SB)) return 6;
    I2C_Send7bitAddress(I2C1,address,I2C_Direction_Receiver);
    if(I2C_wait(I2C_FLAG_ADDR)) return 7;
    (void)(I2C1->SR2);//读取SR2寄存器，将ADDR标志位清零
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    I2C_PECPositionConfig(I2C1,I2C_PECPosition_Current);
    // 读取数据
    uint32_t i;
    for (i = 0; i < len; i++) {
        if(i != len - 1) {
            // 等待ACK发送完成
            if(I2C_wait(I2C_FLAG_BTF)) return 9;
        }

        // 等待ACK数据发送完成
        // 等待接收缓冲区
        if(I2C_wait(I2C_FLAG_RXNE)) return 10;
        data[i] = I2C_ReceiveData(I2C1);

        if (i == len - 2) {
            // 在读取最后一个字节之前，禁用ACK，并发送停止信号
            // 配置自动NACK
            I2C_AcknowledgeConfig(I2C1, DISABLE);
        }
    }
    I2C_GenerateSTOP(I2C1,ENABLE);
    return 0;
}