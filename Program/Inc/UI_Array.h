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
		const uint8_t Signin_Chinese[5][16]={
{0x01},
{0x20,0x10,0x0C,0x07,0x8C,0x54,0x24,0x14,0x28,0x47,0x84,0x0C,0x14,0x04,0x04,0x00},
{0x02,0x42,0x41,0x49,0x70,0x41,0x45,0x59,0x41,0x61,0x50,0x4D,0x41,0x42,0x02,0x00},
{0x42,0x62,0x52,0x4A,0xC6,0x42,0x52,0x62,0xC2,0x00,0xF8,0x00,0x00,0xFF,0x00,0x00},
{0x40,0xC4,0x44,0x44,0x7F,0x24,0x24,0x24,0x20,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00},};
const uint8_t Setting_Chinese[5][16]={
			{0x01},
{0x40,0x40,0x42,0xCC,0x00,0x40,0xA0,0x9E,0x82,0x82,0x82,0x9E,0xA0,0x20,0x20,0x00},
{0x00,0x00,0x00,0x3F,0x90,0x88,0x40,0x43,0x2C,0x10,0x28,0x46,0x41,0x80,0x80,0x00},
{0x00,0x17,0x15,0xD5,0x55,0x57,0x55,0x7D,0x55,0x57,0x55,0xD5,0x15,0x17,0x00,0x00},
{0x40,0x40,0x40,0x7F,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x7F,0x40,0x40,0x40,0x00},};
		const uint8_t Time_Setting_Chinese[5][16]={
			{0x01},
{0x00,0xFC,0x84,0x84,0x84,0xFC,0x00,0x10,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x00},
{0x00,0x3F,0x10,0x10,0x10,0x3F,0x00,0x00,0x01,0x06,0x40,0x80,0x7F,0x00,0x00,0x00},
{0x00,0xF8,0x01,0x06,0x00,0xF0,0x12,0x12,0x12,0xF2,0x02,0x02,0x02,0xFE,0x00,0x00},
{0x00,0xFF,0x00,0x00,0x00,0x1F,0x11,0x11,0x11,0x1F,0x00,0x40,0x80,0x7F,0x00,0x00},};
		const uint8_t DFU_Update_Chinese[9][16]={
			{0x03},
{0x00,0xFE,0x02,0x12,0x92,0x92,0x92,0xFE,0x92,0x92,0x92,0x12,0x02,0xFE,0x00,0x00},
{0x00,0xFF,0x40,0x40,0x5F,0x48,0x48,0x48,0x48,0x48,0x5F,0x40,0x40,0xFF,0x00,0x00},
{0x00,0x80,0x60,0xF8,0x07,0x80,0x60,0x1C,0x10,0x10,0xFF,0x10,0x10,0x10,0x00,0x00},
{0x01,0x00,0x00,0xFF,0x00,0x02,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x02,0x02,0x00},
{0x02,0x02,0xF2,0x92,0x92,0x92,0x92,0xFE,0x92,0x92,0x92,0x92,0xF2,0x02,0x02,0x00},
{0x80,0x80,0x87,0x4C,0x54,0x24,0x34,0x4F,0x44,0x44,0x84,0x84,0x87,0x80,0x80,0x00},
{0x40,0x44,0x54,0x65,0xC6,0x64,0x54,0x44,0x00,0xFC,0x44,0x44,0xC4,0x42,0x40,0x00},
{0x20,0x12,0x4A,0x82,0x7F,0x02,0x0A,0x92,0x60,0x1F,0x00,0x00,0xFF,0x00,0x00,0x00},};
		const uint8_t Hold_to_back_Chinese[9][16]={
			{0x03},
{0x80,0x80,0x80,0x80,0xFF,0x80,0x80,0xA0,0x90,0x88,0x84,0x82,0x80,0x80,0x80,0x00},
{0x00,0x00,0x00,0x00,0xFF,0x40,0x21,0x12,0x04,0x08,0x10,0x20,0x20,0x40,0x40,0x00},
{0x10,0x10,0x10,0xFF,0x90,0x20,0x98 ,0x88,0x88,0xE9,0x8E,0x88,0x88,0xA8,0x98,0x00},
{0x02,0x42,0x81,0x7F,0x00,0x00,0x80,0x84,0x4B,0x28,0x10,0x28,0x47,0x80,0x00,0x00},
{0x40,0x40,0x42,0xCC,0x00,0x00,0xFC,0x24,0xA4,0x24,0x22,0x22,0xA3,0x62,0x00,0x00},
{0x00,0x40,0x20,0x1F,0x20,0x58,0x47,0x50,0x48,0x45,0x42,0x45,0x48,0x50,0x40,0x00},
{0x00,0x00,0xFE,0x02,0x02,0xF2,0x12,0x12,0x12,0xF2,0x02,0x02,0xFE,0x00,0x00,0x00},
{0x00,0x00,0x7F,0x20,0x20,0x27,0x24,0x24,0x24,0x27,0x20,0x20,0x7F,0x00,0x00,0x00},
};
		const uint8_t Version_Chinese[5][16]={
			{0x01},
{0x00,0xFE,0x20,0x20,0x3F,0x20,0x00,0xFC,0x24,0xE4,0x24,0x22,0x23,0xE2,0x00,0x00},
{0x80,0x7F,0x01,0x01,0xFF,0x80,0x60,0x1F,0x80,0x41,0x26,0x18,0x26,0x41,0x80,0x00},
{0x00,0x10,0x10,0x10,0x10,0xD0,0x30,0xFF,0x30,0xD0,0x10,0x10,0x10,0x10,0x00,0x00},
{0x10,0x08,0x04,0x02,0x09,0x08,0x08,0xFF,0x08,0x08,0x09,0x02,0x04,0x08,0x10,0x00},};
		const uint8_t Left_Count_Chinese[5][16]={
			{0x01},
			{0x48,0x4A,0xFA,0x0A,0xFE,0x09,0xF9,0x49,0xA8,0x00,0x00,0xF0,0x00,0x00,0xFF,0x00},
{0x22,0x11,0x0B,0x04,0xFF,0x04,0x09,0x12,0x23,0x00,0x00,0x0F,0x40,0x80,0x7F,0x00},
{0x80,0x80,0x40,0x20,0x50,0x48,0x44,0xC3,0x44,0x48,0x50,0x20,0x40,0x80,0x80,0x00},
{0x00,0x40,0x22,0x1A,0x02,0x42,0x82,0x7F,0x02,0x02,0x02,0x0A,0x12,0x60,0x00,0x00},};
		const uint8_t Started_Chinese[7][16]={
			{0x02},
			{0x00,0x00,0xE2,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,0x00,0x00,0x00},
{0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x78,0x00,0x00},
{0x10,0x10,0x10,0xFF,0x10,0x90,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x00},
{0x04,0x44,0x82,0x7F,0x01,0x00,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00},
{0x80,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x80,0x00},
{0x00,0x80,0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00},};
		const uint8_t Left_Not_Enough[9][16]={
			{0x03},
			{0x00,0x02,0x0C,0x80,0x60,0x80,0x40,0x30,0x0F,0xC8,0x08,0x08,0x28,0x18,0x00,0x00},
{0x02,0x02,0x7E,0x01,0x80,0x40,0x20,0x18,0x06,0x01,0x06,0x18,0x20,0x40,0x80,0x00},
{0x90,0x52,0x34,0x10,0xFF,0x10,0x34,0x52,0x80,0x70,0x8F,0x08,0x08,0xF8,0x08,0x00},
{0x82,0x9A,0x56,0x63,0x22,0x52,0x8E,0x00,0x80,0x40,0x33,0x0C,0x33,0x40,0x80,0x00},
{0x00,0x02,0x02,0x02,0x02,0x82,0x42,0xF2,0x0E,0x42,0x82,0x02,0x02,0x02,0x00,0x00},
{0x10,0x08,0x04,0x02,0x01,0x00,0x00,0xFF,0x00,0x00,0x00,0x01,0x02,0x0C,0x00,0x00},
{0x00,0x00,0x00,0x7E,0x42,0x42,0x42,0xC2,0x42,0x42,0x42,0x7E,0x00,0x00,0x00,0x00},
{0x80,0x40,0x30,0x0E,0x10,0x20,0x40,0x7F,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00},};
		const uint8_t Sure_Power_Off[9][16]={
			{0x03},
			{0x04,0x84,0xE4,0x5C,0x44,0xC4,0x20,0x10,0xE8,0x27,0x24,0xE4,0x34,0x2C,0xE0,0x00},
{0x02,0x01,0x7F,0x10,0x10,0x3F,0x80,0x60,0x1F,0x09,0x09,0x3F,0x49,0x89,0x7F,0x00},
{0x10,0x0C,0x44,0x44,0x44,0x44,0x45,0xC6,0x44,0x44,0x44,0x44,0x44,0x14,0x0C,0x00},
{0x80,0x40,0x20,0x1E,0x20,0x40,0x40,0x7F,0x44,0x44,0x44,0x44,0x44,0x40,0x40,0x00},
{0x00,0x00,0x10,0x11,0x16,0x10,0x10,0xF0,0x10,0x10,0x14,0x13,0x10,0x00,0x00,0x00},
{0x81,0x81,0x41,0x41,0x21,0x11,0x0D,0x03,0x0D,0x11,0x21,0x41,0x41,0x81,0x81,0x00},
{0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00},
{0x04,0x03,0x00,0xFF,0x00,0x83,0x60,0x1F,0x00,0x00,0x00,0x3F,0x40,0x40,0x78,0x00},};
		const uint8_t Classroom_String[3][6]={{"7A403"},{"7A318"},{"7B208"}};
		
		const uint8_t Max_Classroom_Count=3;
