#include "bluetooth_control.h"
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;
uint8_t const BT_Classroom_Minor[3][4]={{"559D"},{"55A0"},{"55D7"}};
uint8_t *BT_UART_Transmit_Data=NULL;
uint8_t *BT_UART_Receive_Data=NULL;
char *BT_Last_Minor=NULL;
uint16_t BT_Left_ADV_Count=0;
uint32_t BT_UART_RX_Tick=0;
uint8_t BT_Last_Receive=0;
uint8_t BT_Receive_Addr=0;
bool BT_UART_DMA_Busy=false;
bool BT_UART_RX_Busy=false;
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
		BT_UART_Handler("AT+HOSTEN3",10);
		memset(BT_UART_Receive_Data,'\0',4);
		HAL_UART_Transmit(&huart1,"AT+POWR3",8,0xFF);
		HAL_Delay(200);
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
		HAL_Delay(200);
		if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+MAJOR0000",12,0xFF);//Reset the minor to 0x2773
		HAL_Delay(200);
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
		HAL_Delay(200);
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
		HAL_Delay(200);
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
		HAL_Delay(200);
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
		HAL_Delay(200);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+NAMEMI-BAND",14,0xFF);//Rename
		HAL_Delay(200);
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
		HAL_Delay(200);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_Last_Minor,0xFF,4);
		BT_Power_Control(false);
		BT_Write_Setup_BKP();
		free(BT_UART_Receive_Data);
		USART1_DeInit();
	}
} 
void BT_UART_Handler(uint8_t *data,uint16_t size)
{
	uint32_t ticks=HAL_GetTick();
	while(BT_UART_DMA_Busy==true)
	{
		if(HAL_GetTick()-ticks>=1000)
		{
			BT_UART_DMA_Busy=false;
			//Reset UART and BT Module
		}
	}
	while(HAL_GetTick()-BT_UART_RX_Tick>=1000)
	{
		BT_UART_RX_Busy=false;
		BT_UART_RX_Tick=0;
		BT_Receive_Addr=0;
	}
	if(BT_UART_Receive_Data!=NULL)
	{
		free(BT_UART_Receive_Data);
		BT_Receive_Addr=0;
	}
	BT_UART_Receive_Data=malloc(5);
	if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,1)==HAL_ERROR)
		{
			Error_Handler();
		}
		BT_UART_RX_Tick=HAL_GetTick();
	BT_UART_DMA_Busy=true;
	if(HAL_UART_Transmit_DMA(&huart1,data,size)==HAL_ERROR)
	{
		Error_Handler();
	}	
}
void Start_beacon(const char *minor)
{
	BT_Power_Control(true);
		USART1_Init();
		if((BT_UART_Receive_Data=malloc(4))==NULL)
		{
			Error_Handler();
		}
		if((BT_UART_Transmit_Data=malloc(12))==NULL)
		{
			Error_Handler();
		}
		
		if((memcmp(minor,BT_Last_Minor,4))!=0)
		{
				if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,"AT+NAMEMI-BAND",14,0xFF);//Rename
		HAL_Delay(200);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
				if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		} 
			HAL_UART_Transmit(&huart1,"AT+MAJOR2773",12,0xFF);
			HAL_Delay(300);
		if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		memset(BT_UART_Receive_Data,'\0',4);
		memcpy(BT_UART_Transmit_Data,"AT+MINOR",8);
		memcpy(BT_UART_Transmit_Data+8,minor,4);
			if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
		HAL_UART_Transmit(&huart1,BT_UART_Transmit_Data,12,0xFF);//Reset the minor to 0x0000
		HAL_Delay(300);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		 __HAL_UART_FLUSH_DRREGISTER(&huart1); 
		memset(BT_UART_Receive_Data,'\0',4);
	
		if(HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,4)==HAL_ERROR)
		{
			Error_Handler();
		}
	}
		HAL_UART_Transmit(&huart1,"AT+ADVEN1",9,0xFF);//Enable Advertising
		HAL_Delay(300);
			if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		BT_Left_ADV_Count--;
		free(BT_UART_Receive_Data);
		free(BT_UART_Transmit_Data);
BT_Last_Minor=(char *)minor;
		BT_Write_Setup_BKP();
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
		HAL_Delay(300);
		if(strcmp((const char*)BT_UART_Receive_Data,"+ERR")==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		free(BT_UART_Receive_Data);
		free(BT_UART_Transmit_Data);
		USART1_DeInit();
		BT_Power_Control(false);
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
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	BT_UART_DMA_Busy=false;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(BT_UART_Receive_Data==NULL)
	{
		//go to error
	}
	else
	{
		*(BT_UART_Receive_Data+BT_Receive_Addr)=BT_Last_Receive;
		BT_Receive_Addr++;	
		if(BT_Receive_Addr>=4)
		{
			if(memcmp(BT_UART_Receive_Data,"+ERR",4)==0)
		{
					HAL_UART_DeInit(&huart1);//Display error
		}
		}
		else if(memcmp(BT_UART_Receive_Data,"+OK",3)==0)
		{
			BT_UART_RX_Busy=false;
			free(BT_UART_Receive_Data);
		}
		else
		{
			if(HAL_UART_Receive_IT(&huart1,&BT_Last_Receive,1)==HAL_ERROR)
		{
			Error_Handler();
		}
		}
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
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	
	if(HAL_UART_GetError(&huart1)!=HAL_UART_ERROR_ORE)
	{
		OLED_Clear();
			OLED_ShowString(0,1,"Fatal Err:UART_HW",false);
	OLED_ShowString(0,2,"Press X Key to reset",false);
	while(1)
	{
		uint8_t key=Get_Key();
		if(key==Key_X)
		{
			NVIC_SystemReset();
		}
	}
	}
}

void USART1_Init(void)
{
	System_Clock_Ctrl(HS_CLK);
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
	//Enable DMA Interruot
	  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

void USART1_DeInit(void)
{
	System_Clock_Ctrl(LS_CLK);
	HAL_NVIC_DisableIRQ(USART1_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Channel4_IRQn);
	HAL_UART_DeInit(&huart1);

}