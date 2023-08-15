/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-02-27     bernard      fix the re-work issue.
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#ifdef ENABLE_MEMORY_HEAP

static void _delayed_work_timeout_handler(void *parameter);

rt_inline rt_err_t _workqueue_work_completion(struct rt_workqueue *queue)
{
    rt_err_t result;

    rt_enter_critical();
    while (1)
    {
        /* try to take condition semaphore */
        result = rt_sem_trytake(&(queue->sem));
        if (result == -OS_ETIMEOUT)
        {
            /* it's timeout, release this semaphore */
            rt_sem_release(&(queue->sem));
        }
        else if (result == OS_EOK)
        {
            /* keep the sem value = 0 */
            result = OS_EOK;
            break;
        }
        else
        {
            result = -OS_ERROR;
            break;
        }
    }
    rt_exit_critical();

    return result;
}

static void _workqueue_thread_entry(void *parameter)
{
    rt_base_t level;
    struct rt_work *work;
    struct rt_workqueue *queue;

    queue = (struct rt_workqueue *) parameter;
    OS_ASSERT(queue != OS_NULL);

    while (1)
    {
        if (rt_list_isempty(&(queue->work_list)))
        {
            /* no software timer exist, suspend self. */
            rt_thread_suspend(rt_thread_self());
            rt_schedule();
        }

        /* we have work to do with. */
        level = rt_hw_interrupt_disable();
        work = rt_list_entry(queue->work_list.next, struct rt_work, list);
        rt_list_remove(&(work->list));
        queue->work_current = work;
        work->flags &= ~WORK_STATE_PENDING;
        work->workqueue = OS_NULL;
        rt_hw_interrupt_enable(level);

        /* do work */
        work->work_func(work, work->work_data);
        level = rt_hw_interrupt_disable();
        /* clean current work */
        queue->work_current = OS_NULL;
        rt_hw_interrupt_enable(level);

        /* ack work completion */
        _workqueue_work_completion(queue);
    }
}

static rt_err_t _workqueue_submit_work(struct rt_workqueue *queue, struct rt_work *work)
{
    rt_base_t level;

    level = rt_hw_interrupt_disable();
    if (work->flags & WORK_STATE_PENDING)
    {
        rt_hw_interrupt_enable(level);
        return -OS_EBUSY;
    }

    if (queue->work_current == work)
    {
        rt_hw_interrupt_enable(level);
        return -OS_EBUSY;
    }

    /* NOTE: the work MUST be initialized firstly */
    rt_list_remove(&(work->list));

    rt_list_insert_after(queue->work_list.prev, &(work->list));
    work->flags |= WORK_STATE_PENDING;

    /* whether the workqueue is doing work */
    if (queue->work_current == OS_NULL)
    {
        rt_hw_interrupt_enable(level);
        /* resume work thread */
        rt_thread_resume(queue->work_thread);
        rt_schedule();
    }
    else
    {
        rt_hw_interrupt_enable(level);
    }

    return OS_EOK;
}

static rt_err_t _workqueue_cancel_work(struct rt_workqueue *queue, struct rt_work *work)
{
    rt_base_t level;

    level = rt_hw_interrupt_disable();
    if (queue->work_current == work)
    {
        rt_hw_interrupt_enable(level);
        return -OS_EBUSY;
    }
    rt_list_remove(&(work->list));
    work->flags &= ~WORK_STATE_PENDING;
    rt_hw_interrupt_enable(level);

    return OS_EOK;
}

