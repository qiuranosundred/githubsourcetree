#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "main.h"


typedef enum
{
    KEY_ID_UP = 0,
    KEY_ID_DOWN,
    KEY_ID_OK,

    KEY_ID_MAX

} key_id_t;

uint8_t bsp_key_read(key_id_t id);


#endif
