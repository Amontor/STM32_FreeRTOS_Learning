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
#include "w25qxx.h"
#include "stdio.h"
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
/* Definitions for W25Qxx */
osThreadId_t W25QxxHandle;
const osThreadAttr_t W25Qxx_attributes = {
  .name = "W25Qxx",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void W25Qxx_Entry(void *argument);

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
  /* creation of W25Qxx */
  W25QxxHandle = osThreadNew(W25Qxx_Entry, NULL, &W25Qxx_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_W25Qxx_Entry */
/**
  * @brief  Function implementing the W25Qxx thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_W25Qxx_Entry */
void W25Qxx_Entry(void *argument)
{
  /* USER CODE BEGIN W25Qxx_Entry */
  uint8_t id[3];
  BSP_W25Qx_Read_ID(id);
  printf("\r\nW25Qxx ID: %x %x %X\r\n", id[0], id[1], id[2]);

  uint8_t data[256];
  for (int i = 0; i < 256; i++)
  {
    data[i] = i;
  }
  BSP_W25Qx_Erase_Block(0x0A);
  printf("\r\nErase chip success.\r\n");
  BSP_W25Qx_Write(data, 0x0A, 256);

  uint8_t read_data[266];
  BSP_W25Qx_Read(read_data, 0x00, 266);
  for (int i = 0; i < 266; i++)
  {
    printf("%x ", read_data[i]);
  }
  /* Infinite loop */
  for(;;)
  {
    osDelay(50);
  }
  /* USER CODE END W25Qxx_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

