#include "app_task.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lvgl.h"
#include "lv_port_disp.h"

#include "input_key.h"
#include "input_manager.h"
#include "ui_manager.h"
#include "device_table.h"

static void input_task(void *arg)
{
    input_manager_init();

    while(1)
    {
        input_key_scan();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

static void ui_task(void *arg)
{
    vTaskDelay(pdMS_TO_TICKS(50));

    lv_init();
    lv_port_disp_init();
    ui_manager_init();

    input_event_t event;

    while(1)
    {
        if(input_get_event(&event, 0))
        {
            ui_manager_handle_input(event);
        }

        ui_manager_update();

        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void app_task_create(void)
{
    devices_register_all();
    device_table_init();

    xTaskCreate(input_task,
                "input_task",
                256,
                NULL,
                3,
                NULL);

    xTaskCreate(ui_task,
                "ui_task",
                2048,
                NULL,
                4,
                NULL);
}
