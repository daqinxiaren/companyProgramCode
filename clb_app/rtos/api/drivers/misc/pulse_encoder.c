/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-08     balanceTWK   the first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#ifdef ENABLE_HWCRYPTO

static rt_err_t rt_pulse_encoder_init(struct rt_device *dev)
{
    struct rt_pulse_encoder_device *pulse_encoder;

    pulse_encoder = (struct rt_pulse_encoder_device *)dev;
    if (pulse_encoder->ops->init)
    {
        return pulse_encoder->ops->init(pulse_encoder);
    }
    else
    {
        return -OS_ENOSYS;
    }
}

static rt_err_t rt_pulse_encoder_open(struct rt_device *dev, rt_uint16_t oflag)
{
    struct rt_pulse_encoder_device *pulse_encoder;

    pulse_encoder = (struct rt_pulse_encoder_device *)dev;
    if (pulse_encoder->ops->control)
    {
        return pulse_encoder->ops->control(pulse_encoder, PULSE_ENCODER_CMD_ENABLE, OS_NULL);
    }
    else
    {
        return -OS_ENOSYS;
    }
}

static rt_err_t rt_pulse_encoder_close(struct rt_device *dev)
{
    struct rt_pulse_encoder_device *pulse_encoder;

    pulse_encoder = (struct rt_pulse_encoder_device *)dev;
    if (pulse_encoder->ops->control)
    {
        return pulse_encoder->ops->control(pulse_encoder, PULSE_ENCODER_CMD_DISABLE, OS_NULL);
    }
    else
    {
        return -OS_ENOSYS;
    }
}

static rt_size_t rt_pulse_encoder_read(struct rt_device *dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    struct rt_pulse_encoder_device *pulse_encoder;

    pulse_encoder = (struct rt_pulse_encoder_device *)dev;
    if (pulse_encoder->ops->get_count)
    {
        *(rt_int32_t *)buffer = pulse_encoder->ops->get_count(pulse_encoder);
    }
    return 1;
}

static rt_err_t rt_pulse_encoder_control(struct rt_device *dev, int cmd, void *args)
{
    rt_err_t result;
    struct rt_pulse_encoder_device *pulse_encoder;

    result = OS_EOK;
    pulse_encoder = (struct rt_pulse_encoder_device *)dev;
    switch (cmd)
    {
    case PULSE_ENCODER_CMD_CLEAR_COUNT:
        result = pulse_encoder->ops->clear_count(pulse_encoder);
        break;
    case PULSE_ENCODER_CMD_GET_TYPE:
        *(enum rt_pulse_encoder_type *)args = pulse_encoder->type;
        break;
    case PULSE_ENCODER_CMD_ENABLE:
    case PULSE_ENCODER_CMD_DISABLE:
        result = pulse_encoder->ops->control(pulse_encoder, cmd, args);
        break;
    default:
        result = -OS_ENOSYS;
        break;
    }

    return result;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops pulse_encoder_ops =
{
    rt_pulse_encoder_init,
    rt_pulse_encoder_open,
    rt_pulse_encoder_close,
    rt_pulse_encoder_read,
    OS_NULL,
    rt_pulse_encoder_control
};
#endif

rt_err_t rt_device_pulse_encoder_register(struct rt_pulse_encoder_device *pulse_encoder, const char *name, void *user_data)
{
    struct rt_device *device;

    OS_ASSERT(pulse_encoder != OS_NULL);
    OS_ASSERT(pulse_encoder->ops != OS_NULL);

    device = &(pulse_encoder->parent);

    device->type        = DEVICE_CLASS_MISCELLANEOUS;
    device->rx_indicate = OS_NULL;
    device->tx_complete = OS_NULL;

#ifdef ENABLE_DEVICE_OPS
    device->ops         = &pulse_encoder_ops;
#else
    device->init        = rt_pulse_encoder_init;
    device->open        = rt_pulse_encoder_open;
    device->close       = rt_pulse_encoder_close;
    device->read        = rt_pulse_encoder_read;
    device->write       = OS_NULL;
    device->control     = rt_pulse_encoder_control;
#endif
    device->user_data   = user_data;

    return rt_device_register(device, name, DEVICE_FLAG_RDONLY | DEVICE_FLAG_STANDALONE);
}

#endif

