#ifndef __BSP_DHT11_H
#define __BSP_DHT11_H

#include "gd32f4xx.h"
#include "systick.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define  DHT_SET   gpio_bit_set(GPIOD, GPIO_PIN_3)
#define  DHT_RESET gpio_bit_reset(GPIOD, GPIO_PIN_3)

#define  DHT      gpio_input_bit_get(GPIOD, GPIO_PIN_3)

void DHT11_init();
u8 DHT11_read_data(u8* dat);
u8 DHT11_get_temperature(u8 *humidity, float *temperature);

#endif // !__BSP_DHT11_H
