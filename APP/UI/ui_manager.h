#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "input_event.h"

#define PAGE_MAIN  0
#define PAGE_MENU  1

void ui_manager_init(void);
void ui_manager_switch_page(int page_id);
void ui_manager_handle_input(input_event_t event);
void ui_manager_update(void);

#endif
