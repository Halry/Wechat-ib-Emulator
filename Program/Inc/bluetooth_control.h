#ifndef __bluetooth_control_h
#define __bluetooth_control_h
#include "main.h"
extern const uint8_t BT_Classroom_Minor[3][4];
void BT_Init(void);
void Start_beacon(const uint8_t *minor);
void Stop_beacon(void);
void USART1_Init(void);
void USART1_DeInit(void);
#endif