#ifndef __HAL_Config_H
#define __HAL_Config_H
#include "main.h"
#include "stm32f1xx_hal.h"
#include "OLED.h"
void SystemClock_Config(void);
void GPIO_Init(void);
void ADC1_Init(void);
void CRC_Init(void);
void RTC_Init(void);
void SPI2_Init(void);
void TIM2_Init(void);
void TIM3_Init(void);
void USART1_UART_Init(void);
void USART3_UART_Init(void);
void USB_PCD_Init(void);
void WWDG_Init(void);
void IWDG_Init(void);
void System_Startup_Init(void);
void _Error_Handler(char * file, int line);
#endif
