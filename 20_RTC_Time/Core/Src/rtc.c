/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    rtc.c
 * @brief   This file provides code for the configuration
 *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include "time.h"

/* 年 月 日 时 分 秒 星期 0-Sunday*/
uint16_t date_info[] = {2024, 3, 10, 15, 0, 0, 0};

void MyRTC_SetTime(uint16_t *time_info)
{
    time_t time_stamp;
    struct tm time_date;

    // 初始化结构体
    time_date.tm_year = time_info[0] - 1900;
    time_date.tm_mon = time_info[1] - 1;
    time_date.tm_mday = time_info[2];
    time_date.tm_hour = time_info[3];
    time_date.tm_min = time_info[4];
    time_date.tm_sec = time_info[5];
    // 转换成时间戳
    time_stamp = mktime(&time_date);

    // 设置RTC时钟, 实测注释掉也没问题
    // __HAL_RCC_PWR_CLK_ENABLE();
    // __HAL_RCC_BKP_CLK_ENABLE();
    // HAL_PWR_EnableBkUpAccess();
    /* Disable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
    /* Set RTC COUNTER MSB word */
    WRITE_REG(hrtc.Instance->CNTH, (time_stamp >> 16U));
    /* Set RTC COUNTER LSB word */
    WRITE_REG(hrtc.Instance->CNTL, (time_stamp & RTC_CNTL_RTC_CNT));
    /* Enable the write protection for RTC registers */
    __HAL_RTC_WRITEPROTECTION_ENABLE(&hrtc);
    /* Wait for synchro */
    while ((hrtc.Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET);

    // 更新一次数据
    MyRTC_GetTime();
}

void MyRTC_GetTime(void)
{
    time_t time_stamp;
    struct tm time_date;

    // 获取RTC时间戳
    time_stamp = RTC->CNTH; // 获取高16位
    time_stamp <<= 16;
    time_stamp += RTC->CNTL; // 获取低16位
    // 解析成结构体信息，存入全局变量
    time_date = *localtime(&time_stamp);
    date_info[0] = time_date.tm_year + 1900;
    date_info[1] = time_date.tm_mon + 1;
    date_info[2] = time_date.tm_mday;
    date_info[3] = time_date.tm_hour;
    date_info[4] = time_date.tm_min;
    date_info[5] = time_date.tm_sec;
    date_info[6] = time_date.tm_wday;
}

void rtc_init_user(void)
{
    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x5050) // 是否第一次配置
    {
        MyRTC_SetTime(date_info);                  // 设置日期和时间
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x5050); // 标记已经初始化过了
    }
    MyRTC_GetTime(); // 更新时间
}
/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

    /* USER CODE BEGIN RTC_Init 0 */

    /* USER CODE END RTC_Init 0 */

    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef DateToUpdate = {0};

    /* USER CODE BEGIN RTC_Init 1 */

    /* USER CODE END RTC_Init 1 */

    /** Initialize RTC Only
     */
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
    if (HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler();
    }

    /* USER CODE BEGIN Check_RTC_BKUP */
#if 0
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 1;
  DateToUpdate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
#endif
    /* USER CODE END RTC_Init 2 */
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle)
{

    if (rtcHandle->Instance == RTC)
    {
        /* USER CODE BEGIN RTC_MspInit 0 */

        /* USER CODE END RTC_MspInit 0 */
        HAL_PWR_EnableBkUpAccess();
        /* Enable BKP CLK enable for backup registers */
        __HAL_RCC_BKP_CLK_ENABLE();
        /* RTC clock enable */
        __HAL_RCC_RTC_ENABLE();
        /* USER CODE BEGIN RTC_MspInit 1 */

        /* USER CODE END RTC_MspInit 1 */
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle)
{

    if (rtcHandle->Instance == RTC)
    {
        /* USER CODE BEGIN RTC_MspDeInit 0 */

        /* USER CODE END RTC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_RTC_DISABLE();
        /* USER CODE BEGIN RTC_MspDeInit 1 */

        /* USER CODE END RTC_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
