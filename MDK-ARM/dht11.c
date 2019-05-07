#include "dwt_stm32_delay.h"
#include "dht11.h"
#include "main_injection.h"

struct _TH dht11Data;

static const int TIMEOUT = 100;

static GPIO_TypeDef *GPIOx;
static uint16_t pin;

static struct {
	uint8_t HI;
	uint8_t HD;
	uint8_t TI;
	uint8_t TD;
	uint8_t crc;
} data;

int GetDhtGetUs(GPIO_PinState state, uint32_t timeout){
	int count = 0;
	while (HAL_GPIO_ReadPin(GPIOx, pin) == state && count++ < timeout){
		DWT_Delay_us(1);
	}
	if (count >= timeout){
		return -1;
	}
	return count;
}

void InitDht11(GPIO_TypeDef *_GPIOx, uint16_t _pin){
	GPIOx = _GPIOx;
	pin = _pin;
	
	GPIO_InitTypeDef gpioInitStruct;
	
	gpioInitStruct.Pin = pin;
	gpioInitStruct.Pull = GPIO_PULLUP;
	gpioInitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpioInitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	HAL_GPIO_Init(GPIOx, &gpioInitStruct);
	
	DWT_Delay_Init();
}

DHT_STATUS UpdateDht11(void){
	uint8_t *buf = (uint8_t *)&data;
	int t;
	
	HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_RESET);
	HAL_Delay(30);
	HAL_GPIO_WritePin(GPIOx, pin, GPIO_PIN_SET);
	GetDhtGetUs(GPIO_PIN_SET, TIMEOUT);
	
	if (GetDhtGetUs(GPIO_PIN_RESET, TIMEOUT) == -1 || GetDhtGetUs(GPIO_PIN_SET, TIMEOUT) == -1){
		return DHT_NOT_RESPONDING;
	}
	for (int byte = 0; byte < 5; byte++){
	  *buf = 0;
		for (int i = 7; i >= 0; --i){
			t = GetDhtGetUs(GPIO_PIN_RESET, TIMEOUT);
			if (GetDhtGetUs(GPIO_PIN_SET, TIMEOUT) > t){
				*buf |= 1 << i;
			}
		}	
		buf++;
	}
	uint8_t sum = (uint8_t)(data.HD + data.HI + data.TD + data.TI);
	if (sum != data.crc){
		return DHT_BAD_CHECKSUM;
	}
	dht11Data.humidity = data.HI;
	dht11Data.temperature = data.TI;
	return DHT_OK;
}
