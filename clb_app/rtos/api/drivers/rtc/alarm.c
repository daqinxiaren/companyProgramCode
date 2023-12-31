/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2012-10-27     heyuanjie87       first version.
 * 2013-05-17     aozima            initial alarm event & mutex in system init.
 * 2020-10-15     zhangsz           add alarm flags hour minute second.
 * 2020-11-09     zhangsz           fix alarm set when modify rtc time.
 */

#include <rtthread.h>
#include <rtdevice.h>

#ifndef _WIN32
#include <sys/time.h>
#endif

#define DEV_RTC_YEARS_MAX         137
#ifdef ENABLE_SOFT_RTC
#define ALARM_DELAY             0
#else
#define ALARM_DELAY             2
#endif
#define ALARM_STATE_INITED   0x02
#define ALARM_STATE_START    0x01
#define ALARM_STATE_STOP     0x00

#if (defined(ENABLE_RTC) && defined(ENABLE_ALARM))
static struct rt_alarm_container _container;

rt_inline rt_uint32_t alarm_mkdaysec(struct tm *time)
{
    rt_uint32_t sec;

    sec = time->tm_sec;
    sec += time->tm_min * 60;
    sec += time->tm_hour * 3600;

    return (sec);
}

static rt_err_t alarm_set(struct rt_alarm *alarm)
{
    rt_device_t device;
    struct rt_rtc_wkalarm wkalarm;
    rt_err_t ret;

    device = rt_device_find("rtc");

    if (device == OS_NULL)
    {
        return (OS_ERROR);
    }

    if (alarm->flag & ALARM_STATE_START)
        wkalarm.enable = OS_TRUE;
    else
        wkalarm.enable = OS_FALSE;

    wkalarm.tm_sec = alarm->wktime.tm_sec;
    wkalarm.tm_min = alarm->wktime.tm_min;
    wkalarm.tm_hour = alarm->wktime.tm_hour;

    ret = rt_device_control(device, DEVICE_CTRL_RTC_SET_ALARM, &wkalarm);
    if ((ret == OS_EOK) && wkalarm.enable)
    {
        ret = rt_device_control(device, DEVICE_CTRL_RTC_GET_ALARM, &wkalarm);
        if (ret == OS_EOK)
        {
            /*
              some RTC device like RX8025,it's alarms precision is 1 minute.
              in this case,low level RTC driver should set wkalarm->tm_sec to 0.
            */
            alarm->wktime.tm_sec = wkalarm.tm_sec;
            alarm->wktime.tm_min = wkalarm.tm_min;
            alarm->wktime.tm_hour = wkalarm.tm_hour;
        }
    }

    return (ret);
}

