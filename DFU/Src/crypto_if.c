#include "crypto_if.h"
#include "stdbool.h"
#include "stdlib.h"
#include "usb_device.h"
extern uint32_t RDY_Sys_Tick;
extern uint32_t RR_Sys_Tick;
uint8_t DRNG_Output_B16[32];
bool Is_DRNG_Get=0;
uint8_t Get_DRNG(void)
{
  uint8_t RNG_State=0;
  uint8_t DRNG_Entropy[20];
  uint8_t DRNG_Nonce[8];
  uint8_t DRNG_PS[12];
  uint8_t DRNG_Output[16];
  //Init DRNG Entropy
  memcpy(DRNG_Entropy,&RDY_Sys_Tick,4);
  memcpy(DRNG_Entropy+4,&RR_Sys_Tick,4);
  memcpy(DRNG_Entropy+8,(uint8_t*)0x1FFFF7E8,12);
  //Init DRNG Nonce
  uint32_t tick=HAL_GetTick();
  memcpy(DRNG_Nonce,&tick,4);
  memcpy(DRNG_Nonce,&RR_Sys_Tick,4);
  //Init DRNG PS
  memcpy(DRNG_PS,(uint8_t*)Room_Flash_Address,12);
  //Start RNG Init
  RNGinitInput_stt RNGinit_st;
  RNGstate_stt RNGstate;
  RNGinit_st.pmEntropyData = DRNG_Entropy;
  RNGinit_st.mEntropyDataSize = sizeof(DRNG_Entropy);
  RNGinit_st.pmNonce =  DRNG_Nonce;
  RNGinit_st.mNonceSize = sizeof(DRNG_Nonce);
  RNGinit_st.pmPersData = DRNG_PS;
  RNGinit_st.mPersDataSize = sizeof(DRNG_PS);
  RNG_State=RNGinit(&RNGinit_st, &RNGstate);
  if(RNG_State!=RNG_SUCCESS)
    {
    return RNG_State;
    }
  RNG_State = RNGgenBytes(&RNGstate, NULL, DRNG_Output, sizeof(DRNG_Output));
  if(RNG_State!=RNG_SUCCESS)
    {
    return RNG_State;
    }
  Base16_Encode(DRNG_Output,16,DRNG_Output_B16,NULL);
  Is_DRNG_Get=true;
  return RNG_State;
}
void Base16_Encode(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t *output_len)
{
  int i;
	const uint8_t hex_digits[16] = "0123456789abcdef";
  for(i = 0; i < input_len; i++)
    {
    output[2 * i] = hex_digits[(input[i]>>4) ];
    output[2 * i + 1] = hex_digits[input[i]&0xF];
    }
  if(output_len!=NULL)
    *output_len = 2 * input_len;
}
