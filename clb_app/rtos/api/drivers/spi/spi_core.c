/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-01-08     bernard      first version.
 * 2012-02-03     bernard      add const attribute to the ops.
 * 2012-05-15     dzzxzz       fixed the return value in attach_device.
 * 2012-05-18     bernard      Changed SPI message to message list.
 *                             Added take/release SPI device/bus interface.
 * 2012-09-28     aozima       fixed rt_spi_release_bus assert error.
 */

#include <drivers/spi.h>

extern rt_err_t rt_spi_bus_device_init(struct rt_spi_bus *bus, const char *name);
extern rt_err_t rt_spidev_device_init(struct rt_spi_device *dev, const char *name);

rt_err_t rt_spi_bus_register(struct rt_spi_bus       *bus,
                             const char              *name,
                             const struct rt_spi_ops *ops)
{
    rt_err_t result;

    result = rt_spi_bus_device_init(bus, name);
    if (result != OS_EOK)
        return result;

    /* initialize mutex lock */
    rt_mutex_init(&(bus->lock), name, IPC_FLAG_FIFO);
    /* set ops */
    bus->ops = ops;
    /* initialize owner */
    bus->owner = OS_NULL;
    /* set bus mode */
    bus->mode = DEV_SPI_BUS_MODE_SPI;

    return OS_EOK;
}

rt_err_t rt_spi_bus_attach_device(struct rt_spi_device *device,
                                  const char           *name,
                                  const char           *bus_name,
                                  void                 *user_data)
{
    rt_err_t result;
    rt_device_t bus;

    /* get physical spi bus */
    bus = rt_device_find(bus_name);
    if (bus != OS_NULL && bus->type == DEVICE_CLASS_SPI_BUS)
    {
        device->bus = (struct rt_spi_bus *)bus;

        /* initialize spidev device */
        result = rt_spidev_device_init(device, name);
        if (result != OS_EOK)
            return result;

        rt_memset(&device->config, 0, sizeof(device->config));
        device->parent.user_data = user_data;

        return OS_EOK;
    }

    /* not found the host bus */
    return -OS_ERROR;
}

rt_err_t rt_spi_configure(struct rt_spi_device        *device,
                          struct rt_spi_configuration *cfg)
{
    rt_err_t result;

    OS_ASSERT(device != OS_NULL);

    /* set configuration */
    device->config.data_width = cfg->data_width;
    device->config.mode       = cfg->mode & DEV_SPI_MODE_MASK ;
    device->config.max_hz     = cfg->max_hz ;

    if (device->bus != OS_NULL)
    {
        result = rt_mutex_take(&(device->bus->lock), WAITING_FOREVER);
        if (result == OS_EOK)
        {
            if (device->bus->owner == device)
            {
                device->bus->ops->configure(device, &device->config);
            }

            /* release lock */
            rt_mutex_release(&(device->bus->lock));
        }
    }

    return OS_EOK;
}

rt_err_t rt_spi_send_then_send(struct rt_spi_device *device,
                               const void           *send_buf1,
                               rt_size_t             send_length1,
                               const void           *send_buf2,
                               rt_size_t             send_length2)
{
    rt_err_t result;
    struct rt_spi_message message;

    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(device->bus != OS_NULL);

    result = rt_mutex_take(&(device->bus->lock), WAITING_FOREVER);
    if (result == OS_EOK)
    {
        if (device->bus->owner != device)
        {
            /* not the same owner as current, re-configure SPI bus */
            result = device->bus->ops->configure(device, &device->config);
            if (result == OS_EOK)
            {
                /* set SPI bus owner */
                device->bus->owner = device;
            }
            else
            {
                /* configure SPI bus failed */
                result = -OS_EIO;
                goto __exit;
            }
        }

        /* send data1 */
        message.send_buf   = send_buf1;
        message.recv_buf   = OS_NULL;
        message.length     = send_length1;
        message.cs_take    = 1;
        message.cs_release = 0;
        message.next       = OS_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -OS_EIO;
            goto __exit;
        }

        /* send data2 */
        message.send_buf   = send_buf2;
        message.recv_buf   = OS_NULL;
        message.length     = send_length2;
        message.cs_take    = 0;
        message.cs_release = 1;
        message.next       = OS_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -OS_EIO;
            goto __exit;
        }

        result = OS_EOK;
    }
    else
    {
        return -OS_EIO;
    }

__exit:
    rt_mutex_release(&(device->bus->lock));

    return result;
}

rt_err_t rt_spi_send_then_recv(struct rt_spi_device *device,
                               const void           *send_buf,
                               rt_size_t             send_length,
                               void                 *recv_buf,
                               rt_size_t             recv_length)
{
    rt_err_t result;
    struct rt_spi_message message;

    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(device->bus != OS_NULL);

    result = rt_mutex_take(&(device->bus->lock), WAITING_FOREVER);
    if (result == OS_EOK)
    {
        if (device->bus->owner != device)
        {
            /* not the same owner as current, re-configure SPI bus */
            result = device->bus->ops->configure(device, &device->config);
            if (result == OS_EOK)
            {
                /* set SPI bus owner */
                device->bus->owner = device;
            }
            else
            {
                /* configure SPI bus failed */
                result = -OS_EIO;
                goto __exit;
            }
        }

        /* send data */
        message.send_buf   = send_buf;
        message.recv_buf   = OS_NULL;
        message.length     = send_length;
        message.cs_take    = 1;
        message.cs_release = 0;
        message.next       = OS_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -OS_EIO;
            goto __exit;
        }

        /* recv data */
        message.send_buf   = OS_NULL;
        message.recv_buf   = recv_buf;
        message.length     = recv_length;
        message.cs_take    = 0;
        message.cs_release = 1;
        message.next       = OS_NULL;

        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            result = -OS_EIO;
            goto __exit;
        }

        result = OS_EOK;
    }
    else
    {
        return -OS_EIO;
    }

