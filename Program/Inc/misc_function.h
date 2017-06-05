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
#define ADC_Sample_Tick_Start 1000
//ADC Value Define
extern bool ADC_Get_Value_Done;
extern uint16_t ADC_Value[3];
uint8_t ADC_Get_Value(void);
uint8_t Key_Scan(void);
#endif