static void alarm_wakeup(struct rt_alarm *alarm, struct tm *now)
{
    rt_uint32_t sec_alarm, sec_now;
    rt_bool_t wakeup = OS_FALSE;
    time_t timestamp;

    sec_alarm = alarm_mkdaysec(&alarm->wktime);
    sec_now = alarm_mkdaysec(now);

    if (alarm->flag & ALARM_STATE_START)
    {
        switch (alarm->flag & 0xFF00)
        {
        case ALARM_ONESHOT:
        {
            sec_alarm = mktime(&alarm->wktime);
            sec_now = mktime(now);
            if (((sec_now - sec_alarm) <= ALARM_DELAY) && (sec_now >= sec_alarm))
            {
                /* stop alarm */
                alarm->flag &= ~ALARM_STATE_START;
                alarm_set(alarm);
                wakeup = OS_TRUE;
            }
        }
        break;
        case ALARM_SECOND:
        {
            alarm->wktime.tm_hour = now->tm_hour;
            alarm->wktime.tm_min = now->tm_min;
            alarm->wktime.tm_sec = now->tm_sec + 1;
            if (alarm->wktime.tm_sec > 59)
            {
                alarm->wktime.tm_sec = 0;
                alarm->wktime.tm_min = alarm->wktime.tm_min + 1;
                if (alarm->wktime.tm_min > 59)
                {
                    alarm->wktime.tm_min = 0;
                    alarm->wktime.tm_hour = alarm->wktime.tm_hour + 1;
                    if (alarm->wktime.tm_hour > 23)
                    {
                        alarm->wktime.tm_hour = 0;
                    }
                }
            }
            wakeup = OS_TRUE;
        }
        break;
        case ALARM_MINUTE:
        {
            alarm->wktime.tm_hour = now->tm_hour;
            if (alarm->wktime.tm_sec == now->tm_sec)
            {
                alarm->wktime.tm_min = now->tm_min + 1;
                if (alarm->wktime.tm_min > 59)
                {
                    alarm->wktime.tm_min = 0;
                    alarm->wktime.tm_hour = alarm->wktime.tm_hour + 1;
                    if (alarm->wktime.tm_hour > 23)
                    {
                        alarm->wktime.tm_hour = 0;
                    }
                }
                wakeup = OS_TRUE;
            }
        }
        break;
        case ALARM_HOUR:
        {
            if ((alarm->wktime.tm_min == now->tm_min) &&
                (alarm->wktime.tm_sec == now->tm_sec))
            {
                alarm->wktime.tm_hour = now->tm_hour + 1;
                if (alarm->wktime.tm_hour > 23)
                {
                    alarm->wktime.tm_hour = 0;
                }
                wakeup = OS_TRUE;
            }
        }
        break;
        case ALARM_DAILY:
        {
            if (((sec_now - sec_alarm) <= ALARM_DELAY) && (sec_now >= sec_alarm))
                wakeup = OS_TRUE;
        }
        break;
        case ALARM_WEEKLY:
        {
            /* alarm at wday */
            sec_alarm += alarm->wktime.tm_wday * 24 * 3600;
            sec_now += now->tm_wday * 24 * 3600;

            if (((sec_now - sec_alarm) <= ALARM_DELAY) && (sec_now >= sec_alarm))
                wakeup = OS_TRUE;
        }
        break;
        case ALARM_MONTHLY:
        {
            /* monthly someday generate alarm signals */
            if (alarm->wktime.tm_mday == now->tm_mday)
            {
                if ((sec_now - sec_alarm) <= ALARM_DELAY)
                    wakeup = OS_TRUE;
            }
        }
        break;
        case ALARM_YAERLY:
        {
            if ((alarm->wktime.tm_mday == now->tm_mday) && \
                    (alarm->wktime.tm_mon == now->tm_mon))
            {
                if ((sec_now - sec_alarm) <= ALARM_DELAY)
                    wakeup = OS_TRUE;
            }
        }
        break;
        }

        if ((wakeup == OS_TRUE) && (alarm->callback != OS_NULL))
        {
            timestamp = time(OS_NULL);
            alarm->callback(alarm, timestamp);
        }
    }
}

static void alarm_update(rt_uint32_t event)
{
    struct rt_alarm *alm_prev = OS_NULL, *alm_next = OS_NULL;
    struct rt_alarm *alarm;
    rt_int32_t sec_now, sec_alarm, sec_tmp;
    rt_int32_t sec_next = 24 * 3600, sec_prev = 0;
    time_t timestamp;
    struct tm now;
    rt_list_t *next;

    rt_mutex_take(&_container.mutex, WAITING_FOREVER);
    if (!rt_list_isempty(&_container.head))
    {
        /* get time of now */
        timestamp = time(OS_NULL);
#ifdef _WIN32
        _gmtime32_s(&now, &timestamp);
#else
        gmtime_r(&timestamp, &now);
#endif

        for (next = _container.head.next; next != &_container.head; next = next->next)
        {
            alarm = rt_list_entry(next, struct rt_alarm, list);
            /* check the overtime alarm */
            alarm_wakeup(alarm, &now);
        }

        timestamp = time(OS_NULL);
#ifdef _WIN32
        _gmtime32_s(&now, &timestamp);
#else
        gmtime_r(&timestamp, &now);
#endif
        sec_now = alarm_mkdaysec(&now);

        for (next = _container.head.next; next != &_container.head; next = next->next)
        {
            alarm = rt_list_entry(next, struct rt_alarm, list);
            /* calculate seconds from 00:00:00 */
            sec_alarm = alarm_mkdaysec(&alarm->wktime);
            if (alarm->flag & ALARM_STATE_START)
            {
                sec_tmp = sec_alarm - sec_now;
                if (sec_tmp > 0)
                {
                    /* find alarm after now(now to 23:59:59) and the most recent */
                    if (sec_tmp < sec_next)
                    {
                        sec_next = sec_tmp;
                        alm_next = alarm;
                    }
                }
                else
                {
                    /* find alarm before now(00:00:00 to now) and furthest from now */
                    if (sec_tmp < sec_prev)
                    {
                        sec_prev = sec_tmp;
                        alm_prev = alarm;
                    }
                }
            }
        }

        /* enable the alarm after now first */
        if (sec_next < 24 * 3600)
        {
            if (alarm_set(alm_next) == OS_EOK)
                _container.current = alm_next;
        }
        else if (sec_prev < 0)
        {
            /* enable the alarm before now */
            if (alarm_set(alm_prev) == OS_EOK)
                _container.current = alm_prev;
        }
        else
        {
            if (_container.current != OS_NULL)
                alarm_set(_container.current);
        }
    }
    rt_mutex_release(&_container.mutex);
}

