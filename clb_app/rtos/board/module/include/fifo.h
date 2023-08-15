/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the fifo.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2021年2月22日
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
 * 2021年2月22日		  kernel       create this file
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
 
#ifndef UTIL_FIFO_H_
#define UTIL_FIFO_H_

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
#include <stdint.h>



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




/**
 ***************<Public typedef>***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */


/**
 **************<Public variables>**************
 * @description		: Structure holding the fifo element parameters.
 * @note			: null 
 * @note			: null  
 */
struct fifo_element
{
	/** next FIFO element */
	struct fifo_element *next;
	/** FIFO data pointer */
	char *data;
	/** FIFO length */
	uint32_t len;
};

/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */



 /** @formatter:on */

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
 * @brief Insert element to fifo tail.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param	param	new node to be inserted
 *
 * @param [ in or out ]	param	new node to be inserted
 *
 * *
 * @return
 *			- p_fifo   description or none
 *			- buff   description ,the operation status or on successful
 *			- len   description ,the operation status or on successful
 *
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
 * 2021年2月22日		  kernel       create this class
 *
 */
int32_t fifo_insert(struct fifo_element **p_fifo, char *buff, uint32_t len);

/**
 * @brief Remove fifo head.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param	p_fifo	new node to be inserted
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
 * 2021年2月22日		  kernel       create this class
 *						
 */
struct fifo_element *fifo_remove(struct fifo_element *p_fifo);


/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* UTIL_FIFO_H_ */

