#include "hw_layer.h"
#include "dht11.h"
#include "main_injection.h"

extern ADC_HandleTypeDef hadc1;

void Init_hw(void){
	InitDht11(DHT_GPIO, DHT_PIN); 
}

static unsigned ReadAnalog(unsigned channel){
	ADC_ChannelConfTypeDef sConfig;
	
	sConfig.Channel = channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100); 
  int result = HAL_ADC_GetValue(&hadc1); 
	HAL_ADC_Stop(&hadc1);
	return result;
}

static int UpdateDHT(void){
	static const unsigned interval = 3000;
	static unsigned lastUpdate = 0;
	unsigned time = HAL_GetTick();
	int result = 0;
	
	if (time - lastUpdate > interval){
		lastUpdate = time;
		int i = 3;
		do{
			switch (UpdateDht11()){
				case DHT_OK:
					result = 0;
				break;
				case DHT_NOT_RESPONDING:
					result = 1;
				break;
				case DHT_BAD_CHECKSUM:
					result = 2;
				break;
				default:
					result = -1;
				break;
			}		
		}while (result != 0 && --i != 0);
	}
	return result;
}

int GetPowerConsumption(void){
	static const float e = 19.336;
	unsigned v = ReadAnalog(POWER);
	return (int)(v * e);
}

int GetLightSaturation(void){
	return ReadAnalog(LIGHT);
}

int GetSoilMoisture(void){
	return ReadAnalog(SOIL);
}

int GetTemperature(void){
	if (UpdateDHT() == 0){
		return dht11Data.temperature;
	}
	RED_ON
	return -1;
}

int GetHumidity(void){
	if (UpdateDHT() == 0){
		return dht11Data.humidity;
	}
	RED_ON
	return -1;
}

unsigned GetControlState(volatile unsigned *control){
	return *control;
}

void SetControlState(volatile unsigned *control, unsigned value){
	*control = value;
}

void ChangeControlState(volatile unsigned *control, int delta){
	*control += delta;
}
