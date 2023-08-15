/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017/10/5      Bernard      the first version
 * 2018/09/17     Jesven       fix: in _signal_deliver THREAD_STAT_MASK to THREAD_STAT_SIGNAL_MASK
 * 2018/11/22     Jesven       in smp version rt_hw_context_switch_to add a param
 */

#include <stdint.h>
#include <string.h>

#include <rthw.h>
#include <rtthread.h>

#ifdef ENABLE_SIGNALS

#ifndef SIGNAL_INFO_MAX
#define SIGNAL_INFO_MAX 32
#endif

#define DBG_TAG     "SIGN"
#define DBG_LVL     DBG_WARNING
#include <rtdbg.h>

#define sig_mask(sig_no)    (1u << sig_no)
#define sig_valid(sig_no)   (sig_no >= 0 && sig_no < SIGNAL_MAX)

struct siginfo_node
{
    siginfo_t si;
    struct rt_slist_node list;
};

static struct rt_mempool *_rt_siginfo_pool;
static void _signal_deliver(rt_thread_t tid);
void rt_thread_handle_sig(rt_bool_t clean_state);

static void _signal_default_handler(int signo)
{
    LOG_I("handled signo[%d] with default action.", signo);
    return ;
}

static void _signal_entry(void *parameter)
{
    rt_thread_t tid = rt_thread_self();

    /* handle signal */
    rt_thread_handle_sig(OS_FALSE);

#ifdef ENABLE_SMP
    {
        struct rt_cpu* pcpu = rt_cpu_self();

        pcpu->current_thread->cpus_lock_nest--;
        if (pcpu->current_thread->cpus_lock_nest == 0)
        {
            pcpu->current_thread->scheduler_lock_nest--;
        }

    }
#else
    /* return to thread */
    tid->sp = tid->sig_ret;
    tid->sig_ret = OS_NULL;
#endif

    LOG_D("switch back to: 0x%08x\n", tid->sp);
    tid->stat &= ~THREAD_STAT_SIGNAL;

#ifdef ENABLE_SMP
    rt_hw_context_switch_to((rt_base_t)&parameter, tid);
#else
    rt_hw_context_switch_to((rt_ubase_t)&(tid->sp));
#endif /*ENABLE_SMP*/
}

/*
 * To deliver a signal to thread, there are cases:
 * 1. When thread is suspended, function resumes thread and
 * set signal stat;
 * 2. When thread is ready:
 *   - If function delivers a signal to self thread, just handle
 *    it.
 *   - If function delivers a signal to another ready thread, OS
 *    should build a slice context to handle it.
 */
static void _signal_deliver(rt_thread_t tid)
{
    rt_ubase_t level;

    level = rt_hw_interrupt_disable();

    /* thread is not interested in pended signals */
    if (!(tid->sig_pending & tid->sig_mask))
    {
        rt_hw_interrupt_enable(level);
        return;
    }

    if ((tid->stat & THREAD_STAT_MASK) == THREAD_SUSPEND)
    {
        /* resume thread to handle signal */
        rt_thread_resume(tid);
        /* add signal state */
        tid->stat |= (THREAD_STAT_SIGNAL | THREAD_STAT_SIGNAL_PENDING);

        rt_hw_interrupt_enable(level);

        /* re-schedule */
        rt_schedule();
    }
    else
    {
        if (tid == rt_thread_self())
        {
            /* add signal state */
            tid->stat |= THREAD_STAT_SIGNAL;

            rt_hw_interrupt_enable(level);

            /* do signal action in self thread context */
            if (rt_interrupt_get_nest() == 0)
            {
                rt_thread_handle_sig(OS_TRUE);
            }
        }
        else if (!((tid->stat & THREAD_STAT_SIGNAL_MASK) & THREAD_STAT_SIGNAL))
        {
            /* add signal state */
            tid->stat |= (THREAD_STAT_SIGNAL | THREAD_STAT_SIGNAL_PENDING);

#ifdef ENABLE_SMP
            {
                int cpu_id;

                cpu_id = tid->oncpu;
                if ((cpu_id != OS_CPU_DETACHED) && (cpu_id != rt_hw_cpu_id()))
                {
                    rt_uint32_t cpu_mask;

                    cpu_mask = OS_CPU_MASK ^ (1 << cpu_id);
                    rt_hw_ipi_send(OS_SCHEDULE_IPI, cpu_mask);
                }
            }
#else
            /* point to the signal handle entry */
            tid->stat &= ~THREAD_STAT_SIGNAL_PENDING;
            tid->sig_ret = tid->sp;
            tid->sp = rt_hw_stack_init((void *)_signal_entry, OS_NULL,
                                       (void *)((char *)tid->sig_ret - 32), OS_NULL);
#endif

            rt_hw_interrupt_enable(level);
            LOG_D("signal stack pointer @ 0x%08x", tid->sp);

            /* re-schedule */
            rt_schedule();
        }
        else
        {
            rt_hw_interrupt_enable(level);
        }
    }
}

