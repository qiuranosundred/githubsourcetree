#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "input_event.h"

void input_manager_init(void);

void input_post_event(input_event_t event);

uint8_t input_get_event(input_event_t * event,
                        uint32_t timeout);

#endif
