#include "crypto_if.h"
#include "stdbool.h"
#include "stdlib.h"
#include "usb_device.h"
extern uint32_t RDY_Sys_Tick;
extern uint32_t RR_Sys_Tick;
uint8_t DRNG_Output_B16[32];
bool Is_DRNG_Get=0;
const uint8_t ed25519_pk[32]={
	0x5d,0xcd,0x08,0x93,0x4d,0xa8,0x40,0xb8,
	0x57,0x7e,0x4b,0x37,0x95,0x15,0x2e,0x59,
	0x31,0x86,0x3c,0xc6,0xea,0x0d,0x2d,0x16,
	0x58,0x45,0x72,0x2a,0x25,0x34,0x77,0xe8};
const uint8_t cc20_key[CRL_CHACHA256_KEY]={
	0x23, 0xC4, 0x7A, 0x79, 0x6C, 0x4E, 0xB7, 0x34,
	0x1A, 0xD4, 0x94, 0xF8, 0x8F, 0x4E, 0x02, 0xE6, 
	0x24, 0x70, 0x0F, 0x4E, 0x0A, 0xE1, 0x6F, 0xAD,
	0x40, 0x69, 0x2E, 0xA9, 0x94, 0x20, 0x4B, 0x07};
uint8_t cc20_iv[CRL_CHACHA_NONCE];
CHACHActx_stt chacha20ctx_st; 
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
bool Verify_FW(void)
{
	uint16_t fw_size=(((uint16_t)(*(uint8_t*)FW_Size_Address))<<8)|((uint16_t)(*(uint8_t*)(FW_Size_Address+1)));
	if(ED25519verify((uint8_t*)FW_Start_Address,fw_size,(uint8_t*)FW_Sign_Address,ed25519_pk)!=SIGNATURE_VALID)
	{
		return false;
	}
	return true;
}
bool Verify_Data(uint8_t *in_data_p,uint8_t in_length,uint8_t *sign)
{
	if(ED25519verify(in_data_p,in_length,sign,ed25519_pk)!=SIGNATURE_VALID)
	{
		return false;
	}
	return true;
}
bool cc20_init(void)
{
  chacha20ctx_st.mFlags = E_SK_DEFAULT; 
  chacha20ctx_st.mIvSize = 8;  
  chacha20ctx_st.mKeySize = 32;
	if(CHACHA_Decrypt_Init(&chacha20ctx_st,cc20_key,cc20_iv)!=CHACHA_SUCCESS)
	{
		return false;
	}
	return true;
}
bool cc20_decrypt(uint8_t *in_data_p,uint8_t in_length,uint8_t *out_data_p,int32_t out_length)
{
	if(CHACHA_Decrypt_Append(&chacha20ctx_st,in_data_p,in_length,out_data_p,&out_length)!=CHACHA_SUCCESS)
	{
		return false;
	}
	return true;
}
void Base16_Encode(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t *output_len)
{
  int i;
	const uint8_t hex_digits[16] = "0123456789ABCDEF";
  for(i = 0; i < input_len; i++)
    {
    output[2 * i] = hex_digits[(input[i]>>4) ];
    output[2 * i + 1] = hex_digits[input[i]&0xF];
    }
  if(output_len!=NULL)
    *output_len = 2 * input_len;
}