#ifdef ENABLE_SMP
void *rt_signal_check(void* context)
{
    rt_base_t level;
    int cpu_id;
    struct rt_cpu* pcpu;
    struct rt_thread *current_thread;

    level = rt_hw_interrupt_disable();
    cpu_id = rt_hw_cpu_id();
    pcpu   = rt_cpu_index(cpu_id);
    current_thread = pcpu->current_thread;

    if (pcpu->irq_nest)
    {
        rt_hw_interrupt_enable(level);
        return context;
    }

    if (current_thread->cpus_lock_nest == 1)
    {
        if (current_thread->stat & THREAD_STAT_SIGNAL_PENDING)
        {
            void *sig_context;

            current_thread->stat &= ~THREAD_STAT_SIGNAL_PENDING;

            rt_hw_interrupt_enable(level);
            sig_context = rt_hw_stack_init((void *)_signal_entry, context,
                    (void *)(context - 32), OS_NULL);
            return sig_context;
        }
    }
    rt_hw_interrupt_enable(level);
    return context;
}
#endif

rt_sighandler_t rt_signal_install(int signo, rt_sighandler_t handler)
{
    rt_base_t level;
    rt_sighandler_t old = OS_NULL;
    rt_thread_t tid = rt_thread_self();

    if (!sig_valid(signo)) return SIG_ERR;

    level = rt_hw_interrupt_disable();
    if (tid->sig_vectors == OS_NULL)
    {
        rt_thread_alloc_sig(tid);
    }

    if (tid->sig_vectors)
    {
        old = tid->sig_vectors[signo];

        if (handler == SIG_IGN) tid->sig_vectors[signo] = OS_NULL;
        else if (handler == SIG_DFL) tid->sig_vectors[signo] = _signal_default_handler;
        else tid->sig_vectors[signo] = handler;
    }
    rt_hw_interrupt_enable(level);

    return old;
}

void rt_signal_mask(int signo)
{
    rt_base_t level;
    rt_thread_t tid = rt_thread_self();

    level = rt_hw_interrupt_disable();

    tid->sig_mask &= ~sig_mask(signo);

    rt_hw_interrupt_enable(level);
}

void rt_signal_unmask(int signo)
{
    rt_base_t level;
    rt_thread_t tid = rt_thread_self();

    level = rt_hw_interrupt_disable();

    tid->sig_mask |= sig_mask(signo);

    /* let thread handle pended signals */
    if (tid->sig_mask & tid->sig_pending)
    {
        rt_hw_interrupt_enable(level);
        _signal_deliver(tid);
    }
    else
    {
        rt_hw_interrupt_enable(level);
    }
}

