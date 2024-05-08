# HX711-Multi-STM32-HAL Library

*Special thanks to Tal Achituv for his amazing work in https://github.com/compugician/HX711-multi*

**HX711-Multi-STM32-HAL** is a HX711 library that specialized for STM32Cube based board and firmware, to interface multiple [Avia Semiconductor HX711 24-Bit Analog-to-Digital Converter (ADC)](http://www.dfrobot.com/image/data/SEN0160/hx711_english.pdf). for reading load cells / weight scales.

## First Setup

To use this library, you must setup the CLK pin and DATA pin in `src/hx711Config.h`. Take a look in line 6 to 19 in this header: 
```c
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
```

## Getting Started

Getting started with **HX711 Multi** is easy. Take a look at this simple example:

```c
#include "hx711.h"
#include "hx711Config.h"
hx711_HandleTypeDef scale;

int main(void) {
	// hx711 setup
    hx711_Init(&scale, 3, HX711_DATA_GPIO, HX711_DATA_PINS, HX711_CLK_GPIO, HX711_CLK_PIN, 128);
    hx711_CoefSet(&scale, 405.0);
    hx711_Tare(&scale, 20, 10000);
	while(1) {
		// loop hx711 scaling
		hx711_loop(&scale);
		
		// get hx711 result
		int weight1 = hx711_getGram(&scale, 0);
		int weight2 = hx711_getGram(&scale, 1);
		int weight3 = hx711_getGram(&scale, 2);
	}
}
```
