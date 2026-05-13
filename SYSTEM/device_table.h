#ifndef __DEVICE_TABLE_H__
#define __DEVICE_TABLE_H__

#include "device.h"

#define DEVICE_TABLE_MAX  16

void device_register(device_t *dev);

void devices_register_all(void);

int  device_table_init(void);

#endif
