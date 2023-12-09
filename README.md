# STM32 + FreeRTOS 学习记录  
板卡为正点原子mini探索板，主控STM32F103RCT6  
## 1.LED_Blink
使用板载 LED1(PA8)、LED2(PD2)，实现流水灯闪烁任务
## 2.Key_Set_Led
轮询方式扫描按键，根据键位控制LED灯
KEY-1:低电平有效，按下后LED1亮LED2灭
KEY-2:低电平有效，按下后LED2亮LED1灭
KEY-3:高电平有效，按下后LED均翻转