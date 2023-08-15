/*
 * COPYRIGHT (C) 2018, Real-Thread Information Technology Ltd
 * 
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-09-12     heyuanjie87  first version.
 * 2014-03-04     Bernard      code cleanup
 */

#include <drivers/watchdog.h>

/* RT-Thread Device Interface */

/*
 * This function initializes watchdog
 */
static rt_err_t rt_watchdog_init(struct rt_device *dev)
{
    rt_watchdog_t *wtd;

    OS_ASSERT(dev != OS_NULL);
    wtd = (rt_watchdog_t *)dev;
    if (wtd->ops->init)
    {
        return (wtd->ops->init(wtd));
    }

    return (-OS_ENOSYS);
}

static rt_err_t rt_watchdog_open(struct rt_device *dev, rt_uint16_t oflag)
{
    return (OS_EOK);
}

static rt_err_t rt_watchdog_close(struct rt_device *dev)
{
    rt_watchdog_t *wtd;

    OS_ASSERT(dev != OS_NULL);
    wtd = (rt_watchdog_t *)dev;

    if (wtd->ops->control(wtd, DEVICE_CTRL_WDT_STOP, OS_NULL) != OS_EOK)
    {
        rt_kprintf(" This watchdog can not be stoped\n");

        return (-OS_ERROR);
    }

    return (OS_EOK);
}

static rt_err_t rt_watchdog_control(struct rt_device *dev,
                                    int              cmd,
                                    void             *args)
{
    rt_watchdog_t *wtd;

    OS_ASSERT(dev != OS_NULL);
    wtd = (rt_watchdog_t *)dev;

    return (wtd->ops->control(wtd, cmd, args));
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops wdt_ops = 
{
    rt_watchdog_init,
    rt_watchdog_open,
    rt_watchdog_close,
    OS_NULL,
    OS_NULL,
    rt_watchdog_control,
};
#endif

/**
 * This function register a watchdog device
 */
rt_err_t rt_hw_watchdog_register(struct rt_watchdog_device *wtd,
                                 const char                *name,
                                 rt_uint32_t                flag,
                                 void                      *data)
{
    struct rt_device *device;
    OS_ASSERT(wtd != OS_NULL);

    device = &(wtd->parent);

    device->type        = DEVICE_CLASS_MISCELLANEOUS;
    device->rx_indicate = OS_NULL;
    device->tx_complete = OS_NULL;

#ifdef ENABLE_DEVICE_OPS
    device->ops         = &wdt_ops;
#else
    device->init        = rt_watchdog_init;
    device->open        = rt_watchdog_open;
    device->close       = rt_watchdog_close;
    device->read        = OS_NULL;
    device->write       = OS_NULL;
    device->control     = rt_watchdog_control;
#endif
    device->user_data   = data;

    /* register a character device */
    return rt_device_register(device, name, flag);
}
