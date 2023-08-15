/**
 *
 * Copyright (c) 2015-2020  Driver  Development
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the analog_device.h API or library.
 *
 * @features			:
 *
 * @author       		: plum <tiansyzxm@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020年5月28日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b> @NOTE </b> 	: null
 *
 * \b @Warning      	: null
 * 
 * *
 * Change Logs:
 * Date           author       notes
 * 2020年5月28日	  plum         create this file
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
 
#ifndef DRIVERS_IMPL_MISC_ANALOG_SWITCH_H_
#define DRIVERS_IMPL_MISC_ANALOG_SWITCH_H_

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
#include <rtthread.h>
#include <drivers/pin.h>

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 ****************<Public macro>****************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/** @formatter:off  */

/** 
 * settings depend check 
 */

//#ifdef RT_USING_POSIX
//#if !defined(RT_USING_DFS) || !defined(RT_USING_DFS_DEVFS)
//#error "POSIX poll/select, stdin need file system(RT_USING_DFS) and device file system(RT_USING_DFS_DEVFS)"
//#endif


//#define XXX_VERSION	 		1 			/**< major version number */


/**
 * macro define	- description 
 * @xxx:	description.
 * @xxx:	description.
 * @xxx:	description.
 */
//#define desc_class(xxx, xxxx)     for(xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/** @formatter:on */

/**
 ***************<Public typedef>***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
typedef struct adg1608_pin_configuration
{
	 rt_base_t adg1608_EN;
	 rt_base_t adg1608_A0;
	 rt_base_t adg1608_A1;
	 rt_base_t adg1608_A2;

}adg1608_pin_config_t, * adg1608_pin_config_p;

enum adg1608_truth_table
{
    terminal_none,
    terminal_1,
    terminal_2,
    terminal_3,
    terminal_4,
    terminal_5,
    terminal_6,
    terminal_7,
    terminal_8
};

typedef struct adg1609_pin_configuration
{
     rt_base_t adg1609_EN;
     rt_base_t adg1609_A0;
     rt_base_t adg1609_A1;

}adg1609_pin_config_t, * adg1609_pin_config_p;

enum adg1609_truth_table
{
    adg_terminal_none,
    adg_terminal_1,
    adg_terminal_2,
    adg_terminal_3,
    adg_terminal_4
};
/**
 **************<Public variables>**************
 * @description		: null
 * @note			: null
 * @note			: null
 */

/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/


/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 * @brief adg1608 device initialization
 * *
 * @author  plum <tiansyzxm@163.com>  write time: 2020年5月28日
 * @create time  : 2020年5月28日
 * *
 * Parameters
 * *
 * @param   adg1608_pin_config_t adg1608 pin configuration
 *
 * *
 * @return
 *          - none
 * @retval
 *          - none
 * *
 * @see  reference @file
 *
 * @note    - none
 *
 * @warning - none
 *
 * Change Logs:
 * Date           author       notes
 * 2020年5月28日     plum         create this class
 *
 */
void adg1608_init(adg1608_pin_config_p config);

/**
* @brief adg1608 device initialization
* *
* @author  plum <tiansyzxm@163.com>  write time: 2020年5月28日
* @create time  : 2020年5月28日
* *
* Parameters
* *
* @param   adg1608_pin_config_t    adg1608 pin configuration
*
* *
* @return
*          - none
* @retval
*          - none
* *
* @see  reference @file
*
* @note    - none
*
* @warning - none
*
* Change Logs:
* Date           author       notes
* 2020年5月28日     plum         create this class
*
*/
void adg1608_switch(adg1608_pin_config_t * config, enum adg1608_truth_table chanel);

/**
 * @brief adg1609 device initialization
 * *
 * @author  plum <tiansyzxm@163.com>  write time: 2020年5月28日
 * @create time  : 2020年5月28日
 * *
 * Parameters
 * *
 * @param   adg1609_pin_config_t adg1609 pin configuration
 * 
 * * 
 * @return 
 *          - none
 * @retval
 *          - none
 * *
 * @see  reference @file
 *
 * @note    - none
 *
 * @warning - none
 *
 * Change Logs:
 * Date           author       notes
 * 2020年5月28日     plum         create this class
 *
 */
void adg1609_init(adg1609_pin_config_t * config);

/**
* @brief adg1609 device initialization
* *
* @author  plum <tiansyzxm@163.com>  write time: 2020年5月28日
* @create time  : 2020年5月28日
* *
* Parameters
* *
* @param   adg1609_pin_config_t	adg1609 pin configuration
*
* *
* @return
*          - none
* @retval
*          - none
* *
* @see  reference @file
*
* @note    - none
*
* @warning - none
*
* Change Logs:
* Date           author       notes
* 2020年5月28日     plum         create this class
*
*/
void adg1609_switch(adg1609_pin_config_t * config, enum adg1609_truth_table chanel);


#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif
/** @} */ //\defgroup driver module API
/** @} */ //\addtogroup driver module
