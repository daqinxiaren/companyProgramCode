/**
 *
 * Copyright (c) 2015-2020  LH  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the iwdg_app.h API or library.
 *
 * @features			:
 *
 * @author       		: www
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020年6月15日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b>@note</b> 		: null
 *
 * \b @warning     		: null
 * 
 * *
 * Change Logs:
 * Date				author		notes
 * 2020年6月15日			www		create this file
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
 
#ifndef PROJECT_LH_BOARD_MODULE_IWDG_IWDG_APP_H_
#define PROJECT_LH_BOARD_MODULE_IWDG_IWDG_APP_H_

/**
 ***************** include file ***************
 */
#include <rtthread.h>
#include "rtdevice.h"
#include <stm32f4xx.h>



/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/** @formatter:off  */
/**
 ****************<Public macro>****************
 */

//#define XXX_VERSION	 		1 			/**< major version number */


/**
 * macro define	- description 
 * @description		: null 
 * @note			: null 
 * @param	param	-	description.
 * @param	param	-	description.
 * @note			: null 
 */
//#define desc_class(xxx, xxxx)
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/** @formatter:on */


/**
 **************<Public variables>**************
 */


/**
 ***************<Public typedef>***************
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
 */

/**
 * @brief development description of class
 * *
 * @author	www  write time: 2020年6月15日
 * @create time  : 2020年6月15日
 * *
 * Parameters
 * *
 * @param				param	new node to be inserted
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
 * 2020年6月15日		  www       create this class
 *						
 */
 int iwdg_init(rt_uint32_t timeout_s);
 void feed_dog(void);
 int get_system_resumed(void);


/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* PROJECT_LH_BOARD_MODULE_IWDG_IWDG_APP_H_ */
/* !< end file   */
