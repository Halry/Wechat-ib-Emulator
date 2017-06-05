#ifndef __UI_h
#define __UI_h
#include "main.h"
#define Key_X 1
#define Key_Up 2
#define Key_Down 3
#define Key_OK 4
#define Key_RST_Combine 5
#define UI_BAT_FULL 0
#define UI_BAT_HALF 1
#define UI_BAT_EMPTY 2
#define UI_BAT_CHRG 3
#define Key_Scan_Count 10
#define Key_Long_Press_Tick_Count 350
void UI_Print_Bat_Stat(uint8_t bat_stat);
void UI_Clear_Below_Stat_Bar(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Key_Handle(void);
void UI_Main(void);
#endif