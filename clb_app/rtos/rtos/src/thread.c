/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-28     Bernard      first version
 * 2006-04-29     Bernard      implement thread timer
 * 2006-04-30     Bernard      added THREAD_DEBUG
 * 2006-05-27     Bernard      fixed the rt_thread_yield bug
 * 2006-06-03     Bernard      fixed the thread timer init bug
 * 2006-08-10     Bernard      fixed the timer bug in thread_sleep
 * 2006-09-03     Bernard      changed rt_timer_delete to rt_timer_detach
 * 2006-09-03     Bernard      implement rt_thread_detach
 * 2008-02-16     Bernard      fixed the rt_thread_timeout bug
 * 2010-03-21     Bernard      change the errno of rt_thread_delay/sleep to
 *                             OS_EOK.
 * 2010-11-10     Bernard      add cleanup callback function in thread exit.
 * 2011-09-01     Bernard      fixed rt_thread_exit issue when the current
 *                             thread preempted, which reported by Jiaxing Lee.
 * 2011-09-08     Bernard      fixed the scheduling issue in rt_thread_startup.
 * 2012-12-29     Bernard      fixed compiling warning.
 * 2016-08-09     ArdaFu       add thread suspend and resume hook.
 * 2017-04-10     armink       fixed the rt_thread_delete and rt_thread_detach
 *                             bug when thread has not startup.
 * 2018-11-22     Jesven       yield is same to rt_schedule
 *                             add support for tasks bound to cpu
 */

#include <rthw.h>
#include <rtthread.h>

extern rt_list_t rt_thread_defunct;

#ifdef ENABLE_HOOK
static void (*rt_thread_suspend_hook)(rt_thread_t thread);
static void (*rt_thread_resume_hook) (rt_thread_t thread);
static void (*rt_thread_inited_hook) (rt_thread_t thread);

/**
 * @ingroup Hook
 * This function sets a hook function when the system suspend a thread.
 *
 * @param hook the specified hook function
 *
 * @note the hook function must be simple and never be blocked or suspend.
 */
void rt_thread_suspend_sethook(void (*hook)(rt_thread_t thread))
{
    rt_thread_suspend_hook = hook;
}

/**
 * @ingroup Hook
 * This function sets a hook function when the system resume a thread.
 *
 * @param hook the specified hook function
 *
 * @note the hook function must be simple and never be blocked or suspend.
 */
void rt_thread_resume_sethook(void (*hook)(rt_thread_t thread))
{
    rt_thread_resume_hook = hook;
}

/**
 * @ingroup Hook
 * This function sets a hook function when a thread is initialized.
 *
 * @param hook the specified hook function
 */
void rt_thread_inited_sethook(void (*hook)(rt_thread_t thread))
{
    rt_thread_inited_hook = hook;
}

#endif

/* must be invoke witch rt_hw_interrupt_disable */
static void _thread_cleanup_execute(rt_thread_t thread)
{
    register rt_base_t level;
#ifdef ENABLE_MODULE
    struct rt_dlmodule *module = OS_NULL;
#endif
    level = rt_hw_interrupt_disable();
#ifdef ENABLE_MODULE
    module = (struct rt_dlmodule*)thread->module_id;
    if (module)
    {
        dlmodule_destroy(module);
    }
#endif
    /* invoke thread cleanup */
    if (thread->cleanup != OS_NULL)
        thread->cleanup(thread);

#ifdef ENABLE_SIGNALS
    rt_thread_free_sig(thread);
#endif
    rt_hw_interrupt_enable(level);
}

void rt_thread_exit(void)
{
    struct rt_thread *thread;
    register rt_base_t level;

    /* get current thread */
    thread = rt_thread_self();

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    _thread_cleanup_execute(thread);

    /* remove from schedule */
    rt_schedule_remove_thread(thread);
    /* change stat */
    thread->stat = THREAD_CLOSE;

    /* remove it from timer list */
    rt_timer_detach(&thread->thread_timer);

    if (rt_object_is_systemobject((rt_object_t)thread) == OS_TRUE)
    {
        rt_object_detach((rt_object_t)thread);
    }
    else
    {
        /* insert to defunct thread list */
        rt_list_insert_after(&rt_thread_defunct, &(thread->tlist));
    }

    /* switch to next task */
    rt_schedule();

    /* enable interrupt */
    rt_hw_interrupt_enable(level);
}

