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
uint16_t ADC_Value1;
uint16_t ADC_Value2;
float Voltage1;
float Voltage2;
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
uint16_t ADC_Read(uint32_t Channel);
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
  OLED_ShowString(0,0,"ADValue1:",16,0);
  OLED_ShowString(0,2,"Voltage1:",16,0);
  OLED_ShowString(0,4,"ADValue2:",16,0);
  OLED_ShowString(0,6,"Voltage2:",16,0);
  HAL_ADC_Start(&hadc1);
  /* Infinite loop */
  for(;;)
  {
    ADC_Value1 = ADC_Read(ADC_CHANNEL_0);
    Voltage1 = ADC_Value1*3.3/4095;
    ADC_Value2 = ADC_Read(ADC_CHANNEL_2);
    Voltage2 = ADC_Value2*3.3/4095;
    OLED_ShowNum(70,0,ADC_Value1,4,16,0);
    OLED_Showdecimal(70,2,Voltage1,1,2,16,0);
    OLED_ShowNum(70,4,ADC_Value2,4,16,0);
    OLED_Showdecimal(70,6,Voltage2,1,2,16,0);
    osDelay(100);
  }
  /* USER CODE END ADC_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
uint16_t ADC_Read(uint32_t Channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = Channel;                                         /* 通道 */
	sConfig.Rank = ADC_REGULAR_RANK_1;                              
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;                  /* 采样时间 */
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)             
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	return (uint16_t)HAL_ADC_GetValue(&hadc1);
}
/* USER CODE END Application */

