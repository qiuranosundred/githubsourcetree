#include "device_table.h"

static device_t *table[DEVICE_TABLE_MAX];
static int       table_count = 0;

void device_register(device_t *dev)
{
    if(table_count < DEVICE_TABLE_MAX)
    {
        table[table_count++] = dev;
    }
}

int device_table_init(void)
{
    /* sort by priority ascending (lower number = higher priority, init first) */
    for(int i = 0; i < table_count - 1; i++)
    {
        for(int j = 0; j < table_count - 1 - i; j++)
        {
            if(table[j]->priority > table[j + 1]->priority)
            {
                device_t *tmp = table[j];
                table[j]      = table[j + 1];
                table[j + 1]  = tmp;
            }
        }
    }

    /* init in priority order */
    for(int i = 0; i < table_count; i++)
    {
        if(table[i]->init)
        {
            int ret = table[i]->init();
            if(ret == 0)
            {
                table[i]->initialized = 1;
            }
        }
    }

    return 0;
}
