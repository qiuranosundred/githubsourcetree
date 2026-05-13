#include "app_data.h"

app_data_t app_data =
{
    .target_torque   = 100,
    .actual_torque   = 0,
    .target_angle    = 0,
    .actual_angle    = 0,
    .gain            = 50,
    .wifi_connected  = 0,
    .temperature     = 25,
    .motor_running   = 0,
    .language        = 0,
};
