/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-7      SummerGift   first version
 */

#include "drv_common.h"
#include "drv_gpio.h"
#include "drv_clk.h"
#ifdef ENABLE_SERIAL
#include "drv_usart.h"
#endif

#include <drv_flash/drv_onchip_flash.h>


#ifdef ENABLE_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv)
{
    rt_hw_cpu_reset();
}
FINSH_FUNCTION_EXPORT_ALIAS(reboot, __cmd_reboot, Reboot System);
#endif /* ENABLE_FINSH */

/* SysTick configuration */
void rt_hw_systick_init(void)
{
#if defined (STM32H7XX)
    HAL_SYSTICK_Config((HAL_RCCEx_GetD1SysClockFreq()) / TICK_PER_SECOND);
#elif defined (SOC_SERIES_STM32MP1)
    HAL_SYSTICK_Config(HAL_RCC_GetSystemCoreClockFreq() / TICK_PER_SECOND);
#else
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / TICK_PER_SECOND);
#endif
#if !defined (SOC_SERIES_STM32MP1)
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
#endif
    NVIC_SetPriority(SysTick_IRQn, 0xFF);
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_IncTick();
    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

uint32_t HAL_GetTick(void)
{
    return rt_tick_get_millisecond();
}

void HAL_SuspendTick(void)
{
}

void HAL_ResumeTick(void)
{
}

void HAL_Delay(__IO uint32_t Delay)
{
    if (rt_thread_self())
    {
        rt_thread_mdelay(Delay);
    }
    else
    {
        for (rt_uint32_t count = 0; count < Delay; count++)
        {
            rt_hw_us_delay(1000);
        }
    }
}

/* re-implement tick interface for STM32 HAL */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char *s, int num)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler */
}

/**
 * This function will delay for some us.
 *
 * @param us the delay time of us
 */
void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t start, now, delta, reload, us_tick;
    start = SysTick->VAL;
    reload = SysTick->LOAD;
    us_tick = SystemCoreClock / 1000000UL;
    do
    {
        now = SysTick->VAL;
        delta = start >= now ? start - now : reload + start - now;
    }
    while (delta < us_tick * us);
}

/**
 * This function will initial STM32 board.
 */
OS_WEAK void rt_hw_board_init()
{
#ifdef SCB_EnableICache
    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();
#endif

#ifdef SCB_EnableDCache
    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();
#endif

    /* HAL_Init() function is called at the beginning of the program */
    HAL_Init();

    /* enable interrupt */
    __set_PRIMASK(0);
    /* System clock initialization */
    SystemClock_Config();
    /* disable interrupt */
    __set_PRIMASK(1);

    rt_hw_systick_init();

    /* Heap initialization */
#if defined(ENABLE_MEMORY_HEAP)
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif

    /* Pin driver initialization is open by default */
#ifdef ENABLE_PIN
    rt_hw_pin_init();
#endif

    /* USART driver initialization is open by default */
#ifdef ENABLE_SERIAL
    rt_hw_usart_init();
#endif

    /* Set the shell console output device */
#ifdef ENABLE_CONSOLE
    rt_console_set_device(CONSOLE_DEVICE_NAME);
#endif

    /* Board underlying hardware initialization */
#ifdef ENABLE_COMPONENTS_INIT
    rt_components_board_init();
#endif
}