__exit:
    rt_mutex_release(&(device->bus->lock));

    return result;
}

rt_size_t rt_spi_transfer(struct rt_spi_device *device,
                          const void           *send_buf,
                          void                 *recv_buf,
                          rt_size_t             length)
{
    rt_err_t result;
    struct rt_spi_message message;

    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(device->bus != OS_NULL);

    result = rt_mutex_take(&(device->bus->lock), WAITING_FOREVER);
    if (result == OS_EOK)
    {
        if (device->bus->owner != device)
        {
            /* not the same owner as current, re-configure SPI bus */
            result = device->bus->ops->configure(device, &device->config);
            if (result == OS_EOK)
            {
                /* set SPI bus owner */
                device->bus->owner = device;
            }
            else
            {
                /* configure SPI bus failed */
                rt_set_errno(-OS_EIO);
                result = 0;
                goto __exit;
            }
        }

        /* initial message */
        message.send_buf   = send_buf;
        message.recv_buf   = recv_buf;
        message.length     = length;
        message.cs_take    = 1;
        message.cs_release = 1;
        message.next       = OS_NULL;

        /* transfer message */
        result = device->bus->ops->xfer(device, &message);
        if (result == 0)
        {
            rt_set_errno(-OS_EIO);
            goto __exit;
        }
    }
    else
    {
        rt_set_errno(-OS_EIO);

        return 0;
    }

__exit:
    rt_mutex_release(&(device->bus->lock));

    return result;
}

struct rt_spi_message *rt_spi_transfer_message(struct rt_spi_device  *device,
                                               struct rt_spi_message *message)
{
    rt_err_t result;
    struct rt_spi_message *index;

    OS_ASSERT(device != OS_NULL);

    /* get first message */
    index = message;
    if (index == OS_NULL)
        return index;

    result = rt_mutex_take(&(device->bus->lock), WAITING_FOREVER);
    if (result != OS_EOK)
    {
        rt_set_errno(-OS_EBUSY);

        return index;
    }

    /* reset errno */
    rt_set_errno(OS_EOK);

    /* configure SPI bus */
    if (device->bus->owner != device)
    {
        /* not the same owner as current, re-configure SPI bus */
        result = device->bus->ops->configure(device, &device->config);
        if (result == OS_EOK)
        {
            /* set SPI bus owner */
            device->bus->owner = device;
        }
        else
        {
            /* configure SPI bus failed */
            rt_set_errno(-OS_EIO);
            goto __exit;
        }
    }

    /* transmit each SPI message */
    while (index != OS_NULL)
    {
        /* transmit SPI message */
        result = device->bus->ops->xfer(device, index);
        if (result == 0)
        {
            rt_set_errno(-OS_EIO);
            break;
        }

        index = index->next;
    }

__exit:
    /* release bus lock */
    rt_mutex_release(&(device->bus->lock));

    return index;
}

rt_err_t rt_spi_take_bus(struct rt_spi_device *device)
{
    rt_err_t result = OS_EOK;

    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(device->bus != OS_NULL);

    result = rt_mutex_take(&(device->bus->lock), WAITING_FOREVER);
    if (result != OS_EOK)
    {
        rt_set_errno(-OS_EBUSY);

        return -OS_EBUSY;
    }

    /* reset errno */
    rt_set_errno(OS_EOK);

    /* configure SPI bus */
    if (device->bus->owner != device)
    {
        /* not the same owner as current, re-configure SPI bus */
        result = device->bus->ops->configure(device, &device->config);
        if (result == OS_EOK)
        {
            /* set SPI bus owner */
            device->bus->owner = device;
        }
        else
        {
            /* configure SPI bus failed */
            rt_set_errno(-OS_EIO);
            /* release lock */
            rt_mutex_release(&(device->bus->lock));

            return -OS_EIO;
        }
    }

    return result;
}

rt_err_t rt_spi_release_bus(struct rt_spi_device *device)
{
    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(device->bus != OS_NULL);
    OS_ASSERT(device->bus->owner == device);

    /* release lock */
    rt_mutex_release(&(device->bus->lock));

    return OS_EOK;
}

rt_err_t rt_spi_take(struct rt_spi_device *device)
{
    rt_err_t result;
    struct rt_spi_message message;

    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(device->bus != OS_NULL);

    rt_memset(&message, 0, sizeof(message));
    message.cs_take = 1;

    result = device->bus->ops->xfer(device, &message);

    return result;
}

rt_err_t rt_spi_release(struct rt_spi_device *device)
{
    rt_err_t result;
    struct rt_spi_message message;

    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(device->bus != OS_NULL);

    rt_memset(&message, 0, sizeof(message));
    message.cs_release = 1;

    result = device->bus->ops->xfer(device, &message);

    return result;
}
