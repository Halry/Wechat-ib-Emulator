#include "HAL_Config.h"
ADC_HandleTypeDef hadc1;
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi2;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
UART_HandleTypeDef huart1;
//DMA
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_spi2_tx;
extern const char *System_Version;
extern const char *HW_Ver;
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
bool Sys_In_HS_CLK=false;
void System_Clock_Ctrl(uint8_t Clock_Sel)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
  HAL_RCC_DeInit();
  if(Clock_Sel==LS_CLK)
    {
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
      {
      _Error_Handler(__FILE__, __LINE__);
      }
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;//AHB=24Mhz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;//APB1=12Mhz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;//APB2=3Mhz
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
      {
      _Error_Handler(__FILE__, __LINE__);
      }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;//ADC=1.5Mhz
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
      {
      _Error_Handler(__FILE__, __LINE__);
      }
    }
  else//HS CLK
    {
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
      {
      _Error_Handler(__FILE__, __LINE__);
      }
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;//AHB=72Mhz
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;//APB1=18Mhz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;//APB2=72Mhz
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
      {
      _Error_Handler(__FILE__, __LINE__);
      }
    PeriphClkInit.PeriphClockSelection =RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
      {
      _Error_Handler(__FILE__, __LINE__);
      }
    }
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY)==false)
    {
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
      {
      _Error_Handler(__FILE__, __LINE__);
      }
    }
  HAL_RCC_GetHCLKFreq();
  HAL_RCC_GetPCLK1Freq();
  HAL_RCC_GetPCLK1Freq();
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  __HAL_RCC_DMA1_CLK_ENABLE();
}
/* ADC1 function */
void ADC1_Init(void)
{
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if(HAL_ADC_Init(&hadc1) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
  HAL_NVIC_SetPriority(ADC1_IRQn,1,0);
  HAL_NVIC_EnableIRQ(ADC1_IRQn);
}
void ADC1_DeInit(void)
{
  HAL_NVIC_DisableIRQ(ADC1_IRQn);
  HAL_ADC_DeInit(&hadc1);
}
/* RTC init function */
void RTC_Init(void)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef DateToUpdate;
  RTC_TamperTypeDef sTamper;
  /**Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if(HAL_RTC_Init(&hrtc) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
  /**Enable the RTC Tamper
  */
  sTamper.Tamper = RTC_TAMPER_1;
  sTamper.Trigger = RTC_TAMPERTRIGGER_HIGHLEVEL;
  if(HAL_RTCEx_SetTamper_IT(&hrtc, &sTamper) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
}
/* TIM2 init function */
void TIM2_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if(HAL_TIM_PWM_Init(&htim2) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if(HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if(HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
  HAL_TIM_MspPostInit(&htim2);
}

/* TIM3 init function */
void TIM3_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if(HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if(HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if(HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
  HAL_TIM_MspPostInit(&htim3);
}
void TIM4_Start(void)
{
  __HAL_RCC_TIM4_CLK_ENABLE();
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 23999;
  htim4.Init.Period = 10;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&htim4) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
  __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE);
  __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
  HAL_NVIC_SetPriority(TIM4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
  if(HAL_TIM_Base_Start_IT(&htim4) != HAL_OK)
    {
    _Error_Handler(__FILE__, __LINE__);
    }
}
/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
        * Free pins are configured automatically as Analog (this feature is enabled through
        * the Code Generation settings)
     PA8   ------> RCC_MCO
*/
void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  //PA0:PW_Button,PA1:Up,PA2:Down,PA3:X
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*Configure GPIO pins : PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*Configure GPIO pins : PA5 PA6 PA7 */
  //PA4:ADC,PA5:BT_RST,PA6:BT_PWRC,PA7:BT_CONCTR,PA15:BT_STAT
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*Configure GPIO pins : PB0 PB1 PB14 PB6
                           PB7 PB8 PB9 */
  //PB0:BT_Ctr PB1:LCD_Ctr PB6:TP_CE PB7:R_LED PB8:USB_Em PB9:LCD_DC PB14:LCD_RES
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_14
                        |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			  /*Configure GPIO pins : PB2 PB4 */
  //PB2:TP_CHRG PB4:TP_STD
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /*Configure GPIO pin : PA8 */
  //MCO Output
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_15, GPIO_PIN_RESET);
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_14
                    |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
  HAL_NVIC_SetPriority(EXTI2_IRQn, 15, 0);//TP_CHRG Detect
  HAL_NVIC_SetPriority(EXTI4_IRQn, 15, 0);//TP_STDBY Detect
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
    {
    }
  /* USER CODE END Error_Handler_Debug */
}
//Use the function only when the system startup (battery plugged) or reset.
void System_Startup_Init(void)
{
  HAL_DeInit();
  HAL_Init();
  System_Clock_Ctrl(LS_CLK);
  GPIO_Init();
//RTC_Init();
  OLED_Init();
  UI_Show_Please_Wait();
  OLED_ShowString(0,3,(uint8_t*)"FW:",true);
  OLED_ShowString(24,3,(uint8_t*)System_Version,true);
  OLED_ShowString(0,5,(uint8_t*)"HW:",true);
  OLED_ShowString(24,5,(uint8_t*)HW_Ver,true);
  TIM4_Start();
  BT_Init();
}
