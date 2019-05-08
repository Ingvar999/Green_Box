#include "serial.h"

static const int rxBufSize = 30;
static const int txBufSize = 50;
uint8_t txBuf[txBufSize] = {0};
static uint8_t rxBuf[rxBufSize] = {0};
static uint8_t ch;
static int pos = 0;
static UART_HandleTypeDef *target;
static uint8_t terminator;
static pStringHandler handler;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart == target){
		if (ch == terminator){
			rxBuf[pos] = 0;
			handler(huart, (char *)rxBuf);
			pos = 0;
		}	
		else{
			if (pos < rxBufSize){
				rxBuf[pos++] = ch;
			}
			else{
				// buffer overflow
			}
		}	
		HAL_UART_Receive_IT(huart, &ch, 1);
	}
}

void BeginSerialStringHandling(UART_HandleTypeDef *_target, uint8_t _terminator, pStringHandler _handler){
	target = _target;
	terminator = _terminator;
	handler = _handler;
	HAL_UART_Receive_IT(target, &ch, 1);
}
