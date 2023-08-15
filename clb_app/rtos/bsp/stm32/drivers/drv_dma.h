/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-10     SummerGift   first version
 * 2020-10-14     Dozingfiretruck   Porting for stm32wbxx
 */

#ifndef __DRV_DMA_H_
#define __DRV_DMA_H_

#include <rtthread.h>
#include "drv_config.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(STM32F0XX) || defined(STM32F1XX) || defined(STM32L0XX) \
	|| defined(STM32L4XX) || defined(STM32G0XX) || defined(STM32G4XX) || defined(SOC_SERIES_STM32WB)
#define DMA_INSTANCE_TYPE              DMA_Channel_TypeDef
#elif defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F7XX)\
	|| defined(STM32H7XX) || defined(SOC_SERIES_STM32MP1)
#define DMA_INSTANCE_TYPE              DMA_Stream_TypeDef
#endif /*  defined(STM32F1XX) || defined(STM32L4XX) */

struct dma_config {
    DMA_INSTANCE_TYPE *Instance;
    rt_uint32_t dma_rcc;
    IRQn_Type dma_irq;

#if defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F7XX)
    rt_uint32_t channel;
#endif

#if defined(STM32L4XX) || defined(STM32G0XX) || defined(STM32G4XX)\
    || defined(STM32H7XX) || defined(SOC_SERIES_STM32MP1) || defined(SOC_SERIES_STM32WB)
    rt_uint32_t request;
#endif
};

#ifdef __cplusplus
}
#endif

#endif /*__DRV_DMA_H_ */
