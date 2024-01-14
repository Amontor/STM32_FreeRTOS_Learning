# STM32 + FreeRTOS 学习记录  
板卡为正点原子mini探索板，主控STM32F103RCT6  
## 1.LED_Blink
使用板载 LED1(PA8)、LED2(PD2)，实现流水灯闪烁任务
## 2.Key_Set_Led
轮询方式扫描按键，根据键位控制LED灯
KEY-1:低电平有效，按下后LED1亮LED2灭
KEY-2:低电平有效，按下后LED2亮LED1灭
KEY-3:高电平有效，按下后LED均翻转
## 3.Linght_Sensor
光敏传感器DO口接PB12，上拉输入模式，轮询方式获取光敏传感器的输出，并更新LED1状态
## 4.Oled_Display
硬件i2c1(SCL-PB6,SDA-PB7)驱动oled 128x64屏幕，并封装字符型、字符串、十进制数、浮点型、图片及汉字显示(需要取模)函数
## 5.Encoder
外部中断实现旋转编码器正反转检测，Encoder_0-->DT引脚-->PB12,Encoder_1-->CLK引脚-->PB13，注册中断处理函数完成对应转向判断
OLED显示添加有符号整数显示
## 6.Timr_interrupt
TIM6定时器中断实现1秒定时，在中断函数中将变量Num++，在OLED上显示。72MHz频率 / (7200 - 1)分频 / (10000 - 1)计数器重载
## 7.Timer_ETR
TIM2定时器实现外部计数功能，PA0引脚每出现一次上升沿，TIM_Count计数器加1，计数到10后产生更新中断，将Num加1