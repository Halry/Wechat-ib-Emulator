#ifndef __OLED_H
#define __OLED_H
#include "main.h"
#include "stdbool.h"
#include "stdint.h"
#define SIZE 16
#define Max_Column	128
#define OLED_CMD_FLAG 0x00
#define OLED_DATA_FLAG 0x01

//
#define OLED_DC_CLR() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET)
#define OLED_DC_SET() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)
//
#define OLED_RST_CLR() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET)
#define OLED_RST_SET() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET)
//
#define OLED_SS_CLR() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)
#define OLED_SS_SET() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)

void OLED_Init(void);
void OLED_WR_Byte(uint8_t data,bool cmd_flag);
void OLED_Clear(void);
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr);
void OLED_WR_R_Byte(uint8_t data,bool cmd_flag);
void OLED_R_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_R_ShowString(uint8_t x,uint8_t y,uint8_t *chr);
#endif
