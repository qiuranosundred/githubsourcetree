#include "input_key.h"

#include "bsp_key.h"
#include "input_manager.h"

#define KEY_SCAN_PERIOD        20

#define KEY_SHORT_PRESS_TICK   2
#define KEY_LONG_PRESS_TICK    50

typedef struct
{
    uint16_t press_tick;
    uint8_t  long_sent;
} key_state_t;

static key_state_t key_state[KEY_ID_MAX];

static uint8_t key_is_pressed(key_id_t id)
{
    return (bsp_key_read(id) == 0);
}

static void scan_one_key(key_id_t id,
                         input_event_t short_evt,
                         input_event_t long_evt)
{
    uint8_t pressed = key_is_pressed(id);

    if(pressed)
    {
        key_state[id].press_tick++;

        if(key_state[id].press_tick >= KEY_LONG_PRESS_TICK)
        {
            if(key_state[id].long_sent == 0)
            {
                input_post_event(long_evt);
                key_state[id].long_sent = 1;
            }
        }
    }
    else
    {
        if(key_state[id].press_tick >= KEY_SHORT_PRESS_TICK &&
           key_state[id].press_tick < KEY_LONG_PRESS_TICK)
        {
            input_post_event(short_evt);
        }

        key_state[id].press_tick = 0;
        key_state[id].long_sent = 0;
    }
}

uint8_t input_key_is_held(uint8_t key_id)
{
    if(key_id >= KEY_ID_MAX) return 0;
    return key_is_pressed((key_id_t)key_id);
}

void input_key_init(void)
{
}

void input_key_scan(void)
{
    scan_one_key(KEY_ID_UP,
                 INPUT_KEY_UP,
                 INPUT_KEY_UP_LONG);

    scan_one_key(KEY_ID_DOWN,
                 INPUT_KEY_DOWN,
                 INPUT_KEY_DOWN_LONG);

    scan_one_key(KEY_ID_OK,
                 INPUT_KEY_OK,
                 INPUT_KEY_OK_LONG);
}
