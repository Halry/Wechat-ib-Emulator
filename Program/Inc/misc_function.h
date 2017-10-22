#ifndef __misc_function_h
#define __misc_function_h
#include "main.h"
#include "misc_function.h"
//========ADC section
//ADC Channel
#define ADC_Verf_Channel_Value 0
#define ADC_Bat_Channel_Value 1
#define ADC_TempSensor_Value 2
//ADC Function Flag
#define ADC_Sample_Busy 1
#define ADC_Sample_Start 0
#define ADC_Sample_Tick_Start 2000
//Voltage define
#define ADC_BAT_HALF 2.375
#define ADC_BAT_EMPTY 2
#define ADC_BAT_SHUTDOWN 1.875
//Pwr Control Define
#define PWR_STDBY 0
#define PWR_SLEP 1
#define PWR_STOP 2
#define screen_off 3
//Key Handle define
#define Key_X 1
#define Key_Up 2
#define Key_Down 3
#define Key_OK 4
#define GPIO_Key_X GPIO_PIN_0
#define GPIO_Key_Up GPIO_PIN_1
#define GPIO_Key_Down GPIO_PIN_2
#define GPIO_Key_OK GPIO_PIN_3
extern bool UI_BAT_Charging;
uint8_t Key_Scan(void);
void System_low_power(uint8_t low_power_type);
uint16_t Get_Key(bool is_cnt);
void Reset_Key_State(void);
#endif