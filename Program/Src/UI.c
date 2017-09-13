#include "UI.h"
#include "UI_Array.h"
//Status bar Icon
extern uint8_t Key_Pressed;
extern bool Key_Hold;
uint8_t display_bat_stat=0;
extern bool UI_BAT_Charging;
extern uint16_t BT_Left_ADV_Count;
extern const char *System_Version;
extern const char *HW_Ver;
extern RTC_HandleTypeDef hrtc;
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
void UI_Update_Left_Count(void)
{
	uint8_t *bt_left_adv_count_disp=malloc(4);
	uint8_t bt_left_adv_count_high=((BT_Left_ADV_Count/100)%10)+'0';
	memcpy(bt_left_adv_count_disp,&bt_left_adv_count_high,1);
	uint8_t bt_left_adv_count_mid=((BT_Left_ADV_Count/10)%10)+'0';
	memcpy(bt_left_adv_count_disp+1,&bt_left_adv_count_mid,1);
	uint8_t bt_left_adv_count_low=((BT_Left_ADV_Count)%10)+'0';
	memcpy(bt_left_adv_count_disp+2,&bt_left_adv_count_low,1);
	OLED_ShowString(0,0,bt_left_adv_count_disp,false);
  free(bt_left_adv_count_disp);
}
void UI_Main(void)
{
	uint8_t key;
	UI_Clear_Below_Stat_Bar();
	bool select_settings=false;
	bool displayed=false;
	const uint8_t *Chinese_ptr[2]={Signin_Chinese[0],Setting_Chinese[0]};
	UI_Update_Left_Count();
	OLED_R_ShowChinese(0,1,Chinese_ptr[0]);
	OLED_ShowChinese(0,3,Chinese_ptr[1]);
	//Display Left Count

	while(1)
	{
		key=Get_Key();
		if(key==Key_Down)
		{
			OLED_R_ShowChinese(0,3,Chinese_ptr[1]);
			OLED_ShowChinese(0,1,Chinese_ptr[0]);
			select_settings=true;
		}
		else if(key==Key_Up&&select_settings==true)
		{
				OLED_R_ShowChinese(0,1,Chinese_ptr[0]);
	OLED_ShowChinese(0,3,Chinese_ptr[1]);
			select_settings=false;
		}
		else if(key==Key_OK)
		{
			if(select_settings==true)
			{
				UI_Settings();
			}
				else
				{
			UI_Classroom_Selection();
				}
		}
		else if(key==Key_X&&Key_Hold==true)
		{
		UI_Power_Off();
			
		}
	}
}
void UI_Power_Off(void)
{
	uint8_t key;
	UI_Clear_Below_Stat_Bar();
			OLED_ShowChinese(0,1,Sure_Power_Off[0]);
			OLED_ShowString(64,1,(uint8_t *)"?",true);
	uint16_t ticks=HAL_GetTick();
	bool key_released=false;
		while(1)
			{
				key=Get_Key();
				if(key==Key_OK)
				{
					System_low_power(PWR_STDBY);
				}
				else if(key==0)
				{
					if(HAL_GetTick()-ticks>1000)
					{
					key_released=true;
					}
				}
				else if(key==Key_X&&key_released==true)
				{
					UI_Main();
				}
			}
}
void UI_Classroom_Selection(void)
{
	uint8_t key;
	uint8_t select=0;
	bool displayed=false;
	UI_Clear_Below_Stat_Bar();

	while(1)
	{
		key=Get_Key();
		if(displayed==false)
		{
			for(uint8_t i=0;i<Max_Classroom_Count;i++)
	{
		if(i==select)
		{

				OLED_R_ShowString(0,i+i+1,Classroom_String[i],true);

		}
		else
		{
			
				OLED_ShowString(0,i+i+1,Classroom_String[i],true);
		}
	}
	displayed=true;
		}
		

		if(key==Key_Down&&(select<(Max_Classroom_Count-1)))
		{
			select++;
			displayed=false;
		}
		else if(key==Key_Up&&select>0)
		{
			select--;
			displayed=false;
		}
		else if(key==Key_X)
		{
			UI_Main();
		}
		else if(key==Key_OK)
		{
			UI_BT_Adverising(select);
		}
	}
}
void UI_BT_Adverising(uint8_t select)
{
	if(select>(Max_Classroom_Count-1))
	{
		UI_Classroom_Selection();
	}
	UI_Clear_Below_Stat_Bar();
	if(BT_Left_ADV_Count>0)
	{
	OLED_ShowString(0,1,Classroom_String[select],true);
		OLED_ShowChinese(40,1,Started_Chinese[0]);
	Start_beacon((uint8_t*)BT_Classroom_Minor[select]);
	while(1)
	{
		uint8_t key=Get_Key();
		if(key==Key_X)
		{
			
			Stop_beacon();
			UI_Main();
		}
	}
}
	else
	{
		OLED_ShowChinese(0,1,Left_Not_Enough[0]);
	}
}
void UI_Settings(void)
{
	uint8_t select=0;
	uint8_t key;
		UI_Clear_Below_Stat_Bar();
	bool displayed=false;
	const uint8_t *Chinese_ptr[3]={Time_Setting_Chinese[0],DFU_Update_Chinese[0],Version_Chinese[0]};
	while(1)
	{
				if(displayed==false)
		{
			for(uint8_t i=0;i<=2;i++)
	{
		if(i==select)
		{
				OLED_R_ShowChinese(0,i+i+1,Chinese_ptr[i]);
		}
		else
		{
				OLED_ShowChinese(0,i+i+1,Chinese_ptr[i]);
		}
	}
	displayed=true;
		}
		key=Get_Key();
		if(key==Key_Down&&(select<2))
		{
			select++;
displayed=false;
		}
		else if(key==Key_Up&&select>0)
		{
			select--;
displayed=false;	
		}
		else if(key==Key_X)
		{
			UI_Main();
		}
		else if(key==Key_OK)
		{
			UI_Settings_Selection(select);
		}
	}
}
void UI_Settings_Selection(uint8_t select)
{
	uint8_t key;
	UI_Clear_Below_Stat_Bar();
	switch(select)
	{
		case 0://Time setting select
		{
			
		}
		case 1://DFU
		{
			HAL_PWR_EnableBkUpAccess();
			uint16_t DR0_BK=0;
			DR0_BK=HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1);
			DR0_BK=DR0_BK|1;
			HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,DR0_BK);
			HAL_PWR_DisableBkUpAccess();
			NVIC_SystemReset();
		}
		case 2://Version
		{
			OLED_ShowString(0,1,"SW:",true);
			OLED_ShowString(24,1,System_Version,true);
			OLED_ShowString(0,3,"HW:",true);
			OLED_ShowString(24,3,HW_Ver,true);
			while(1)
			{
				key=Get_Key();
				if(key==Key_X)
				{
					UI_Settings();
				}
			}
		}
		default:
		{
		}
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
