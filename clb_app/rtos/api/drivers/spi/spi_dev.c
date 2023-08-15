/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rtthread.h>
#include <drivers/spi.h>

/* SPI bus device interface, compatible with RT-Thread 0.3.x/1.0.x */
static rt_size_t _spi_bus_device_read(rt_device_t dev,
                                      rt_off_t    pos,
                                      void       *buffer,
                                      rt_size_t   size)
{
    struct rt_spi_bus *bus;

    bus = (struct rt_spi_bus *)dev;
    OS_ASSERT(bus != OS_NULL);
    OS_ASSERT(bus->owner != OS_NULL);

    return rt_spi_transfer(bus->owner, OS_NULL, buffer, size);
}

static rt_size_t _spi_bus_device_write(rt_device_t dev,
                                       rt_off_t    pos,
                                       const void *buffer,
                                       rt_size_t   size)
{
    struct rt_spi_bus *bus;

    bus = (struct rt_spi_bus *)dev;
    OS_ASSERT(bus != OS_NULL);
    OS_ASSERT(bus->owner != OS_NULL);

    return rt_spi_transfer(bus->owner, buffer, OS_NULL, size);
}

static rt_err_t _spi_bus_device_control(rt_device_t dev,
                                        int         cmd,
                                        void       *args)
{
    /* TODO: add control command handle */
    switch (cmd)
    {
    case 0: /* set device */
        break;
    case 1: 
        break;
    }

    return OS_EOK;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops spi_bus_ops = 
{
    OS_NULL,
    OS_NULL,
    OS_NULL,
    _spi_bus_device_read,
    _spi_bus_device_write,
    _spi_bus_device_control
};
#endif

rt_err_t rt_spi_bus_device_init(struct rt_spi_bus *bus, const char *name)
{
    struct rt_device *device;
    OS_ASSERT(bus != OS_NULL);

    device = &bus->parent;

    /* set device type */
    device->type    = DEVICE_CLASS_SPI_BUS;
    /* initialize device interface */
#ifdef ENABLE_DEVICE_OPS
    device->ops     = &spi_bus_ops;
#else
    device->init    = OS_NULL;
    device->open    = OS_NULL;
    device->close   = OS_NULL;
    device->read    = _spi_bus_device_read;
    device->write   = _spi_bus_device_write;
    device->control = _spi_bus_device_control;
#endif

    /* register to device manager */
    return rt_device_register(device, name, DEVICE_FLAG_RDWR);
}

/* SPI Dev device interface, compatible with RT-Thread 0.3.x/1.0.x */
static rt_size_t _spidev_device_read(rt_device_t dev,
                                     rt_off_t    pos,
                                     void       *buffer,
                                     rt_size_t   size)
{
    struct rt_spi_device *device;

    device = (struct rt_spi_device *)dev;
    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(device->bus != OS_NULL);

    return rt_spi_transfer(device, OS_NULL, buffer, size);
}

static rt_size_t _spidev_device_write(rt_device_t dev,
                                      rt_off_t    pos,
                                      const void *buffer,
                                      rt_size_t   size)
{
    struct rt_spi_device *device;

    device = (struct rt_spi_device *)dev;
    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(device->bus != OS_NULL);

    return rt_spi_transfer(device, buffer, OS_NULL, size);
}

static rt_err_t _spidev_device_control(rt_device_t dev,
                                       int         cmd,
                                       void       *args)
{
    switch (cmd)
    {
    case 0: /* set device */
        break;
    case 1: 
        break;
    }

    return OS_EOK;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops spi_device_ops = 
{
    OS_NULL,
    OS_NULL,
    OS_NULL,
    _spidev_device_read,
    _spidev_device_write,
    _spidev_device_control
};
#endif

rt_err_t rt_spidev_device_init(struct rt_spi_device *dev, const char *name)
{
    struct rt_device *device;
    OS_ASSERT(dev != OS_NULL);

    device = &(dev->parent);

    /* set device type */
    device->type    = DEVICE_CLASS_SPI_DEVICE;
#ifdef ENABLE_DEVICE_OPS
    device->ops     = &spi_device_ops;
#else
    device->init    = OS_NULL;
    device->open    = OS_NULL;
    device->close   = OS_NULL;
    device->read    = _spidev_device_read;
    device->write   = _spidev_device_write;
    device->control = _spidev_device_control;
#endif

    /* register to device manager */
    return rt_device_register(device, name, DEVICE_FLAG_RDWR);
}
