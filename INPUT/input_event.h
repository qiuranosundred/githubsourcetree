#ifndef __INPUT_EVENT_H__
#define __INPUT_EVENT_H__

//输入事件 所有输入设备共用
#include "stdint.h"

typedef enum
{
    INPUT_NONE = 0,

    INPUT_KEY_UP,
    INPUT_KEY_DOWN,
    INPUT_KEY_OK,

    INPUT_KEY_UP_LONG,
    INPUT_KEY_DOWN_LONG,
    INPUT_KEY_OK_LONG,

} input_event_t;

#endif
