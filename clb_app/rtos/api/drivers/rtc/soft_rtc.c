/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-30     armink       the first version
 */

#include <time.h>
#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>

#ifdef ENABLE_SOFT_RTC

/* 2018-01-30 14:44:50 = RTC_TIME_INIT(2018, 1, 30, 14, 44, 50)  */
#define RTC_TIME_INIT(year, month, day, hour, minute, second)        \
    {.tm_year = year - 1900, .tm_mon = month - 1, .tm_mday = day, .tm_hour = hour, .tm_min = minute, .tm_sec = second}

#ifndef SOFT_RTC_TIME_DEFAULT
#define SOFT_RTC_TIME_DEFAULT                    RTC_TIME_INIT(2018, 1, 1, 0, 0 ,0)
#endif

static struct rt_device soft_rtc_dev;
static rt_tick_t init_tick;
static time_t init_time;

#ifdef ENABLE_ALARM

static struct rt_rtc_wkalarm wkalarm;
static struct rt_timer alarm_time;

static void alarm_timeout(void *param)
{
    rt_alarm_update(param, 1);
}

static void soft_rtc_alarm_update(struct rt_rtc_wkalarm *palarm)
{
    rt_tick_t next_tick;

    if (palarm->enable)
    {
        next_tick = TICK_PER_SECOND;
        rt_timer_control(&alarm_time, TIMER_CTRL_SET_TIME, &next_tick);
        rt_timer_start(&alarm_time);
    }
    else
    {
        rt_timer_stop(&alarm_time);
    }
}

#endif

static rt_err_t soft_rtc_control(rt_device_t dev, int cmd, void *args)
{
    time_t *t;
    struct tm time_temp;

    OS_ASSERT(dev != OS_NULL);
    memset(&time_temp, 0, sizeof(struct tm));

    switch (cmd)
    {
    case DEVICE_CTRL_RTC_GET_TIME:
        t = (time_t *) args;
        *t = init_time + (rt_tick_get() - init_tick) / TICK_PER_SECOND;
        break;
    case DEVICE_CTRL_RTC_SET_TIME:
    {
        t = (time_t *) args;
        init_time = *t - (rt_tick_get() - init_tick) / TICK_PER_SECOND;
#ifdef ENABLE_ALARM
        soft_rtc_alarm_update(&wkalarm);
#endif
        break;
    }
#ifdef ENABLE_ALARM
    case DEVICE_CTRL_RTC_GET_ALARM:
        *((struct rt_rtc_wkalarm *)args) = wkalarm;
        break;
    case DEVICE_CTRL_RTC_SET_ALARM:
        wkalarm = *((struct rt_rtc_wkalarm *)args);
        soft_rtc_alarm_update(&wkalarm);
        break;
#endif
    }

    return OS_EOK;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops soft_rtc_ops = 
{
    OS_NULL,
    OS_NULL,
    OS_NULL,
    OS_NULL,
    OS_NULL,
    soft_rtc_control
};
#endif

int rt_soft_rtc_init(void)
{
    static rt_bool_t init_ok = OS_FALSE;
    struct tm time_new = SOFT_RTC_TIME_DEFAULT;

    if (init_ok)
    {
        return 0;
    }
    /* make sure only one 'rtc' device */
    OS_ASSERT(!rt_device_find("rtc"));

#ifdef ENABLE_ALARM
    rt_timer_init(&alarm_time,
                  "alarm",
                  alarm_timeout,
                  &soft_rtc_dev,
                  0,
                  TIMER_FLAG_SOFT_TIMER|TIMER_FLAG_ONE_SHOT);
#endif

    init_tick = rt_tick_get();
    init_time = mktime(&time_new);

    soft_rtc_dev.type    = DEVICE_CLASS_RTC;

    /* register rtc device */
#ifdef ENABLE_DEVICE_OPS
    soft_rtc_dev.ops     = &soft_rtc_ops;
#else
    soft_rtc_dev.init    = OS_NULL;
    soft_rtc_dev.open    = OS_NULL;
    soft_rtc_dev.close   = OS_NULL;
    soft_rtc_dev.read    = OS_NULL;
    soft_rtc_dev.write   = OS_NULL;
    soft_rtc_dev.control = soft_rtc_control;
#endif

    /* no private */
    soft_rtc_dev.user_data = OS_NULL;

    rt_device_register(&soft_rtc_dev, "rtc", DEVICE_FLAG_RDWR);
    rt_kprintf("\tSOFT RTC register success\r\n!");
    init_ok = OS_TRUE;

    return 0;
}
INIT_DEVICE_EXPORT(rt_soft_rtc_init);

#endif /* ENABLE_SOFT_RTC */
