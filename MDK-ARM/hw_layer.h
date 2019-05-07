#pragma once
#include "stm32f7xx_hal.h"

extern ADC_HandleTypeDef hadc1;

#define PUMP &TIM3->CCR2
#define AIR_BLOWER &TIM4->CCR3
#define LEDS &TIM4->CCR4
#define COIL &TIM13->CCR1

#define DHT_GPIO GPIOA
#define DHT_PIN GPIO_PIN_5

#define LIGHT ADC_CHANNEL_10
#define POWER ADC_CHANNEL_3
#define SOIL ADC_CHANNEL_13

void Init_hw(void);

int GetPowerConsumption(void);
int GetLightSaturation(void);
int GetSoilMoisture(void);
int GetTemperature(void);
int GetHumidity(void);

unsigned GetControlState(volatile unsigned *control);
void SetControlState(volatile unsigned *control, unsigned value);
void ChangeControlState(volatile unsigned *control, int delta);

