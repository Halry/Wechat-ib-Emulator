/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "stdlib.h"
/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;
extern uint8_t USB_RX_Buffer[64];
extern uint16_t USB_RXed;
extern bool Is_Connected;
extern bool Not_Tampered;
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
extern uint8_t DRNG_Output_B16[32];
uint8_t Device_ID_B16[24];
extern uint8_t cc20_iv[8];
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
		case USB_In_TRT:
			USB_HND_TRT();
		break;
		case USB_In_RDN:
			USB_HND_RDN();
		break;
		case USB_In_CDN:
			USB_HND_CDN();
		break;
		case USB_In_FDN:
			USB_HND_FDN();
		break;
		#ifdef PROTOTYPE_DFU
    case USB_In_PCDN:
      USB_HND_PCDN();
      break;
    case USB_In_PRDN:
      USB_HND_PRDN();
      break;
    case USB_In_PFDN:
      USB_HND_PFDN();
      break;
		#endif
    default:
      HAL_NVIC_SystemReset();
    }
}
void USB_Not_Handled_Handler(void)
{
  if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&USB_RXed==1&&Is_Connected==false)
    {
    if(Not_Tampered==false)
      {
      CDC_Transmit_FS((uint8_t *)"!Tampered!\r\n",12);
      }
			Base16_Encode((uint8_t*)0x1FFFF7E8,12,Device_ID_B16,NULL);
    CDC_Transmit_FS((uint8_t *)"RDY\r\n",5);
    RDY_Sys_Tick=HAL_GetTick();
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
    Is_Connected=true;
    Clean_USB_RX_Buf();
    }
  if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&Is_Connected==true)
    {
    if(Not_Tampered==true)
      {
      if((memcmp(USB_RX_Buffer,"FDN",3))==0)
        {
					CDC_Transmit_FS((uint8_t*)"Start\r\n",7);
					Clean_USB_RX_Buf();
					USB_In_Handler=USB_In_FDN;
					FW_Dwn_Stage=FW_DWN_INIT;
					FW_Already_Dwn=0;
        }
      else if((memcmp(USB_RX_Buffer,"CDN",3))==0)
        {
					if(Is_DRNG_Get==true)
	{
        CDC_Transmit_FS((uint8_t*)"Start\r\n",7);
					Clean_USB_RX_Buf();
					USB_In_Handler=USB_In_CDN;
	}
        }
      else if((memcmp(USB_RX_Buffer,"RDN",3))==0)
        {
					if(Is_DRNG_Get==true)
	{
        	CDC_Transmit_FS((uint8_t*)"Start\r\n",7);
					Clean_USB_RX_Buf();
					USB_In_Handler=USB_In_RDN;
	}
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
						CDC_Transmit_FS((uint8_t *)"\r\n",2);
            Clean_USB_RX_Buf();
            }
          else
            {
            CDC_Transmit_FS(&DRNG_Output_B16[0],32);
							CDC_Transmit_FS((uint8_t *)"\r\n",2);
							Clean_USB_RX_Buf();
            }
          }
        else
          {
          CDC_Transmit_FS(&DRNG_Output_B16[0],32);
						CDC_Transmit_FS((uint8_t *)"\r\n",2);
						Clean_USB_RX_Buf();
          }
        }
      else if((memcmp(USB_RX_Buffer,"IDR",3))==0)
        {
        CDC_Transmit_FS(Device_ID_B16,24);
					CDC_Transmit_FS((uint8_t *)"\r\n",2);
					Clean_USB_RX_Buf();
        }
      else if((memcmp(USB_RX_Buffer,"RST",3))==0)
        {
					Write_BKP(RTC_BKP_DR1,Read_BKP(RTC_BKP_DR1)&~0x0002);
        HAL_NVIC_SystemReset();
        }
      }
    else
      {
      if((memcmp(USB_RX_Buffer,"TRT",3))==0)
        {
					CDC_Transmit_FS((uint8_t*)"Start\r\n",7);
					Clean_USB_RX_Buf();
					USB_In_Handler=USB_In_TRT;
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
						CDC_Transmit_FS((uint8_t *)"\r\n",2);
            Clean_USB_RX_Buf();
            }
          else
            {
            CDC_Transmit_FS(&DRNG_Output_B16[0],32);
							CDC_Transmit_FS((uint8_t *)"\r\n",2);
							Clean_USB_RX_Buf();
            }
          }
        else
          {
          CDC_Transmit_FS(&DRNG_Output_B16[0],32);
						CDC_Transmit_FS((uint8_t *)"\r\n",2);
						Clean_USB_RX_Buf();
          }
        }
      else if((memcmp(USB_RX_Buffer,"IDR",3))==0)
        {
        CDC_Transmit_FS(Device_ID_B16,24);
					CDC_Transmit_FS((uint8_t *)"\r\n",2);
					Clean_USB_RX_Buf();
        }
      }
