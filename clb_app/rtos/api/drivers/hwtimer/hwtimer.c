/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author         Notes
 * 2015-08-31     heyuanjie87    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#ifdef ENABLE_HWTIMER

rt_inline rt_uint32_t timeout_calc(rt_hwtimer_t *timer, rt_hwtimerval_t *tv)
{
    float overflow;
    float timeout;
    rt_uint32_t counter;
    int i, index = 0;
    float tv_sec;
    float devi_min = 1;
    float devi;

    /* changed to second */
    overflow = timer->info->maxcnt/(float)timer->freq;
    tv_sec = tv->sec + tv->usec/(float)1000000;

    if (tv_sec < (1/(float)timer->freq))
    {
        /* little timeout */
        i = 0;
        timeout = 1/(float)timer->freq;
    }
    else
    {
        for (i = 1; i > 0; i ++)
        {
            timeout = tv_sec/i;

            if (timeout <= overflow)
            {
                counter = timeout*timer->freq;
                devi = tv_sec - (counter/(float)timer->freq)*i;
                /* Minimum calculation error */
                if (devi > devi_min)
                {
                    i = index;
                    timeout = tv_sec/i;
                    break;
                }
                else if (devi == 0)
                {
                    break;
                }
                else if (devi < devi_min)
                {
                    devi_min = devi;
                    index = i;
                }
            }
        }
    }

    timer->cycles = i;
    timer->reload = i;
    timer->period_sec = timeout;
    counter = timeout*timer->freq;

    return counter;
}

static rt_err_t rt_hwtimer_init(struct rt_device *dev)
{
    rt_err_t result = OS_EOK;
    rt_hwtimer_t *timer;

    timer = (rt_hwtimer_t *)dev;
    /* try to change to 1MHz */
    if ((1000000 <= timer->info->maxfreq) && (1000000 >= timer->info->minfreq))
    {
        timer->freq = 1000000;
    }
    else
    {
        timer->freq = timer->info->minfreq;
    }
    timer->mode = HWTIMER_MODE_ONESHOT;
    timer->cycles = 0;
    timer->overflow = 0;

    if (timer->ops->init)
    {
        timer->ops->init(timer, 1);
    }
    else
    {
        result = -OS_ENOSYS;
    }

    return result;
}

static rt_err_t rt_hwtimer_open(struct rt_device *dev, rt_uint16_t oflag)
{
    rt_err_t result = OS_EOK;
    rt_hwtimer_t *timer;

    timer = (rt_hwtimer_t *)dev;
    if (timer->ops->control != OS_NULL)
    {
        timer->ops->control(timer, HWTIMER_CTRL_FREQ_SET, &timer->freq);
    }
    else
    {
        result = -OS_ENOSYS;
    }

    return result;
}

static rt_err_t rt_hwtimer_close(struct rt_device *dev)
{
    rt_err_t result = OS_EOK;
    rt_hwtimer_t *timer;

    timer = (rt_hwtimer_t*)dev;
    if (timer->ops->init != OS_NULL)
    {
        timer->ops->init(timer, 0);
    }
    else
    {
        result = -OS_ENOSYS;
    }

    dev->flag &= ~DEVICE_FLAG_ACTIVATED;
    dev->rx_indicate = OS_NULL;

    return result;
}

static rt_size_t rt_hwtimer_read(struct rt_device *dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_hwtimer_t *timer;
    rt_hwtimerval_t tv;
    rt_uint32_t cnt;
    rt_base_t level;
    rt_int32_t overflow;
    float t;

    timer = (rt_hwtimer_t *)dev;
    if (timer->ops->count_get == OS_NULL)
        return 0;

    level = rt_hw_interrupt_disable();
    cnt = timer->ops->count_get(timer);
    overflow = timer->overflow;
    rt_hw_interrupt_enable(level);

    if (timer->info->cntmode == HWTIMER_CNTMODE_DW)
    {
        cnt = (timer->freq * timer->period_sec) - cnt;
    }

    t = overflow * timer->period_sec + cnt/(float)timer->freq;
    tv.sec = t;
    tv.usec = (t - tv.sec) * 1000000;
    size = size > sizeof(tv)? sizeof(tv) : size;
    rt_memcpy(buffer, &tv, size);

    return size;
}