static rt_err_t _workqueue_cancel_delayed_work(struct rt_work *work)
{
    rt_base_t level;
    int ret = OS_EOK;

    if (!work->workqueue)
    {
        ret = -EINVAL;
        goto __exit;
    }

    if (work->flags & WORK_STATE_PENDING)
    {
        /* Remove from the queue if already submitted */
        ret = _workqueue_cancel_work(work->workqueue, work);
        if (ret)
        {
            goto __exit;
        }
    }
    else
    {
        if (work->flags & WORK_STATE_SUBMITTING)
        {
            level = rt_hw_interrupt_disable();
            rt_timer_stop(&(work->timer));
            rt_timer_detach(&(work->timer));
            work->flags &= ~WORK_STATE_SUBMITTING;
            rt_hw_interrupt_enable(level);
        }
    }

    level = rt_hw_interrupt_disable();
    /* Detach from workqueue */
    work->workqueue = OS_NULL;
    work->flags &= ~(WORK_STATE_PENDING);
    rt_hw_interrupt_enable(level);

__exit:
    return ret;
}

static rt_err_t _workqueue_submit_delayed_work(struct rt_workqueue *queue,
        struct rt_work *work, rt_tick_t ticks)
{
    rt_base_t level;
    rt_err_t ret = OS_EOK;

    /* Work cannot be active in multiple queues */
    if (work->workqueue && work->workqueue != queue)
    {
        ret = -OS_EINVAL;
        goto __exit;
    }

    /* Cancel if work has been submitted */
    if (work->workqueue == queue)
    {
        ret = _workqueue_cancel_delayed_work(work);
        if (ret < 0)
        {
            goto __exit;
        }
    }

    level = rt_hw_interrupt_disable();
    /* Attach workqueue so the timeout callback can submit it */
    work->workqueue = queue;
    rt_hw_interrupt_enable(level);

    if (!ticks)
    {
        /* Submit work if no ticks is 0 */
        ret = _workqueue_submit_work(work->workqueue, work);
    }
    else
    {
        level = rt_hw_interrupt_disable();
        /* Add timeout */
        work->flags |= WORK_STATE_SUBMITTING;
        rt_timer_init(&(work->timer), "work", _delayed_work_timeout_handler, work, ticks,
                      TIMER_FLAG_ONE_SHOT | TIMER_FLAG_SOFT_TIMER);
        rt_hw_interrupt_enable(level);
        rt_timer_start(&(work->timer));
    }

__exit:
    return ret;
}

static void _delayed_work_timeout_handler(void *parameter)
{
    struct rt_work *delayed_work;
    rt_base_t level;

    delayed_work = (struct rt_work *)parameter;
    level = rt_hw_interrupt_disable();
    rt_timer_stop(&(delayed_work->timer));
    rt_timer_detach(&(delayed_work->timer));
    delayed_work->flags &= ~WORK_STATE_SUBMITTING;
    delayed_work->type &= ~WORK_TYPE_DELAYED;
    rt_hw_interrupt_enable(level);
    _workqueue_submit_work(delayed_work->workqueue, delayed_work);
}

struct rt_workqueue *rt_workqueue_create(const char *name, rt_uint16_t stack_size, rt_uint8_t priority)
{
    struct rt_workqueue *queue = OS_NULL;

    queue = (struct rt_workqueue *)KERNEL_MALLOC(sizeof(struct rt_workqueue));
    if (queue != OS_NULL)
    {
        /* initialize work list */
        rt_list_init(&(queue->work_list));
        queue->work_current = OS_NULL;
        rt_sem_init(&(queue->sem), "wqueue", 0, IPC_FLAG_FIFO);

        /* create the work thread */
        queue->work_thread = rt_thread_create(name, _workqueue_thread_entry, queue, stack_size, priority, 10);
        if (queue->work_thread == OS_NULL)
        {
            KERNEL_FREE(queue);
            return OS_NULL;
        }

        rt_thread_startup(queue->work_thread);
    }

    return queue;
}

rt_err_t rt_workqueue_destroy(struct rt_workqueue *queue)
{
    OS_ASSERT(queue != OS_NULL);

    rt_thread_delete(queue->work_thread);
    KERNEL_FREE(queue);

    return OS_EOK;
}

rt_err_t rt_workqueue_dowork(struct rt_workqueue *queue, struct rt_work *work)
{
    OS_ASSERT(queue != OS_NULL);
    OS_ASSERT(work != OS_NULL);

    return _workqueue_submit_work(queue, work);
}

