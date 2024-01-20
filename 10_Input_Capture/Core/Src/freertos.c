/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "tim.h"
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

/* USER CODE END Variables */
/* Definitions for IC */
osThreadId_t ICHandle;
const osThreadAttr_t IC_attributes = {
  .name = "IC",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void IC_Entry(void *argument);

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
  /* creation of IC */
  ICHandle = osThreadNew(IC_Entry, NULL, &IC_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_IC_Entry */
/**
  * @brief  Function implementing the IC thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_IC_Entry */
void IC_Entry(void *argument)
{
  /* USER CODE BEGIN IC_Entry */
  uint16_t IC_Value = 0; // 定义IC计数值变�?

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // 启动定时�?2的PWM输出通道1
  HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_1);

  OLED_Init();
  OLED_Display_On();
  OLED_Clear();
  OLED_ShowString(0,0,"Freq:",16,0);
  OLED_ShowString(0,2,"Real Pulse:",16,0);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 100); // 设置Pulse
  /* Infinite loop */
  for(;;)
  {
    IC_Value = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1); // 读取通道1
    if(IC_Value == 0){
        OLED_ShowNum(60,0,IC_Value,5,16,0);
    } else {
        IC_Value = 1000000 / (IC_Value + 1);
        OLED_ShowNum(60,0,IC_Value,5,16,0);
    }
    uint16_t real_pulse = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1); // 读取PWM的Pulse
    OLED_ShowNum(80,2,real_pulse,5,16,0);
    osDelay(1);
  }
  /* USER CODE END IC_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

