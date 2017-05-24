#ifndef __HAL_Config_H
#define __HAL_Config_H
#include "main.h"
#include "stm32f1xx_hal.h"
void SystemClock_Config(void);
static void GPIO_Init(void);
static void ADC1_Init(void);
static void CRC_Init(void);
static void RTC_Init(void);
static void SPI2_Init(void);
static void TIM2_Init(void);
static void TIM3_Init(void);
static void USART1_UART_Init(void);
static void USART3_UART_Init(void);
static void USB_PCD_Init(void);
static void WWDG_Init(void);
static void IWDG_Init(void);
void System_Startup_Init(void);
#endif
