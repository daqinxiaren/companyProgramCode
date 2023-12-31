/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-11     zylx         first version
 */

#ifndef __TIM_CONFIG_H__
#define __TIM_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TIM_DEV_INFO_CONFIG
#define TIM_DEV_INFO_CONFIG                     \
    {                                           \
        .maxfreq = 1000000,                     \
        .minfreq = 3000,                        \
        .maxcnt  = 0xFFFF,                      \
        .cntmode = HWTIMER_CNTMODE_UP,          \
    }
#endif /* TIM_DEV_INFO_CONFIG */

#ifdef ENABLE_TIM1
#ifndef TIM1_CONFIG
#define TIM1_CONFIG                                         \
    {                                                       \
       .tim_handle.Instance     = TIM1,                     \
       .tim_irqn                = TIM1_TRG_COM_TIM11_IRQn,  \
       .name                    = "timer1",                 \
    }
#endif /* TIM1_CONFIG */
#endif /* ENABLE_TIM1 */

#ifdef ENABLE_TIM2
#ifndef TIM2_CONFIG
#define TIM2_CONFIG                                         \
    {                                                       \
       .tim_handle.Instance     = TIM2,                     \
       .tim_irqn                = TIM2_IRQn,                \
       .name                    = "timer2",                 \
    }
#endif /* TIM2_CONFIG */
#endif /* ENABLE_TIM2 */

#ifdef ENABLE_TIM3
#ifndef TIM3_CONFIG
#define TIM3_CONFIG                                         \
    {                                                       \
       .tim_handle.Instance     = TIM3,                     \
       .tim_irqn                = TIM3_IRQn,                \
       .name                    = "timer3",                 \
    }
#endif /* TIM3_CONFIG */
#endif /* ENABLE_TIM3 */

#ifdef ENABLE_TIM11
#ifndef TIM11_CONFIG
#define TIM11_CONFIG                                        \
    {                                                       \
       .tim_handle.Instance     = TIM11,                    \
       .tim_irqn                = TIM1_TRG_COM_TIM11_IRQn,  \
       .name                    = "timer11",                \
    }
#endif /* TIM11_CONFIG */
#endif /* ENABLE_TIM11 */

#ifdef ENABLE_TIM13
#ifndef TIM13_CONFIG
#define TIM13_CONFIG                                        \
    {                                                       \
       .tim_handle.Instance     = TIM13,                    \
       .tim_irqn                = TIM8_UP_TIM13_IRQn,       \
       .name                    = "timer13",                \
    }
#endif /* TIM13_CONFIG */
#endif /* ENABLE_TIM13 */

#ifdef ENABLE_TIM14
#ifndef TIM14_CONFIG
#define TIM14_CONFIG                                        \
    {                                                       \
       .tim_handle.Instance     = TIM14,                    \
       .tim_irqn                = TIM8_TRG_COM_TIM14_IRQn,  \
       .name                    = "timer14",                \
    }
#endif /* TIM14_CONFIG */
#endif /* ENABLE_TIM14 */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_CONFIG_H__ */
