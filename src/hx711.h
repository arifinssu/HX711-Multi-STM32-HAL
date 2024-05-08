#ifndef HX711_H_
#define HX711_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	GPIO_TypeDef **DOUT;
	uint16_t *PIN;
	GPIO_TypeDef *PD_SCK;
	uint16_t PD_SCK_PIN;
	uint8_t COUNT;
	uint8_t GAIN;
	int32_t *OFFSETS;
	int32_t *VALUEGRAM;
	float	COEF;
	int32_t VALUE[];
} hx711_HandleTypeDef;

void hx711_Init(hx711_HandleTypeDef *scale, int count, GPIO_TypeDef **dout, uint16_t *pin, GPIO_TypeDef *pd_sck, uint16_t pd_sck_pin, uint8_t gain);
void hx711_DeInit(hx711_HandleTypeDef *scale);
uint8_t hx711_IsReady(hx711_HandleTypeDef *scale);
void hx711_SetGain(hx711_HandleTypeDef *scale, uint8_t gain);
uint8_t hx711_GetCount(hx711_HandleTypeDef *scale);
uint8_t hx711_Tare(hx711_HandleTypeDef *scale, uint8_t times, uint16_t tolerance);
void hx711_Read(hx711_HandleTypeDef *scale, int32_t *result);
void hx711_ReadRaw(hx711_HandleTypeDef *scale, int32_t *result);
void hx711_PowerDown(hx711_HandleTypeDef *scale);
void hx711_PowerUp(hx711_HandleTypeDef *scale);
void hx711_loop(hx711_HandleTypeDef *scale);
int32_t hx711_getGram(hx711_HandleTypeDef *scale, uint8_t dataNum);
void hx711_CoefSet(hx711_HandleTypeDef *scale, float coef);
float hx711_CoefGet(hx711_HandleTypeDef *scale);
void hx711_weight(hx711_HandleTypeDef *scale);

#ifdef __cplusplus
}
#endif

#endif /* HX711_H_ */
