/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __BSP_INTERNAL_TIMER_H__
#define __BSP_INTERNAL_TIMER_H__

/* include */
#include <stm32f4xx.h>
#include <rtthread.h>
#include "app_board_config.h"

/* macro */
/* STM32F407XGT6*/
#define BSP_HWTIMER_DEV_NAME   "timer2"     /* 定时器名称 */
#define BSP_HWTIMER_TIMX     	 TIM2

/* type declaration */

/* function declaration */
/*初始化定时器*/
int bsp_internal_hwtimer_init(uint8_t sec,uint32_t usec);

#endif /*__BSP_INTERNAL_TIMER_H__*/


