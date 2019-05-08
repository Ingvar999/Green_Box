#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "main_injection.h"
#include "hw_layer.h"
#include "serial.h"
#include "task_manager.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart3;

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
			PrintSerial(huart, "ok\n");
		break;
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
	
	//PWM start
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	
	Init_hw();
	
	BeginSerialStringHandling(&huart3, '\n', HandleHostString);
}

void Loop(){
	HAL_Delay(1700);
	PrintSerial(&huart3, "%d %d %d %d %d\n", GetHumidity(), GetTemperature(),
		GetLightSaturation(), GetSoilMoisture(), GetPowerConsumption());
}
	
