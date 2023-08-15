/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the crc16.h API or library.
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
 
#ifndef CRC16_H_
#define CRC16_H_

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
#include <stdint.h>
#include <stddef.h>



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

#define CRC16_TABLE_SIZE 256			/**< CRC16 TABLE */


/**
 * macro define	- description 
 * @xxx:	description.
 * @xxx:	description.
 * @xxx:	description.
 */
#define DECLARE_CRC16_TABLE(_table) \
									static uint16_t _table[CRC16_TABLE_SIZE]



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
 * @brief development description of class
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
 void crc16_populate_msb(uint16_t * table, const uint16_t polynomial);

 uint16_t crc16(const uint16_t * table, const uint8_t *pdata, size_t nbytes,uint16_t crc);



/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* UTIL_CRC16_H_ */

