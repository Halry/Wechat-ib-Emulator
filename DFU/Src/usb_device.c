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
uint8_t FW_Dwn_Stage=0;
uint16_t FW_Dwn_size=0;
uint16_t FW_Already_Dwn=0;
uint8_t *FW_temp_pointer=NULL;
uint32_t RDY_Sys_Tick=0;
uint32_t RR_Sys_Tick=0;
extern bool Is_DRNG_Get;
extern uint8_t DRNG_Output[16];
/* init function */				        
void USB_DEVICE_Init(void)
{
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

  USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

  USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

  USBD_Start(&hUsbDeviceFS);
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);//Enable Enum
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
		case USB_In_PFDN:
			USB_HND_PFDN();
		break;
		default:
			HAL_NVIC_SystemReset();
	}
}
void USB_Not_Handled_Handler(void)
{
	if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&USB_RXed==1)
		{
			if(Is_Tampered==true)
			{
				CDC_Transmit_FS((uint8_t *)"!Tampered!\r\n",12);
			}
			CDC_Transmit_FS((uint8_t *)"RDY\r\n",5);
			RDY_Sys_Tick=HAL_GetTick();
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
			Is_Connected=true;
			Clean_USB_RX_Buf();
		}
		if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&Is_Connected==true)
		{
			if(Is_Tampered==false)
			{
			if((memcmp(USB_RX_Buffer,"FDN",3))==0)
			{
				//Download Firmware
			}
			else if((memcmp(USB_RX_Buffer,"KDN",3))==0)
			{
				//Download Key File
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
				RR_Sys_Tick=HAL_GetTick();
				//Give Random to PC
				if(Is_DRNG_Get==false)
				{
				uint8_t status=Get_DRNG();
				if(status!=RNG_SUCCESS)
				{
					CDC_Transmit_FS((uint8_t *)"RNG Err:",8);	
					CDC_Transmit_FS(&status,1);
					Clean_USB_RX_Buf();
				}
				else
				{
					CDC_Transmit_FS(&DRNG_Output[0],16);
				}
			}
				else
				{
					CDC_Transmit_FS(&DRNG_Output[0],16);
				}
			}
			else if((memcmp(USB_RX_Buffer,"IDR",3))==0)
			{
				CDC_Transmit_FS((uint8_t*)0x1FFFF7E8,12);
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
				///Send RR and Device id
				RR_Sys_Tick=HAL_GetTick();
				//Give Random to PC
				uint8_t status=Get_DRNG();
				if(status!=RNG_SUCCESS)
				{
					HAL_NVIC_SystemReset();
				}
				CDC_Transmit_FS(&DRNG_Output[0],16);
				CDC_Transmit_FS((uint8_t*)0x1FFFF7E8,12);
			}
			}
			#ifdef PROTOTYPE_DFU
			if((memcmp(USB_RX_Buffer,"PFDN",4))==0)
			{
				Clean_USB_RX_Buf();
				Relocate_USB_Buf(6);
				USB_In_Handler=USB_In_PFDN;
				CDC_Transmit_FS((uint8_t*)"FS\r\n",4);
				FW_Dwn_Stage=FW_DWN_INIT;
			}
			else if((memcmp(USB_RX_Buffer,"PCDN",4))==0)
			{
				Clean_USB_RX_Buf();
				USB_In_Handler=USB_In_PCDN;
			}
			else if((memcmp(USB_RX_Buffer,"PRDN",4))==0)
			{
				Clean_USB_RX_Buf();
				Relocate_USB_Buf(64);
				USB_In_Handler=USB_In_PRDN;
			}
			#endif
			CDC_Transmit_FS((uint8_t *)"UCD\r\n",4);
			Clean_USB_RX_Buf();
		}
		else if(Is_Connected==true)
		{
			CDC_Transmit_FS((USB_RX_Buffer+USB_RXed-1),1);
		}
}
void USB_HND_FDN(void)
{
	
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
	if(*(USB_RX_Buffer)-'0'>6)
	{
		USB_In_Handler=USB_Not_Hnd;
		CDC_Transmit_FS((uint8_t*)"Err\r\n",5);
	}
	if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&(*(USB_RX_Buffer)-'0')>0&&USB_RXed==(((*USB_RX_Buffer)-'0')*10+2))
		{
			uint8_t temp_flash[FLASH_PAGE_SIZE];
			uint8_t *p=((uint8_t *)Room_Flash_Address);
			memcpy(temp_flash,p,FLASH_PAGE_SIZE);
			memcpy(temp_flash,USB_RX_Buffer,USB_RXed-1);
			*temp_flash=*temp_flash-'0';
			HAL_FLASH_Unlock();
			EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
			EraseInitStruct.PageAddress = Room_Flash_Address;
			EraseInitStruct.NbPages     =1;
			uint32_t erase_error=0;
			if(HAL_FLASHEx_Erase(&EraseInitStruct,&erase_error)!=HAL_OK)
			{
				//go to error
				HAL_NVIC_SystemReset();
			}
			if(Flash_Write(temp_flash,(uint8_t *)Room_Flash_Address,FLASH_PAGE_SIZE)!=0)
			{
				//go to error
				HAL_NVIC_SystemReset();
			}
			HAL_FLASH_Lock();
			USB_In_Handler=USB_Not_Hnd;
			CDC_Transmit_FS((uint8_t*)"Done\r\n",6);
			
		}
}
void USB_HND_PFDN(void)
{
	if(FW_Dwn_Stage==FW_DWN_INIT)
	{
	if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&USB_RXed==6)
	{
		ASCII_to_Integer(USB_RX_Buffer,5);
		FW_Dwn_size=(*(USB_RX_Buffer)*10000)+(*(USB_RX_Buffer+1)*1000)+(*(USB_RX_Buffer+2)*100)+(*(USB_RX_Buffer+3)*10)+(*(USB_RX_Buffer+4));
		FW_Already_Dwn=0;
		FW_temp_pointer=USB_RX_Buffer;
		uint8_t FW_Temp[64];
		Repoint_USB_Buf(64,FW_Temp);
		HAL_FLASH_Unlock();
			EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
			EraseInitStruct.PageAddress = FW_Start_Address;
			EraseInitStruct.NbPages     =64;
			uint32_t erase_error=0;
			if(HAL_FLASHEx_Erase(&EraseInitStruct,&erase_error)!=HAL_OK)
			{
				//go to error
				HAL_NVIC_SystemReset();
			}
			HAL_FLASH_Lock();
			FW_Dwn_Stage=FW_DWN_ERASED;
			CDC_Transmit_FS((uint8_t*)"FWST\r\n",6);
		memset(USB_RX_Buffer,'\0',64);
	}
	}
	else if(FW_Dwn_Stage==FW_DWN_ERASED)
	{
		if(FW_Dwn_size-FW_Already_Dwn>=64)
		{
			HAL_FLASH_Unlock();
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
			Flash_Write(USB_RX_Buffer,(uint8_t*)FW_Start_Address+FW_Already_Dwn,64);
			HAL_FLASH_Lock();
			memset(USB_RX_Buffer,'\0',64);
			USB_RXed=0;
			FW_Already_Dwn+=64;
		}
		else
		{
			HAL_FLASH_Unlock();
			Flash_Write(USB_RX_Buffer,(uint8_t*)FW_Start_Address+FW_Already_Dwn,FW_Dwn_size-FW_Already_Dwn);
			HAL_FLASH_Lock();
			memset(USB_RX_Buffer,'\0',64);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
			Repoint_USB_Buf(64,FW_temp_pointer);
			FW_Dwn_Stage=FW_DWN_LOADED;
			USB_In_Handler=USB_Not_Hnd;
			CDC_Transmit_FS((uint8_t*)"Done\r\n",6);
		}
	}
}
#endif
void Clean_USB_RX_Buf(void)
{
	memset(USB_RX_Buffer,'\0',64);
	USB_RXed=0;
}
void Relocate_USB_Buf(uint16_t size)
{
	USB_RX_Buffer=realloc(USB_RX_Buffer,size);
	memset(USB_RX_Buffer,'\0',size);
	USB_RX_Max_Size=size+1;
	USB_RXed=0;
}
void Repoint_USB_Buf(uint16_t size,uint8_t *src)
{
	free(USB_RX_Buffer);
	USB_RX_Buffer=src;
	USB_RX_Max_Size=size+1;
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
