#pragma once
#include "stm32f7xx_hal.h"

#include "stdlib.h"

extern uint8_t txBuf[];

#define PrintSerial(pHuart, ...) HAL_UART_Transmit_IT(pHuart, txBuf, sprintf((char *)txBuf, __VA_ARGS__))
	
typedef void (*pStringHandler)(UART_HandleTypeDef *huart, const char *data);

void BeginSerialStringHandling(UART_HandleTypeDef *target, uint8_t terminator, pStringHandler handler);
