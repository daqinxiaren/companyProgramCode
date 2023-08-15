/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-06-19     thread-liu   the first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>
#include <stdlib.h>

#ifdef ENABLE_DAC

#define DBG_TAG "dac"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static rt_size_t _dac_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    rt_err_t result = OS_EOK;
    rt_size_t i;
    struct rt_dac_device *dac = (struct rt_dac_device *)dev;
    rt_uint32_t *value = (rt_uint32_t *)buffer;

    for (i = 0; i < size; i += sizeof(int))
    {
        result = dac->ops->convert(dac, pos + i, value);
        if (result != OS_EOK)
        {
            return 0;
        }
        value++;
    }

    return i;
}

static rt_err_t _dac_control(rt_device_t dev, int cmd, void *args)
{
    rt_err_t result = OS_EOK;
    rt_dac_device_t dac = (struct rt_dac_device *)dev;

    if (dac->ops->enabled == OS_NULL)
    {
        return -OS_ENOSYS;
    }
    if (cmd == RT_DAC_CMD_ENABLE)
    {
        result = dac->ops->enabled(dac, (rt_uint32_t)args);
    }
    else if (cmd == RT_DAC_CMD_DISABLE)
    {
        result = dac->ops->disabled(dac, (rt_uint32_t)args);
    }

    return result;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops dac_ops =
{
    OS_NULL,
    OS_NULL,
    OS_NULL,
    OS_NULL,
    _dac_write,
    _dac_control,
};
#endif

rt_err_t rt_hw_dac_register(rt_dac_device_t device, const char *name, const struct rt_dac_ops *ops, const void *user_data)
{
    rt_err_t result = OS_EOK;
    OS_ASSERT(ops != OS_NULL && ops->convert != OS_NULL);

    device->parent.type = DEVICE_CLASS_MISCELLANEOUS;
    device->parent.rx_indicate = OS_NULL;
    device->parent.tx_complete = OS_NULL;

#ifdef ENABLE_DEVICE_OPS
    device->parent.ops         = &dac_ops;
#else
    device->parent.init        = OS_NULL;
    device->parent.open        = OS_NULL;
    device->parent.close       = OS_NULL;
    device->parent.read        = OS_NULL;
    device->parent.write       = _dac_write;
    device->parent.control     = _dac_control;
#endif
    device->ops = ops;
    device->parent.user_data = (void *)user_data;

    result = rt_device_register(&device->parent, name, DEVICE_FLAG_RDWR);

    return result;
}

rt_uint32_t rt_dac_write(rt_dac_device_t dev, rt_uint32_t channel, rt_uint32_t value)
{
    OS_ASSERT(dev);

    dev->ops->convert(dev, channel, &value);

    return OS_EOK;
}

rt_err_t rt_dac_enable(rt_dac_device_t dev, rt_uint32_t channel)
{
    rt_err_t result = OS_EOK;

    OS_ASSERT(dev);
    if (dev->ops->enabled != OS_NULL)
    {
        result = dev->ops->enabled(dev, channel);
    }
    else
    {
        result = -OS_ENOSYS;
    }

    return result;
}

rt_err_t rt_dac_disabled(rt_dac_device_t dev, rt_uint32_t channel)
{
    rt_err_t result = OS_EOK;

    OS_ASSERT(dev);
    if (dev->ops->disabled != OS_NULL)
    {
        result = dev->ops->disabled(dev, channel);
    }
    else
    {
        result = -OS_ENOSYS;
    }

    return result;
}

#ifdef FINSH_ENABLE_MSH

static int dac(int argc, char **argv)
{
    int result = OS_EOK;
    static rt_dac_device_t dac_device = OS_NULL;
    char *result_str;
    
    if (argc > 1)
    {
        if (!strcmp(argv[1], "probe"))
        {
            if (argc == 3)
            {
                dac_device = (rt_dac_device_t)rt_device_find(argv[2]);
                result_str = (dac_device == OS_NULL) ? "failure" : "success";
                rt_kprintf("probe %s %s \n", argv[2], result_str);
            }
            else
            {
                rt_kprintf("dac probe <dac_name>   - probe dac by name\n");
            }
        }
        else
        {
            if (dac_device == OS_NULL)
            {
                rt_kprintf("Please using 'dac probe <dac_name>' first\n");
                return -OS_ERROR;
            }
            if (!strcmp(argv[1], "enable"))
            {
                if (argc == 3)
                {
                    result = rt_dac_enable(dac_device, atoi(argv[2]));
                    result_str = (result == OS_EOK) ? "success" : "failure";
                    rt_kprintf("%s channel %d enables %s \n", dac_device->parent.parent.name, atoi(argv[2]), result_str);
                }
                else
                {
                    rt_kprintf("dac enable <channel>   - enable dac channel\n");
                }
            }
            else if (!strcmp(argv[1], "write"))
            {
                if (argc == 4)
                {
                    rt_dac_write(dac_device, atoi(argv[2]), atoi(argv[3]));
                    rt_kprintf("%s channel %d write value is %d \n", dac_device->parent.parent.name, atoi(argv[2]), atoi(argv[3]));
                }
                else
                {
                    rt_kprintf("dac write <channel> <value>     - write dac value on the channel\n");
                }
            }
            else if (!strcmp(argv[1], "disable"))
            {
                if (argc == 3)
                {
                    result = rt_dac_disabled(dac_device, atoi(argv[2]));
                    result_str = (result == OS_EOK) ? "success" : "failure";
                    rt_kprintf("%s channel %d disable %s \n", dac_device->parent.parent.name, atoi(argv[2]), result_str);
                }
                else
                {
                    rt_kprintf("dac disable <channel>  - disable dac channel\n");
                }
            }
            else
            {
                rt_kprintf("Unknown command. Please enter 'dac' for help\n");
            }
        }
    }
    else
    {
        rt_kprintf("Usage: \n");
        rt_kprintf("dac probe <dac_name>           - probe dac by name\n");
        rt_kprintf("dac write <channel> <value>    - write dac value on the channel\n");
        rt_kprintf("dac disable <channel>          - disable dac channel\n");
        rt_kprintf("dac enable <channel>           - enable dac channel\n");
        result = -OS_ERROR;
    }
    return OS_EOK;
}
MSH_CMD_EXPORT(dac, dac function);

#endif

#endif /* FINSH_ENABLE_MSH */
