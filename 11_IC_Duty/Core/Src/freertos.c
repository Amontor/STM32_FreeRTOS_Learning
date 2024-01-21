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

/* USER CODE END Variables */
/* Definitions for IC_Duty */
osThreadId_t IC_DutyHandle;
const osThreadAttr_t IC_Duty_attributes = {
  .name = "IC_Duty",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void IC_Duty_Entry(void *argument);

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
  /* creation of IC_Duty */
  IC_DutyHandle = osThreadNew(IC_Duty_Entry, NULL, &IC_Duty_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_IC_Duty_Entry */
/**
  * @brief  Function implementing the IC_Duty thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_IC_Duty_Entry */
void IC_Duty_Entry(void *argument)
{
  /* USER CODE BEGIN IC_Duty_Entry */
  uint16_t pulse_value = 0;
  uint16_t duty_value = 0;

  OLED_Init();
  OLED_Display_On();
  OLED_Clear();
  OLED_ShowString(0,0,"Freq:",16,0);
  OLED_ShowString(0,2,"Duty:",16,0);
  OLED_ShowString(0,4,"real_pulse:",16,0);

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_2);

  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 100);
  /* Infinite loop */
  for(;;)
  {
    pulse_value = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
    duty_value  = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2);
    uint16_t freq = 0;
    uint16_t duty = 0;
    if(pulse_value != 0){
        freq = 1000000 / (pulse_value + 1);
        duty  = 100 * (duty_value + 1) / pulse_value;
    }

    OLED_ShowNum(40,0,freq,5,16,0);
    OLED_ShowNum(40,2,duty,5,16,0);

    uint16_t real_pulse = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1);
    OLED_ShowNum(40,4,real_pulse,5,16,0);
    
    osDelay(20);
  }
  /* USER CODE END IC_Duty_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

