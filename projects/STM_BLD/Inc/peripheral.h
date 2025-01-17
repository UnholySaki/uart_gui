#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "stm32f103xb.h"
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_rcc.h>
#include <stm32f1xx_hal_uart.h>

void SystemClock_Config(void);
void MX_USART1_UART_Init(UART_HandleTypeDef huart);
void MX_GPIO_Init(void);

#endif