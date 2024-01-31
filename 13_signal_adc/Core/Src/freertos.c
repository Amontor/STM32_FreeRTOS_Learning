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
#include "oled.h"
#include "adc.h"
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
uint16_t ADValue;
float Voltage;
/* USER CODE END Variables */
/* Definitions for ADC */
osThreadId_t ADCHandle;
const osThreadAttr_t ADC_attributes = {
  .name = "ADC",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ADC_Entry(void *argument);

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
  /* creation of ADC */
  ADCHandle = osThreadNew(ADC_Entry, NULL, &ADC_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ADC_Entry */
/**
  * @brief  Function implementing the ADC thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ADC_Entry */
void ADC_Entry(void *argument)
{
  /* USER CODE BEGIN ADC_Entry */
  OLED_Init();
  OLED_Display_On();
  OLED_Clear();
  OLED_ShowString(0,0,"ADValue:",16,0);
  OLED_ShowString(0,2,"Voltage:",16,0);
  HAL_ADC_Start(&hadc1);
  /* Infinite loop */
  for(;;)
  {
    if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
    {
      ADValue = HAL_ADC_GetValue(&hadc1);
      HAL_ADC_Start(&hadc1);
    }
    Voltage = ADValue * 3.3 / 4096;
    OLED_ShowNum(64,0,ADValue,6,16,0);
    OLED_Showdecimal(64,2,Voltage,2,2,16,0);
    osDelay(100);
  }
  /* USER CODE END ADC_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

