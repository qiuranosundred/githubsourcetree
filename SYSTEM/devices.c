/*
 * 外设注册表
 * 添加新设备: 定义一个 device_t, 在 devices_register_all() 里注册即可
 */

#include "device.h"
#include "device_table.h"

/* ==================== BSP 设备 init wrapper ==================== */

/* ---------- W25Qxx SPI Flash ---------- */
#include "W25Qxx.h"

static int flash_init(void)
{
    W25QXX_Init();
    uint16_t id = W25QXX_ReadID();
    return (id != 0 && id != 0xFFFF) ? 0 : -1;
}

static device_t dev_flash =
{
    .name     = "W25Qxx Flash",
    .priority = DEV_PRIO_HIGH,
    .init     = flash_init,
    .deinit   = NULL,
};

/* ---------- FM24CL64 I2C FRAM ---------- */
/* software I2C, no explicit init needed — I2C pins already configured by CubeMX */

static int fram_init(void)
{
    return 0;
}

static device_t dev_fram =
{
    .name     = "FM24CL64 FRAM",
    .priority = DEV_PRIO_NORMAL,
    .init     = fram_init,
    .deinit   = NULL,
};

/* ---------- 预留: WiFi 模块 (UART) ---------- */
static device_t dev_wifi =
{
    .name     = "WiFi Module",
    .priority = DEV_PRIO_LOW,
    .init     = NULL,  /* TODO */
    .deinit   = NULL,
};

/* ---------- 预留: 扫码枪 (UART) ---------- */
static device_t dev_scanner =
{
    .name     = "Scanner",
    .priority = DEV_PRIO_LOW,
    .init     = NULL,  /* TODO */
    .deinit   = NULL,
};

/* ==================== 注册入口 ==================== */

void devices_register_all(void)
{
    device_register(&dev_flash);
    device_register(&dev_fram);

    /* 将来添加新设备在这里注册:
    device_register(&dev_wifi);
    device_register(&dev_scanner);
    */
}
