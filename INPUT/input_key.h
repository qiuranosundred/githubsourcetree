#ifndef __INPUT_KEY_H__
#define __INPUT_KEY_H__

#include <stdint.h>

void input_key_init(void);

void input_key_scan(void);

uint8_t input_key_is_held(uint8_t key_id);

#endif
