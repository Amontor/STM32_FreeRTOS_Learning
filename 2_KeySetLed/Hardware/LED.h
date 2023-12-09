#ifndef __LED_H__
#define __LED_H__

#include "stdint.h"
#include "main.h"

#define LED_OFF GPIO_PIN_RESET
#define LED_ON  GPIO_PIN_SET

void SET_LED1(GPIO_PinState status);
void SET_LED2(GPIO_PinState status);
void Toggle_LED(void);

#endif
