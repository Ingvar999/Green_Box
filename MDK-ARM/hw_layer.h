#pragma once
#include "stm32f7xx_hal.h"

extern ADC_HandleTypeDef hadc1;

#define PUMP TIM4->CCR3
#define AIR_BLOWER TIM4->CCR4
#define LEDS TIM3->CCR2
#define COIL TIM13->CCR1

#define DHT_GPIO GPIOF
#define DHT_PIN GPIO_PIN_12



void Init_hw();

int GetPowerConsumption(void);
int GetLightSaturation(void);
int GetSoilMoisture(void);
int GetTemperature(void);
int GetHumidity(void);

unsigned GetControlState(unsigned *control);
void SetControlState(unsigned *control);
void ChangeControlState(unsigned *control, int delta);

