/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-13     balanceTWK   the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rtdbg.h>

#ifdef ENABLE_INPUT_CAPTURE

static rt_err_t rt_inputcapture_init(struct rt_device *dev)
{
    rt_err_t ret;
    struct rt_inputcapture_device *inputcapture;

    OS_ASSERT(dev != OS_NULL);

    ret = OS_EOK;
    inputcapture = (struct rt_inputcapture_device *)dev;
    inputcapture->watermark = INPUT_CAPTURE_RB_SIZE / 2;
    if (inputcapture->ops->init)
    {
        ret = inputcapture->ops->init(inputcapture);
    }

    return ret;
}

static rt_err_t rt_inputcapture_open(struct rt_device *dev, rt_uint16_t oflag)
{
    rt_err_t ret;
    struct rt_inputcapture_device *inputcapture;

    OS_ASSERT(dev != OS_NULL);

    ret = OS_EOK;
    inputcapture = (struct rt_inputcapture_device *)dev;
    if (inputcapture->ringbuff == OS_NULL)
    {
        inputcapture->ringbuff = rt_ringbuffer_create(sizeof(struct rt_inputcapture_data) * INPUT_CAPTURE_RB_SIZE);
    }
    if (inputcapture->ops->open)
    {
        ret = inputcapture->ops->open(inputcapture);
    }

    return ret;
}

static rt_err_t rt_inputcapture_close(struct rt_device *dev)
{
    rt_err_t ret;
    struct rt_inputcapture_device *inputcapture;

    OS_ASSERT(dev != OS_NULL);

    ret = -OS_ERROR;
    inputcapture = (struct rt_inputcapture_device *)dev;

    if (inputcapture->ops->close)
    {
        ret = inputcapture->ops->close(inputcapture);
    }

    if (ret != OS_EOK)
    {
        return ret;
    }

    if (inputcapture->ringbuff)
    {
        rt_ringbuffer_destroy(inputcapture->ringbuff);
        inputcapture->ringbuff = OS_NULL;
    }
    return ret;
}

static rt_size_t rt_inputcapture_read(struct rt_device *dev,
                                 rt_off_t          pos,
                                 void             *buffer,
                                 rt_size_t         size)
{
    rt_size_t receive_size;
    struct rt_inputcapture_device *inputcapture;

    OS_ASSERT(dev != OS_NULL);

    inputcapture = (struct rt_inputcapture_device *)dev;
    receive_size = rt_ringbuffer_get(inputcapture->ringbuff, (rt_uint8_t *)buffer, sizeof(struct rt_inputcapture_data) * size);

    return receive_size / sizeof(struct rt_inputcapture_data);
}

static rt_err_t rt_inputcapture_control(struct rt_device *dev, int cmd, void *args)
{
    rt_err_t result;
    struct rt_inputcapture_device *inputcapture;

    OS_ASSERT(dev != OS_NULL);

    result = OS_EOK;
    inputcapture = (struct rt_inputcapture_device *)dev;
    switch (cmd)
    {
    case INPUTCAPTURE_CMD_CLEAR_BUF:
        if (inputcapture->ringbuff)
        {
            rt_ringbuffer_reset(inputcapture->ringbuff);
        }
        break;
    case INPUTCAPTURE_CMD_SET_WATERMARK:
        inputcapture->watermark = *(rt_size_t *)args;
        break;
    default:
        result = -OS_ENOSYS;
        break;
    }

    return result;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops inputcapture_ops =
{
    rt_inputcapture_init,
    rt_inputcapture_open,
    rt_inputcapture_close,
    rt_inputcapture_read,
    OS_NULL,
    rt_inputcapture_control
};
#endif

rt_err_t rt_device_inputcapture_register(struct rt_inputcapture_device *inputcapture, const char *name, void *user_data)
{
    struct rt_device *device;

    OS_ASSERT(inputcapture != OS_NULL);
    OS_ASSERT(inputcapture->ops != OS_NULL);
    OS_ASSERT(inputcapture->ops->get_pulsewidth != OS_NULL);

    device = &(inputcapture->parent);

    device->type        = DEVICE_CLASS_MISCELLANEOUS;
    device->rx_indicate = OS_NULL;
    device->tx_complete = OS_NULL;
    inputcapture->ringbuff = OS_NULL;

#ifdef ENABLE_DEVICE_OPS
    device->ops         = &inputcapture_ops;
#else
    device->init        = rt_inputcapture_init;
    device->open        = rt_inputcapture_open;
    device->close       = rt_inputcapture_close;
    device->read        = rt_inputcapture_read;
    device->write       = OS_NULL;
    device->control     = rt_inputcapture_control;
#endif
    device->user_data   = user_data;

    return rt_device_register(device, name, DEVICE_FLAG_RDONLY | DEVICE_FLAG_STANDALONE);
}

/**
 * This function is ISR for inputcapture interrupt.
 * level: OS_TRUE denotes high level pulse, and OS_FALSE denotes low level pulse.
 */
void rt_hw_inputcapture_isr(struct rt_inputcapture_device *inputcapture, rt_bool_t level)
{
    struct rt_inputcapture_data data;
    rt_size_t receive_size;
    if (inputcapture->ops->get_pulsewidth(inputcapture, &data.pulsewidth_us) != OS_EOK)
    {
        return;
    }

    data.is_high = level;
    if (rt_ringbuffer_put(inputcapture->ringbuff, (rt_uint8_t *)&data, sizeof(struct rt_inputcapture_data)) == 0)
    {
        LOG_W("inputcapture ringbuffer doesn't have enough space.");
    }

    receive_size =  rt_ringbuffer_data_len(inputcapture->ringbuff) / sizeof(struct rt_inputcapture_data);

    if (receive_size >= inputcapture->watermark)
    {
        /* indicate to upper layer application */
        if (inputcapture->parent.rx_indicate != OS_NULL)
            inputcapture->parent.rx_indicate(&inputcapture->parent, receive_size);
    }
}

#endif
