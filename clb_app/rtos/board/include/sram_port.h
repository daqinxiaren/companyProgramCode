/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-04     zylx         The first version for STM32F4xx
 */

#ifndef __SDRAM_PORT_H__
#define __SDRAM_PORT_H__

#include <drv_config.h>

#if defined(ENABLE_EXTERNAL_SRAM)

#if defined(STM32F1XX)
#include <stm32f1xx_ll_fsmc.h>
#elif  defined(STM32F4XX)
#include <stm32f4xx_ll_fsmc.h>
#endif
/** @formatter:off  */
/* parameters for sdram peripheral */
/* Bank1 */
#if !defined(SRAM_TARGET_BANK)
    #define SRAM_TARGET_BANK            3						//3
#endif

/* stm32f4 Bank1:0x60000000 */
#if !defined(SRAM_BANK_ADDR)
#if SRAM_TARGET_BANK == 1
    #define SRAM_BANK_ADDR              ((uint32_t)0x60000000)
#else
    #define SRAM_BANK_ADDR              ((uint32_t)0x68000000)
#endif
#endif

/* data width: 8, 16, 32 */
#if !defined(SRAM_DATA_WIDTH)
    #define SRAM_DATA_WIDTH             16
#endif

/* sram size */
#if !defined(SRAM_SIZE)
#if     defined(IS61WV102416BLL)
    #define SRAM_SIZE                    ((uint32_t)0x100000)
#elif   defined(IS61WV204816BLL)
    #define SRAM_SIZE                    ((uint32_t)0x400000)
#endif
#endif

/* Timing configuration for IS61WV102416BLL-10MLI */
#if defined (IS61WV102416BLL)
#define ADDRESS_SETUP_TIME               2						//0
#define ADDRESS_HOLD_TIME                1						//0
#define DATA_SETUP_TIME                  1						//9
#define DATA_HOLD_TIME                   1
#define BUS_TURN_AROUND_DURATION         0
#define CLK_DIVISION                     2						//0
#define DATA_LATENCY                     2						//0
#define ACCESS_MODE                      FSMC_ACCESS_MODE_A
/* Timing configuration for IS61WV102416BLL-10MLI */
#endif
#if defined (IS61WV204816BLL)
#define ADDRESS_SETUP_TIME               4						//0
#define ADDRESS_HOLD_TIME                0						//0
#define DATA_SETUP_TIME                  5					   //9 //8
#define DATA_HOLD_TIME                   0
#define BUS_TURN_AROUND_DURATION         0
#define CLK_DIVISION                     15						//0
#define DATA_LATENCY                     16						//0
#define ACCESS_MODE                      FSMC_ACCESS_MODE_A
/* Timing configuration for IS61WV204816BLL-10MLI */
#endif
#endif
/** @formatter:on  */

#endif