#ifdef PROTOTYPE_DFU
    if((memcmp(USB_RX_Buffer,"PFDN",4))==0)
      {
      Clean_USB_RX_Buf();
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
	if(FW_Dwn_Stage==FW_DWN_INIT)
    {
				FW_Dwn_size=(((uint16_t)*USB_RX_Buffer)<<8)|(*(USB_RX_Buffer+1));
				memcpy((void *)&cc20_iv[0],USB_RX_Buffer+2,8);
				Clean_USB_RX_Buf();
				FW_Dwn_Stage=FW_DWN_SIZE_LOADED;
			
		}
		else if(FW_Dwn_Stage==FW_DWN_SIZE_LOADED)
		{
			if(USB_RXed==64)
			{
				uint8_t temp_flash[FLASH_PAGE_SIZE];
				uint8_t temp_sign[64];
				memcpy(temp_sign,USB_RX_Buffer,64);
				memcpy(temp_flash,(uint8_t *)Room_Flash_Address,FLASH_PAGE_SIZE);
				uint8_t fw_size_high=FW_Dwn_size>>8;
				uint8_t fw_size_low=FW_Dwn_size;
				memcpy(temp_flash+0x3D,&fw_size_high,1);
				memcpy(temp_flash+0x3D+1,&fw_size_low,1);
				memcpy(temp_flash+0x3F,temp_sign,64);
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
			HAL_FLASH_Unlock();
      EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
      EraseInitStruct.PageAddress = FW_Start_Address;
      EraseInitStruct.NbPages     =64;
			erase_error=0;
      if(HAL_FLASHEx_Erase(&EraseInitStruct,&erase_error)!=HAL_OK)
        {
        //go to error
        HAL_NVIC_SystemReset();
        }
      HAL_FLASH_Lock();
      FW_Dwn_Stage=FW_DWN_ERASED;
			cc20_init();
			Clean_USB_RX_Buf();
			}
			
		}
		else if(FW_Dwn_Stage==FW_DWN_ERASED)
		{
			if(FW_Dwn_size-FW_Already_Dwn>=64)
			{
				uint8_t temp_decrypted[64];
				int32_t data_size;
				if(cc20_decrypt(USB_RX_Buffer,64,temp_decrypted,data_size)!=true)
				{
					CDC_Transmit_FS((uint8_t*)"DErr\r\n",6);
					FW_Dwn_Stage=FW_DWN_INIT;
					USB_In_Handler=USB_Not_Hnd;
				}
				else
				{
				HAL_FLASH_Unlock();
      HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
				Flash_Write(temp_decrypted,(uint8_t*)FW_Start_Address+FW_Already_Dwn,64);
				HAL_FLASH_Lock();
				FW_Already_Dwn+=64;
					}
				Clean_USB_RX_Buf();
				
		}
			else if(FW_Dwn_size-FW_Already_Dwn<64)
      {
				uint8_t temp_decrypted[USB_RXed];
				int32_t data_size;
				if(cc20_decrypt(USB_RX_Buffer,FW_Dwn_size-FW_Already_Dwn,temp_decrypted,data_size)!=true)
				{
					CDC_Transmit_FS((uint8_t*)"DErr\r\n",6);
					FW_Dwn_Stage=FW_DWN_INIT;
					USB_In_Handler=USB_Not_Hnd;
				}
				else
				{
				HAL_FLASH_Unlock();
				Flash_Write(temp_decrypted,(uint8_t*)FW_Start_Address+FW_Already_Dwn,FW_Dwn_size-FW_Already_Dwn);
				HAL_FLASH_Lock();
				Clean_USB_RX_Buf();
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
				}
      CDC_Transmit_FS((uint8_t*)"Loaded\r\n",8);
				if(Verify_FW()!=true)
			{
				CDC_Transmit_FS((uint8_t*)"VErr\r\n",6);
				FW_Dwn_Stage=FW_DWN_INIT;
				USB_In_Handler=USB_Not_Hnd;
			}
			else
			{
				HAL_NVIC_SystemReset();
			CDC_Transmit_FS((uint8_t*)"Reset\r\n",10);
			FW_Dwn_Stage=FW_DWN_INIT;
				USB_In_Handler=USB_Not_Hnd;
			}
			}
		}
}
void USB_HND_TRT(void)
{
	if(USB_RXed==64)
	{
		uint8_t temp_decrypted[56];
		memcpy(cc20_iv,USB_RX_Buffer,8);
		cc20_init();
		cc20_decrypt(USB_RX_Buffer+8,56,temp_decrypted,56);
		if(memcmp(temp_decrypted,Device_ID_B16,24)==0)
		{
			if(memcmp(temp_decrypted+24,DRNG_Output_B16,32)==0)
			{
				//Reset Tamper Flag
				Write_BKP(RTC_BKP_DR1,Read_BKP(RTC_BKP_DR1)|0x0001);
				CDC_Transmit_FS((uint8_t *)"Reset\r\n",7);
				HAL_NVIC_SystemReset();
			}
		}
	}
}
void USB_HND_CDN(void)
{
		static bool already_received=false;
	uint8_t temp_decrypted[34];
		static uint8_t temp_receive[98];
	if(USB_RXed==64)
	{
		memcpy(cc20_iv,USB_RX_Buffer,8);
		memcpy(temp_receive,USB_RX_Buffer+8,56);
		already_received=true;
		Clean_USB_RX_Buf();
	}
	else if(USB_RXed==42&&already_received==true)
	{
		memcpy(temp_receive+56,USB_RX_Buffer,42);
		cc20_init();
		cc20_decrypt(temp_receive,34,temp_decrypted,34);
		if(memcmp(temp_decrypted,DRNG_Output_B16,32)==0)
		{
			if(Verify_Data(temp_decrypted,34,temp_receive+34)==true)
			{
			uint16_t temp_count=((uint16_t)*(temp_decrypted+32))|(((uint16_t)*(temp_decrypted+33))<<8);
			Write_BKP(RTC_BKP_DR8,temp_count);
			CDC_Transmit_FS((uint8_t *)"Reset\r\n",7);
				HAL_NVIC_SystemReset();
			}
		
		}
		    Clean_USB_RX_Buf();
		USB_In_Handler=USB_Not_Hnd;
}
	}
