#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "main_injection.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim13;

extern UART_HandleTypeDef huart3;
extern ADC_HandleTypeDef hadc1;

const int bufSize = 20;
static uint8_t txbuf[bufSize] = {0};
static uint8_t rxbuf[bufSize] = {0};
static uint8_t ch;
static int pos = 0;

void HandleHostString(UART_HandleTypeDef *huart, const char *data){
	//int val = atoi(data);
	//TIM4->CCR4 = TIM4->CCR3 = val;
	//HAL_UART_Transmit_IT(huart, txbuf, sprintf((char *)txbuf, "Changed %d\n", val));
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
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	GPIO_InitTypeDef gpioInitStruct;
	
	// Leds init
	gpioInitStruct.Pin = GREEN_LED_PIN | RED_LED_PIN | BLUE_LED_PIN;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(LEDS_GPIO, &gpioInitStruct);
	
	gpioInitStruct.Pin = GPIO_PIN_4;
	gpioInitStruct.Pull = GPIO_NOPULL;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &gpioInitStruct);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	
	//PWM init
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	
	HAL_UART_Receive_IT(&huart3, &ch, 1);
}

void Loop(){
					HAL_ADC_Start(&hadc1); 
          HAL_ADC_PollForConversion(&hadc1, 100); 
          int adc = HAL_ADC_GetValue(&hadc1); 
          HAL_ADC_Stop(&hadc1); 
          HAL_UART_Transmit_IT(&huart3, txbuf, sprintf((char *)txbuf, "%d\n", adc));
  switch (UpdateDht11()){
		case DHT_OK:
			GREEN_ON
		HAL_UART_Transmit_IT(&huart3, txbuf, sprintf((char *)txbuf, "%d%% %d\n", dht11Data.humidity, dht11Data.temperature));
		break;
		case DHT_NOT_RESPONDING:
			RED_ON
		break;
		case DHT_BAD_CHECKSUM:
			BLUE_ON
		break;
	}		
	HAL_Delay(1700);
	LEDS_OFF
}
	
