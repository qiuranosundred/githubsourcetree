#include "bsp_key.h"

uint8_t bsp_key_read(key_id_t id)
{
    switch(id)
    {
        case KEY_ID_UP:

            return HAL_GPIO_ReadPin(KEY1_GPIO_Port,
                                    KEY1_Pin);

        case KEY_ID_DOWN:

            return HAL_GPIO_ReadPin(KEY2_GPIO_Port,
                                    KEY2_Pin);

        case KEY_ID_OK:

            return HAL_GPIO_ReadPin(KEY3_GPIO_Port,
                                    KEY3_Pin);
    }

    return 1;
}
