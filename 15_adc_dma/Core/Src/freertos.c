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
uint16_t AD_Value[2];
float Voltage[2];
/* USER CODE END Variables */
/* Definitions for ADC_DMA */
osThreadId_t ADC_DMAHandle;
const osThreadAttr_t ADC_DMA_attributes = {
  .name = "ADC_DMA",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for adc_Mutex */
osMutexId_t adc_MutexHandle;
const osMutexAttr_t adc_Mutex_attributes = {
  .name = "adc_Mutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ADC_DMA_Entry(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of adc_Mutex */
  adc_MutexHandle = osMutexNew(&adc_Mutex_attributes);

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
  /* creation of ADC_DMA */
  ADC_DMAHandle = osThreadNew(ADC_DMA_Entry, NULL, &ADC_DMA_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ADC_DMA_Entry */
/**
  * @brief  Function implementing the ADC_DMA thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ADC_DMA_Entry */
void ADC_DMA_Entry(void *argument)
{
  /* USER CODE BEGIN ADC_DMA_Entry */
  OLED_Init();
  OLED_Display_On();
  OLED_Clear();
  OLED_ShowString(0,0,"ADValue1:",16,0);
  OLED_ShowString(0,2,"Vloatge1:",16,0);
  OLED_ShowString(0,4,"ADValue2:",16,0);
  OLED_ShowString(0,6,"Vloatge2:",16,0);
  
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)AD_Value,sizeof(AD_Value));
  /* Infinite loop */
  for(;;)
  {
    osMutexAcquire(adc_MutexHandle,osWaitForever);
    OLED_ShowNum(70,0,AD_Value[0],4,16,0);
    OLED_Showdecimal(70,2,Voltage[0],1,2,16,0);
    OLED_ShowNum(70,4,AD_Value[1],4,16,0);
    OLED_Showdecimal(70,6,Voltage[1],1,2,16,0);
    osMutexRelease(adc_MutexHandle);
    //转换完后再重新打开，否则会跑飞
    //HAL_ADC_Start_DMA(&hadc1,(uint32_t*)AD_Value,sizeof(AD_Value));
    osDelay(10);
  }
  /* USER CODE END ADC_DMA_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC1)
  {
    //HAL_ADC_Stop_DMA(&hadc1);
    osMutexAcquire(adc_MutexHandle,3);
    Voltage[0] = (float)AD_Value[0]*3.3/4096;
    Voltage[1] = (float)AD_Value[1]*3.3/4096;
    osMutexRelease(adc_MutexHandle);
  }
}
/* USER CODE END Application */

