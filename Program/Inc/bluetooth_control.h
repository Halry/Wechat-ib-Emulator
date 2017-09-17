#ifndef __bluetooth_control_h
#define __bluetooth_control_h
#include "main.h"
extern const uint8_t BT_Classroom_Minor[3][4];
extern char *BT_Last_Minor;
void BT_Init(void);
bool Start_beacon(uint8_t *minor);
bool Stop_beacon(void);
void USART1_Init(void);
void USART1_DeInit(void);
void BT_Power_Control(bool power);
void BT_Read_Setup_BKP(void);
void BT_Write_Setup_BKP(void);
void BT_UART_Handler(uint8_t *data,uint16_t size);
#endif