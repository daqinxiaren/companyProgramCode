/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* includes */
#include "app_led_control.h"
#include "led.h"
#include "bsp_internal_led_control.h"

/** macro */
#define APP_LED_MUTEX "app_led_mutex"

/* type declaration */

/* variable declaration */
app_led_control_env_t app_led_control_env;
#define env app_led_control_env

rt_uint32_t g_app_led_input_state = 0; //0为不点亮，非0时点亮显示

/* function declaration */
/***********************************固定灯配置与操作*************************/
/**
 * @brief   固定灯设置亮（zxb D3和D4常闪）(针对不同板子实际情况配置)
 * @paraml
 * @return
 * @note   led control thread thread create
**/
static void app_led_control_constant_on(void)
{
	bsp_internal_led_on(BSP_LED_LIGHT_D3);
	bsp_internal_led_on(BSP_LED_LIGHT_D4);
}

/**
 * @brief   固定灯设置灭（zxb D3和D4常闪）(针对不同板子实际情况配置)
 * @paraml
 * @return
 * @note   led control thread thread create
**/
void app_led_control_constant_off(void)
{
	bsp_internal_led_off(BSP_LED_LIGHT_D3);
	bsp_internal_led_off(BSP_LED_LIGHT_D4);
}
/*******************************主控线程****************************************/
/**
 * @brief
 * @param
 * @return
 * @note   app led control thread entry
**/
static void app_led_control_thread_entry(void * parameter)
{

	while(1)
	{
		app_led_control_constant_on();
		rt_thread_mdelay(500);

		app_led_control_constant_off();
		rt_thread_mdelay(500);
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   led control thread thread create
**/
void app_led_control_thread_create(void)
{
	env.led_thread = rt_thread_create("led_thread", \
			                             app_led_control_thread_entry, \
			                             OS_NULL, \
										        APP_LED_CONTROL_STACK, \
										        APP_LED_CONTROL_PRO, \
										           20);
	if(env.led_thread != OS_NULL)
	{
		rt_thread_startup(env.led_thread);
		rt_kprintf("led control thread create success!\n");
	}
	else {
		rt_kprintf("led control thread create failed!\n");
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   led control thread init
**/
void app_led_control_thread_init(void)
{
	bsp_internal_led_init(BSP_LED_LIGHT_D3);
	bsp_internal_led_init(BSP_LED_LIGHT_D4);
}

