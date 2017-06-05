#include "UI.h"
#include "UI_Array.h"
//Status bar Icon
extern uint8_t Key_Pressed;
extern bool Key_Hold;
uint8_t display_bat_stat=0;
extern bool UI_BAT_Charging;
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
	UI_Clear_Below_Stat_Bar();
	bool select_set=false;
	OLED_R_ShowChinese(0,1,Signin_Chinese[0]);
	OLED_R_ShowChinese(16,1,Signin_Chinese[2]);
	OLED_ShowChinese(0,3,Setting_Chinese[0]);
	OLED_ShowChinese(16,3,Setting_Chinese[2]);
	while(1)
	{
		uint8_t key=Get_Key();
		if(key==Key_Down)
		{
			select_set=1;
			OLED_ShowChinese(0,1,Signin_Chinese[0]);
	OLED_ShowChinese(16,1,Signin_Chinese[2]);
	OLED_R_ShowChinese(0,3,Setting_Chinese[0]);
	OLED_R_ShowChinese(16,3,Setting_Chinese[2]);
			select_set=true;
		}
		else if(key==Key_Up&&select_set==true)
		{
			OLED_R_ShowChinese(0,1,Signin_Chinese[0]);
	OLED_R_ShowChinese(16,1,Signin_Chinese[2]);
	OLED_ShowChinese(0,3,Setting_Chinese[0]);
	OLED_ShowChinese(16,3,Setting_Chinese[2]);
		}
		else if(key==Key_OK)
		{
			if(select_set==true)
			{
				UI_Settings_Selection();
			}
				else
				{
			UI_Classroom_Selection();
				}
		}
		else if(key==Key_X&&Key_Hold==true)
		{
			System_low_power(PWR_STDBY);
		}
	}
}
void UI_Classroom_Selection(void)
{
	uint8_t select=0;
	UI_Clear_Below_Stat_Bar();
	OLED_R_ShowString(0,1,Classroom_String[0]);
	OLED_ShowString(0,2,Classroom_String[1]);
	OLED_ShowString(0,3,Classroom_String[2]);
	while(1)
	{
		uint8_t key=Get_Key();

		if(key==Key_Down&&(select<2))
		{
			select++;
			OLED_ShowString(0,(select-1)+1,Classroom_String[select-1]);
			OLED_R_ShowString(0,select+1,Classroom_String[select]);
		}
		else if(key==Key_Up&&select>0)
		{
			select--;
			OLED_ShowString(0,(select+1)+1,Classroom_String[select+1]);
			OLED_R_ShowString(0,select+1,Classroom_String[select]);
		}
		else if(key==Key_X)
		{
			UI_Main();
		}
		else if(key==Key_OK)
		{
			UI_BT_Adverising(select);
			OLED_R_ShowString(0,1,Classroom_String[0]);
	OLED_ShowString(0,2,Classroom_String[1]);
	OLED_ShowString(0,3,Classroom_String[2]);
			select=0;
		}
	}
}
void UI_BT_Adverising(uint8_t select)
{
	UI_Clear_Below_Stat_Bar();
	OLED_ShowString(0,1,Classroom_String[select]);
	OLED_ShowString(0,2,"Started");
	Start_beacon(BT_Classroom_Minor[select]);
	while(1)
	{
		uint8_t key=Get_Key();
		if(key==Key_X)
		{
			Stop_beacon();
			UI_Classroom_Selection();
		}
	}
}
void UI_Settings_Selection(void)
{
	
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
