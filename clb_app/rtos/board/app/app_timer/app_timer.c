/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* include */

#include <drv_config.h>

#if defined(ENABLE_TIM)
//#include <timer_app.h>
//#include "ad7606_app.h"
#include "bsp_internal_timer.h"
#include "app_timer.h"
#include "app_ad7606_data.h"

/* macro */

/* variable declaration */
//static app_timer_speed_t app_timer_speed = {0};

/* function delcaration */

/**
 * @brief  定时器超时回调函数
 * @param
 * @return if ok return 0 else fail
 * @note
**/
static rt_err_t app_timer_timeout_cb(rt_device_t dev, rt_size_t size)
{
	//    rt_kprintf("this is hwtimer timeout callback fucntion!\n");
	//    rt_kprintf("tick is :%d !\n", rt_tick_get());
	//timer2_interrupt_deal();
	app_ad7606_fill_data();
   return 0;
}

/**
 * @brief  脉冲获取回调函数，处理收到的脉冲数
 * @param
 * @return
 * @note
**/
/*
static void app_timer_capture_cb(void)
{
	app_timer_speed.capture_cnt ++;
	if(app_timer_speed.timeout_flag)
	{
		app_timer_speed.get_counter = app_timer_speed.capture_cnt;
		app_timer_speed.capture_cnt = 0;
		app_timer_speed.timeout_flag = 0;
	}
	rt_kprintf("in bsp_internal_timer_capture_cb get captrue cont!!!!!\n");
}
*/
/**
 * @brief   获取定时器保存的速度信息
 * @param   speed：保存速度信息的结构体地址
 * @return
 * @note
**/
/*
void app_timer_get_speed(app_timer_speed_t* speed)
{
	app_timer_speed_t* speed_p = speed;
	if (OS_NULL == speed)
	{
		rt_kprintf("Error: in bsp_internal_timer_get_speed  speed is NULL!\n");
	}

	speed_p->timeout_flag = app_timer_speed.timeout_flag;
	speed_p->capture_cnt = app_timer_speed.capture_cnt;
	speed_p->get_counter = app_timer_speed.get_counter;
	speed_p->send_counter = app_timer_speed.send_counter;
}
*/
/**
 * @brief  定时器初始化
 * @param
 * @return if ok return 0 else fail
 * @note
**/
int app_timer_init(uint8_t sec,uint32_t usec)
{
    //HAL_TIM_IC_Register_CaptureCallback(app_timer_capture_cb);
    return bsp_internal_timer_init(sec, usec, app_timer_timeout_cb);
}

#endif
