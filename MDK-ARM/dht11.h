#ifndef DHT11
#define DHT11

#include "stm32f7xx_hal.h"

typedef enum {DHT_OK, DHT_NOT_RESPONDING, DHT_BAD_CHECKSUM} DHT_STATUS;
struct _TH {
	uint8_t temperature, humidity;
};

extern struct _TH dht11Data;

void InitDht11(GPIO_TypeDef *GPIOx, uint16_t pin);
DHT_STATUS UpdateDht11(void);

#endif
