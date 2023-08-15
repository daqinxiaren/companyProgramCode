/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		   : file for the iap_boot.c API or library.
 *
 * @features			   : description
 *
 * @author       		   : kernel <kernel_sa@163.com> 
 * 
 * @version      		   : V 1.0.0
 *
 * @date         		   : Apr 23, 2021
 *
 * @environment  		   : null
 *
 * @description  		   : null 
 *
 * @module       		   : null
 *
 * \n <b> @NOTE </b> 	: null
 *
 * \b @Warning      		: null
 * 
 * *
 * Change Logs:
 * Date           author       notes
 * Apr 23, 2021		   kernel       create this file
 */

/**
 *************************< include file >***************************
 */
#include <rtthread.h>
#include <rtdef.h>

//#ifdef  XXX    /**< XXX module api major */

/**
 * \defgroup module API
 *
 * The xxx API module defines a set of functions that a xxx device
 * driver must implement.
 * "@addtogroup group name"
 * 
 * @{
 */

/**
 * @formatter:off
 *************************< private macro >**************************
 */
/**
 * @name public variables
 * @{
 **/



/**
 * xxx  macro define	
 *                    - description 
 * @param  	xxx      :- description.
 * @return  			 - description.
 */
//#define desc_class(xxx, xxxx) 							           "\"
//		  	for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)
#define NVIC_VTOR_MASK  	 									0x3FFFFF80
#define PART_ADDR 												0x8060000

/**
 * @formatter:on 
 *************************< private typedef >***********************
 */
#ifdef BASE_RTOS
typedef void (*iap_jump_func)(void);
static iap_jump_func jump_func = OS_NULL;

#define IAP_PRINTF rt_kprintf
#endif
/**
 *************************< private variables >*********************
 */

/** 
 * @} //public variables
 **/

/**
 *************************< private functions >*********************
 */

/**
 *************************< public functions >**********************
 * @defgroup xxx Utils 
 * @{
 */


/**
 * Function    ota_app_vtor_reconfig
 * Description Set Vector Table base location to the start addr of app(RT_APP_PART_ADDR).
*/
int vtor_reconfig()
{
    /* Set the Vector Table base location by user application firmware definition */
    SCB->VTOR = PART_ADDR & 0x3FFFFF80;
    return 0;
}
INIT_BOARD_EXPORT(vtor_reconfig);

/** 
 * @} //xxx Utils 
 **/

/** 
 * end defgroup module API
 * @}  
 */

//#endif  /**< end XXX module api major */