static rt_uint32_t days_of_year_month(int tm_year, int tm_mon)
{
    rt_uint32_t ret, year;

    year = tm_year + 1900;
    if (tm_mon == 1)
    {
        ret = 28 + ((!(year % 4) && (year % 100)) || !(year % 400));
    }
    else if (((tm_mon <= 6) && (tm_mon % 2 == 0)) || ((tm_mon > 6) && (tm_mon % 2 == 1)))
    {
        ret = 31;
    }
    else
    {
        ret = 30;
    }

    return (ret);
}

static rt_bool_t is_valid_date(struct tm *date)
{
    if ((date->tm_year < 0) || (date->tm_year > DEV_RTC_YEARS_MAX))
    {
        return (OS_FALSE);
    }

    if ((date->tm_mon < 0) || (date->tm_mon > 11))
    {
        return (OS_FALSE);
    }

    if ((date->tm_mday < 1) || \
            (date->tm_mday > days_of_year_month(date->tm_year, date->tm_mon)))
    {
        return (OS_FALSE);
    }

    return (OS_TRUE);
}

static rt_err_t alarm_setup(rt_alarm_t alarm, struct tm *wktime)
{
    rt_err_t ret = OS_ERROR;
    time_t timestamp;
    struct tm *setup, now;

    setup = &alarm->wktime;
    *setup = *wktime;
    timestamp = time(OS_NULL);

#ifdef _WIN32
    _gmtime32_s(&now, &timestamp);
#else
    gmtime_r(&timestamp, &now);
#endif

    /* if these are a "don't care" value,we set them to now*/
    if ((setup->tm_sec > 59) || (setup->tm_sec < 0))
        setup->tm_sec = now.tm_sec;
    if ((setup->tm_min > 59) || (setup->tm_min < 0))
        setup->tm_min = now.tm_min;
    if ((setup->tm_hour > 23) || (setup->tm_hour < 0))
        setup->tm_hour = now.tm_hour;

    switch (alarm->flag & 0xFF00)
    {
    case ALARM_SECOND:
    {
        alarm->wktime.tm_hour = now.tm_hour;
        alarm->wktime.tm_min = now.tm_min;
        alarm->wktime.tm_sec = now.tm_sec + 1;
        if (alarm->wktime.tm_sec > 59)
        {
            alarm->wktime.tm_sec = 0;
            alarm->wktime.tm_min = alarm->wktime.tm_min + 1;
            if (alarm->wktime.tm_min > 59)
            {
                alarm->wktime.tm_min = 0;
                alarm->wktime.tm_hour = alarm->wktime.tm_hour + 1;
                if (alarm->wktime.tm_hour > 23)
                {
                    alarm->wktime.tm_hour = 0;
                }
            }
        }
    }
    break;
    case ALARM_MINUTE:
    {
        alarm->wktime.tm_hour = now.tm_hour;
        alarm->wktime.tm_min = now.tm_min + 1;
        if (alarm->wktime.tm_min > 59)
        {
            alarm->wktime.tm_min = 0;
            alarm->wktime.tm_hour = alarm->wktime.tm_hour + 1;
            if (alarm->wktime.tm_hour > 23)
            {
                alarm->wktime.tm_hour = 0;
            }
        }
    }
    break;
    case ALARM_HOUR:
    {
        alarm->wktime.tm_hour = now.tm_hour + 1;
        if (alarm->wktime.tm_hour > 23)
        {
            alarm->wktime.tm_hour = 0;
        }
    }
    break;
    case ALARM_DAILY:
    {
        /* do nothing but needed */
    }
    break;
    case ALARM_ONESHOT:
    {
        /* if these are "don't care" value we set them to now */
        if (setup->tm_year == ALARM_TM_NOW)
            setup->tm_year = now.tm_year;
        if (setup->tm_mon == ALARM_TM_NOW)
            setup->tm_mon = now.tm_mon;
        if (setup->tm_mday == ALARM_TM_NOW)
            setup->tm_mday = now.tm_mday;
        /* make sure the setup is valid */
        if (!is_valid_date(setup))
            goto _exit;
    }
    break;
    case ALARM_WEEKLY:
    {
        /* if tm_wday is a "don't care" value we set it to now */
        if ((setup->tm_wday < 0) || (setup->tm_wday > 6))
            setup->tm_wday = now.tm_wday;
    }
    break;
    case ALARM_MONTHLY:
    {
        /* if tm_mday is a "don't care" value we set it to now */
        if ((setup->tm_mday < 1) || (setup->tm_mday > 31))
            setup->tm_mday = now.tm_mday;
    }
    break;
    case ALARM_YAERLY:
    {
        /* if tm_mon is a "don't care" value we set it to now */
        if ((setup->tm_mon < 0) || (setup->tm_mon > 11))
            setup->tm_mon = now.tm_mon;

        if (setup->tm_mon == 1)
        {
            /* tm_mon is February */

            /* tm_mday should be 1~29.otherwise,it's a "don't care" value */
            if ((setup->tm_mday < 1) || (setup->tm_mday > 29))
                setup->tm_mday = now.tm_mday;
        }
        else if (((setup->tm_mon <= 6) && (setup->tm_mon % 2 == 0)) || \
                 ((setup->tm_mon > 6) && (setup->tm_mon % 2 == 1)))
        {
            /* Jan,Mar,May,Jul,Aug,Oct,Dec */

            /* tm_mday should be 1~31.otherwise,it's a "don't care" value */
            if ((setup->tm_mday < 1) || (setup->tm_mday > 31))
                setup->tm_mday = now.tm_mday;
        }
        else
        {
            /* tm_mday should be 1~30.otherwise,it's a "don't care" value */
            if ((setup->tm_mday < 1) || (setup->tm_mday > 30))
                setup->tm_mday = now.tm_mday;
        }
    }
    break;
    default:
    {
        goto _exit;
    }
    }

    if ((setup->tm_hour == 23) && (setup->tm_min == 59) && (setup->tm_sec == 59))
    {
        /*
           for insurance purposes, we will generate an alarm
           signal two seconds ahead of.
        */
        setup->tm_sec = 60 - ALARM_DELAY;
    }
    /* set initialized state */
    alarm->flag |= ALARM_STATE_INITED;
    ret = OS_EOK;

_exit:

    return (ret);
}

