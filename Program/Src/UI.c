#include "UI.h"
#include "UI_Array.h"
//Status bar Icon
const uint8_t Classroom_String[3][5]={{"7A105"},{"7A318"},{"7B208"}};
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

