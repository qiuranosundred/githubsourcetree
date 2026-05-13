#ifndef __APP_DATA_H__
#define __APP_DATA_H__

#include <stdint.h>

typedef struct
{
    /* 扭矩 */
    int32_t target_torque;
    int32_t actual_torque;

    /* 角度 (预留) */
    int32_t target_angle;
    int32_t actual_angle;

    /* 增益 (预留) */
    int32_t gain;

    /* 状态 */
    uint8_t wifi_connected;
    int16_t temperature;

    /* 运行状态 */
    uint8_t motor_running;

    /* 语言: 0=English, 1=中文 */
    uint8_t language;

} app_data_t;

extern app_data_t app_data;

#endif
