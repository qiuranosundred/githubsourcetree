#include "ui_manager.h"

#include <stddef.h>

#include "page.h"
#include "page_main.h"
#include "page_menu.h"

#define PAGE_COUNT 2

static page_t *pages[PAGE_COUNT];
static page_t *current_page = NULL;

void ui_manager_init(void)
{
    pages[PAGE_MAIN] = &page_main;
    pages[PAGE_MENU] = &page_menu;

    ui_manager_switch_page(PAGE_MAIN);
}

void ui_manager_switch_page(int page_id)
{
    if(page_id < 0 || page_id >= PAGE_COUNT) return;

    if(current_page && current_page->on_exit)
    {
        current_page->on_exit();
    }

    current_page = pages[page_id];

    if(current_page && current_page->on_enter)
    {
        current_page->on_enter();
    }
}

void ui_manager_handle_input(input_event_t event)
{
    if(current_page && current_page->on_key)
    {
        current_page->on_key(event);
    }
}

void ui_manager_update(void)
{
    if(current_page && current_page->on_update)
    {
        current_page->on_update();
    }
}
