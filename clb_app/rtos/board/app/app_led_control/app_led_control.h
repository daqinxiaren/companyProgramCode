/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_LED_CONTROL_H__
#define __APP_LED_CONTROL_H__

/* includes */
#include "app_board_config.h"

/* macro */
#define APP_LED_CONTROL_STACK       DEF_LED_CONTROL_STACK
#define APP_LED_CONTROL_PRO         DEF_LED_CONTROL_PRO

/* type declaration */
typedef struct
{
	rt_thread_t   led_thread;
}app_led_control_env_t;

/* variable declaration */
extern app_led_control_env_t app_led_control_env;

/* function declaration */
void app_led_control_thread_init(void);
void app_led_control_thread_create(void);

#endif /* __APP_LED_CONTROL_H__ */

