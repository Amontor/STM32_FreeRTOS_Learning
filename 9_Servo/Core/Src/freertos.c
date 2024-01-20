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
#include "Key.h"
#include "oled.h"
#include "tim.h"
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
uint8_t Key_Num;
uint16_t Servo_Angle;
uint16_t Servo_Pulse_Set;
uint16_t Servo_Pulse_Get;
/* USER CODE END Variables */
/* Definitions for Servo_task */
osThreadId_t Servo_taskHandle;
const osThreadAttr_t Servo_task_attributes = {
  .name = "Servo_task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Servo_Entry(void *argument);

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
  /* creation of Servo_task */
  Servo_taskHandle = osThreadNew(Servo_Entry, NULL, &Servo_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Servo_Entry */
/**
  * @brief  Function implementing the Servo_task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Servo_Entry */
void Servo_Entry(void *argument)
{
  /* USER CODE BEGIN Servo_Entry */
  OLED_Init();
  OLED_Display_On();
  OLED_Clear();
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  OLED_ShowString(0,0,"Servo Angle:",12,0);
  OLED_ShowString(0,2,"Pulse Width:",12,0);
  OLED_ShowString(0,4,"Real Pulse",12,0);
  /* Infinite loop */
  for(;;)
  {
    Key_Num = Get_GetNum();
    if(Key_Num == KEY1)
    {
        Servo_Angle += 30;
        if(Servo_Angle > 180)
        {
            Servo_Angle = 0;
        }
        Servo_Pulse_Set = Servo_Angle * 2000 / 180 + 500;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Servo_Pulse_Set);
    }
    Servo_Pulse_Get = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1);
    OLED_ShowNum(80,0,Servo_Angle,5,12,0);
    OLED_ShowNum(80,2,Servo_Pulse_Set,5,12,0);
    OLED_ShowNum(80,4,Servo_Pulse_Get,5,12,0);
    osDelay(1);
  }
  /* USER CODE END Servo_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