/** \brief send a rtc alarm event
 *
 * \param dev pointer to RTC device(currently unused,you can ignore it)
 * \param event RTC event(currently unused)
 * \return none
 */
void rt_alarm_update(rt_device_t dev, rt_uint32_t event)
{
    rt_event_send(&_container.event, 1);
}

/** \brief modify the alarm setup
 *
 * \param alarm pointer to alarm
 * \param cmd control command
 * \param arg argument
 */
rt_err_t rt_alarm_control(rt_alarm_t alarm, int cmd, void *arg)
{
    rt_err_t ret = OS_ERROR;

    OS_ASSERT(alarm != OS_NULL);

    rt_mutex_take(&_container.mutex, WAITING_FOREVER);
    switch (cmd)
    {
    case ALARM_CTRL_MODIFY:
    {
        struct rt_alarm_setup *setup;

        OS_ASSERT(arg != OS_NULL);
        setup = arg;
        rt_alarm_stop(alarm);
        alarm->flag = setup->flag & 0xFF00;
        alarm->wktime = setup->wktime;
        ret = alarm_setup(alarm, &alarm->wktime);
    }
    break;
    }

    rt_mutex_release(&_container.mutex);

    return (ret);
}

/** \brief start an alarm
 *
 * \param alarm pointer to alarm
 * \return OS_EOK
 */
