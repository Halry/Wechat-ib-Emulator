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
extern bool OLED_Ready;
void UI_Print_Bat_Stat(uint8_t bat_stat)
{
  if(OLED_Ready==true)
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
}
void UI_Clear_Below_Stat_Bar(void)
{
  for(uint8_t i=1; i<8; i++)
    {
    OLED_WR_Byte(0xb0+i,OLED_CMD_FLAG);
    OLED_WR_Byte(0x00,OLED_CMD_FLAG);
    OLED_WR_Byte(0x10,OLED_CMD_FLAG);
    for(uint8_t n=0; n<128; n++)OLED_WR_Byte(0,OLED_DATA_FLAG);
    }
}
void UI_Update_Left_Count(void)
{
  OLED_ShowChar(0,0,((BT_Left_ADV_Count/100)%10)+'0',false);
  OLED_ShowChar(6,0,((BT_Left_ADV_Count/10)%10)+'0',false);
  OLED_ShowChar(12,0,(BT_Left_ADV_Count%10)+'0',false);
}
void UI_Main(void)
{
  uint8_t key;
  UI_Clear_Below_Stat_Bar();
  bool select_settings=false;
  bool displayed=false;
  const uint8_t *Chinese_ptr[2]= {Signin_Chinese[0],Setting_Chinese[0]};
  UI_Update_Left_Count();
  OLED_R_ShowChinese(0,1,Chinese_ptr[0]);
  OLED_ShowChinese(0,3,Chinese_ptr[1]);
  //Display Left Count
  while(1)
    {
    key=Get_Key(false);
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
    else if(key==Key_X&&Get_Key(true)>=100)
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
  while(Get_Key(true)!=0);
  while(1)
    {
    key=Get_Key(false);
    if(key==Key_OK)
      {
      System_low_power(PWR_STDBY);
      }
    else if(key==Key_X)
      {
      Reset_Key_State();
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
    key=Get_Key(false);
    if(displayed==false)
      {
      for(uint8_t i=0; i<BT_Classroom_Count; i++)
        {
        if(i==select)
          {
          if(i<3)
            {
            OLED_R_ShowString(0,i+i+1,BT_Classroom_Name+i*6,true);
            }
          else
            {
            OLED_R_ShowString(80,(i-3)+(i-3)+1,BT_Classroom_Name+i*6,true);
            }
          }
        else
          {
          if(i<3)
            {
            OLED_ShowString(0,i+i+1,BT_Classroom_Name+i*6,true);
            }
          else
            {
            OLED_ShowString(80,(i-3)+(i-3)+1,BT_Classroom_Name+i*6,true);
            }
          }
        }
      displayed=true;
      }
    if(key==Key_Down&&(select<(BT_Classroom_Count-1)))
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
  if(select>(BT_Classroom_Count-1))
    {
    UI_Classroom_Selection();
    }
  UI_Clear_Below_Stat_Bar();
  if(BT_Left_ADV_Count>0)
    {
    Start_beacon(BT_Classroom_Minor+(select*4));
    OLED_ShowString(0,1,BT_Classroom_Name+select*6,true);
    OLED_ShowChinese(40,1,Started_Chinese[0]);
    uint8_t Left_Tick=60;
    OLED_ShowChar(0,3,(Left_Tick/10)%10+'0',true);
    OLED_ShowChar(8,3,(Left_Tick%10)+'0',true);
    uint32_t Start_Ticks=HAL_GetTick();
    uint32_t left_count_tick=Start_Ticks;
    while(1)
      {
      if(HAL_GetTick()-left_count_tick>=1000)
        {
        Left_Tick--;
        OLED_ShowChar(0,3,(Left_Tick/10)%10+'0',true);
        OLED_ShowChar(8,3,(Left_Tick%10)+'0',true);
        left_count_tick=HAL_GetTick();
        }
      uint8_t key=Get_Key(false);
      if(key==Key_X)
        {
        Stop_beacon();
        UI_Main();
        }
      if(HAL_GetTick()-Start_Ticks>=60500)
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
  const uint8_t *Chinese_ptr[3]= {Time_Setting_Chinese[0],DFU_Update_Chinese[0],Version_Chinese[0]};
  while(1)
    {
    if(displayed==false)
      {
      for(uint8_t i=0; i<=2; i++)
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
    key=Get_Key(false);
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
  UI_Clear_Below_Stat_Bar();
  switch(select)
    {
    case 0://Time setting select
      {
      }
    case 1://DFU
      {
      while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==GPIO_PIN_RESET);
      HAL_PWR_EnableBkUpAccess();
      uint16_t DR0_BK=0;
      DR0_BK=HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1);
      DR0_BK=DR0_BK|0x0002;
      HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,DR0_BK);
      HAL_PWR_DisableBkUpAccess();
				HAL_Delay(1000);
      HAL_NVIC_SystemReset();
      }
    case 2://Version
      {
      OLED_ShowString(0,1,(uint8_t*)"SW:",true);
      OLED_ShowString(24,1,(uint8_t*)System_Version,true);
      OLED_ShowString(0,3,(uint8_t*)"HW:",true);
      OLED_ShowString(24,3,(uint8_t*)HW_Ver,true);
			#ifdef SYS_DBG 
				OLED_ShowString(0,5,(uint8_t*)"Debug FW!",true);
				#endif
				#ifndef SYS_DBG
				OLED_ShowString(0,5,(uint8_t*)"Design by Halry",true);
				#endif
      while(1)
        {
        uint8_t key=Get_Key(false);
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
void UI_Show_Please_Wait(void)
{
  UI_Clear_Below_Stat_Bar();
  OLED_ShowChinese(40,1,(uint8_t *)Please_Wait_Chinese);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
  uint8_t pin_state=0;
  switch(GPIO_Pin)
    {
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
