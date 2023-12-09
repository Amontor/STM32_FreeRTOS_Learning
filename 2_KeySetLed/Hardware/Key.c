#include "Key.h"
#include "gpio.h"
#include "cmsis_os.h"

uint8_t Get_GetNum(void)
{
    uint8_t KeyNum = 0;
    if(HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET)  //按键1按下
    {
        osDelay(20);
        while (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == GPIO_PIN_RESET);
        KeyNum = KEY1;
    }

    if(HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == GPIO_PIN_RESET)  //按键2按下
    {
        osDelay(20);
        while (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == GPIO_PIN_RESET);
        KeyNum = KEY2;
    }

    if(HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin) == GPIO_PIN_SET)  //按键3按下
    {
        osDelay(20);
        while (HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin) == GPIO_PIN_SET);
        KeyNum = KEY3;
    }

    return KeyNum;
}
