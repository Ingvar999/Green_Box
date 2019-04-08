#include <cstdio>
#include <cstdlib>

#include "main_injection.h"

extern UART_HandleTypeDef huart3;

static uint8_t txbuf[10] = {0};
static uint8_t rxbuf[10] = {0};

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart == &huart3){
		int val = atoi((char *)rxbuf);
		HAL_UART_Transmit_IT(huart, txbuf, sprintf((char *)txbuf, "%d\n", val));
		HAL_UART_Receive_IT(huart, rxbuf, 3);
	}
}

void Init_Peripheral(){
	__HAL_RCC_GPIOB_CLK_ENABLE();
	//__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitTypeDef gpioInitStruct;
	
	// Leds init
	gpioInitStruct.Pin = GREEN_LED_PIN | RED_LED_PIN | BLUE_LED_PIN;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(LEDS_GPIO, &gpioInitStruct);
	
	//HAL_GPIO_WritePin(LEDS_GPIO, GREEN_LED_PIN | RED_LED_PIN, GPIO_PIN_SET);
	
	//USART3 init
	/*gpioInitStruct.Pin = USART3_RX_PIN | USART3_TX_PIN;
	gpioInitStruct.Pull = GPIO_PULLUP;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInitStruct.Mode = GPIO_MODE_AF_PP;
	gpioInitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(USART3_GPIO, &gpioInitStruct);*/
	
	HAL_UART_Receive_IT(&huart3, rxbuf, 3);
}

void Loop(){

}
	