rt_err_t rt_alarm_start(rt_alarm_t alarm)
{
    rt_int32_t sec_now, sec_old, sec_new;
    rt_err_t ret = OS_EOK;
    time_t timestamp;
    struct tm now;

    if (alarm == OS_NULL)
        return (OS_ERROR);
    rt_mutex_take(&_container.mutex, WAITING_FOREVER);

    if (!(alarm->flag & ALARM_STATE_START))
    {
        if (alarm_setup(alarm, &alarm->wktime) != OS_EOK)
        {
            ret = OS_ERROR;
            goto _exit;
        }

        timestamp = time(OS_NULL);
#ifdef _WIN32
        _gmtime32_s(&now, &timestamp);
#else
        gmtime_r(&timestamp, &now);
#endif

        alarm->flag |= ALARM_STATE_START;

        /* set alarm */
        if (_container.current == OS_NULL)
        {
            ret = alarm_set(alarm);
        }
        else
        {
            sec_now = alarm_mkdaysec(&now);
            sec_old = alarm_mkdaysec(&_container.current->wktime);
            sec_new = alarm_mkdaysec(&alarm->wktime);

            if ((sec_new < sec_old) && (sec_new > sec_now))
            {
                ret = alarm_set(alarm);
            }
            else if ((sec_new > sec_now) && (sec_old < sec_now))
            {
                ret = alarm_set(alarm);
            }
            else if ((sec_new < sec_old) && (sec_old < sec_now))
            {
                ret = alarm_set(alarm);
            }
            else
            {
                ret = OS_EOK;
                goto _exit;
            }
        }

        if (ret == OS_EOK)
        {
            _container.current = alarm;
        }
    }

_exit:
    rt_mutex_release(&_container.mutex);

    return (ret);
}

/** \brief stop an alarm
 *
 * \param alarm pointer to alarm
 * \return OS_EOK
 */
rt_err_t rt_alarm_stop(rt_alarm_t alarm)
{
    rt_err_t ret = OS_EOK;

    if (alarm == OS_NULL)
        return (OS_ERROR);
    rt_mutex_take(&_container.mutex, WAITING_FOREVER);
    if (!(alarm->flag & ALARM_STATE_START))
        goto _exit;
    /* stop alarm */
    alarm->flag &= ~ALARM_STATE_START;

    if (_container.current == alarm)
    {
        ret = alarm_set(alarm);
        _container.current = OS_NULL;
    }

    if (ret == OS_EOK)
        alarm_update(0);

_exit:
    rt_mutex_release(&_container.mutex);

    return (ret);
}

/** \brief delete an alarm
 *
 * \param alarm pointer to alarm
 * \return OS_EOK
 */
rt_err_t rt_alarm_delete(rt_alarm_t alarm)
{
    rt_err_t ret = OS_EOK;

    if (alarm == OS_NULL)
        return OS_ERROR;
    rt_mutex_take(&_container.mutex, WAITING_FOREVER);
    /* stop the alarm */
    alarm->flag &= ~ALARM_STATE_START;
    if (_container.current == alarm)
    {
        ret = alarm_set(alarm);
        _container.current = OS_NULL;
        /* set new alarm if necessary */
        alarm_update(0);
    }
    rt_list_remove(&alarm->list);
    rt_free(alarm);

    rt_mutex_release(&_container.mutex);

    return (ret);
}

/** \brief create an alarm
 *
 * \param flag set alarm mode e.g: ALARM_DAILY
 * \param setup pointer to setup infomation
 */
