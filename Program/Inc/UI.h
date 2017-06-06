#ifndef __UI_h
#define __UI_h
#include "main.h"

#define UI_BAT_FULL 0
#define UI_BAT_HALF 1
#define UI_BAT_EMPTY 2
#define UI_BAT_CHRG 3

void UI_Print_Bat_Stat(uint8_t bat_stat);
void UI_Clear_Below_Stat_Bar(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Key_Handle(void);
void UI_Main(void);
void UI_Classroom_Selection(void);
void UI_Settings_Selection(void);
void UI_BT_Adverising(uint8_t select);

#endif