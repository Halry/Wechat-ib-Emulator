#include "main.h"
const uint8_t Battery_Full_Icon[12]={
	0x0C,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F};
const uint8_t Battery_Half_Icon[12]={
	0x0C,0x3F,0x21,0x21,0x21,0x21,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F};
const uint8_t Battery_Empty_Icon[12]={
	0x0C,0x3F,0x21,0x21,0x23,0x27,0x2D,0x39,0x31,0x21,0x21,0x3F};
const uint8_t Battery_Charging_Icon[12]={
	0x0C,0x3F,0x21,0x33,0x33,0x3F,0x2D,0x2D,0x2D,0x2D,0x2D,0x3F};
const uint8_t Auto_Icon[5]={0x3C,0x12,0x11,0x12,0x3C};
	const uint8_t Prototype_Icon[5]={0x3F,0x09,0x09,0x0F,0x00};
	const uint8_t Lock_Icon[5]={0x3C,0x27,0x25,0x27,0x3C};
	const uint8_t Broadcast_Icon[5]={0x22,0x0A,0x1A,0x02,0x3E};
	const uint8_t Unlock_Icon[5]={0x3C,0x27,0x25,0x25,0x3C};
	const uint8_t Error_Icon[5]={0x3F,0x25,0x25,0x25,0x21};
	//Chinese
	//Signin
		const uint8_t Signin_Chinese[4][16]={
{0x00,0x10,0x18,0x0C,0x86,0x9C,0xC4,0x64,0x70,0xCC,0x86,0x84,0x1C,0x04,0x04,0x00},
{0x00,0x02,0x43,0x41,0x79,0x42,0x42,0x7E,0x52,0x42,0x72,0x5F,0x41,0x41,0x03,0x00},/*"??",0*/
{0x00,0xC4,0xC4,0xB4,0x9C,0xC4,0x44,0x74,0xC4,0x80,0xF8,0x00,0x00,0xFE,0xFE,0x00},
{0x00,0x64,0x64,0x24,0x3F,0x3F,0x24,0x24,0x24,0x00,0x0F,0x40,0xC0,0x7F,0x3F,0x00},/*"��?",1*/};
		const uint8_t Setting_Chinese[4][16]={
			{0x00,0x40,0x46,0xCC,0x08,0xC0,0x60,0x3C,0x04,0x04,0x04,0x7C,0x40,0x40,0x40,0x00},
{0x00,0x00,0x00,0x7F,0xB0,0x98,0x41,0x47,0x2D,0x31,0x39,0x6D,0x43,0x40,0xC0,0x00},/*"����",0*/
{0x00,0x00,0x5C,0x5C,0xD4,0xD4,0xDC,0xF4,0xF4,0xDC,0xD4,0xD4,0xD4,0x5C,0x00,0x00},
{0x00,0x40,0x40,0x40,0x7F,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x7F,0x40,0x40,0x40,0x00},/*"??",1*/};
		const uint8_t Time_Setting_Chinese[4][16]={
			{0x00,0xF8,0xF8,0x88,0x88,0xF8,0x00,0x30,0xB0,0x30,0x30,0x30,0xFE,0x30,0x30,0x00},
{0x00,0x3F,0x3F,0x08,0x08,0x1F,0x00,0x00,0x01,0x03,0x40,0x40,0x7F,0x00,0x00,0x00},
{0x00,0xF0,0xF0,0x06,0x0C,0xE0,0x20,0x24,0x24,0x24,0xE4,0x04,0x04,0xFC,0x00,0x00},
{0x00,0x7F,0x7F,0x00,0x00,0x0F,0x09,0x09,0x09,0x09,0x0F,0x00,0x40,0x7F,0x00,0x00},};
		const uint8_t DFU_Update_Chinese[8][16]={
{0x00,0xFE,0xFE,0x06,0x26,0x26,0x26,0xFE,0xFE,0x26,0x26,0x26,0x06,0xFE,0x00,0x00},
{0x00,0xFF,0xFF,0x60,0x60,0x6F,0x69,0x69,0x69,0x69,0x6F,0x60,0x60,0x7F,0x00,0x00},/*"1��",0*/
{0x00,0xC0,0x70,0xF8,0x06,0xC0,0x78,0x28,0x20,0x20,0xFE,0x20,0x20,0x20,0x00,0x00},
{0x00,0x00,0x00,0x7F,0x00,0x02,0x02,0x02,0x02,0x02,0x7F,0x02,0x02,0x02,0x02,0x00},/*"?t",1*/
{0x00,0x04,0x04,0xF4,0x94,0x94,0x94,0xFC,0xFC,0x94,0x94,0x94,0xF4,0x04,0x04,0x00},
{0x00,0xC0,0x40,0x43,0x4E,0x3A,0x32,0x3F,0x23,0x62,0x42,0x42,0x43,0x40,0x40,0x00},/*"?��",2*/
{0x00,0xC8,0xD8,0xE8,0xCE,0xCC,0xF8,0xC8,0xC0,0xFC,0xFC,0x44,0x44,0xC2,0x42,0x00},
{0x00,0x32,0x1A,0xCA,0x7F,0x02,0x0A,0x12,0xE2,0x3F,0x07,0x00,0x00,0xFF,0x00,0x00},/*"D?",3*/};
		const uint8_t Hold_to_back_Chinese[10][16]={
			
{0x80,0x80,0x80,0x80,0xFF,0x80,0x80,0xA0,0x90,0x88,0x84,0x82,0x80,0x80,0x80,0x00},
{0x00,0x00,0x00,0x00,0xFF,0x40,0x21,0x12,0x04,0x08,0x10,0x20,0x20,0x40,0x40,0x00},/*"��",0*/
{0x10,0x10,0x10,0xFF,0x90,0x20,0x98,0x88,0x88,0xE9,0x8E,0x88,0x88,0xA8,0x98,0x00},
{0x02,0x42,0x81,0x7F,0x00,0x00,0x80,0x84,0x4B,0x28,0x10,0x28,0x47,0x80,0x00,0x00},/*"��",1*/
{0x00,0x40,0x20,0x1F,0x20,0x58,0x47,0x50,0x48,0x45,0x42,0x45,0x48,0x50,0x40,0x00},/*"��",3*/
{0x00,0x00,0xFE,0x02,0x02,0xF2,0x12,0x12,0x12,0xF2,0x02,0x02,0xFE,0x00,0x00,0x00},
{0x00,0x00,0x7F,0x20,0x20,0x27,0x24,0x24,0x24,0x27,0x20,0x20,0x7F,0x00,0x00,0x00},/*"��",4*/
};
		const uint8_t Version_Chinese[4][16]=
		{
			
{0x00,0xFE,0x20,0x20,0x3F,0x20,0x00,0xFC,0x24,0xE4,0x24,0x22,0x23,0xE2,0x00,0x00},
{0x80,0x7F,0x01,0x01,0xFF,0x80,0x60,0x1F,0x80,0x41,0x26,0x18,0x26,0x41,0x80,0x00},/*"?",0*/
{0x00,0x10,0x10,0x10,0x10,0xD0,0x30,0xFF,0x30,0xD0,0x10,0x10,0x10,0x10,0x00,0x00},
{0x10,0x08,0x04,0x02,0x09,0x08,0x08,0xFF,0x08,0x08,0x09,0x02,0x04,0x08,0x10,0x00},/*"?",1*/

		};
		const uint8_t Classroom_String[3][6]={{"7A105"},{"7A318"},{"7B208"}};
const uint8_t Max_Classroom_Count=3;