static rt_err_t _rt_thread_init(struct rt_thread *thread,
                                const char       *name,
                                void (*entry)(void *parameter),
                                void             *parameter,
                                void             *stack_start,
                                rt_uint32_t       stack_size,
                                rt_uint8_t        priority,
                                rt_uint32_t       tick)
{
    /* init thread list */
    rt_list_init(&(thread->tlist));

    thread->entry = (void *)entry;
    thread->parameter = parameter;

    /* stack init */
    thread->stack_addr = stack_start;
    thread->stack_size = stack_size;

    /* init thread stack */
    rt_memset(thread->stack_addr, '#', thread->stack_size);
#ifdef ARCH_CPU_STACK_GROWS_UPWARD
    thread->sp = (void *)rt_hw_stack_init(thread->entry, thread->parameter,
                                          (void *)((char *)thread->stack_addr),
                                          (void *)rt_thread_exit);
#else
    thread->sp = (void *)rt_hw_stack_init(thread->entry, thread->parameter,
                                          (rt_uint8_t *)((char *)thread->stack_addr + thread->stack_size - sizeof(rt_ubase_t)),
                                          (void *)rt_thread_exit);
#endif

    /* priority init */
    OS_ASSERT(priority < THREAD_PRIORITY_MAX);
    thread->init_priority    = priority;
    thread->current_priority = priority;

    thread->number_mask = 0;
#if THREAD_PRIORITY_MAX > 32
    thread->number = 0;
    thread->high_mask = 0;
#endif

    /* tick init */
    thread->init_tick      = tick;
    thread->remaining_tick = tick;

    /* error and flags */
    thread->error = OS_EOK;
    thread->stat  = THREAD_INIT;

#ifdef ENABLE_SMP
    /* not bind on any cpu */
    thread->bind_cpu = OS_CPUS_NUMBER;
    thread->oncpu = OS_CPU_DETACHED;

    /* lock init */
    thread->scheduler_lock_nest = 0;
    thread->cpus_lock_nest = 0;
    thread->critical_lock_nest = 0;
#endif /*ENABLE_SMP*/

    /* initialize cleanup function and user data */
    thread->cleanup   = 0;
    thread->user_data = 0;

    /* initialize thread timer */
    rt_timer_init(&(thread->thread_timer),
                  thread->name,
                  rt_thread_timeout,
                  thread,
                  0,
                  TIMER_FLAG_ONE_SHOT);

    /* initialize signal */
#ifdef ENABLE_SIGNALS
    thread->sig_mask    = 0x00;
    thread->sig_pending = 0x00;

#ifndef ENABLE_SMP
    thread->sig_ret     = OS_NULL;
#endif
    thread->sig_vectors = OS_NULL;
    thread->si_list     = OS_NULL;
#endif

#ifdef ENABLE_LWP
    thread->lwp = OS_NULL;
#endif

    OBJECT_HOOK_CALL(rt_thread_inited_hook, (thread));

    return OS_EOK;
}

/**
 * @addtogroup Thread
 */

/**@{*/

/**
 * This function will initialize a thread, normally it's used to initialize a
 * static thread object.
 *
 * @param thread the static thread object
 * @param name the name of thread, which shall be unique
 * @param entry the entry function of thread
 * @param parameter the parameter of thread enter function
 * @param stack_start the start address of thread stack
 * @param stack_size the size of thread stack
 * @param priority the priority of thread
 * @param tick the time slice if there are same priority thread
 *
 * @return the operation status, OS_EOK on OK, -OS_ERROR on error
 */
rt_err_t rt_thread_init(struct rt_thread *thread,
                        const char       *name,
                        void (*entry)(void *parameter),
                        void             *parameter,
                        void             *stack_start,
                        rt_uint32_t       stack_size,
                        rt_uint8_t        priority,
                        rt_uint32_t       tick)
{
    /* thread check */
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT(stack_start != OS_NULL);

    /* initialize thread object */
    rt_object_init((rt_object_t)thread, OBJECT_CLASS_THREAD, name);

    return _rt_thread_init(thread,
                           name,
                           entry,
                           parameter,
                           stack_start,
                           stack_size,
                           priority,
                           tick);
}
OS_MODE_EXPORT(rt_thread_init);

/**
 * This function will return self thread object
 *
 * @return the self thread object
 */
