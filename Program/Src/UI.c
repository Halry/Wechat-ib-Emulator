#include "UI.h"
#include "UI_Array.h"
//Status bar Icon
extern uint8_t Key_Pressed;
extern bool Key_Hold;
uint8_t display_bat_stat=0;
extern bool UI_BAT_Charging;
extern uint16_t BT_Left_ADV_Count;
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

	OLED_R_ShowChinese(0,1,Chinese_ptr[0]);
	OLED_ShowChinese(0,3,Chinese_ptr[1]);
	//Display Left Count
	UI_Update_Left_Count();
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
				UI_Settings_Selection();
			}
				else
				{
			UI_Classroom_Selection();
				}
		}
		else if(key==Key_X&&Key_Hold==true)
		{
			UI_Clear_Below_Stat_Bar();
			OLED_ShowChinese(0,0,Sure_Power_Off[0]);
			OLED_ShowString(64,0,"?",true);
			while(1)
			{
				key=Get_Key();
				if(key==Key_OK)
				{
					System_low_power(PWR_STDBY);
				}
				else if(key==Key_X)
				{
					UI_Main();
				}
			}
			
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
	Start_beacon(BT_Classroom_Minor[select]);
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
void UI_Settings_Selection(void)
{
	uint8_t select=0;
	uint8_t key;
		UI_Clear_Below_Stat_Bar();
	const uint8_t *Chinese_ptr[3]={Time_Setting_Chinese[0],DFU_Update_Chinese[0],Version_Chinese[0]};
	while(1)
	{
//		key=Get_Key();
//		if(key==Key_Down&&(select<=2))
//		{
//			select++;
//			OLED_ShowString(0,(select-1)+1,Classroom_String[select-1]);
//			OLED_R_ShowString(0,select+1,Classroom_String[select]);
//		}
//		else if(key==Key_Up&&select>0)
//		{
//			select--;
//			OLED_ShowString(0,(select+1)+1,Classroom_String[select+1]);
//			OLED_R_ShowString(0,select+1,Classroom_String[select]);
//		}
//		else if(key==Key_X)
//		{
//			UI_Main();
//		}
//		else if(key==Key_OK)
//		{
//			UI_BT_Adverising(select);
//			OLED_R_ShowString(0,1,Classroom_String[0]);
//	OLED_ShowString(0,2,Classroom_String[1]);
//	OLED_ShowString(0,3,Classroom_String[2]);
//		}
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
