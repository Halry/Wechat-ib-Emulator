#include "misc_function.h"
uint8_t Key_Pressed=0;
uint16_t Key_Pressed_Cnt=0;
extern uint8_t display_bat_stat;
bool UI_BAT_Charging=false;
uint8_t Key_Scan(void)
{
  static uint8_t Key_Press;
  static uint8_t Key_Last_Press;
  static uint16_t Key_Press_Cnt;
  Key_Press=0;
  if(HAL_GPIO_ReadPin(GPIOA,GPIO_Key_X)==GPIO_PIN_SET)
    {
    Key_Press+=Key_X;
    }
  if(HAL_GPIO_ReadPin(GPIOA,GPIO_Key_Up)==GPIO_PIN_RESET)
    {
    Key_Press+=Key_Up;
    }
  if(HAL_GPIO_ReadPin(GPIOA,GPIO_Key_Down)==GPIO_PIN_RESET)
    {
    Key_Press+=Key_Down;
    }
  if(HAL_GPIO_ReadPin(GPIOA,GPIO_Key_OK)==GPIO_PIN_RESET)
    {
    Key_Press+=Key_OK;
    }
  if(Key_Press==0)
    {
    Key_Last_Press=0;
    Key_Press_Cnt=0;
    Key_Pressed=0;
    Key_Pressed_Cnt=0;
    return 0;
    }
  else if(Key_Press==Key_Last_Press)
    {
    Key_Press_Cnt++;
    if(Key_Press==Key_OK+Key_X+Key_Up&&Key_Press_Cnt>=5)
      {
      HAL_NVIC_SystemReset();
      }
    if(Key_Press_Cnt>=5)
      {
      Key_Pressed=Key_Press;
      Key_Pressed_Cnt=Key_Press_Cnt;
      return Key_Pressed;
      }
    }
  Key_Last_Press=Key_Press;
  return 0;
}
uint16_t Get_Key(bool is_cnt)
{
  static uint8_t Key_Last_Return;
  if(is_cnt==true)
    {
    return Key_Pressed_Cnt;
    }
  uint8_t temp_key=Key_Pressed;
  if(Key_Pressed_Cnt<100&&Key_Pressed==Key_Last_Return)
    {
    return 0;
    }
  Key_Last_Return=Key_Pressed;
  return temp_key;
}
void Reset_Key_State(void)
{
  Key_Pressed_Cnt=0;
  Key_Pressed=0;
}
//low power action
void System_low_power(uint8_t low_power_type)
{
  switch(low_power_type)
    {
    case PWR_STDBY:
      __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
      __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
      HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
      HAL_PWR_EnterSTANDBYMode();
    }
}


