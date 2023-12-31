/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-07     aozima       the first version
 * 2018-11-16     Ernest Chen  add finsh command and update adc function
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>
#include <stdlib.h>

#ifdef ENABLE_ADC

#define DBG_TAG "adc"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static rt_size_t _adc_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_err_t result = OS_EOK;
    rt_size_t i;
    struct rt_adc_device *adc = (struct rt_adc_device *)dev;
    rt_uint32_t *value = (rt_uint32_t *)buffer;

    for (i = 0; i < size; i += sizeof(int))
    {
        result = adc->ops->convert(adc, pos + i, value);
        if (result != OS_EOK)
        {
            return 0;
        }
        value++;
    }

    return i;
}

static rt_err_t _adc_control(rt_device_t dev, int cmd, void *args)
{
    rt_err_t result = OS_EOK;
    rt_adc_device_t adc = (struct rt_adc_device *)dev;

    if (adc->ops->enabled == OS_NULL)
    {
        return -OS_ENOSYS;
    }
    if (cmd == DEV_ADC_CMD_ENABLE)
    {
        result = adc->ops->enabled(adc, (rt_uint32_t)args, OS_TRUE);
    }
    else if (cmd == DEV_ADC_CMD_DISABLE)
    {
        result = adc->ops->enabled(adc, (rt_uint32_t)args, OS_FALSE);
    }

    return result;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops adc_ops =
{
    OS_NULL,
    OS_NULL,
    OS_NULL,
    _adc_read,
    OS_NULL,
    _adc_control,
};
#endif

rt_err_t rt_hw_adc_register(rt_adc_device_t device, const char *name, const struct rt_adc_ops *ops, const void *user_data)
{
    rt_err_t result = OS_EOK;
    OS_ASSERT(ops != OS_NULL && ops->convert != OS_NULL);

    device->parent.type = DEVICE_CLASS_MISCELLANEOUS;
    device->parent.rx_indicate = OS_NULL;
    device->parent.tx_complete = OS_NULL;

#ifdef ENABLE_DEVICE_OPS
    device->parent.ops         = &adc_ops;
#else
    device->parent.init        = OS_NULL;
    device->parent.open        = OS_NULL;
    device->parent.close       = OS_NULL;
    device->parent.read        = _adc_read;
    device->parent.write       = OS_NULL;
    device->parent.control     = _adc_control;
#endif
    device->ops = ops;
    device->parent.user_data = (void *)user_data;

    result = rt_device_register(&device->parent, name, DEVICE_FLAG_RDWR);

    return result;
}

rt_uint32_t rt_adc_read(rt_adc_device_t dev, rt_uint32_t channel)
{
    rt_uint32_t value;

    OS_ASSERT(dev);

    dev->ops->convert(dev, channel, &value);

    return value;
}

rt_err_t rt_adc_enable(rt_adc_device_t dev, rt_uint32_t channel)
{
    rt_err_t result = OS_EOK;

    OS_ASSERT(dev);
    if (dev->ops->enabled != OS_NULL)
    {
        result = dev->ops->enabled(dev, channel, OS_TRUE);
    }
    else
    {
        result = -OS_ENOSYS;
    }

    return result;
}

rt_err_t rt_adc_disable(rt_adc_device_t dev, rt_uint32_t channel)
{
    rt_err_t result = OS_EOK;

    OS_ASSERT(dev);
    if (dev->ops->enabled != OS_NULL)
    {
        result = dev->ops->enabled(dev, channel, OS_FALSE);
    }
    else
    {
        result = -OS_ENOSYS;
    }

    return result;
}

#ifdef FINSH_ENABLE_MSH

static int adc(int argc, char **argv)
{
    int value = 0;
    int result = OS_EOK;
    static rt_adc_device_t adc_device = OS_NULL;
    char *result_str;

    if (argc > 1)
    {
        if (!strcmp(argv[1], "probe"))
        {
            if (argc == 3)
            {
                adc_device = (rt_adc_device_t)rt_device_find(argv[2]);
                result_str = (adc_device == OS_NULL) ? "failure" : "success";
                rt_kprintf("probe %s %s \n", argv[2], result_str);
            }
            else
            {
                rt_kprintf("adc probe <adc_name>   - probe adc by name\n");
            }
        }
        else
        {
            if (adc_device == OS_NULL)
            {
                rt_kprintf("Please using 'adc probe <adc_name>' first\n");
                return -OS_ERROR;
            }
            if (!strcmp(argv[1], "enable"))
            {
                if (argc == 3)
                {
                    result = rt_adc_enable(adc_device, atoi(argv[2]));
                    result_str = (result == OS_EOK) ? "success" : "failure";
                    rt_kprintf("%s channel %d enables %s \n", adc_device->parent.parent.name, atoi(argv[2]), result_str);
                }
                else
                {
                    rt_kprintf("adc enable <channel>   - enable adc channel\n");
                }
            }
            else if (!strcmp(argv[1], "read"))
            {
                if (argc == 3)
                {
                    value = rt_adc_read(adc_device, atoi(argv[2]));
                    rt_kprintf("%s channel %d  read value is 0x%08X \n", adc_device->parent.parent.name, atoi(argv[2]), value);
                }
                else
                {
                    rt_kprintf("adc read <channel>     - read adc value on the channel\n");
                }
            }
            else if (!strcmp(argv[1], "disable"))
            {
                if (argc == 3)
                {
                    result = rt_adc_disable(adc_device, atoi(argv[2]));
                    result_str = (result == OS_EOK) ? "success" : "failure";
                    rt_kprintf("%s channel %d disable %s \n", adc_device->parent.parent.name, atoi(argv[2]), result_str);
                }
                else
                {
                    rt_kprintf("adc disable <channel>  - disable adc channel\n");
                }
            }
            else
            {
                rt_kprintf("Unknown command. Please enter 'adc' for help\n");
            }
        }
    }
    else
    {
        rt_kprintf("Usage: \n");
        rt_kprintf("adc probe <adc_name>   - probe adc by name\n");
        rt_kprintf("adc read <channel>     - read adc value on the channel\n");
        rt_kprintf("adc disable <channel>  - disable adc channel\n");
        rt_kprintf("adc enable <channel>   - enable adc channel\n");
        result = -OS_ERROR;
    }
    return OS_EOK;
}
MSH_CMD_EXPORT(adc, adc function);

#endif

#endif /* FINSH_ENABLE_MSH */
