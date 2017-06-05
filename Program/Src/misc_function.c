#include "misc_function.h"
extern ADC_HandleTypeDef hadc1;
ADC_ChannelConfTypeDef sConfig;
uint16_t ADC_Value[3];
bool ADC_Get_Value_Done=false;
uint8_t ADC_Sample_Current_Channel=0;
uint8_t ADC_Busy_Tick=0;
uint8_t ADC_Action_Tick=0;
uint8_t ADC_Get_Value(void)
{
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
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
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
	if(ADC_Action_Tick>=10)
	{
		
	}
}
uint8_t Key_Scan(void)
{
	uint8_t temp_key=0;
	bool yes_key=0;
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==GPIO_PIN_RESET)
	{
		yes_key=true;
		temp_key=Key_OK;
	}
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_SET)
	{
		if(yes_key==true)
		{
			temp_key=Key_RST_Combine;
		}
		else
		{
			temp_key=Key_X;
		}
	}
	else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)==GPIO_PIN_RESET)
	{
		temp_key=Key_Down;
	}
	else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==GPIO_PIN_RESET)
	{
		temp_key=Key_Up;
	}
	else
	{
		temp_key=0;
	}
	return temp_key;
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
	sConfig.SamplingTime=ADC_SAMPLETIME_1CYCLE_5;
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
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
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
		HAL_ADC_DeInit(&hadc1);
			ADC_Sample_Current_Channel=0;
			ADC_Busy_Tick=0;
			ADC_Get_Value_Done=true;
		}
	}
}
