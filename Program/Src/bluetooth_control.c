#include "bluetooth_control.h"
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;
bool bluetooth_inited=false;
uint8_t const BT_Classroom_Minor[3][4]={{"5581"},{"55A6"},{"55D7"}};
uint8_t *BT_UART_Receive_Data=NULL;
uint8_t *BT_UART_Transmit_Data=NULL;
uint8_t *BT_Last_Minor=NULL;
void BT_Init()
{
	BT_Read_Setup_BKP();
	//read out bluetooth inited and last time minor from bkp
	if(bluetooth_inited==false)
	{
		BT_Power_Control(true);
		USART1_Init();
		if((BT_UART_Receive_Data=malloc(4))==NULL)
		{
			Error_Handler();
		}
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+HOSTEN3",10,0xFF);
		HAL_Delay(100);
		if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\1',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		memset(BT_UART_Receive_Data,'\0',4);
		HAL_UART_Transmit(&huart1,"AT+POWR3",8,0xFF);
		HAL_Delay(100);
		if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+STRUUIDFDA50693A4E24FB1AFCFC6EB07647825",42,0xFF);//Set the uuid to wechat
		HAL_Delay(100);
		if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+MAJOR2773",12,0xFF);//Reset the minor to 0x2773
		HAL_Delay(100);
		if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+MINOR0000",12,0xFF);//Reset the minor to 0x0000
		HAL_Delay(100);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+ADVIN0",9,0xFF);//Set the boardcast time to 100ms(faster advertising)
		HAL_Delay(100);
		if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+ISCEN2",9,0xFF);//Reset the minor to 0x0000
		HAL_Delay(100);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+PASS172839",13,0xFF);//Reset the minor to 0x0000
		HAL_Delay(100);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+ADVEN0",9,0xFF);//Disable advertising
		HAL_Delay(100);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		BT_Power_Control(false);
		bluetooth_inited=true;
		BT_Write_Setup_BKP(false);
		free(BT_UART_Receive_Data);
		USART1_DeInit();
	}
} 
void Start_beacon(const uint8_t *minor)
{
		USART1_Init();
		if((BT_UART_Receive_Data=malloc(4))==NULL)
		{
			Error_Handler();
		}
		if((BT_UART_Transmit_Data=malloc(12))==NULL)
		{
			Error_Handler();
		}
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		memcpy(BT_UART_Transmit_Data,"AT+MINOR",8);
		memcpy(BT_UART_Transmit_Data+8,minor,4);
		HAL_UART_Transmit(&huart1,BT_UART_Transmit_Data,12,0xFF);//Reset the minor to 0x0000
		HAL_Delay(100);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+ADVEN1",9,0xFF);//Enable Advertising
		HAL_Delay(100);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		free(BT_UART_Receive_Data);
		free(BT_UART_Transmit_Data);
		BT_Last_Minor=minor;
		BT_Write_Setup_BKP(true);
		USART1_DeInit();
}
void Stop_beacon(void)
{
	USART1_Init();
		if((BT_UART_Receive_Data=malloc(4))==NULL)
		{
			Error_Handler();
		}
		if((BT_UART_Transmit_Data=malloc(12))==NULL)
		{
			Error_Handler();
		}
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+ADVEN0",9,0xFF);//Disable Advertising
		HAL_Delay(100);
		if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		free(BT_UART_Receive_Data);
		free(BT_UART_Transmit_Data);
		USART1_DeInit();
}
void BT_Power_Control(bool power)
{
	if(power==true)
		{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);//Set Connection Ctr pin to high
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);//Wakeup Module
	HAL_Delay(500);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);//Set Connection Ctr pin to high
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);//Wakeup Module
		}
}
void BT_Read_Setup_BKP(void)
{
	if((BT_Last_Minor=malloc(4))==NULL)
		{
			Error_Handler();
		}
		uint32_t BT_Minor_BKP;
		BT_Minor_BKP=HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR9);//Store Low
		BT_Minor_BKP|=((HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR10)))<<16;//Store High
		*BT_Last_Minor=(BT_Minor_BKP>>24);
		*(BT_Last_Minor+1)=(BT_Minor_BKP>>16);
		*(BT_Last_Minor+2)=(BT_Minor_BKP>>8);
		*(BT_Last_Minor+3)=BT_Minor_BKP;
		bluetooth_inited=((HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR8))>>16);
}
void BT_Write_Setup_BKP(bool Is_Minor)
{
	if(Is_Minor==true)
	{
		if(BT_Last_Minor==NULL)
	{
		Error_Handler();
	}
		HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR9,((*(BT_Last_Minor+2)<<8)|*(BT_Last_Minor+3)));
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR10,((*(BT_Last_Minor)<<8)|*(BT_Last_Minor+1)));
	}
	else
	{
		HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR8,((bluetooth_inited)<<15));
	}
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	OLED_Clear();
	OLED_ShowString(0,0,"Fatal Err:UART_STACK");
}
void USART1_Init(void)
{
	OverClock_to_HSE();
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling=UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	HAL_NVIC_SetPriority(USART1_IRQn,0,0);
HAL_NVIC_EnableIRQ(USART1_IRQn);
}
void USART1_DeInit(void)
{
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	HAL_UART_DeInit(&huart1);
	Downclock_to_HSI();
}