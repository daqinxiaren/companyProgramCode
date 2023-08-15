/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the app_led.c API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2021年3月14日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b> @NOTE </b> 	: null
 *
 * \b @Warning      		: null
 * 
 * *
 * Change Logs:
 * Date           author       notes
 * 2021年3月14日		  kernel       create this file
 * 
 * 
 */
 
/**
 * \addtogroup dev module
 * @{
 */


/**
 * \defgroup module API
 *
 * The xxx API module defines a set of functions that a xxx device
 * driver must implement.
 *
 * @{
 */
 

/**
 ***************** include file *******************
 */
#include <drivers/pin.h>
#include <drv_gpio.h>
#include "led.h"

//#ifdef  XXX    /**< XXX module api major */

/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/

/**@}*/

/**
 * @formatter:off
 ****************<Private macro>********************
 */

/**
 * xxx  macro define	
 *                    - description 
 * @param  	xxx      :- description.
 */
//#define desc_class(xxx, xxxx) 									"\"
//		for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/**
 * @formatter:on 
 ***************<Private typedef>*******************
 */

/**
 * xxx  typedef define	
 * @description		: null 
 * @note			      : null 
 */


/**
 ***************<Private define>********************
 */
/* d1 d2 can't control */
#define D3  GET_PIN(G,8)
#define D4  GET_PIN(B,6)


/**
 * xxx  typedef define	
 * @description		: null 
 * @note			      : null 
 */


/**
 **************<Private variables>******************
 */



/**
 **************<Private functions>******************
 */



/**
 **************<Public IMPL functions>**************
 */

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @brief led initialization
 * *
 * Parameters
 * *
 * @param  xxx	               new node to be inserted
 * @param [ in or out ]	xxx	new node to be inserted
 * * 
 * @return 
 *			- xxx   description  or none ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			description
 *							
 * @warning			description 
 *	
 * Change Logs:
 * Date           author       notes
 * 2021年3月14日		  kernel       create this class
 *						
 */
void led_init()
{
	rt_pin_mode(D3, PIN_MODE_OUTPUT);
	rt_pin_mode(D4, PIN_MODE_OUTPUT);

	rt_pin_mode(V1_T, PIN_MODE_INPUT);
	rt_pin_mode(V2_T, PIN_MODE_INPUT);
}


/**
 * @brief led control
 * *
 * Parameters
 * *
 * @param  xxx	               new node to be inserted
 * @param [ in or out ]	xxx	new node to be inserted
 * *
 * @return
 *			- xxx   description  or none ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date           author       notes
 * 2021年3月14日		  kernel       create this class
 *
 */

uint8_t v1_test(void)
{
	uint8_t ret=0;
	if(rt_pin_read(V1_T)==0)
		ret=1;
	return ret;
}

uint8_t v2_test(void)
{
	uint8_t ret=0;
	if(rt_pin_read(V2_T)==0)
		ret=1;
	return ret;
}

void led_contol(enum led_name name,enum led_status status)
{
	switch(name)
	{
		case d3:

			if(on == status)
			{
				rt_pin_write(D3,PIN_LOW);
			}
			else
			{
				rt_pin_write(D3,PIN_HIGH);
			}

			break;

		case d4:

			if(on == status)
			{
				rt_pin_write(D4,PIN_LOW);
			}
			else
			{
				rt_pin_write(D4,PIN_HIGH);
			}

			break;

		default:

			break;
	}
}

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

//#endif /**< end XXX module api major */
