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
## 8.PWM_LED
使用TIM1 CH1(PA8)实现PWM呼吸灯功能，预分频720-1，重装载值100-1，通过不断设置set_compare来修改占空比，duty=compare/reload
## 9.Servo
通过按键(PC5)控制PWM的占空比，以达到控制舵机旋转角度的效果。由于没有舵机，所以通过OLED显示舵机旋转角度，并控制LED步进亮度(PA8,TIM1_CH1)
## 10.Input_Capture
使用TIM3_CH1输入捕获功能，PA6引脚输入，TIM2_CH1作为PWM输出，PA0引脚输出，捕获上升沿，计算出PWM信号频率
## 11.IC_Duty
使用TIM3_CH1和TIM3_CH2输入捕获功能，分别采集信号的上升沿和下降沿，同时在定时器从模式中对计数寄存器进行自动清空，达到自动采集的效果。
由TIM2_CH1(PA0)产生可变占空比的信号，TIM3_CH1和TIM3_CH2公用PA6引脚进行采样，计算出PWM信号的频率和占空比，并显示在OLED上
## 12.Encoder_Intf
使用TIM3的编码器模式对编码器旋转信号进行计数，通过TIM2产生1秒钟定时，在中断中采集编码器信号，即可达到计算编码器速度功能
## 13.Signal_adc
adc1通道0单次转换模式，将光敏传感器的模拟输出引脚接到ADC1 CH0通道(PA0),在主函数中轮询该次转换是否完成，如果完成则将转换结果显示在OLED上
## 14.multi_adc
ADC1多通道模式，ADC1的CH0 CH2接光敏传感器模拟输出口，两个通道均为单次非扫描模式，每次读取转换之前先配置通道属性，不需要设置regular rank
## 15.adc_dma
ADC1多通道DMA扫描模式，硬件连接同上。~~注：实际使用中无法开启一次DMA后让其自动循环，程序会跑飞（待解决），在中断函数中先关闭DMA，主函数中重新开启~~
2024-2-1已解决：某些型号使用ADC+DMA时需要在 MX_DMA_Init 函数中屏蔽中断，否则程序会卡在 DMA 中断中。屏蔽后即可单次使能一直转换。
## 16.uart_dma
使用DMA实现UART1发送和接收功能，使用DMA时必须开启串口中断，并且由于设置DMA为单次模式，每次在回调函数中都要重新开启对应的DMA转运。经过测试，DMA发送功能正常，
但接收时经常出现漏数据或延迟收包的问题，所以转而使用空闲中断+DMA来接收不定长数据。参考：https://blog.csdn.net/qq_39344192/article/details/131470735