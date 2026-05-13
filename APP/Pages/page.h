#ifndef __PAGE_H__
#define __PAGE_H__

#include "input_event.h"

typedef struct
{
    void (*on_enter)(void);
    void (*on_exit)(void);
    void (*on_key)(input_event_t event);
    void (*on_update)(void);
} page_t;

#endif
