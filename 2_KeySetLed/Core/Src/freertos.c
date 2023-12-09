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
#include "LED.h"
#include "Key.h"
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
/* Definitions for KEY_Scan */
osThreadId_t KEY_ScanHandle;
const osThreadAttr_t KEY_Scan_attributes = {
  .name = "KEY_Scan",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Key_Entry(void *argument);

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
  /* creation of KEY_Scan */
  KEY_ScanHandle = osThreadNew(Key_Entry, NULL, &KEY_Scan_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Key_Entry */
/**
  * @brief  Function implementing the KEY_Scan thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Key_Entry */
void Key_Entry(void *argument)
{
  /* USER CODE BEGIN Key_Entry */
  uint8_t key_num;
  uint8_t key3_flag = 0;
  /* Infinite loop */
  for(;;)
  {
    key_num = Get_GetNum();
    switch (key_num)
    {
    case KEY1:
        SET_LED1(LED_ON);
        SET_LED2(LED_OFF);
        key3_flag = 0;
        break;
    case KEY2:
        SET_LED1(LED_OFF);
        SET_LED2(LED_ON);
        key3_flag = 0;
        break;
    case KEY3:
        if(key3_flag == 0){
            SET_LED1(LED_ON);
            SET_LED2(LED_ON);
            key3_flag = 1;
        } else {
            Toggle_LED();
        }
        break;
    default:
        break;
    }
  }
  /* USER CODE END Key_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

