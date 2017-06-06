#include "misc_function.h"
extern ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig;
uint16_t ADC_Value[3];
bool ADC_Get_Value_Done=false;
uint8_t ADC_Sample_Current_Channel=0;
uint8_t ADC_Busy_Tick=0;
uint16_t ADC_Action_Tick=0;
uint8_t Key_Pressed=0;
uint8_t Key_Last_Pressed=0;
uint16_t Key_Pressed_Tick=0;
uint8_t Key_Press=0;
uint8_t Key_Last_Press=0;
bool Key_Hold=false;
extern uint8_t display_bat_stat;
bool UI_BAT_Charging=false;
uint8_t ADC_Get_Value(void)
{
	ADC1_Init();
	if(ADC_Sample_Current_Channel!=0)
	{
		if(ADC_Busy_Tick>10)
		{
			HAL_NVIC_DisableIRQ(ADC1_IRQn);
			HAL_ADC_DeInit(&hadc1);
			ADC_Busy_Tick=0;
		ADC_Sample_Current_Channel=0;
		memset(ADC_Value,'\0',3);
		}
		ADC_Busy_Tick++;
		return ADC_Sample_Busy;
	}
	ADC_Get_Value_Done=false;
	ADC1_Init();
	if(HAL_ADCEx_Calibration_Start(&hadc1)!=HAL_OK)
	{
		_Error_Handler("Err",0);
		//return a error
	}
	sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
	}
	HAL_ADC_Start_IT(&hadc1);
	ADC_Busy_Tick=0;
	return ADC_Sample_Start;
}
void ADC_Action(bool Is_Init)
{
	ADC_Action_Tick++;
	if(ADC_Action_Tick>=ADC_Sample_Tick_Start||Is_Init==true)
	{
		ADC_Get_Value();
	}
}
void Key_Scan(void)
{
	Key_Press=0;
	bool yes_key=0;
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==GPIO_PIN_RESET)
	{
		yes_key=true;
		Key_Press=Key_OK;
	}
	else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_RESET)
	{
		Key_Press=Key_Down;
	}
	else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_RESET)
	{
		Key_Press=Key_Up;
	}
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET)
	{
		if(yes_key==true)
		{
			Key_Press=Key_RST_Combine;
		}
		else
		{
			Key_Press=Key_X;
		}
	}
	if(Key_Last_Press!=Key_Press)
	{
		Key_Pressed_Tick=0;
		Key_Hold=false;
	}
		Key_Pressed_Tick++;
	if(Key_Pressed_Tick>=Key_Press_Count)
	{
		if(Key_Last_Pressed!=Key_Press)
		{
		Key_Pressed=Key_Press;
		Key_Last_Pressed=Key_Pressed;
		Key_Press=0;
		}
		else
		{
			if(Key_Pressed_Tick>=Key_Hold_Tick&&Key_Press!=0)
			{
			Key_Hold=true;
			Key_Pressed=Key_Press;
			}
		}
	}
	Key_Last_Press=Key_Press;
}
uint8_t Get_Key(void)
{
	uint8_t key;
	key=Key_Pressed;
	Key_Pressed=0;
	return key;
}
void System_low_power(uint8_t low_power_type)
{
	switch(low_power_type)
	{
		case PWR_STDBY:
			HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
			HAL_PWR_EnterSTANDBYMode();
	}
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	ADC_Value[ADC_Sample_Current_Channel]=HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop_IT(&hadc1);
	
	switch(ADC_Sample_Current_Channel)
	{
		case(ADC_Bat_Channel_Value-1):
		{
			sConfig.Channel=ADC_CHANNEL_4;
	sConfig.Rank=1;
	sConfig.SamplingTime=ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  ADC_Sample_Current_Channel++;
	HAL_ADC_Start_IT(&hadc1);
	break;
		}
		case(ADC_TempSensor_Value-1):
		{
	sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	HAL_ADC_Start_IT(&hadc1);
	ADC_Sample_Current_Channel++;
	break;
}
		default:
		{
			ADC1_DeInit();
			ADC_Sample_Current_Channel=0;
			ADC_Busy_Tick=0;
			ADC_Get_Value_Done=true;
		}
	}
	if(UI_BAT_Charging==false)
	{
	float bat=((float)ADC_Value[ADC_Bat_Channel_Value]/(float)ADC_Value[ADC_Verf_Channel_Value]);
	if(bat>ADC_BAT_HALF)
	{
		UI_Print_Bat_Stat(UI_BAT_FULL);
	}
	else if(bat<=ADC_BAT_HALF)
	{
		UI_Print_Bat_Stat(UI_BAT_HALF);
	}
	else if(bat<=ADC_BAT_EMPTY)
	{
		UI_Print_Bat_Stat(UI_BAT_EMPTY);
	}
	else if(bat<=ADC_BAT_SHUTDOWN)
	{
		System_low_power(PWR_STDBY);
	}
}
}
