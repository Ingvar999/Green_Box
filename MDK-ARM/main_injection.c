#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "main_injection.h"
#include "hw_layer.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim13;

extern UART_HandleTypeDef huart3;

const int bufSize = 50;
static uint8_t txbuf[bufSize] = {0};
static uint8_t rxbuf[bufSize] = {0};
static uint8_t ch;
static int pos = 0;

void HandleHostString(UART_HandleTypeDef *huart, const char *data){
	int val = atoi(data + 1);
	switch (data[0]){
		case 'p':
			SetControlState(PUMP, val);
		break;
		case 'l':
			SetControlState(LEDS, val);
		break;
		case 'a':
			SetControlState(AIR_BLOWER, val);
		break;
		case 'c':
			SetControlState(COIL, val);
		break;
		default:
			HAL_UART_Transmit_IT(&huart3, txbuf, sprintf((char *)txbuf, "ok\n"));
		break;
	}
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
	
	//PWM init
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	
	HAL_UART_Receive_IT(&huart3, &ch, 1);
	
	Init_hw();
}

void Loop(){
	HAL_Delay(1700);
	HAL_UART_Transmit_IT(&huart3, txbuf, sprintf((char *)txbuf, "%d %d %d %d %d\n", GetHumidity(), GetTemperature(),
		GetLightSaturation(), GetSoilMoisture(), GetPowerConsumption()));
}
	
