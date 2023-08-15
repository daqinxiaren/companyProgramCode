/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the app_switch.c API or library.
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
#include "switch_init.h"
#include "switch_app.h"

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
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


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

/**
 * xxx  typedef define	
 * @description		: null 
 * @note			      : null 
 */


/**
 **************<Private variables>******************
 */
adg1608_pin_config_t adg1608_wave = {

		GET_PIN(B,9),
		GET_PIN(B,8),
		GET_PIN(F,6),
		GET_PIN(B,7),
};

 adg1608_pin_config_t adg1608_axle_box = {

 		GET_PIN(B,14),
 		GET_PIN(B,15),
 		GET_PIN(B,1),
 		GET_PIN(A,3),
 };

 adg1608_pin_config_t adg1608_motor_gear = {

 		GET_PIN(E,5),
 		GET_PIN(E,6),
 		GET_PIN(E,2),
 		GET_PIN(C,13),
 };


struct switch_change switchs={
		.wave_channel=1,
		.axle_box_channel=1,
#if defined(TFDSN_CLB_SLAVE)//slave
		.motor_gear_channel=1,
#else
		.motor_gear_channel=5,
#endif
};

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
 * @brief all adg1608 device initialization
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
 * 2021年3月14日		kernel       create this class
 *
 */
void adg1608_init_all()
{
	adg1608_init(&adg1608_wave);
	adg1608_init(&adg1608_axle_box);
	adg1608_init(&adg1608_motor_gear);

	adg1608_switch(&adg1608_wave,switchs.wave_channel);//init ch
	adg1608_switch(&adg1608_axle_box,switchs.axle_box_channel);
	adg1608_switch(&adg1608_motor_gear,switchs.motor_gear_channel);
}
/**
 * @brief all adg1608 device switch
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
 * 2021年3月14日		kernel       create this class
 *						
 */
void adg1608_switch_all()
{
	switchs.cur_wave_channel=switchs.wave_channel;
	switchs.cur_axle_box_channel=switchs.axle_box_channel;
	switchs.cur_motor_gear_channel=switchs.motor_gear_channel;


	switchs.wave_channel+=2;
	if(switchs.wave_channel>7)//1/3/5/7
		switchs.wave_channel=1;


	switchs.axle_box_channel++;
	if(switchs.axle_box_channel>8)
		switchs.axle_box_channel=1;

	switchs.motor_gear_channel++;
	if(switchs.motor_gear_channel>8)
	{
		#if defined(TFDSN_CLB_SLAVE)
		switchs.motor_gear_channel=1;
		#else
		switchs.motor_gear_channel=5;
		#endif
	}

	//rt_kprintf("wave=%d-%d axle=%d-%d gear_motor=%d-%d \n",switchs.wave_channel,switchs.cur_wave_channel,switchs.axle_box_channel,\
				switchs.cur_axle_box_channel,switchs.motor_gear_channel,switchs.cur_motor_gear_channel);
#ifndef TFDSN_WAVE
	adg1608_switch(&adg1608_wave,switchs.wave_channel);
#endif
	adg1608_switch(&adg1608_axle_box,switchs.axle_box_channel);
	adg1608_switch(&adg1608_motor_gear,switchs.motor_gear_channel);

}



/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

//#endif /**< end XXX module api major */