int rt_signal_wait(const rt_sigset_t *set, rt_siginfo_t *si, rt_int32_t timeout)
{
    int ret = OS_EOK;
    rt_base_t   level;
    rt_thread_t tid = rt_thread_self();
    struct siginfo_node *si_node = OS_NULL, *si_prev = OS_NULL;

    /* current context checking */
    ENABLE_DEBUG_IN_THREAD_CONTEXT;

    /* parameters check */
    if (set == NULL || *set == 0 || si == NULL )
    {
        ret = -OS_EINVAL;
        goto __done_return;
    }

    /* clear siginfo to avoid unknown value */
    memset(si, 0x0, sizeof(rt_siginfo_t));

    level = rt_hw_interrupt_disable();

    /* already pending */
    if (tid->sig_pending & *set) goto __done;

    if (timeout == 0)
    {
        ret = -OS_ETIMEOUT;
        goto __done_int;
    }

    /* suspend self thread */
    rt_thread_suspend(tid);
    /* set thread stat as waiting for signal */
    tid->stat |= THREAD_STAT_SIGNAL_WAIT;

    /* start timeout timer */
    if (timeout != WAITING_FOREVER)
    {
        /* reset the timeout of thread timer and start it */
        rt_timer_control(&(tid->thread_timer),
                         TIMER_CTRL_SET_TIME,
                         &timeout);
        rt_timer_start(&(tid->thread_timer));
    }
    rt_hw_interrupt_enable(level);

    /* do thread scheduling */
    rt_schedule();

    level = rt_hw_interrupt_disable();

    /* remove signal waiting flag */
    tid->stat &= ~THREAD_STAT_SIGNAL_WAIT;

    /* check errno of thread */
    if (tid->error == -OS_ETIMEOUT)
    {
        tid->error = OS_EOK;
        rt_hw_interrupt_enable(level);

        /* timer timeout */
        ret = -OS_ETIMEOUT;
        goto __done_return;
    }

__done:
    /* to get the first matched pending signals */
    si_node = (struct siginfo_node *)tid->si_list;
    while (si_node)
    {
        int signo;

        signo = si_node->si.si_signo;
        if (sig_mask(signo) & *set)
        {
            *si  = si_node->si;

            LOG_D("sigwait: %d sig raised!", signo);
            if (si_prev) si_prev->list.next = si_node->list.next;
            else
            {
                struct siginfo_node *node_next;

                if (si_node->list.next)
                {
                    node_next = (void *)rt_slist_entry(si_node->list.next, struct siginfo_node, list);
                    tid->si_list = node_next;
                }
                else
                {
                    tid->si_list = OS_NULL;
                }
            }

            /* clear pending */
            tid->sig_pending &= ~sig_mask(signo);
            rt_mp_free(si_node);
            break;
        }

        si_prev = si_node;
        if (si_node->list.next)
        {
            si_node = (void *)rt_slist_entry(si_node->list.next, struct siginfo_node, list);
        }
        else
        {
            si_node = OS_NULL;
        }
     }

__done_int:
    rt_hw_interrupt_enable(level);

__done_return:
    return ret;
}

void rt_thread_handle_sig(rt_bool_t clean_state)
{
    rt_base_t level;

    rt_thread_t tid = rt_thread_self();
    struct siginfo_node *si_node;

    level = rt_hw_interrupt_disable();
    if (tid->sig_pending & tid->sig_mask)
    {
        /* if thread is not waiting for signal */
        if (!(tid->stat & THREAD_STAT_SIGNAL_WAIT))
        {
            while (tid->sig_pending & tid->sig_mask)
            {
                int signo, error;
                rt_sighandler_t handler;

                si_node = (struct siginfo_node *)tid->si_list;
                if (!si_node) break;

                /* remove this sig info node from list */
                if (si_node->list.next == OS_NULL)
                    tid->si_list = OS_NULL;
                else
                    tid->si_list = (void *)rt_slist_entry(si_node->list.next, struct siginfo_node, list);

                signo   = si_node->si.si_signo;
                handler = tid->sig_vectors[signo];
                tid->sig_pending &= ~sig_mask(signo);
                rt_hw_interrupt_enable(level);

                LOG_D("handle signal: %d, handler 0x%08x", signo, handler);
                if (handler) handler(signo);

                level = rt_hw_interrupt_disable();
                error = -OS_EINTR;

                rt_mp_free(si_node); /* release this siginfo node */
                /* set errno in thread tcb */
                tid->error = error;
            }

            /* whether clean signal status */
            if (clean_state == OS_TRUE)
            {
                tid->stat &= ~THREAD_STAT_SIGNAL;
            }
            else
            {
                return;
            }
        }
    }
    rt_hw_interrupt_enable(level);
}

