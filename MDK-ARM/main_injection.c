#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "main_injection.h"

extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim4;

const int bufSize = 20;
static uint8_t txbuf[bufSize] = {0};
static uint8_t rxbuf[bufSize] = {0};
static uint8_t ch;
static int pos = 0;

void HandleHostString(UART_HandleTypeDef *huart, const char *data){
	int val = atoi(data);
	TIM4->CCR4 = val;
	HAL_UART_Transmit_IT(huart, txbuf, sprintf((char *)txbuf, "%d\n", val));
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart == &huart3){
		if (ch == '\n'){
			rxbuf[pos] = 0;
			HandleHostString(huart, (char *)rxbuf);
			pos = 0;
		}	
		else{
			if (pos < bufSize){
				rxbuf[pos++] = ch;
			}
			else{
				// buffer overflow
			}
		}	
		HAL_UART_Receive_IT(huart, &ch, 1);
	}
}

void Init_Peripheral(){
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef gpioInitStruct;
	
	// Leds init
	gpioInitStruct.Pin = GREEN_LED_PIN | RED_LED_PIN | BLUE_LED_PIN;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(LEDS_GPIO, &gpioInitStruct);
	
	//HAL_GPIO_WritePin(LEDS_GPIO, GREEN_LED_PIN | RED_LED_PIN, GPIO_PIN_SET);
	
	//PWM init
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	
	//USART3 init
	/*gpioInitStruct.Pin = USART3_RX_PIN | USART3_TX_PIN;
	gpioInitStruct.Pull = GPIO_PULLUP;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	gpioInitStruct.Mode = GPIO_MODE_AF_PP;
	gpioInitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(USART3_GPIO, &gpioInitStruct);*/
	
	HAL_UART_Receive_IT(&huart3, &ch, 1);
}

void Loop(){

}
	
