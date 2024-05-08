#include "hx711.h"

void hx711_Init(hx711_HandleTypeDef *scale, int count, GPIO_TypeDef **dout, uint16_t *pin, GPIO_TypeDef *pd_sck, uint16_t pd_sck_pin, uint8_t gain) {
	scale->PD_SCK = pd_sck;
	scale->PD_SCK_PIN = pd_sck_pin;
	scale->DOUT = dout;
	scale->PIN = pin;
	scale->COUNT = count;
	scale->COEF = 0;
	HAL_GPIO_WritePin(scale->PD_SCK, scale->PD_SCK_PIN, GPIO_PIN_RESET);
	for (int i=0; i<count; i++) {
		HAL_GPIO_ReadPin(scale->DOUT[i], scale->PIN[i]);
	}
	hx711_SetGain(scale, gain);
	scale->OFFSETS = (int32_t *) malloc(scale->COUNT*sizeof(int32_t));
	for (int i=0; i<scale->COUNT; ++i) {
		scale->OFFSETS[i] = 0;
	}
}

void hx711_CoefSet(hx711_HandleTypeDef *scale, float coef) {
	scale->COEF = coef;
}

float hx711_CoefGet(hx711_HandleTypeDef *scale) {
	return scale->COEF;
}

void hx711_weight(hx711_HandleTypeDef *scale) {
	for (int i = 0; i<scale->COUNT; ++i) {
		scale->VALUE[i] = scale->VALUE[i] / scale->COEF;
	}
}

void hx711_loop(hx711_HandleTypeDef *scale) {
    if (hx711_IsReady(scale)) {
		hx711_Read(scale, scale->VALUE);
		// hx711_weight(scale);
	}
}

int32_t hx711_getGram(hx711_HandleTypeDef *scale, uint8_t dataNum) {
    // return scale->VALUE[dataNum];
	uint32_t result = scale->VALUE[dataNum] / scale->COEF;
	if (result < 0) result = 0;
	return result;
}

void hx711_DeInit(hx711_HandleTypeDef *scale) {
	free(scale->OFFSETS);
}

uint8_t hx711_IsReady(hx711_HandleTypeDef *scale) {
	uint8_t result = 1;
	for (int i = 0; i<scale->COUNT; ++i) {
		if (HAL_GPIO_ReadPin(scale->DOUT[i], scale->PIN[i]) == GPIO_PIN_SET) {
			result = 0;
		}
	}
	return result;
}

void hx711_SetGain(hx711_HandleTypeDef *scale, uint8_t gain) {
	switch (gain) {
		case 128:  
			scale->GAIN = 1;
			break;
		case 64: 
			scale->GAIN = 3;
			break;
		case 32:  
			scale->GAIN = 2;
			break;
	}
	HAL_GPIO_WritePin(scale->PD_SCK, scale->PD_SCK_PIN, GPIO_PIN_RESET);
	hx711_Read(scale, NULL);
}

uint8_t hx711_GetCount(hx711_HandleTypeDef *scale) {
	return scale->COUNT;
}

uint8_t hx711_Tare(hx711_HandleTypeDef *scale, uint8_t times, uint16_t tolerance) {
	int i,j;
	int32_t values[scale->COUNT];
	int32_t minValues[scale->COUNT];
	int32_t maxValues[scale->COUNT];
	for (i=0; i<scale->COUNT; ++i) {
		minValues[i]=0x7FFFFFFF;
		maxValues[i]=0x80000000;
	}
	for (i=0; i<times; ++i) {
		hx711_ReadRaw(scale, values);
		for (j=0; j<scale->COUNT; ++j) {
			if (values[j]<minValues[j]) {
				minValues[j]=values[j];
			}
			if (values[j]>maxValues[j]) {
				maxValues[j]=values[j];
			}
		}
	}
	if (tolerance!=0 && times>1) {
		for (i=0; i<scale->COUNT; ++i) {
			if (abs(maxValues[i]-minValues[i])>tolerance) {
				return 0;
			}
		}
	}
	for (i=0; i<scale->COUNT; ++i) {
		scale->OFFSETS[i] = values[i];
	}
	return 1;
}

void hx711_Read(hx711_HandleTypeDef *scale, int32_t *result) {
	hx711_ReadRaw(scale, result);
	if (NULL!=result) {
		for (int j = 0; j < scale->COUNT; ++j) {
			result[j] -= scale->OFFSETS[j];
		}
	}
}

void hx711_ReadRaw(hx711_HandleTypeDef *scale, int32_t *result) {
	int i,j;
	// if (!hx711_IsReady(scale)) return;
	while (!hx711_IsReady(scale));
	for (i = 0; i < 24; ++i) {
		HAL_GPIO_WritePin(scale->PD_SCK, scale->PD_SCK_PIN, GPIO_PIN_SET);
		if (NULL!=result) {
			for (j = 0; j < scale->COUNT; ++j) {
				if (HAL_GPIO_ReadPin(scale->DOUT[j], scale->PIN[j]) == GPIO_PIN_SET) {
					result[j] |= 1 << (23-i);
				} else {
					result[j] &= ~(1 << (23-i));
				}
			}
		}
		HAL_GPIO_WritePin(scale->PD_SCK, scale->PD_SCK_PIN, GPIO_PIN_RESET);
	}
	for (i = 0; i < scale->GAIN; ++i) {
		HAL_GPIO_WritePin(scale->PD_SCK, scale->PD_SCK_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(scale->PD_SCK, scale->PD_SCK_PIN, GPIO_PIN_RESET);
	}
	if (NULL!=result) {
		for (j = 0; j < scale->COUNT; ++j) {
			if ( ( result[j] & 0x00800000 ) ) {
				result[j] |= 0xFF000000;
			} else {
				result[j] &= 0x00FFFFFF;
			}
		}
	}
}

void hx711_PowerDown(hx711_HandleTypeDef *scale) {
	HAL_GPIO_WritePin(scale->PD_SCK, scale->PD_SCK_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(scale->PD_SCK, scale->PD_SCK_PIN, GPIO_PIN_SET);
}

void hx711_PowerUp(hx711_HandleTypeDef *scale) {
	HAL_GPIO_WritePin(scale->PD_SCK, scale->PD_SCK_PIN, GPIO_PIN_RESET);
}
