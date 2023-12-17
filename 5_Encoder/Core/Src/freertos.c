/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
int32_t Encoder_count = 0;
/* USER CODE END Variables */
/* Definitions for Encoder */
osThreadId_t EncoderHandle;
const osThreadAttr_t Encoder_attributes = {
  .name = "Encoder",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Encoder_Entry(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Encoder */
  EncoderHandle = osThreadNew(Encoder_Entry, NULL, &Encoder_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Encoder_Entry */
/**
  * @brief  Function implementing the Encoder thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Encoder_Entry */
void Encoder_Entry(void *argument)
{
  /* USER CODE BEGIN Encoder_Entry */
  OLED_Init();
  OLED_Display_On();
  OLED_Clear();
  OLED_ShowString(0,0,"Num:",16,0);
  /* Infinite loop */
  for(;;)
  {
    OLED_ShowSignedNum(32,0,Encoder_count,5,16,0);
    osDelay(10);
  }
  /* USER CODE END Encoder_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//DT引脚接PB12, clk引脚接PB13,根据时序完成对应的判断
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == Encoder_0_Pin)
    {
        if(HAL_GPIO_ReadPin(Encoder_1_GPIO_Port, Encoder_1_Pin) == GPIO_PIN_RESET)
        {
            Encoder_count++;
        }
    }
    if(GPIO_Pin == Encoder_1_Pin)
    {
        if(HAL_GPIO_ReadPin(Encoder_0_GPIO_Port, Encoder_0_Pin) == GPIO_PIN_RESET)
        {
            Encoder_count--;
        }
    }
}
/* USER CODE END Application */

