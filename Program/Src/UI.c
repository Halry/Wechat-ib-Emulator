#include "UI.h"
#include "UI_Array.h"
//Status bar Icon
const uint8_t Classroom_String[3][5]={{"7A105"},{"7A318"},{"7B208"}};
uint8_t Key_Pressed=0;
uint8_t Key_Pressed_Tick=0;
uint8_t Key_Press=0;
void UI_Print_Bat_Stat(uint8_t bat_stat)
{
const uint8_t* bat_bmp_ptr;
	switch(bat_stat)
	{
		case UI_BAT_FULL:
		{
			bat_bmp_ptr=Battery_Full_Icon;
			break;
		}
		case UI_BAT_HALF:
		{
			bat_bmp_ptr=Battery_Half_Icon;
			break;
		}
		case UI_BAT_EMPTY:
		{
			bat_bmp_ptr=Battery_Empty_Icon;
			break;
		}
		case UI_BAT_CHRG:
		{
			bat_bmp_ptr=Battery_Charging_Icon;
			break;
		}
	}
		OLED_DrawBMP(115,0,127,1,bat_bmp_ptr);
}
void UI_Clear_Below_Stat_Bar(void)
{
	for(uint8_t i=1;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD_FLAG);    
		OLED_WR_Byte (0x00,OLED_CMD_FLAG);      
		OLED_WR_Byte (0x10,OLED_CMD_FLAG);        
		for(uint8_t n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA_FLAG); 
	} 
}
void UI_Main(void)
{
	while(1)
	{
		
	}
}
void Key_Handle(void)
{
	uint8_t temp_key;
	temp_key=Key_Scan();
	if(temp_key!=Key_Press)
	{
		Key_Press=temp_key;
		Key_Pressed_Tick=0;
	}
	else
	{
		Key_Pressed_Tick++;
	}
	if(Key_Pressed_Tick>Key_Scan_Count)
	{
		Key_Pressed=Key_Press;
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint8_t pin_state=0;
	switch(GPIO_Pin){
		case GPIO_PIN_2://TP_CHRG Detect:
			{
				pin_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
				if(pin_state==GPIO_PIN_SET)
				{
					
				}
				else if(pin_state==GPIO_PIN_RESET)
				{
					UI_Print_Bat_Stat(UI_BAT_CHRG);
				}
				break;
		}
		case GPIO_PIN_4:
		{
			pin_state=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4);
			if(pin_state==GPIO_PIN_SET)
				{
					
				}
				else if(pin_state==GPIO_PIN_RESET)
				{
					UI_Print_Bat_Stat(UI_BAT_FULL);
				}
				break;
		}
	}
			
}
