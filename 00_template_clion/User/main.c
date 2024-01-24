#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_USART.h"

TaskHandle_t            StartTask_Handler;
TaskHandle_t            Task1_Handler;
TaskHandle_t            Task2_Handler;

void task1(void *pvParameters) {
    while(1) {
        vTaskDelay(300);
        gpio_bit_set(GPIOE, GPIO_PIN_3);
        vTaskDelay(300);
        gpio_bit_reset(GPIOE, GPIO_PIN_3);
    }
}

void task2(void *pvParameters) {
    while(1) {
        vTaskDelay(1000);
        gpio_bit_set(GPIOD, GPIO_PIN_7);
        vTaskDelay(1000);
        gpio_bit_reset(GPIOD, GPIO_PIN_7);
    }
}

void start_task(void *pvParameters) {
    taskENTER_CRITICAL();

    xTaskCreate((TaskFunction_t)task1,
                (const char*   )"task1",
                50,
                NULL,
                2,
                (TaskHandle_t*  )&Task1_Handler);
    xTaskCreate((TaskFunction_t)task2,
                (const char*   )"task2",
                50,
                NULL,
                2,
                (TaskHandle_t*  )&Task2_Handler);
    vTaskDelete(StartTask_Handler);

    taskEXIT_CRITICAL();
}

void GPIO_config() {
    // 1. 时钟初始化
    rcu_periph_clock_enable(RCU_GPIOE);
    // 2. 配置GPIO 输入输出模式
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
    // 3. 配置GPIO 模式的操作方式
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);

    // 1. 时钟初始化
    rcu_periph_clock_enable(RCU_GPIOD);
    // 2. 配置GPIO 输入输出模式
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
    // 3. 配置GPIO 模式的操作方式
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_7);
}


int main(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

    systick_config();
    GPIO_config();
    USART_config();
    xTaskCreate((TaskFunction_t)start_task,
                (const char*   )"start_task",
                128,
                NULL,
                1,
                (TaskHandle_t*  )&StartTask_Handler);
    vTaskStartScheduler();

    while(1) {}
}
