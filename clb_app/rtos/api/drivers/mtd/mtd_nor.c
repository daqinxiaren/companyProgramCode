/*
 * COPYRIGHT (C) 2018, Real-Thread Information Technology Ltd
 * 
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-5-30     Bernard      the first version
 */

#include <drivers/mtd_nor.h>

#ifdef ENABLE_MTD_NOR

/**
 * RT-Thread Generic Device Interface
 */
static rt_err_t _mtd_init(rt_device_t dev)
{
    return OS_EOK;
}

static rt_err_t _mtd_open(rt_device_t dev, rt_uint16_t oflag)
{
    return OS_EOK;
}

static rt_err_t _mtd_close(rt_device_t dev)
{
    return OS_EOK;
}

static rt_size_t _mtd_read(rt_device_t dev,
                           rt_off_t    pos,
                           void       *buffer,
                           rt_size_t   size)
{
    return size;
}

static rt_size_t _mtd_write(rt_device_t dev,
                            rt_off_t    pos,
                            const void *buffer,
                            rt_size_t   size)
{
    return size;
}

static rt_err_t _mtd_control(rt_device_t dev, int cmd, void *args)
{
    return OS_EOK;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops mtd_nor_ops = 
{
    _mtd_init,
    _mtd_open,
    _mtd_close,
    _mtd_read,
    _mtd_write,
    _mtd_control
};
#endif

rt_err_t rt_mtd_nor_register_device(const char               *name,
                                    struct rt_mtd_nor_device *device)
{
    rt_device_t dev;

    dev = RT_DEVICE(device);
    OS_ASSERT(dev != OS_NULL);

    /* set device class and generic device interface */
    dev->type        = DEVICE_CLASS_MTD;
#ifdef ENABLE_DEVICE_OPS
    dev->ops         = &mtd_nor_ops;
#else
    dev->init        = _mtd_init;
    dev->open        = _mtd_open;
    dev->read        = _mtd_read;
    dev->write       = _mtd_write;
    dev->close       = _mtd_close;
    dev->control     = _mtd_control;
#endif

    dev->rx_indicate = OS_NULL;
    dev->tx_complete = OS_NULL;

    /* register to RT-Thread device system */
    return rt_device_register(dev, name, DEVICE_FLAG_RDWR | DEVICE_FLAG_STANDALONE);
}

#endif
