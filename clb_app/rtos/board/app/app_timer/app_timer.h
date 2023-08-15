/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_TIMER_H__
#define __APP_TIMER_H__

/* include */
#include <stm32f4xx.h>
#include <rtthread.h>
#include "bsp_internal_timer.h"

/* macro */
/* STM32F407XGT6*/

/* type declaration */
typedef struct
{
	uint8_t timeout_flag;
	uint32_t capture_cnt;
	uint32_t get_counter;
	uint32_t send_counter;
}app_timer_speed_t;

/* function declaration */
/*初始化定时器*/
int app_timer_init(uint8_t sec,uint32_t usec);
/*获取定时器保存的速度信息*/
void app_timer_get_speed(app_timer_speed_t* speed);
/*注册脉冲处理函数*/
extern void HAL_TIM_IC_Register_CaptureCallback(void (* pCallback)(void));

#endif /*__APP_TIMER_H__*/


