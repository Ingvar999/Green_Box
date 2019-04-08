#pragma once

#include "main.h"
#include "stm32f7xx_hal.h"

#define LEDS_GPIO GPIOB
#define GREEN_LED_PIN GPIO_PIN_0
#define BLUE_LED_PIN GPIO_PIN_7
#define RED_LED_PIN GPIO_PIN_14

#define USART3_GPIO GPIOD
#define USART3_TX_PIN GPIO_PIN_8
#define USART3_RX_PIN GPIO_PIN_9

#define LEDS_OFF HAL_GPIO_WritePin(LEDS_GPIO, RED_LED_PIN | BLUE_LED_PIN | GREEN_LED_PIN, GPIO_PIN_RESET);
#define RED_ON HAL_GPIO_WritePin(LEDS_GPIO, RED_LED_PIN, GPIO_PIN_SET);
#define BLUE_ON HAL_GPIO_WritePin(LEDS_GPIO, BLUE_LED_PIN, GPIO_PIN_SET);
#define GREEN_ON HAL_GPIO_WritePin(LEDS_GPIO, GREEN_LED_PIN, GPIO_PIN_SET);

void Init_Peripheral(void);
void Loop(void);
