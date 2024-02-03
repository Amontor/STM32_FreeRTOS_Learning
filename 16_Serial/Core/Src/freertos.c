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
#include "usart.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
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
#define rxBufferLen 40 // 接收数据的最大长度
#define __HAL_DMA_SET_COUNTER(__HANDLE__, __COUNTER__) ((__HANDLE__)->Instance->CNDTR = (uint16_t)(__COUNTER__))
char rxBuffer[rxBufferLen]; // 用于存放接收到的数据
/* USER CODE END Variables */
/* Definitions for Serial */
osThreadId_t SerialHandle;
const osThreadAttr_t Serial_attributes = {
  .name = "Serial",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Serial_Entry(void *argument);

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
  /* creation of Serial */
  SerialHandle = osThreadNew(Serial_Entry, NULL, &Serial_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Serial_Entry */
/**
  * @brief  Function implementing the Serial thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Serial_Entry */
void Serial_Entry(void *argument)
{
  /* USER CODE BEGIN Serial_Entry */
  __HAL_UART_CLEAR_IDLEFLAG(&huart1); // 清除空闲中断标志位
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); // 开启UART的空闲中断
  HAL_UART_Receive_DMA(&huart1, (uint8_t*)rxBuffer, rxBufferLen ); // 启动DMA接收
  uint8_t TXbuff[] = "\r\n**** UART-Hyperterminal communication based on DMA ***\r\n ";
  /* Infinite loop */
  for(;;)
  {
	//HAL_UART_Transmit_DMA(&huart1, (uint8_t *)TXbuff, sizeof(TXbuff)-1);
    osDelay(1000);
  }
  /* USER CODE END Serial_Entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    // if(huart->Instance == USART1)
    // {
    //     HAL_UART_Receive_DMA(&huart1, &rxData[0], sizeof(rxData));
    // }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // if(huart->Instance == USART1)
    // {
    //     HAL_UART_Transmit_DMA(&huart1, &rxData[0], sizeof(rxData));
    //     HAL_UART_Receive_DMA(&huart1, &rxData[0], sizeof(rxData));
    // }
}

void UART_DMAIdleCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
        // 停止DMA接收
        __HAL_DMA_DISABLE(huart->hdmarx);
        // 计算接收到的数据长度
        uint8_t dataLen = rxBufferLen - __HAL_DMA_GET_COUNTER(huart->hdmarx) + '0'; 
        // 发送数据长度，使用轮询方法
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)rxBuffer, rxBufferLen);

        // 重启DMA接收
        __HAL_DMA_SET_COUNTER(huart->hdmarx, rxBufferLen); // 重设DMA计数器
        __HAL_DMA_ENABLE(huart->hdmarx); // 使能DMA接收
    }
}

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
/* USER CODE END Application */