rt_thread_t rt_thread_self(void)
{
#ifdef ENABLE_SMP
    rt_base_t lock;
    rt_thread_t self;

    lock = rt_hw_local_irq_disable();
    self = rt_cpu_self()->current_thread;
    rt_hw_local_irq_enable(lock);
    return self;
#else
    extern rt_thread_t rt_current_thread;

    return rt_current_thread;
#endif
}
OS_MODE_EXPORT(rt_thread_self);

/**
 * This function will start a thread and put it to system ready queue
 *
 * @param thread the thread to be started
 *
 * @return the operation status, OS_EOK on OK, -OS_ERROR on error
 */
rt_err_t rt_thread_startup(rt_thread_t thread)
{
    /* thread check */
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT((thread->stat & THREAD_STAT_MASK) == THREAD_INIT);
    OS_ASSERT(rt_object_get_type((rt_object_t)thread) == OBJECT_CLASS_THREAD);

    /* set current priority to initialize priority */
    thread->current_priority = thread->init_priority;

    /* calculate priority attribute */
#if THREAD_PRIORITY_MAX > 32
    thread->number      = thread->current_priority >> 3;            /* 5bit */
    thread->number_mask = 1L << thread->number;
    thread->high_mask   = 1L << (thread->current_priority & 0x07);  /* 3bit */
#else
    thread->number_mask = 1L << thread->current_priority;
#endif

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_THREAD, ("startup a thread:%s with priority:%d\n",
                                   thread->name, thread->init_priority));
    /* change thread stat */
    thread->stat = THREAD_SUSPEND;
    /* then resume it */
    rt_thread_resume(thread);
    if (rt_thread_self() != OS_NULL)
    {
        /* do a scheduling */
        rt_schedule();
    }

    return OS_EOK;
}
OS_MODE_EXPORT(rt_thread_startup);

/**
 * This function will detach a thread. The thread object will be removed from
 * thread queue and detached/deleted from system object management.
 *
 * @param thread the thread to be deleted
 *
 * @return the operation status, OS_EOK on OK, -OS_ERROR on error
 */
rt_err_t rt_thread_detach(rt_thread_t thread)
{
    rt_base_t lock;

    /* thread check */
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT(rt_object_get_type((rt_object_t)thread) == OBJECT_CLASS_THREAD);
    OS_ASSERT(rt_object_is_systemobject((rt_object_t)thread));

    if ((thread->stat & THREAD_STAT_MASK) == THREAD_CLOSE)
        return OS_EOK;

    if ((thread->stat & THREAD_STAT_MASK) != THREAD_INIT)
    {
        /* remove from schedule */
        rt_schedule_remove_thread(thread);
    }

    _thread_cleanup_execute(thread);

    /* release thread timer */
    rt_timer_detach(&(thread->thread_timer));

    /* change stat */
    thread->stat = THREAD_CLOSE;

    if (rt_object_is_systemobject((rt_object_t)thread) == OS_TRUE)
    {
        rt_object_detach((rt_object_t)thread);
    }
    else
    {
        /* disable interrupt */
        lock = rt_hw_interrupt_disable();
        /* insert to defunct thread list */
        rt_list_insert_after(&rt_thread_defunct, &(thread->tlist));
        /* enable interrupt */
        rt_hw_interrupt_enable(lock);
    }

    return OS_EOK;
}
OS_MODE_EXPORT(rt_thread_detach);

#ifdef ENABLE_MEMORY_HEAP
/**
 * This function will create a thread object and allocate thread object memory
 * and stack.
 *
 * @param name the name of thread, which shall be unique
 * @param entry the entry function of thread
 * @param parameter the parameter of thread enter function
 * @param stack_size the size of thread stack
 * @param priority the priority of thread
 * @param tick the time slice if there are same priority thread
 *
 * @return the created thread object
 */
rt_thread_t rt_thread_create(const char *name,
                             void (*entry)(void *parameter),
                             void       *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t  priority,
                             rt_uint32_t tick)
{
    struct rt_thread *thread;
    void *stack_start;

    thread = (struct rt_thread *)rt_object_allocate(OBJECT_CLASS_THREAD,
                                                    name);
    if (thread == OS_NULL)
        return OS_NULL;

    stack_start = (void *)KERNEL_MALLOC(stack_size);
    if (stack_start == OS_NULL)
    {
        /* allocate stack failure */
        rt_object_delete((rt_object_t)thread);

        return OS_NULL;
    }

    _rt_thread_init(thread,
                    name,
                    entry,
                    parameter,
                    stack_start,
                    stack_size,
                    priority,
                    tick);

    return thread;
}
OS_MODE_EXPORT(rt_thread_create);

