#include "input_manager.h"

#include "FreeRTOS.h"
#include "queue.h"

#define INPUT_QUEUE_SIZE 10

static QueueHandle_t input_queue = NULL;

void input_manager_init(void)
{
    input_queue = xQueueCreate(INPUT_QUEUE_SIZE,
                               sizeof(input_event_t));
}

void input_post_event(input_event_t event)
{
    if(input_queue == NULL)
    {
        return;
    }

    xQueueSend(input_queue,
               &event,
               0);
}

uint8_t input_get_event(input_event_t * event,
                        uint32_t timeout)
{
    if(input_queue == NULL)
    {
        return 0;
    }

    return xQueueReceive(input_queue,
                         event,
                         timeout);
}
