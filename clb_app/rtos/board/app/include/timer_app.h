/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file                : Header file for the ad7606_app1.h API or library.
 *
 * @features            :
 *
 * @author              : www
 * 
 * @version             : V 1.0.0
 *
 * @date                : 2020年6月8日
 *
 * @environment         : null
 *
 * @description         : null
 *
 * @module              : null
 *
 * \n <b>@note</b>      : null
 *
 * \b @warning          : null
 * 
 * *
 * Change Logs:
 * Date             author      notes
 * 2020年6月8日            www     create this file
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
 

#ifndef PROJECT_LH_BOARD_MODULE_TIMER_TIMER_APP_H_
#define PROJECT_LH_BOARD_MODULE_TIMER_TIMER_APP_H_

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

#include <rtthread.h>
#include <stm32f4xx.h>

/**
 * @addtogroup group name
 */

/**@{*/


/**@}*/


/**
 **************<Public functions>**************
 */

/**
 * @brief development description of class
 * *
 * @author  www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param               param   new node to be inserted
 * @param [ in or out ] param   new node to be inserted
 *
 * *
 * @return
 *          - xxx   description or none
 *          - xxx   description ,the operation status or on successful
 * @retval
 *          - xxx   description
 * *
 * @see  reference @file
 *
 * @note            定时器2
 *
 * @warning         description
 *
 * Change Logs:
 * Date           author       notes
 * 2020年6月8日          www       create this class
 *
 */

/*初始化定时器*/
int hwtimer_init(uint8_t sec,uint32_t usec);

 /*定时器使能*/
void enable_timx_it(void);

/*定时器关闭*/
void disable_timx_it(void);

/*pwm*/
extern int pwm_init(uint32_t period,uint32_t pulse);
/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/** @formatter:off  */
/**
 ****************<Public macro>****************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

//#define XXX_VERSION	 		1 			/**< major version number */


/**
 * macro define	- description 
 * @xxx:	description.
 * @xxx:	description.
 * @xxx:	description.
 */
//#define desc_class(xxx, xxxx)
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/** @formatter:on */

/**
 ***************<Public typedef>***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */


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
 * @brief development description of class
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param	param	new node to be inserted
 * 
 * @param [ in or out ]	param	new node to be inserted
 *
 * * 
 * @return 
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
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
 * 2020年6月8日		  kernel       create this class
 *						
 */




/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* PROJECT_LH_BOARD_MODULE_TIMER_TIMER_APP_H_ */

