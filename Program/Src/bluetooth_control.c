#include "bluetooth_control.h"
extern UART_HandleTypeDef huart1;
bool bluetooth_inited=false;
const uint8_t BT_Classroom_Minor[3][4]={{"5581"},{"55A6"},{"55D7"}};
uint8_t *BT_UART_Receive_Data=NULL;
void BT_Init()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);//Set Connection Ctr pin to high
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);//Wakeup Module
	//read out bluetooth inited from bkp
	if(bluetooth_inited==false)
	{
		BT_UART_Receive_Data=malloc(5);
			HAL_UART_Receive_IT(&huart1,BT_UART_Receive_Data,5);	
		HAL_UART_Transmit(&huart1,"AT+HOSTEN3",10,0xFF);
		

		HAL_Delay(1000);
		if(strcmp(BT_UART_Receive_Data,"OK")==0)
		{
			OLED_Clear();
		}
	}
	
}
void Start_beacon(uint8_t *minor)
{
	
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	OLED_Clear();
}
	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		OLED_Clear();
	}