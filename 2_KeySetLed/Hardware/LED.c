#include "LED.h"

void SET_LED1(GPIO_PinState status)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, status);
}

void SET_LED2(GPIO_PinState status)
{
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, status);
}

void Toggle_LED(void)
{
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}