void rt_thread_alloc_sig(rt_thread_t tid)
{
    int index;
    rt_base_t level;
    rt_sighandler_t *vectors;

    vectors = (rt_sighandler_t *)KERNEL_MALLOC(sizeof(rt_sighandler_t) * SIGNAL_MAX);
    OS_ASSERT(vectors != OS_NULL);

    for (index = 0; index < SIGNAL_MAX; index ++)
    {
        vectors[index] = _signal_default_handler;
    }

    level = rt_hw_interrupt_disable();
    tid->sig_vectors = vectors;
    rt_hw_interrupt_enable(level);
}

void rt_thread_free_sig(rt_thread_t tid)
{
    rt_base_t level;
    struct siginfo_node *si_node;
    rt_sighandler_t *sig_vectors;

    level = rt_hw_interrupt_disable();
    si_node = (struct siginfo_node *)tid->si_list;
    tid->si_list = OS_NULL;

    sig_vectors = tid->sig_vectors;
    tid->sig_vectors = OS_NULL;
    rt_hw_interrupt_enable(level);

    if (si_node)
    {
        struct rt_slist_node *node;
        struct rt_slist_node *node_to_free;

        LOG_D("free signal info list");
        node = &(si_node->list);
        do
        {
            node_to_free = node;
            node = node->next;
            si_node = rt_slist_entry(node_to_free, struct siginfo_node, list);
            rt_mp_free(si_node);
        } while (node);
    }

    if (sig_vectors)
    {
        KERNEL_FREE(sig_vectors);
    }
}

int rt_thread_kill(rt_thread_t tid, int sig)
{
    siginfo_t si;
    rt_base_t level;
    struct siginfo_node *si_node;

    OS_ASSERT(tid != OS_NULL);
    if (!sig_valid(sig)) return -OS_EINVAL;

    LOG_I("send signal: %d", sig);
    si.si_signo = sig;
    si.si_code  = SI_USER;
    si.si_value.sival_ptr = OS_NULL;

    level = rt_hw_interrupt_disable();
    if (tid->sig_pending & sig_mask(sig))
    {
        /* whether already emits this signal? */
        struct rt_slist_node *node;
        struct siginfo_node  *entry;

        si_node = (struct siginfo_node *)tid->si_list;
        if (si_node)
            node = (struct rt_slist_node *)&si_node->list;
        else
            node = OS_NULL;

        /* update sig info */
        for (; (node) != OS_NULL; node = node->next)
        {
            entry = rt_slist_entry(node, struct siginfo_node, list);
            if (entry->si.si_signo == sig)
            {
                memcpy(&(entry->si), &si, sizeof(siginfo_t));
                rt_hw_interrupt_enable(level);
                return 0;
            }
        }
    }
    rt_hw_interrupt_enable(level);

    si_node = (struct siginfo_node *) rt_mp_alloc(_rt_siginfo_pool, 0);
    if (si_node)
    {
        rt_slist_init(&(si_node->list));
        memcpy(&(si_node->si), &si, sizeof(siginfo_t));

        level = rt_hw_interrupt_disable();

        if (tid->si_list)
        {
            struct siginfo_node *si_list;

            si_list = (struct siginfo_node *)tid->si_list;
            rt_slist_append(&(si_list->list), &(si_node->list));
        }
        else
        {
            tid->si_list = si_node;
        }

        /* a new signal */
        tid->sig_pending |= sig_mask(sig);

        rt_hw_interrupt_enable(level);
    }
    else
    {
        LOG_E("The allocation of signal info node failed.");
    }

    /* deliver signal to this thread */
    _signal_deliver(tid);

    return OS_EOK;
}

int rt_system_signal_init(void)
{
    _rt_siginfo_pool = rt_mp_create("signal", SIGNAL_INFO_MAX, sizeof(struct siginfo_node));
    if (_rt_siginfo_pool == OS_NULL)
    {
        LOG_E("create memory pool for signal info failed.");
        OS_ASSERT(0);
    }

    return 0;
}

#endif