/**
 * This function will delete a thread. The thread object will be removed from
 * thread queue and deleted from system object management in the idle thread.
 *
 * @param thread the thread to be deleted
 *
 * @return the operation status, OS_EOK on OK, -OS_ERROR on error
 */
rt_err_t rt_thread_delete(rt_thread_t thread)
{
    rt_base_t lock;

    /* thread check */
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT(rt_object_get_type((rt_object_t)thread) == OBJECT_CLASS_THREAD);
    OS_ASSERT(rt_object_is_systemobject((rt_object_t)thread) == OS_FALSE);

    if ((thread->stat & THREAD_STAT_MASK) == THREAD_CLOSE)
        return OS_EOK;

    if ((thread->stat & THREAD_STAT_MASK) != THREAD_INIT)
    {
        /* remove from schedule */
        rt_schedule_remove_thread(thread);
    }

    _thread_cleanup_execute(thread);

    /* release thread timer */
    rt_timer_detach(&(thread->thread_timer));

    /* disable interrupt */
    lock = rt_hw_interrupt_disable();

    /* change stat */
    thread->stat = THREAD_CLOSE;

    /* insert to defunct thread list */
    rt_list_insert_after(&rt_thread_defunct, &(thread->tlist));

    /* enable interrupt */
    rt_hw_interrupt_enable(lock);

    return OS_EOK;
}
OS_MODE_EXPORT(rt_thread_delete);
#endif

/**
 * This function will let current thread yield processor, and scheduler will
 * choose a highest thread to run. After yield processor, the current thread
 * is still in READY state.
 *
 * @return OS_EOK
 */
rt_err_t rt_thread_yield(void)
{
    struct rt_thread *thread;
    rt_base_t lock;

    thread = rt_thread_self();
    lock = rt_hw_interrupt_disable();
    thread->remaining_tick = thread->init_tick;
    thread->stat |= THREAD_STAT_YIELD;
    rt_schedule();
    rt_hw_interrupt_enable(lock);

    return OS_EOK;
}
OS_MODE_EXPORT(rt_thread_yield);

/**
 * This function will let current thread sleep for some ticks.
 *
 * @param tick the sleep ticks
 *
 * @return OS_EOK
 */
rt_err_t rt_thread_sleep(rt_tick_t tick)
{
    register rt_base_t temp;
    struct rt_thread *thread;

    /* set to current thread */
    thread = rt_thread_self();
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT(rt_object_get_type((rt_object_t)thread) == OBJECT_CLASS_THREAD);

    /* disable interrupt */
    temp = rt_hw_interrupt_disable();

    /* suspend thread */
    rt_thread_suspend(thread);

    /* reset the timeout of thread timer and start it */
    rt_timer_control(&(thread->thread_timer), TIMER_CTRL_SET_TIME, &tick);
    rt_timer_start(&(thread->thread_timer));

    /* enable interrupt */
    rt_hw_interrupt_enable(temp);

    rt_schedule();

    /* clear error number of this thread to OS_EOK */
    if (thread->error == -OS_ETIMEOUT)
        thread->error = OS_EOK;

    return OS_EOK;
}

/**
 * This function will let current thread delay for some ticks.
 *
 * @param tick the delay ticks
 *
 * @return OS_EOK
 */
rt_err_t rt_thread_delay(rt_tick_t tick)
{
    return rt_thread_sleep(tick);
}
OS_MODE_EXPORT(rt_thread_delay);

/**
 * This function will let current thread delay until (*tick + inc_tick).
 *
 * @param tick the tick of last wakeup.
 * @param inc_tick the increment tick
 *
 * @return OS_EOK
 */
