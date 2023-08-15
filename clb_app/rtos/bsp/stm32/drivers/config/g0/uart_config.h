/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     zylx         first version
 */

#ifndef __UART_CONFIG_H__
#define __UART_CONFIG_H__

#include <rtthread.h>

#if defined(ENABLE_LPUART1)
#ifndef LPUART1_CONFIG
#define LPUART1_CONFIG                                              \
    {                                                               \
        .name = "lpuart1",                                          \
        .Instance = LPUART1,                                        \
        .irq_type = USART3_4_LPUART1_IRQn,                          \
    }
#define LPUART1_IRQHandler      USART3_4_LPUART1_IRQHandler
#endif /* LPUART1_CONFIG */
#if defined(BSP_LPUART1_RX_ENABLE_DMA)
#ifndef LPUART1_DMA_CONFIG
#define LPUART1_DMA_CONFIG                                          \
    {                                                               \
        .Instance = LPUART1_RX_DMA_INSTANCE,                        \
        .request =  LPUART1_RX_DMA_REQUEST,                         \
        .dma_rcc  = LPUART1_RX_DMA_RCC,                             \
        .dma_irq  = LPUART1_RX_DMA_IRQ,                             \
    }
#endif /* LPUART1_DMA_CONFIG */
#endif /* BSP_LPUART1_RX_ENABLE_DMA */
#endif /* ENABLE_LPUART1 */

#if defined(ENABLE_UART1)
#ifndef UART1_CONFIG
#define UART1_CONFIG                                                \
    {                                                               \
        .name = "uart1",                                            \
        .Instance = USART1,                                         \
        .irq_type = USART1_IRQn,                                    \
    }
#endif /* UART1_CONFIG */
#endif /* ENABLE_UART1 */

#if defined(BSP_UART1_RX_ENABLE_DMA)
#ifndef UART1_DMA_RX_CONFIG
#define UART1_DMA_RX_CONFIG                                            \
    {                                                               \
        .Instance = UART1_RX_DMA_INSTANCE,                          \
        .request =  UART1_RX_DMA_REQUEST,                           \
        .dma_rcc  = UART1_RX_DMA_RCC,                               \
        .dma_irq  = UART1_RX_DMA_IRQ,                               \
    }
#endif /* UART1_DMA_RX_CONFIG */
#endif /* BSP_UART1_RX_ENABLE_DMA */

#if defined(ENABLE_UART2)
#ifndef UART2_CONFIG
#define UART2_CONFIG                                                \
    {                                                               \
        .name = "uart2",                                            \
        .Instance = USART2,                                         \
        .irq_type = USART2_IRQn,                                    \
    }
#endif /* UART2_CONFIG */
#endif /* ENABLE_UART2 */

#if defined(BSP_UART2_RX_ENABLE_DMA)
#ifndef UART2_DMA_RX_CONFIG
#define UART2_DMA_RX_CONFIG                                            \
    {                                                               \
        .Instance = UART2_RX_DMA_INSTANCE,                          \
        .request =  UART2_RX_DMA_REQUEST,                           \
        .dma_rcc  = UART2_RX_DMA_RCC,                               \
        .dma_irq  = UART2_RX_DMA_IRQ,                               \
    }
#endif /* UART2_DMA_RX_CONFIG */
#endif /* BSP_UART2_RX_ENABLE_DMA */

#if defined(ENABLE_UART3)
#ifndef UART3_CONFIG
#ifndef STM32G0XX
#define UART3_CONFIG                                                \
    {                                                               \
        .name = "uart3",                                            \
        .Instance = USART3,                                         \
        .irq_type = USART3_IRQn,                                    \
    }
#else
#define UART3_CONFIG                                                \
    {                                                               \
        .name = "uart3",                                            \
        .Instance = USART3,                                         \
        .irq_type = USART3_4_LPUART1_IRQn,                          \
    }
#endif /* STM32G0XX */
#endif /* UART3_CONFIG */
#endif /* ENABLE_UART3 */

#if defined(BSP_UART3_RX_ENABLE_DMA)
#ifndef UART3_DMA_RX_CONFIG
#define UART3_DMA_RX_CONFIG                                            \
    {                                                               \
        .Instance = UART3_RX_DMA_INSTANCE,                          \
        .request =  UART3_RX_DMA_REQUEST,                           \
        .dma_rcc  = UART3_RX_DMA_RCC,                               \
        .dma_irq  = UART3_RX_DMA_IRQ,                               \
    }
#endif /* UART3_DMA_RX_CONFIG */
#endif /* BSP_UART3_RX_ENABLE_DMA */

#if defined(ENABLE_UART4)
#ifndef UART4_CONFIG
#ifndef STM32G0XX
#define UART4_CONFIG                                                \
    {                                                               \
        .name = "uart4",                                            \
        .Instance = UART4,                                          \
        .irq_type = UART4_IRQn,                                     \
    }
#else
#define UART4_CONFIG                                                \
    {                                                               \
        .name = "uart4",                                            \
        .Instance = USART4,                                         \
        .irq_type = USART3_4_LPUART1_IRQn,                          \
    }
#endif /* UART4_CONFIG */
#endif /* STM32G0XX */
#endif /* ENABLE_UART4 */

#if defined(BSP_UART4_RX_ENABLE_DMA)
#ifndef UART4_DMA_RX_CONFIG
#define UART4_DMA_RX_CONFIG                                            \
    {                                                               \
        .Instance = UART4_RX_DMA_INSTANCE,                          \
        .request =  UART4_RX_DMA_REQUEST,                           \
        .dma_rcc  = UART4_RX_DMA_RCC,                               \
        .dma_irq  = UART4_RX_DMA_IRQ,                               \
    }
#endif /* UART4_DMA_RX_CONFIG */
#endif /* BSP_UART4_RX_ENABLE_DMA */

#if defined(ENABLE_UART5)
#ifndef UART5_CONFIG
#define UART5_CONFIG                                                \
    {                                                               \
        .name = "uart5",                                            \
        .Instance = UART5,                                          \
        .irq_type = UART5_IRQn,                                     \
    }
#endif /* UART5_CONFIG */
#endif /* ENABLE_UART5 */

#if defined(BSP_UART5_RX_ENABLE_DMA)
#ifndef UART5_DMA_RX_CONFIG
#define UART5_DMA_RX_CONFIG                                            \
    {                                                               \
        .Instance = DMA_NOT_AVAILABLE,                              \
    }
#endif /* UART5_DMA_RX_CONFIG */
#endif /* BSP_UART5_RX_ENABLE_DMA */

#ifdef __cplusplus
}
#endif

#endif /* __UART_CONFIG_H__ */
