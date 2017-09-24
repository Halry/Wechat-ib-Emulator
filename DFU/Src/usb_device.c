/**
  ******************************************************************************
  * @file           : USB_DEVICE  
  * @version        : v2.0_Cube
  * @brief          : This file implements the USB Device 
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "stdlib.h"
/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;
extern uint8_t *USB_RX_Buffer;
extern uint16_t USB_RXed;
extern bool Is_Connected;
extern bool Is_Tampered;
extern uint16_t USB_RX_Max_Size;
uint8_t USB_In_Handler=0;
FLASH_EraseInitTypeDef EraseInitStruct;
/* init function */				        
void USB_DEVICE_Init(void)
{
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

  USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

  USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

  USBD_Start(&hUsbDeviceFS);
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);//Enable Enum
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);//Show device in BL
}
void USB_Deivce_DeInit(void)
{
	USBD_DeInit(&hUsbDeviceFS);
}
void USB_Receive_Handle(void)
{
	switch(USB_In_Handler)
	{
		case USB_Not_Hnd:
			USB_Not_Handled_Handler();
			break;
		case USB_In_PCDN:
			USB_HND_PCDN();
		break;
		case USB_In_PRDN:
			USB_HND_PRDN();
		break;
		default:
			HAL_NVIC_SystemReset();
	}
}
void USB_Not_Handled_Handler(void)
{
	if(*(USB_RX_Buffer+USB_RXed-1)==0x1B&&USB_RXed==1)
		{
			if(Is_Tampered==true)
			{
				CDC_Transmit_FS((uint8_t *)"!Tampered!\r",11);
			}
			CDC_Transmit_FS((uint8_t *)"RDY",3);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
			Is_Connected=true;
			Clean_USB_RX_Buf();
		}
		if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&Is_Connected==true)
		{
			if(Is_Tampered==false)
			{
			if((memcmp(USB_RX_Buffer,"ZDN",3))==0)
			{
				//Init ZModem download
			}
			else if((memcmp(USB_RX_Buffer,"CDN",3))==0)
			{
				//Get Encrypted Counter Data
			}
			else if((memcmp(USB_RX_Buffer,"RDN",3))==0)
			{
				//Get Encrypted Classroom Data
			}
			else if((memcmp(USB_RX_Buffer,"RR",2))==0)
			{
				//Give Random to PC
			}
			else if((memcmp(USB_RX_Buffer,"IDR",3))==0)
			{
				//Give Device ID to PC
			}
			else if((memcmp(USB_RX_Buffer,"RST",3))==0)
			{
				HAL_NVIC_SystemReset();
			}
			}
			else
			{
				if((memcmp(USB_RX_Buffer,"TRT",3))==0)
			{
				//Get Tamper Reset Data
			}
			}
			#ifdef PROTOTYPE_DFU
			if((memcmp(USB_RX_Buffer,"PFDN",4))==0)
			{
				//Download Unecrypted fw
			}
			else if((memcmp(USB_RX_Buffer,"PCDN",4))==0)
			{
				Clean_USB_RX_Buf();
				USB_In_Handler=USB_In_PCDN;
			}
			else if((memcmp(USB_RX_Buffer,"PRDN",4))==0)
			{
				Clean_USB_RX_Buf();
				Relocate_USB_Buf(56);
				USB_In_Handler=USB_In_PRDN;
			}
			#endif
			CDC_Transmit_FS((uint8_t *)"UCD\r",4);
			Clean_USB_RX_Buf();
		}
		else if(Is_Connected==true)
		{
			CDC_Transmit_FS((USB_RX_Buffer+USB_RXed-1),1);
		}
}
#ifdef PROTOTYPE_DFU
void USB_HND_PCDN(void)
{
	CDC_Transmit_FS((USB_RX_Buffer+USB_RXed-1),1);
	if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&USB_RXed==4)
	{
		ASCII_to_Integer(USB_RX_Buffer,3);
		uint16_t temp=(*(USB_RX_Buffer)*100)+(*(USB_RX_Buffer+1)*10)+(*(USB_RX_Buffer+2));
		Write_BKP(RTC_BKP_DR8,temp);
		Clean_USB_RX_Buf();
		USB_In_Handler=USB_Not_Hnd;//Go back to no handle.
	}
}
void USB_HND_PRDN(void)
{
	CDC_Transmit_FS((USB_RX_Buffer+USB_RXed-1),1);
	if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&*(USB_RX_Buffer)>0&&USB_RXed==(((*USB_RX_Buffer)-'0')*9+2))
		{
			uint8_t temp_flash[FLASH_PAGE_SIZE];
			 uint8_t *p=((uint8_t *)0x0801FC00);
			memcpy(temp_flash,p,FLASH_PAGE_SIZE);
			memcpy(temp_flash,USB_RX_Buffer,USB_RXed-2);
			HAL_FLASH_Unlock();
			EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
			EraseInitStruct.PageAddress = 0x0801FC00;
			EraseInitStruct.NbPages     =1;
			uint32_t erase_error=0;
			if(HAL_FLASHEx_Erase(&EraseInitStruct,&erase_error)!=HAL_OK)
			{
				//go to error
				HAL_NVIC_SystemReset();
			}
			if(Flash_Write(temp_flash,(uint8_t *)0x0801FC00,FLASH_PAGE_SIZE)!=0)
			{
				//go to error
				HAL_NVIC_SystemReset();
			}
			HAL_FLASH_Lock();
		}
}
void USB_HND_PFDN(void)
{
}
#endif
void Clean_USB_RX_Buf(void)
{
	memset(USB_RX_Buffer,'\0',64);
	USB_RXed=0;
}
void Relocate_USB_Buf(uint16_t size)
{
	free(USB_RX_Buffer);
	USB_RX_Buffer=malloc(size);
	USB_RX_Max_Size=size;
	USB_RXed=0;
}
uint16_t Flash_Write(uint8_t *src, uint8_t *dest, uint32_t Len)
{
  uint32_t i = 0;
  
  for(i = 0; i < Len; i+=4)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by byte */ 
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(dest+i), *(uint32_t*)(src+i)) == HAL_OK)
    {
     /* Check the written value */
      if(*(uint32_t *)(src + i) != *(uint32_t*)(dest+i))
      {
        /* Flash content doesn't match SRAM content */
        return 2;
      }
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return 1;
    }
  }
  return 0;
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