rt_err_t rt_thread_delay_until(rt_tick_t *tick, rt_tick_t inc_tick)
{
    register rt_base_t level;
    struct rt_thread *thread;

    OS_ASSERT(tick != OS_NULL);

    /* set to current thread */
    thread = rt_thread_self();
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT(rt_object_get_type((rt_object_t)thread) == OBJECT_CLASS_THREAD);

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    if (rt_tick_get() - *tick < inc_tick)
    {
        *tick = *tick + inc_tick - rt_tick_get();

        /* suspend thread */
        rt_thread_suspend(thread);

        /* reset the timeout of thread timer and start it */
        rt_timer_control(&(thread->thread_timer), TIMER_CTRL_SET_TIME, tick);
        rt_timer_start(&(thread->thread_timer));

        /* enable interrupt */
        rt_hw_interrupt_enable(level);

        rt_schedule();

        /* clear error number of this thread to OS_EOK */
        if (thread->error == -OS_ETIMEOUT)
        {
            thread->error = OS_EOK;
        }
    }
    else
    {
        rt_hw_interrupt_enable(level);
    }

    /* get the wakeup tick */
    *tick = rt_tick_get();

    return OS_EOK;
}
OS_MODE_EXPORT(rt_thread_delay_until);

/**
 * This function will let current thread delay for some milliseconds.
 *
 * @param ms the delay ms time
 *
 * @return OS_EOK
 */
rt_err_t rt_thread_mdelay(rt_int32_t ms)
{
    rt_tick_t tick;

    tick = rt_tick_from_millisecond(ms);

    return rt_thread_sleep(tick);
}
OS_MODE_EXPORT(rt_thread_mdelay);

/**
 * This function will control thread behaviors according to control command.
 *
 * @param thread the specified thread to be controlled
 * @param cmd the control command, which includes
 *  THREAD_CTRL_CHANGE_PRIORITY for changing priority level of thread;
 *  THREAD_CTRL_STARTUP for starting a thread;
 *  THREAD_CTRL_CLOSE for delete a thread;
 *  THREAD_CTRL_BIND_CPU for bind the thread to a CPU.
 * @param arg the argument of control command
 *
 * @return OS_EOK
 */
rt_err_t rt_thread_control(rt_thread_t thread, int cmd, void *arg)
{
    register rt_base_t temp;

    /* thread check */
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT(rt_object_get_type((rt_object_t)thread) == OBJECT_CLASS_THREAD);

    switch (cmd)
    {
    case THREAD_CTRL_CHANGE_PRIORITY:
        /* disable interrupt */
        temp = rt_hw_interrupt_disable();

        /* for ready thread, change queue */
        if ((thread->stat & THREAD_STAT_MASK) == THREAD_READY)
        {
            /* remove thread from schedule queue first */
            rt_schedule_remove_thread(thread);

            /* change thread priority */
            thread->current_priority = *(rt_uint8_t *)arg;

            /* recalculate priority attribute */
#if THREAD_PRIORITY_MAX > 32
            thread->number      = thread->current_priority >> 3;            /* 5bit */
            thread->number_mask = 1 << thread->number;
            thread->high_mask   = 1 << (thread->current_priority & 0x07);   /* 3bit */
#else
            thread->number_mask = 1 << thread->current_priority;
#endif

            /* insert thread to schedule queue again */
            rt_schedule_insert_thread(thread);
        }
        else
        {
            thread->current_priority = *(rt_uint8_t *)arg;

            /* recalculate priority attribute */
#if THREAD_PRIORITY_MAX > 32
            thread->number      = thread->current_priority >> 3;            /* 5bit */
            thread->number_mask = 1 << thread->number;
            thread->high_mask   = 1 << (thread->current_priority & 0x07);   /* 3bit */
#else
            thread->number_mask = 1 << thread->current_priority;
#endif
        }

        /* enable interrupt */
        rt_hw_interrupt_enable(temp);
        break;

    case THREAD_CTRL_STARTUP:
        return rt_thread_startup(thread);

    case THREAD_CTRL_CLOSE:

        if (rt_object_is_systemobject((rt_object_t)thread) == OS_TRUE)
        {
            return rt_thread_detach(thread);
        }
#ifdef ENABLE_MEMORY_HEAP
        else
        {
            return rt_thread_delete(thread);
        }
#endif

#ifdef ENABLE_SMP
    case THREAD_CTRL_BIND_CPU:
    {
        rt_uint8_t cpu;

        if ((thread->stat & THREAD_STAT_MASK) != THREAD_INIT)
        {
            /* we only support bind cpu before started phase. */
            return OS_ERROR;
        }

        cpu = (rt_uint8_t)(size_t)arg;
        thread->bind_cpu = cpu > OS_CPUS_NUMBER? OS_CPUS_NUMBER : cpu;
        break;
    }
#endif /*ENABLE_SMP*/
    default:
        break;
    }

    return OS_EOK;
}
OS_MODE_EXPORT(rt_thread_control);

