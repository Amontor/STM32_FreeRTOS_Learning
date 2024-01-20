#include "Key.h"
#include "gpio.h"
#include "cmsis_os.h"

uint8_t Get_GetNum(void)
{
    uint8_t KeyNum = 0;
    if(HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET)  //按键1按下
    {
        osDelay(20);
        while (HAL_GPIO_ReadPin(KEY_0_GPIO_Port, KEY_0_Pin) == GPIO_PIN_RESET);
        KeyNum = KEY1;
    }

    return KeyNum;
}
