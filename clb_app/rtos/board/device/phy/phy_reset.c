/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-23     flybreak     first version
 */

#include <drv_config.h>
#include <drv_gpio.h>

#if defined(ENABLE_ETH)

#ifdef LH_TFDSN_CLB
/**
    PC1     ------> ETH_MDC
    PA1     ------> ETH_REF_CLK
    PA2     ------> ETH_MDIO
    PA7     ------> ETH_CRS_DV
    PC4     ------> ETH_RXD0
    PC5     ------> ETH_RXD1
    PG11     ------> ETH_TX_EN
    PG13     ------> ETH_TXD0
    PG14     ------> ETH_TXD1
    PA0      ------> ETH_RESET
 *
 */
#define RESET_IO 			GET_PIN(A, 0)
#endif

void phy_reset(void)
{
	rt_pin_write(RESET_IO, PIN_LOW);
	rt_thread_mdelay(50);
	rt_pin_write(RESET_IO, PIN_HIGH);
	rt_kprintf("\tPHY init\t\t\t [reset]!\r\n");
}
//INIT_BOARD_EXPORT(phy_reset);

int phy_init(void)
{
	rt_pin_mode(RESET_IO, PIN_MODE_OUTPUT);
	rt_pin_write(RESET_IO, PIN_HIGH);
	rt_kprintf("NOTO:\r\n");
	rt_kprintf("\tPHY init\t\t\t [success]!\r\n");
	return OS_EOK;
}

INIT_BOARD_EXPORT(phy_init);

#endif
