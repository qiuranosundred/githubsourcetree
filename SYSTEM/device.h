#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stdint.h>

typedef enum
{
    DEV_PRIO_HIGHEST = 1,
    DEV_PRIO_HIGH    = 3,
    DEV_PRIO_NORMAL  = 5,
    DEV_PRIO_LOW     = 7,
    DEV_PRIO_LOWEST  = 9,
} device_priority_t;

typedef struct device
{
    const char       *name;
    device_priority_t priority;
    int             (*init)(void);
    int             (*deinit)(void);
    uint8_t           initialized;

    /* for future use: per-device private data */
    void             *priv;
} device_t;

#endif
