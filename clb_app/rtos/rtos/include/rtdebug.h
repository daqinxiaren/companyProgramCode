/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __RTDEBUG_H__
#define __RTDEBUG_H__

#include <rtconfig.h>

/* settings depend check */
#ifdef ENABLE_POSIX
#if !defined(ENABLE_DFS) || !defined(ENABLE_DFS_DEVFS)
#error "POSIX poll/select, stdin need file system(ENABLE_DFS) and device file system(ENABLE_DFS_DEVFS)"
#endif

#if !defined(ENABLE_LIBC)
#error "POSIX layer need standard C library(ENABLE_LIBC)"
#endif

#endif

#ifdef ENABLE_POSIX_TERMIOS
#if !defined(ENABLE_POSIX)
#error "termios need POSIX layer(ENABLE_POSIX)"
#endif
#endif

/* Using this macro to control all kernel debug features. */
#ifdef ENABLE_DEBUG

/* Turn on some of these (set to non-zero) to debug kernel */
#ifndef ENABLE_DEBUG_MEM
#define ENABLE_DEBUG_MEM                   0
#endif

#ifndef ENABLE_DEBUG_MEMHEAP
#define ENABLE_DEBUG_MEMHEAP               0
#endif

#ifndef ENABLE_DEBUG_MODULE
#define ENABLE_DEBUG_MODULE                0
#endif

#ifndef ENABLE_DEBUG_SCHEDULER
#define ENABLE_DEBUG_SCHEDULER             0
#endif

#ifndef ENABLE_DEBUG_SLAB
#define ENABLE_DEBUG_SLAB                  0
#endif

#ifndef ENABLE_DEBUG_THREAD
#define ENABLE_DEBUG_THREAD                0
#endif

#ifndef ENABLE_DEBUG_TIMER
#define ENABLE_DEBUG_TIMER                 0
#endif

#ifndef ENABLE_DEBUG_IRQ
#define ENABLE_DEBUG_IRQ                   0
#endif

#ifndef ENABLE_DEBUG_IPC
#define ENABLE_DEBUG_IPC                   0
#endif

#ifndef ENABLE_DEBUG_INIT
#define ENABLE_DEBUG_INIT                  0
#endif

/* Turn on this to enable context check */
#ifndef ENABLE_DEBUG_CONTEXT_CHECK
#define ENABLE_DEBUG_CONTEXT_CHECK         1
#endif

#define ENABLE_DEBUG_LOG(type, message)                                           \
do                                                                            \
{                                                                             \
    if (type)                                                                 \
        rt_kprintf message;                                                   \
}                                                                             \
while (0)

#define OS_ASSERT(EX)                                                         \
if (!(EX))                                                                    \
{                                                                             \
    rt_assert_handler(#EX, __FUNCTION__, __LINE__);                           \
}

/* Macro to check current context */
#if ENABLE_DEBUG_CONTEXT_CHECK
#define ENABLE_DEBUG_NOT_IN_INTERRUPT                                             \
do                                                                            \
{                                                                             \
    rt_base_t level;                                                          \
    level = rt_hw_interrupt_disable();                                        \
    if (rt_interrupt_get_nest() != 0)                                         \
    {                                                                         \
        rt_kprintf("Function[%s] shall not be used in ISR\n", __FUNCTION__);  \
        OS_ASSERT(0)                                                          \
    }                                                                         \
    rt_hw_interrupt_enable(level);                                            \
}                                                                             \
while (0)

/* "In thread context" means:
 *     1) the scheduler has been started
 *     2) not in interrupt context.
 */
#define ENABLE_DEBUG_IN_THREAD_CONTEXT                                            \
do                                                                            \
{                                                                             \
    rt_base_t level;                                                          \
    level = rt_hw_interrupt_disable();                                        \
    if (rt_thread_self() == OS_NULL)                                          \
    {                                                                         \
        rt_kprintf("Function[%s] shall not be used before scheduler start\n", \
                   __FUNCTION__);                                             \
        OS_ASSERT(0)                                                          \
    }                                                                         \
    ENABLE_DEBUG_NOT_IN_INTERRUPT;                                                \
    rt_hw_interrupt_enable(level);                                            \
}                                                                             \
while (0)
#else
#define ENABLE_DEBUG_NOT_IN_INTERRUPT
#define ENABLE_DEBUG_IN_THREAD_CONTEXT
#endif

#else /* ENABLE_DEBUG */

#define OS_ASSERT(EX)
#define ENABLE_DEBUG_LOG(type, message)
#define ENABLE_DEBUG_NOT_IN_INTERRUPT
#define ENABLE_DEBUG_IN_THREAD_CONTEXT

#endif /* ENABLE_DEBUG */

#endif /* __RTDEBUG_H__ */
