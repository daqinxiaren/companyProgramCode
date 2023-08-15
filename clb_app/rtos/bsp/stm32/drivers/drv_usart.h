/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018.10.30     SummerGift   first version
 * 2019.03.05     whj4674672   add stm32h7
 * 2020-10-14     Dozingfiretruck   Porting for stm32wbxx
 */

#ifndef __DRV_USART_H__
#define __DRV_USART_H__

#include "drv_config.h"
#ifdef ENABLE_SERIAL
#include <rtthread.h>
#include <rthw.h>
#include <drv_common.h>
#include "drv_dma.h"

#ifdef __cplusplus
extern "C" {
#endif

int rt_hw_usart_init(void);

#if defined(STM32F0XX) || defined(STM32F1XX) || defined(STM32L4XX) \
    || defined(STM32L0XX) || defined(STM32G0XX) || defined(STM32G4XX) || defined(SOC_SERIES_STM32WB)
#define DMA_INSTANCE_TYPE              DMA_Channel_TypeDef
#elif defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F7XX) \
    || defined(STM32H7XX) || defined(SOC_SERIES_STM32MP1)
#define DMA_INSTANCE_TYPE              DMA_Stream_TypeDef
#endif /*  defined(STM32F1XX) || defined(STM32L4XX) */

#if defined(STM32F1XX) || defined(STM32L4XX) || defined(STM32F2XX) \
    || defined(STM32F4XX) || defined(STM32L0XX) || defined(STM32G0XX) \
    || defined(STM32G4XX) || defined(SOC_SERIES_STM32WB)
#define UART_INSTANCE_CLEAR_FUNCTION    __HAL_UART_CLEAR_FLAG
#elif defined(STM32F7XX) || defined(STM32F0XX) || defined(STM32H7XX) \
    || defined(SOC_SERIES_STM32MP1)
#define UART_INSTANCE_CLEAR_FUNCTION    __HAL_UART_CLEAR_IT
#endif

/* stm32 config class */
struct stm32_uart_config
{
    const char *name;
    USART_TypeDef *Instance;
    IRQn_Type irq_type;
    struct dma_config *dma_rx;
    struct dma_config *dma_tx;
};

/* stm32 uart dirver class */
struct stm32_uart
{
    UART_HandleTypeDef handle;
    struct stm32_uart_config *config;

#ifdef DEV_SERIAL_ENABLE_DMA
    struct
    {
        DMA_HandleTypeDef handle;
        rt_size_t last_index;
    } dma_rx;
    struct
    {
        DMA_HandleTypeDef handle;
    } dma_tx;
#endif
    rt_uint16_t uart_dma_flag;
    struct rt_serial_device serial;
};

#ifdef __cplusplus
}
#endif

#endif

#endif  /* __DRV_USART_H__ */
