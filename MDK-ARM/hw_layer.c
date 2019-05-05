#include "hw_layer.h"
#include "dht11.h"

void Init_hw(){
	InitDht11(DHT_GPIO, DHT_PIN);
}

int GetPowerConsumption(void);
int GetLightSaturation(void);
int GetSoilMoisture(void);
int GetTemperature(void);
int GetHumidity(void);

unsigned GetControlState(unsigned *control);
void SetControlState(unsigned *control);
void ChangeControlState(unsigned *control, int delta);