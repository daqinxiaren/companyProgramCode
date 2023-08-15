/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-01-20     Bernard      the first version
 */

#include <drivers/pin.h>

#ifdef ENABLE_FINSH
#include <finsh.h>
#endif

static struct rt_device_pin _hw_pin;
static rt_size_t _pin_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    struct rt_device_pin_status *status;
    struct rt_device_pin *pin = (struct rt_device_pin *)dev;

    /* check parameters */
    OS_ASSERT(pin != OS_NULL);

    status = (struct rt_device_pin_status *) buffer;
    if (status == OS_NULL || size != sizeof(*status)) return 0;

    status->status = pin->ops->pin_read(dev, status->pin);
    return size;
}

static rt_size_t _pin_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    struct rt_device_pin_status *status;
    struct rt_device_pin *pin = (struct rt_device_pin *)dev;

    /* check parameters */
    OS_ASSERT(pin != OS_NULL);

    status = (struct rt_device_pin_status *) buffer;
    if (status == OS_NULL || size != sizeof(*status)) return 0;

    pin->ops->pin_write(dev, (rt_base_t)status->pin, (rt_base_t)status->status);

    return size;
}

static rt_err_t  _pin_control(rt_device_t dev, int cmd, void *args)
{
    struct rt_device_pin_mode *mode;
    struct rt_device_pin *pin = (struct rt_device_pin *)dev;

    /* check parameters */
    OS_ASSERT(pin != OS_NULL);

    mode = (struct rt_device_pin_mode *) args;
    if (mode == OS_NULL) return -OS_ERROR;

    pin->ops->pin_mode(dev, (rt_base_t)mode->pin, (rt_base_t)mode->mode);

    return 0;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops pin_ops =
{
    OS_NULL,
    OS_NULL,
    OS_NULL,
    _pin_read,
    _pin_write,
    _pin_control
};
#endif

int rt_device_pin_register(const char *name, const struct rt_pin_ops *ops, void *user_data)
{
    _hw_pin.parent.type         = DEVICE_CLASS_MISCELLANEOUS;
    _hw_pin.parent.rx_indicate  = OS_NULL;
    _hw_pin.parent.tx_complete  = OS_NULL;

#ifdef ENABLE_DEVICE_OPS
    _hw_pin.parent.ops          = &pin_ops;
#else
    _hw_pin.parent.init         = OS_NULL;
    _hw_pin.parent.open         = OS_NULL;
    _hw_pin.parent.close        = OS_NULL;
    _hw_pin.parent.read         = _pin_read;
    _hw_pin.parent.write        = _pin_write;
    _hw_pin.parent.control      = _pin_control;
#endif

    _hw_pin.ops                 = ops;
    _hw_pin.parent.user_data    = user_data;

    /* register a character device */
    rt_device_register(&_hw_pin.parent, name, DEVICE_FLAG_RDWR);

    return 0;
}

rt_err_t rt_pin_attach_irq(rt_int32_t pin, rt_uint32_t mode,
                             void (*hdr)(void *args), void  *args)
{
    OS_ASSERT(_hw_pin.ops != OS_NULL);
    if(_hw_pin.ops->pin_attach_irq)
    {
        return _hw_pin.ops->pin_attach_irq(&_hw_pin.parent, pin, mode, hdr, args);
    }
    return OS_ENOSYS;
}
rt_err_t rt_pin_detach_irq(rt_int32_t pin)
{
    OS_ASSERT(_hw_pin.ops != OS_NULL);
    if(_hw_pin.ops->pin_detach_irq)
    {
        return _hw_pin.ops->pin_detach_irq(&_hw_pin.parent, pin);
    }
    return OS_ENOSYS;
}

rt_err_t rt_pin_irq_enable(rt_base_t pin, rt_uint32_t enabled)
{
    OS_ASSERT(_hw_pin.ops != OS_NULL);
    if(_hw_pin.ops->pin_irq_enable)
    {
        return _hw_pin.ops->pin_irq_enable(&_hw_pin.parent, pin, enabled);
    }
    return OS_ENOSYS;
}

/* RT-Thread Hardware PIN APIs */
void rt_pin_mode(rt_base_t pin, rt_base_t mode)
{
    OS_ASSERT(_hw_pin.ops != OS_NULL);
    _hw_pin.ops->pin_mode(&_hw_pin.parent, pin, mode);
}
FINSH_FUNCTION_EXPORT_ALIAS(rt_pin_mode, pinMode, set hardware pin mode);

void rt_pin_write(rt_base_t pin, rt_base_t value)
{
    OS_ASSERT(_hw_pin.ops != OS_NULL);
    _hw_pin.ops->pin_write(&_hw_pin.parent, pin, value);
}
FINSH_FUNCTION_EXPORT_ALIAS(rt_pin_write, pinWrite, write value to hardware pin);

int  rt_pin_read(rt_base_t pin)
{
    OS_ASSERT(_hw_pin.ops != OS_NULL);
    return _hw_pin.ops->pin_read(&_hw_pin.parent, pin);
}
FINSH_FUNCTION_EXPORT_ALIAS(rt_pin_read, pinRead, read status from hardware pin);

rt_base_t rt_pin_get(const char *name)
{
    OS_ASSERT(_hw_pin.ops != OS_NULL);
    OS_ASSERT(name[0] == 'P');
    
    if(_hw_pin.ops->pin_get == OS_NULL)
    {
        return -OS_ENOSYS;
    }

    return _hw_pin.ops->pin_get(name);
}
FINSH_FUNCTION_EXPORT_ALIAS(rt_pin_get, pinGet, get pin number from hardware pin);

