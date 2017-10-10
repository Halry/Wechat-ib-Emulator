#ifndef __CRYPTO_IF_H
#define __CRYPTO_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
uint8_t Get_DRNG(void);
void Base16_Encode(const uint8_t *input, uint16_t input_len, uint8_t *output, uint16_t *output_len);
	bool Verify_FW(uint8_t *sign);
#ifdef __cplusplus
}
#endif

#endif