void USB_HND_RDN(void)
{
	static uint8_t already_received=0;
	static uint8_t temp_decrypted[117];
	static uint8_t temp_receive[181];
	if(USB_RXed==64)
	{
		if(already_received==0)
		{
		already_received=1;
		memcpy(cc20_iv,USB_RX_Buffer,8);
		memcpy(temp_receive,USB_RX_Buffer+8,56);
		Clean_USB_RX_Buf();
		}
		else if(already_received==1)
		{
			already_received=2;
			memcpy(temp_receive+56,USB_RX_Buffer,64);
		Clean_USB_RX_Buf();
		}
	}
	else if(USB_RXed==61&&already_received==2)
	{
		memcpy(temp_receive+120,USB_RX_Buffer,61);
		cc20_init();
		cc20_decrypt(temp_receive,117,temp_decrypted,117);
		uint8_t sign[64];
		memcpy(sign,temp_receive+117,64);
		if(memcmp(temp_decrypted,Device_ID_B16,24)==0)
		{
			if(memcmp(temp_decrypted+24,DRNG_Output_B16,32)==0)
			{
				if(Verify_Data(temp_decrypted,117,sign)==true)
				{
				uint8_t temp_flash[FLASH_PAGE_SIZE];
    memcpy(temp_flash,(uint8_t *)Room_Flash_Address,FLASH_PAGE_SIZE);
    memcpy(temp_flash,temp_decrypted+24+32,61);
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
    CDC_Transmit_FS((uint8_t *)"Reset\r\n",7);
			HAL_NVIC_SystemReset();
			}
		}
		}
		USB_In_Handler=USB_Not_Hnd;
		Clean_USB_RX_Buf();
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
  if(*(USB_RX_Buffer)-'0'>6)
    {
    USB_In_Handler=USB_Not_Hnd;
    CDC_Transmit_FS((uint8_t*)"Err\r\n",5);
    }
  if(*(USB_RX_Buffer+USB_RXed-1)==0x0D&&(*(USB_RX_Buffer)-'0')>0&&USB_RXed==(((*USB_RX_Buffer)-'0')*10+2))
    {
    uint8_t temp_flash[FLASH_PAGE_SIZE];
    memcpy(temp_flash,(uint8_t *)Room_Flash_Address,FLASH_PAGE_SIZE);
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
      Clean_USB_RX_Buf();
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
      Clean_USB_RX_Buf();
      FW_Already_Dwn+=64;
      }
    else
      {
      HAL_FLASH_Unlock();
      Flash_Write(USB_RX_Buffer,(uint8_t*)FW_Start_Address+FW_Already_Dwn,FW_Dwn_size-FW_Already_Dwn);
      HAL_FLASH_Lock();
      Clean_USB_RX_Buf();
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
      FW_Dwn_Stage=FW_DWN_INIT;
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
