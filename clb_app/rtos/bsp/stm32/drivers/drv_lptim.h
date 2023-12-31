/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author          Notes
 * 2019-05-06     Zero-Free       first version
 */

#ifndef  __DRV_PMTIMER_H__
#define  __DRV_PMTIMER_H__

#include <rtthread.h>
#include "drv_config.h"

#ifdef ENABLE_PM

#ifdef __cplusplus
extern "C" {
#endif

rt_uint32_t stm32l4_lptim_get_countfreq(void);
rt_uint32_t stm32l4_lptim_get_tick_max(void);
rt_uint32_t stm32l4_lptim_get_current_tick(void);

rt_err_t stm32l4_lptim_start(rt_uint32_t load);
void stm32l4_lptim_stop(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* __DRV_PMTIMER_H__ */
