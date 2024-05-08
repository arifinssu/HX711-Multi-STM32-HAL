#ifndef HX711CONFIG_H_
#define HX711CONFIG_H_

#include "main.h"

#define HX711_CLK_GPIO  HX_CLK_GPIO_Port
#define HX711_CLK_PIN   HX_CLK_Pin

GPIO_TypeDef *HX711_DATA_GPIO[] = {
    HX_IN1_GPIO_Port,
    HX_IN2_GPIO_Port,
    HX_IN3_GPIO_Port
};

uint16_t HX711_DATA_PINS[] = {
    HX_IN1_Pin,
    HX_IN2_Pin,
    HX_IN3_Pin
};

#endif /* HX711CONFIG_H_ */