/**
 * This function will suspend the specified thread.
 *
 * @param thread the thread to be suspended
 *
 * @return the operation status, OS_EOK on OK, -OS_ERROR on error
 *
 * @note if suspend self thread, after this function call, the
 * rt_schedule() must be invoked.
 */
rt_err_t rt_thread_suspend(rt_thread_t thread)
{
    register rt_base_t stat;
    register rt_base_t temp;

    /* thread check */
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT(rt_object_get_type((rt_object_t)thread) == OBJECT_CLASS_THREAD);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_THREAD, ("thread suspend:  %s\n", thread->name));

    stat = thread->stat & THREAD_STAT_MASK;
    if ((stat != THREAD_READY) && (stat != THREAD_RUNNING))
    {
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_THREAD, ("thread suspend: thread disorder, 0x%2x\n",
                                       thread->stat));
        return -OS_ERROR;
    }

    /* disable interrupt */
    temp = rt_hw_interrupt_disable();
    if (stat == THREAD_RUNNING)
    {
        /* not suspend running status thread on other core */
        OS_ASSERT(thread == rt_thread_self());
    }

    /* change thread stat */
    rt_schedule_remove_thread(thread);
    thread->stat = THREAD_SUSPEND | (thread->stat & ~THREAD_STAT_MASK);

    /* stop thread timer anyway */
    rt_timer_stop(&(thread->thread_timer));

    /* enable interrupt */
    rt_hw_interrupt_enable(temp);

    OBJECT_HOOK_CALL(rt_thread_suspend_hook, (thread));
    return OS_EOK;
}
OS_MODE_EXPORT(rt_thread_suspend);

/**
 * This function will resume a thread and put it to system ready queue.
 *
 * @param thread the thread to be resumed
 *
 * @return the operation status, OS_EOK on OK, -OS_ERROR on error
 */
rt_err_t rt_thread_resume(rt_thread_t thread)
{
    register rt_base_t temp;

    /* thread check */
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT(rt_object_get_type((rt_object_t)thread) == OBJECT_CLASS_THREAD);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_THREAD, ("thread resume:  %s\n", thread->name));

    if ((thread->stat & THREAD_STAT_MASK) != THREAD_SUSPEND)
    {
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_THREAD, ("thread resume: thread disorder, %d\n",
                                       thread->stat));

        return -OS_ERROR;
    }

    /* disable interrupt */
    temp = rt_hw_interrupt_disable();

    /* remove from suspend list */
    rt_list_remove(&(thread->tlist));

    rt_timer_stop(&thread->thread_timer);

    /* enable interrupt */
    rt_hw_interrupt_enable(temp);

    /* insert to schedule ready list */
    rt_schedule_insert_thread(thread);

    OBJECT_HOOK_CALL(rt_thread_resume_hook, (thread));
    return OS_EOK;
}
OS_MODE_EXPORT(rt_thread_resume);

/**
 * This function is the timeout function for thread, normally which is invoked
 * when thread is timeout to wait some resource.
 *
 * @param parameter the parameter of thread timeout function
 */
void rt_thread_timeout(void *parameter)
{
    struct rt_thread *thread;

    thread = (struct rt_thread *)parameter;

    /* thread check */
    OS_ASSERT(thread != OS_NULL);
    OS_ASSERT((thread->stat & THREAD_STAT_MASK) == THREAD_SUSPEND);
    OS_ASSERT(rt_object_get_type((rt_object_t)thread) == OBJECT_CLASS_THREAD);

    /* set error number */
    thread->error = -OS_ETIMEOUT;

    /* remove from suspend list */
    rt_list_remove(&(thread->tlist));

    /* insert to schedule ready list */
    rt_schedule_insert_thread(thread);

    /* do schedule */
    rt_schedule();
}
OS_MODE_EXPORT(rt_thread_timeout);

/**
 * This function will find the specified thread.
 *
 * @param name the name of thread finding
 *
 * @return the found thread
 *
 * @note please don't invoke this function in interrupt status.
 */
rt_thread_t rt_thread_find(char *name)
{
    return (rt_thread_t)rt_object_find(name, OBJECT_CLASS_THREAD);
}
OS_MODE_EXPORT(rt_thread_find);

/**@}*/
