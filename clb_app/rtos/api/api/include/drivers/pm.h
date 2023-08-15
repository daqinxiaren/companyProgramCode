/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-06-02     Bernard      the first version
 * 2018-08-02     Tanek        split run and sleep modes, support custom mode
 * 2019-04-28     Zero-Free    improve PM mode and device ops interface
 * 2020-11-23     zhangsz      update pm mode select
 * 2020-11-27     zhangsz      update pm 2.0
 */

#ifndef __PM_H__
#define __PM_H__

#include <rtthread.h>

#ifndef POWER_HAS_CUSTOM_CONFIG

/* All modes used for rt_pm_request() and rt_pm_release() */
enum
{
    /* sleep modes */
    POWER_SLEEP_MODE_NONE = 0,
    POWER_SLEEP_MODE_IDLE,
    POWER_SLEEP_MODE_LIGHT,
    POWER_SLEEP_MODE_DEEP,
    POWER_SLEEP_MODE_STANDBY,
    POWER_SLEEP_MODE_SHUTDOWN,
    POWER_SLEEP_MODE_MAX,
};

enum
{
    /* run modes*/
    POWER_RUN_MODE_HIGH_SPEED = 0,
    POWER_RUN_MODE_NORMAL_SPEED,
    POWER_RUN_MODE_MEDIUM_SPEED,
    POWER_RUN_MODE_LOW_SPEED,
    POWER_RUN_MODE_MAX,
};

enum
{
    POWER_FREQUENCY_PENDING = 0x01,
};

#define POWER_DEFAULT_SLEEP_MODE POWER_SLEEP_MODE_NONE
#define POWER_DEFAULT_DEEPSLEEP_MODE POWER_SLEEP_MODE_DEEP
#define POWER_DEFAULT_RUN_MODE   POWER_RUN_MODE_NORMAL_SPEED

/* The name of all modes used in the msh command "pm_dump" */
#define POWER_SLEEP_MODE_NAMES     \
{                               \
    "None Mode",                \
    "Idle Mode",                \
    "LightSleep Mode",          \
    "DeepSleep Mode",           \
    "Standby Mode",             \
    "Shutdown Mode",            \
}

#define POWER_RUN_MODE_NAMES       \
{                               \
    "High Speed",               \
    "Normal Speed",             \
    "Medium Speed",             \
    "Low Mode",                 \
}

/**
 * Modules used for
 * pm_module_request(POWER_BOARD_ID, POWER_SLEEP_MODE_IDLE)
 * pm_module_release(POWER_BOARD_ID, POWER_SLEEP_MODE_IDLE)
 * pm_module_release_all(POWER_BOARD_ID, POWER_SLEEP_MODE_IDLE)
 */
enum pm_module_id {
    POWER_NONE_ID = 0,
    POWER_POWER_ID,
    POWER_BOARD_ID,
    POWER_BSP_ID,
    POWER_MAIN_ID,
    POWER_PMS_ID,
    POWER_PMC_ID,
    POWER_TASK_ID,
    POWER_SPI_ID,
    POWER_I2C_ID,
    POWER_UART_ID,
    POWER_CAN_ID,
    POWER_ETH_ID,
    POWER_SENSOR_ID,
    POWER_LCD_ID,
    POWER_KEY_ID,
    POWER_TP_ID,
    POWER_MODULE_MAX_ID, /* enum must! */
};

#else /* POWER_HAS_CUSTOM_CONFIG */

#include <pm_cfg.h>

#endif /* POWER_HAS_CUSTOM_CONFIG */

/**
 * device control flag to request or release power
 */
#define POWER_DEVICE_CTRL_REQUEST   0x01
#define POWER_DEVICE_CTRL_RELEASE   0x00

struct rt_pm;

/**
 * low power mode operations
 */
struct rt_pm_ops
{
    void (*sleep)(struct rt_pm *pm, rt_uint8_t mode);
    void (*run)(struct rt_pm *pm, rt_uint8_t mode);
    void (*timer_start)(struct rt_pm *pm, rt_uint32_t timeout);
    void (*timer_stop)(struct rt_pm *pm);
    rt_tick_t (*timer_get_tick)(struct rt_pm *pm);
};

struct rt_device_pm_ops
{
    int (*suspend)(const struct rt_device *device, rt_uint8_t mode);
    void (*resume)(const struct rt_device *device, rt_uint8_t mode);
    int (*frequency_change)(const struct rt_device *device, rt_uint8_t mode);
};

struct rt_device_pm
{
    const struct rt_device *device;
    const struct rt_device_pm_ops *ops;
};

struct rt_pm_module
{
    rt_uint8_t req_status;
    rt_bool_t busy_flag;
    rt_uint32_t timeout;
    rt_uint32_t start_time;
};

/**
 * power management
 */
struct rt_pm
{
    struct rt_device parent;

    /* modes */
    rt_uint8_t modes[POWER_SLEEP_MODE_MAX];
    rt_uint8_t sleep_mode;    /* current sleep mode */
    rt_uint8_t run_mode;      /* current running mode */

    /* modules request status*/
    struct rt_pm_module module_status[POWER_MODULE_MAX_ID];

    /* the list of device, which has PM feature */
    rt_uint8_t device_pm_number;
    struct rt_device_pm *device_pm;

    /* if the mode has timer, the corresponding bit is 1*/
    rt_uint8_t timer_mask;
    rt_uint8_t flags;

    const struct rt_pm_ops *ops;
};

enum
{
    POWER_ENTER_SLEEP = 0,
    POWER_EXIT_SLEEP,
};

struct rt_pm_notify
{
    void (*notify)(rt_uint8_t event, rt_uint8_t mode, void *data);
    void *data;
};

void rt_pm_request(rt_uint8_t sleep_mode);
void rt_pm_release(rt_uint8_t sleep_mode);
void rt_pm_release_all(rt_uint8_t sleep_mode);
int rt_pm_run_enter(rt_uint8_t run_mode);

void rt_pm_device_register(struct rt_device *device, const struct rt_device_pm_ops *ops);
void rt_pm_device_unregister(struct rt_device *device);

void rt_pm_notify_set(void (*notify)(rt_uint8_t event, rt_uint8_t mode, void *data), void *data);
void rt_pm_default_set(rt_uint8_t sleep_mode);

void rt_system_pm_init(const struct rt_pm_ops *ops,
                       rt_uint8_t              timer_mask,
                       void                 *user_data);
void rt_pm_module_request(uint8_t module_id, rt_uint8_t sleep_mode);
void rt_pm_module_release(uint8_t module_id, rt_uint8_t sleep_mode);
void rt_pm_module_release_all(uint8_t module_id, rt_uint8_t sleep_mode);
void rt_pm_module_delay_sleep(rt_uint8_t module_id, rt_tick_t timeout);
rt_uint32_t rt_pm_module_get_status(void);
rt_uint8_t rt_pm_get_sleep_mode(void);

#endif /* __PM_H__ */
