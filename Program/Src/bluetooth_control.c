#include "bluetooth_control.h"
extern UART_InitTypeDef huart1;
bool bluetooth_inited;
const uint8_t BT_Classroom_Minor[3][4]={{"5581"},{"55A6"},{"55D7"}};
uint8_t *BT_UART_Receive_Data=NULL;
void BT_Init()
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);//Set Connection Ctr pin to high
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);//Wakeup Module
	//read out bluetooth inited from bkp
	if(bluetooth_inited==false)
	{
		BT_UART_Receive_Data=malloc(5);
		
	}
	
}
void Start_beacon(uint8_t *minor)
{
	
}