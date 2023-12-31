/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-09-30     Bernard      first version.
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#define API_COMPLETED    1
#define API_UNCOMPLETED  0

void rt_completion_init(struct rt_completion *completion)
{
    rt_base_t level;
    OS_ASSERT(completion != OS_NULL);

    level = rt_hw_interrupt_disable();
    completion->flag = API_UNCOMPLETED;
    rt_list_init(&completion->suspended_list);
    rt_hw_interrupt_enable(level);
}
OS_MODE_EXPORT(rt_completion_init);

rt_err_t rt_completion_wait(struct rt_completion *completion,
                            rt_int32_t            timeout)
{
    rt_err_t result;
    rt_base_t level;
    rt_thread_t thread;
    OS_ASSERT(completion != OS_NULL);

    result = OS_EOK;
    thread = rt_thread_self();

    level = rt_hw_interrupt_disable();
    if (completion->flag != API_COMPLETED)
    {
        /* only one thread can suspend on complete */
        OS_ASSERT(rt_list_isempty(&(completion->suspended_list)));

        if (timeout == 0)
        {
            result = -OS_ETIMEOUT;
            goto __exit;
        }
        else
        {
            /* reset thread error number */
            thread->error = OS_EOK;

            /* suspend thread */
            rt_thread_suspend(thread);
            /* add to suspended list */
            rt_list_insert_before(&(completion->suspended_list),
                                  &(thread->tlist));

            /* current context checking */
            ENABLE_DEBUG_NOT_IN_INTERRUPT;

            /* start timer */
            if (timeout > 0)
            {
                /* reset the timeout of thread timer and start it */
                rt_timer_control(&(thread->thread_timer),
                                 TIMER_CTRL_SET_TIME,
                                 &timeout);
                rt_timer_start(&(thread->thread_timer));
            }
            /* enable interrupt */
            rt_hw_interrupt_enable(level);

            /* do schedule */
            rt_schedule();

            /* thread is waked up */
            result = thread->error;

            level = rt_hw_interrupt_disable();
        }
    }
    /* clean completed flag */
    completion->flag = API_UNCOMPLETED;

__exit:
    rt_hw_interrupt_enable(level);

    return result;
}
OS_MODE_EXPORT(rt_completion_wait);

void rt_completion_done(struct rt_completion *completion)
{
    rt_base_t level;
    OS_ASSERT(completion != OS_NULL);

    if (completion->flag == API_COMPLETED)
        return;

    level = rt_hw_interrupt_disable();
    completion->flag = API_COMPLETED;

    if (!rt_list_isempty(&(completion->suspended_list)))
    {
        /* there is one thread in suspended list */
        struct rt_thread *thread;

        /* get thread entry */
        thread = rt_list_entry(completion->suspended_list.next,
                               struct rt_thread,
                               tlist);

        /* resume it */
        rt_thread_resume(thread);
        rt_hw_interrupt_enable(level);

        /* perform a schedule */
        rt_schedule();
    }
    else
    {
        rt_hw_interrupt_enable(level);
    }
}
OS_MODE_EXPORT(rt_completion_done);