rt_alarm_t rt_alarm_create(rt_alarm_callback_t callback, struct rt_alarm_setup *setup)
{
    struct rt_alarm *alarm;

    if (setup == OS_NULL)
        return (OS_NULL);

    alarm = rt_malloc(sizeof(struct rt_alarm));
    if (alarm == OS_NULL)
        return (OS_NULL);

    rt_list_init(&alarm->list);

    alarm->wktime = setup->wktime;
    alarm->flag = setup->flag & 0xFF00;
    alarm->callback = callback;
    rt_mutex_take(&_container.mutex, WAITING_FOREVER);
    rt_list_insert_after(&_container.head, &alarm->list);
    rt_mutex_release(&_container.mutex);

    return (alarm);
}

/** \brief rtc alarm service thread entry
 *
 */
static void rt_alarmsvc_thread_init(void *param)
{
    rt_uint32_t recv;

    _container.current = OS_NULL;

    while (1)
    {
        if (rt_event_recv(&_container.event, 0xFFFF,
                          EVENT_FLAG_OR | EVENT_FLAG_CLEAR,
                          WAITING_FOREVER, &recv) == OS_EOK)
        {
            alarm_update(recv);
        }
    }
}

struct _alarm_flag
{
    const char* name;
    rt_uint32_t flag;
};

static const struct _alarm_flag _alarm_flag_tbl[] =
{
    {"N",        0xffff}, /* none */
    {"O",       ALARM_ONESHOT}, /* only alarm onece */
    {"D",       ALARM_DAILY}, /* alarm everyday */
    {"W",       ALARM_WEEKLY}, /* alarm weekly at Monday or Friday etc. */
    {"Mo",      ALARM_MONTHLY}, /* alarm monthly at someday */
    {"Y",       ALARM_YAERLY}, /* alarm yearly at a certain date */
    {"H",       ALARM_HOUR}, /* alarm each hour at a certain min:second */
    {"M",       ALARM_MINUTE}, /* alarm each minute at a certain second */
    {"S",       ALARM_SECOND}, /* alarm each second */
};

static rt_uint8_t _alarm_flag_tbl_size = sizeof(_alarm_flag_tbl) / sizeof(_alarm_flag_tbl[0]);

static rt_uint8_t get_alarm_flag_index(rt_uint32_t alarm_flag)
{
    for (rt_uint8_t index = 0; index < _alarm_flag_tbl_size; index++)
    {
        alarm_flag &= 0xff00;
        if (alarm_flag == _alarm_flag_tbl[index].flag)
        {
            return index;
        }
    }

    return 0;
}

void rt_alarm_dump(void)
{
    rt_list_t *next;
    rt_alarm_t alarm;
    rt_uint8_t index = 0;

    rt_kprintf("| id | YYYY-MM-DD hh:mm:ss | week | flag | en |\n");
    rt_kprintf("+----+---------------------+------+------+----+\n");
    for (next = _container.head.next; next != &_container.head; next = next->next)
    {
        alarm = rt_list_entry(next, struct rt_alarm, list);
        rt_uint8_t flag_index = get_alarm_flag_index(alarm->flag);
        rt_kprintf("| %2d | %04d-%02d-%02d %02d:%02d:%02d |  %2d  |  %2s  | %2d |\n",
            index++, alarm->wktime.tm_year + 1900, alarm->wktime.tm_mon + 1, alarm->wktime.tm_mday,
            alarm->wktime.tm_hour, alarm->wktime.tm_min, alarm->wktime.tm_sec,
            alarm->wktime.tm_wday, _alarm_flag_tbl[flag_index].name, alarm->flag & ALARM_STATE_START);
    }
    rt_kprintf("+----+---------------------+------+------+----+\n");
}

FINSH_FUNCTION_EXPORT_ALIAS(rt_alarm_dump, __cmd_alarm_dump, dump alarm info);

/** \brief initialize alarm service system
 *
 * \param none
 * \return none
 */
int rt_alarm_system_init(void)
{
    rt_thread_t tid;

    rt_list_init(&_container.head);
    rt_event_init(&_container.event, "alarmsvc", IPC_FLAG_FIFO);
    rt_mutex_init(&_container.mutex, "alarmsvc", IPC_FLAG_FIFO);

    tid = rt_thread_create("alarmsvc",
                           rt_alarmsvc_thread_init, OS_NULL,
                           2048, 10, 5);
    if (tid != OS_NULL)
        rt_thread_startup(tid);

    return 0;
}

INIT_PREV_EXPORT(rt_alarm_system_init);
#endif
