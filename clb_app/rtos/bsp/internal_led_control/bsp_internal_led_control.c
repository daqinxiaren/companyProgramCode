/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* include */
#include "bsp_internal_led_control.h"
#include "drv_gpio.h"

/* macro */

/* variable declaration */
static rt_base_t bsp_led_pin_list[9];

/* function delcaration */

/**
 * @brief LED灯控制
 * @param
 * @return if ok return 0 else fail
 * @note
**/
void bsp_internal_led_contol(enum bsp_led_name name,enum bsp_led_status status)
{
	if (name < BSP_LED_LIGHT_D2 || name > BSP_LED_LIGHT_D9)
	{
		rt_kprintf(" error:   in bsp_internal_led_contol: LED name could not control -> name: %d\n", name);
		return;
	}

	if (bsp_led_pin_list[name] != OS_NULL)
	{
  		if(BSP_LED_STATUS_ON == status)
   		{
  			rt_pin_write(bsp_led_pin_list[name],PIN_LOW);
    	}
    	else
    	{
    		rt_pin_write(bsp_led_pin_list[name],PIN_HIGH);
    	}
	}
	else
	{
		rt_kprintf(" error:   in bsp_internal_led_contol： LED name not define -> name: %d\n", name);
	}

}
/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/

void bsp_internal_led_init(enum bsp_led_name name)
{
	if (name < BSP_LED_LIGHT_D2 || name > BSP_LED_LIGHT_D9)
	{
		rt_kprintf(" error:   in bsp_internal_led_init： LED name could not control -> name: %d\n", name);
		return;
	}
	memset(bsp_led_pin_list, 0, sizeof(rt_base_t));

	switch(name)
	{
		case BSP_LED_LIGHT_D2:
			bsp_led_pin_list[name] = BSP_LED_PIN_D2;
			break;

		case BSP_LED_LIGHT_D3:
			bsp_led_pin_list[name] = BSP_LED_PIN_D3;
			break;

		case BSP_LED_LIGHT_D4:
			bsp_led_pin_list[name] = BSP_LED_PIN_D4;
			break;

		case BSP_LED_LIGHT_D5:
			bsp_led_pin_list[name] = BSP_LED_PIN_D5;
			break;
/* 预留
		case BSP_LED_LIGHT_D6:
			bsp_led_pin_list[name] = BSP_LED_PIN_D6;
			break;
*/
		case BSP_LED_LIGHT_D8:
			bsp_led_pin_list[name] = BSP_LED_PIN_D8;
			break;

		case BSP_LED_LIGHT_D9:
			bsp_led_pin_list[name] = BSP_LED_PIN_D9;
			break;

		default:
			break;
	}
	if (bsp_led_pin_list[name] != OS_NULL)
	{
	    rt_pin_mode(bsp_led_pin_list[name], PIN_MODE_OUTPUT);
	}
	else
	{
		rt_kprintf(" error:   in bsp_internal_led_init： LED name not define -> name: %d\n", name);
	}

	return;
}

/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/
void bsp_internal_led_on(enum bsp_led_name name)
{
	bsp_internal_led_contol(name,BSP_LED_STATUS_ON);
	return;
}

/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/
void bsp_internal_led_off(enum bsp_led_name name)
{
	bsp_internal_led_contol(name,BSP_LED_STATUS_OFF);
	return;
}


