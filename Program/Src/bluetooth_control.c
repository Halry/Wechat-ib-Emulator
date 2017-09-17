#include "bluetooth_control.h"
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;
uint8_t const BT_Classroom_Minor[3][4]={{"559D"},{"55A0"},{"55D7"}};
uint8_t *BT_UART_Transmit_Data=NULL;
char *BT_Last_Minor=NULL;
uint16_t BT_Left_ADV_Count=0;
void BT_Init()
{
	uint8_t null_minor[4];
	memset(null_minor,'\0',4);
	BT_Read_Setup_BKP();
	//read out bluetooth inited and last time minor from bkp
	if(memcmp(null_minor,BT_Last_Minor,4)==0)
	{
		BT_Power_Control(true);
		USART1_Init();
		BT_UART_Handler((uint8_t *)"AT+HOSTEN3",10);
		BT_UART_Handler((uint8_t *)"AT+POWR3",8);
		BT_UART_Handler((uint8_t *)"AT+STRUUIDFDA50693A4E24FB1AFCFC6EB07647825",42);//Set the uuid to wechat
		BT_UART_Handler((uint8_t *)"AT+MAJOR0000",12);//Reset the minor to 0x2773
		BT_UART_Handler((uint8_t *)"AT+MINOR0000",12);//Reset the minor to 0x0000
		BT_UART_Handler((uint8_t *)"AT+ADVIN0",9);//Set the boardcast time to 100ms(faster advertising)
		BT_UART_Handler((uint8_t *)"AT+ISCEN2",9);//Reset the minor to 0x0000
		BT_UART_Handler((uint8_t *)"AT+PASS172839",13);//Reset the minor to 0x0000
		BT_UART_Handler((uint8_t *)"AT+NAMEMI-BAND",14);//Rename
		BT_UART_Handler((uint8_t *)"AT+ADVEN0",9);//Disable advertising
		memset(BT_Last_Minor,0xFF,4);
		BT_Power_Control(false);
		BT_Write_Setup_BKP();
		USART1_DeInit();
	}
} 
void BT_UART_Handler(uint8_t *data,uint16_t size)
{
	if(HAL_UART_Transmit(&huart1,data,size,500)!=HAL_OK)
	{
		OLED_Clear();
		OLED_ShowString(0,1,(uint8_t*)"Fatal Err:UART_Time",false);
	OLED_ShowString(0,2,(uint8_t*)"Press X Key to reset",false);
	while(1)
	{
		uint8_t key=Get_Key();
		if(key==Key_X)
		{
			NVIC_SystemReset();
		}
	}
	}	
	HAL_Delay(500);
}
bool Start_beacon(uint8_t *minor)
{
	UI_Show_Please_Wait();
	BT_Power_Control(true);
	USART1_Init();
	if((BT_UART_Transmit_Data=malloc(12))==NULL)
		{
			Error_Handler();
		}
		BT_UART_Handler((uint8_t*)"AT+NAMEMI-BAND",14);//Rename
		BT_UART_Handler((uint8_t*)"AT+MAJOR2773",12);
		memcpy(BT_UART_Transmit_Data,"AT+MINOR",8);
		memcpy(BT_UART_Transmit_Data+8,minor,4);
		BT_UART_Handler(BT_UART_Transmit_Data,12);
		BT_UART_Handler((uint8_t *)"AT+ADVEN1",9);//Enable Advertising
		BT_Left_ADV_Count--;
		free(BT_UART_Transmit_Data);
		BT_Last_Minor=(char *)minor;
		BT_Write_Setup_BKP();
		USART1_DeInit();
		return true;
}
bool Stop_beacon(void)
{
	UI_Show_Please_Wait();
		USART1_Init();
		BT_UART_Handler((uint8_t *)"AT+ADVEN0",9);//Disable Advertising
		BT_UART_Handler((uint8_t *)"AT+MINOR0000",12);//Reset the minor to 0x0000
		USART1_DeInit();
		BT_Power_Control(false);
	return true;
}
void BT_Power_Control(bool power)
{
	if(power==true)
		{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);//Set Connection Ctr pin to high
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);//Wakeup Module
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
		}
}
void BT_Read_Setup_BKP(void)
{
	HAL_PWR_EnableBkUpAccess();
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
		BT_Left_ADV_Count=HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR8);
		#ifdef SYS_DBG
		BT_Left_ADV_Count=999;
		#endif
		HAL_PWR_DisableBkUpAccess();
}
void BT_Write_Setup_BKP(void)
{
	HAL_PWR_EnableBkUpAccess();
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR9,((*(BT_Last_Minor+2)<<8)|*(BT_Last_Minor+3)));
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR10,((*(BT_Last_Minor)<<8)|*(BT_Last_Minor+1)));
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR8,BT_Left_ADV_Count);
	HAL_PWR_DisableBkUpAccess();
}
void USART1_Init(void)
{
	System_Clock_Ctrl(HS_CLK);
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling=UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

void USART1_DeInit(void)
{
	System_Clock_Ctrl(LS_CLK);
	HAL_UART_DeInit(&huart1);

}