rt_err_t rt_workqueue_submit_work(struct rt_workqueue *queue, struct rt_work *work, rt_tick_t time)
{
    OS_ASSERT(queue != OS_NULL);
    OS_ASSERT(work != OS_NULL);

    if (time > 0)
    {
        work->type |= WORK_TYPE_DELAYED;
    }

    if (work->type & WORK_TYPE_DELAYED)
    {
        return _workqueue_submit_delayed_work(queue, work, time);
    }
    else
    {
        return _workqueue_submit_work(queue, work);
    }
}

rt_err_t rt_workqueue_critical_work(struct rt_workqueue *queue, struct rt_work *work)
{
    rt_base_t level;
    OS_ASSERT(queue != OS_NULL);
    OS_ASSERT(work != OS_NULL);

    level = rt_hw_interrupt_disable();
    if (queue->work_current == work)
    {
        rt_hw_interrupt_enable(level);
        return -OS_EBUSY;
    }

    /* NOTE: the work MUST be initialized firstly */
    rt_list_remove(&(work->list));

    rt_list_insert_after(queue->work_list.prev, &(work->list));
    if (queue->work_current == OS_NULL)
    {
        rt_hw_interrupt_enable(level);
        /* resume work thread */
        rt_thread_resume(queue->work_thread);
        rt_schedule();
    }
    else rt_hw_interrupt_enable(level);

    return OS_EOK;
}

rt_err_t rt_workqueue_cancel_work(struct rt_workqueue *queue, struct rt_work *work)
{
    OS_ASSERT(queue != OS_NULL);
    OS_ASSERT(work != OS_NULL);

    if (work->type & WORK_TYPE_DELAYED)
    {
        return _workqueue_cancel_delayed_work(work);
    }
    else
    {
        return _workqueue_cancel_work(queue, work);
    }
}

rt_err_t rt_workqueue_cancel_work_sync(struct rt_workqueue *queue, struct rt_work *work)
{
    rt_base_t level;

    OS_ASSERT(queue != OS_NULL);
    OS_ASSERT(work != OS_NULL);

    level = rt_hw_interrupt_disable();
    if (queue->work_current == work) /* it's current work in the queue */
    {
        /* wait for work completion */
        rt_sem_take(&(queue->sem), WAITING_FOREVER);
    }
    else
    {
        rt_list_remove(&(work->list));
    }
    work->flags &= ~WORK_STATE_PENDING;
    rt_hw_interrupt_enable(level);

    return OS_EOK;
}

rt_err_t rt_workqueue_cancel_all_work(struct rt_workqueue *queue)
{
    struct rt_list_node *node, *next;
    OS_ASSERT(queue != OS_NULL);

    rt_enter_critical();
    for (node = queue->work_list.next; node != &(queue->work_list); node = next)
    {
        next = node->next;
        rt_list_remove(node);
    }
    rt_exit_critical();

    return OS_EOK;
}

void rt_delayed_work_init(struct rt_delayed_work *work, void (*work_func)(struct rt_work *work,
                          void *work_data), void *work_data)
{
    rt_work_init(&work->work, work_func, work_data);
}

#ifdef ENABLE_SYSTEM_WORKQUEUE
static struct rt_workqueue *sys_workq;

rt_err_t rt_work_submit(struct rt_work *work, rt_tick_t time)
{
    return rt_workqueue_submit_work(sys_workq, work, time);
}

rt_err_t rt_work_cancel(struct rt_work *work)
{
    return rt_workqueue_cancel_work(sys_workq, work);
}

int rt_work_sys_workqueue_init(void)
{
    if (sys_workq != OS_NULL)
        return 0;

    sys_workq = rt_workqueue_create("sys_work", SYSTEM_WORKQUEUE_STACKSIZE,
                                    SYSTEM_WORKQUEUE_PRIORITY);

    return OS_EOK;
}

INIT_PREV_EXPORT(rt_work_sys_workqueue_init);
#endif
#endif