static rt_size_t rt_hwtimer_write(struct rt_device *dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    rt_uint32_t t;
    rt_hwtimer_mode_t opm = HWTIMER_MODE_PERIOD;
    rt_hwtimer_t *timer;

    timer = (rt_hwtimer_t *)dev;
    if ((timer->ops->start == OS_NULL) || (timer->ops->stop == OS_NULL))
        return 0;

    if (size != sizeof(rt_hwtimerval_t))
        return 0;

    timer->ops->stop(timer);
    timer->overflow = 0;

    t = timeout_calc(timer, (rt_hwtimerval_t*)buffer);
    if ((timer->cycles <= 1) && (timer->mode == HWTIMER_MODE_ONESHOT))
    {
        opm = HWTIMER_MODE_ONESHOT;
    }

    if (timer->ops->start(timer, t, opm) != OS_EOK)
        size = 0;

    return size;
}

static rt_err_t rt_hwtimer_control(struct rt_device *dev, int cmd, void *args)
{
    rt_err_t result = OS_EOK;
    rt_hwtimer_t *timer;

    timer = (rt_hwtimer_t *)dev;

    switch (cmd)
    {
    case HWTIMER_CTRL_STOP:
    {
        if (timer->ops->stop != OS_NULL)
        {
            timer->ops->stop(timer);
        }
        else
        {
            result = -OS_ENOSYS;
        }
    }
    break;
    case HWTIMER_CTRL_FREQ_SET:
    {
        rt_uint32_t *f;

        if (args == OS_NULL)
        {
            result = -OS_EEMPTY;
            break;
        }

        f = (rt_uint32_t*)args;
        if ((*f > timer->info->maxfreq) || (*f < timer->info->minfreq))
        {
            result = -OS_ERROR;
            break;
        }

        if (timer->ops->control != OS_NULL)
        {
            result = timer->ops->control(timer, cmd, args);
            if (result == OS_EOK)
            {
                timer->freq = *f;
            }
        }
        else
        {
            result = -OS_ENOSYS;
        }
    }
    break;
    case HWTIMER_CTRL_INFO_GET:
    {
        if (args == OS_NULL)
        {
            result = -OS_EEMPTY;
            break;
        }

        *((struct rt_hwtimer_info*)args) = *timer->info;
    }
    break;
    case HWTIMER_CTRL_MODE_SET:
    {
        rt_hwtimer_mode_t *m;

        if (args == OS_NULL)
        {
            result = -OS_EEMPTY;
            break;
        }

        m = (rt_hwtimer_mode_t*)args;

        if ((*m != HWTIMER_MODE_ONESHOT) && (*m != HWTIMER_MODE_PERIOD))
        {
            result = -OS_ERROR;
            break;
        }

        timer->mode = *m;
    }
    break;
    default:
    {
        result = -OS_ENOSYS;
    }
    break;
    }

    return result;
}

void rt_device_hwtimer_isr(rt_hwtimer_t *timer)
{
    OS_ASSERT(timer != OS_NULL);

    timer->overflow ++;

    if (timer->cycles != 0)
    {
        timer->cycles --;
    }

    if (timer->cycles == 0)
    {
        timer->cycles = timer->reload;

        if (timer->mode == HWTIMER_MODE_ONESHOT)
        {
            if (timer->ops->stop != OS_NULL)
            {
                timer->ops->stop(timer);
            }
        }

        if (timer->parent.rx_indicate != OS_NULL)
        {
            timer->parent.rx_indicate(&timer->parent, sizeof(struct rt_hwtimerval));
        }
    }
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops hwtimer_ops = 
{
    rt_hwtimer_init,
    rt_hwtimer_open,
    rt_hwtimer_close,
    rt_hwtimer_read,
    rt_hwtimer_write,
    rt_hwtimer_control
};
#endif

rt_err_t rt_device_hwtimer_register(rt_hwtimer_t *timer, const char *name, void *user_data)
{
    struct rt_device *device;

    OS_ASSERT(timer != OS_NULL);
    OS_ASSERT(timer->ops != OS_NULL);
    OS_ASSERT(timer->info != OS_NULL);

    device = &(timer->parent);

    device->type        = DEVICE_CLASS_TIMER;
    device->rx_indicate = OS_NULL;
    device->tx_complete = OS_NULL;

#ifdef ENABLE_DEVICE_OPS
    device->ops         = &hwtimer_ops;
#else
    device->init        = rt_hwtimer_init;
    device->open        = rt_hwtimer_open;
    device->close       = rt_hwtimer_close;
    device->read        = rt_hwtimer_read;
    device->write       = rt_hwtimer_write;
    device->control     = rt_hwtimer_control;
#endif
    device->user_data   = user_data;

    return rt_device_register(device, name, DEVICE_FLAG_RDWR | DEVICE_FLAG_STANDALONE);
}

#endif /* ENABLE_HWTIMER */
