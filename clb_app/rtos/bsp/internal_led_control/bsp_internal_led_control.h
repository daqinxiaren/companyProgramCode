/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __BSP_INTERNAL_LED_CONTROL_H__
#define __BSP_INTERNAL_LED_CONTROL_H__

/* include */
#include <stm32f4xx.h>
#include <rtthread.h>
#include "app_board_config.h"

/* macro */
/* STM32F407XGT6*/


/* type declaration */
enum bsp_led_name
{
	 BSP_LED_LIGHT_D1 = 0,
    BSP_LED_LIGHT_D2,
    BSP_LED_LIGHT_D3,
    BSP_LED_LIGHT_D4,
    BSP_LED_LIGHT_D5,
    BSP_LED_LIGHT_D6,
    BSP_LED_LIGHT_D7,
    BSP_LED_LIGHT_D8,
    BSP_LED_LIGHT_D9,
};

enum bsp_led_status
{
	BSP_LED_STATUS_ON = 0,
	BSP_LED_STATUS_OFF,
};

#define BSP_LED_PIN_D2             DEF_LED_PIN_D2
#define BSP_LED_PIN_D3             DEF_LED_PIN_D3
#define BSP_LED_PIN_D4             DEF_LED_PIN_D4
#define BSP_LED_PIN_D5             DEF_LED_PIN_D5
#define BSP_LED_PIN_D6             DEF_LED_PIN_D6
#define BSP_LED_PIN_D8             DEF_LED_PIN_D8
#define BSP_LED_PIN_D9             DEF_LED_PIN_D9

/* function declaration */
void bsp_internal_led_init(enum bsp_led_name name);
void bsp_internal_led_on(enum bsp_led_name name);
void bsp_internal_led_off(enum bsp_led_name name);



#endif /*__BSP_INTERNAL_LED_CONTROL_H__*/


