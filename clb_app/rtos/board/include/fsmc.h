/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the fsmc_ad7606.h API or library.
 *
 * @features			:
 *
 * @author       		:  
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020.06.02
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
 * 2020.06.02		kernel       create this file
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
 
#ifndef _FSMC_AD7606_H_
#define _FSMC_AD7606_H_

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

#include <drv_config.h>
#if defined(STM32F1XX)
#include <stm32f1xx_ll_fsmc.h>
#elif  defined(STM32F4XX)
#include <stm32f4xx_ll_fsmc.h>
#endif

void HAL_AD7606_MspDeInit(void);
void HAL_AD7606_MspInit(void);
/* Timing configuration for AD7606_ */
//#define AD7606_ADDRESS_SETUP_TIME               5						//0
//#define AD7606_ADDRESS_HOLD_TIME                0						//0
//#define AD7606_DATA_SETUP_TIME                  7						//9
//#define AD7606_BUS_TURN_AROUND_DURATION         1
//#define AD7606_CLK_DIVISION                     0						//0
//#define AD7606_DATA_LATENCY                     0						//0
//#define AD7606_ACCESS_MODE                      FSMC_ACCESS_MODE_A
#define AD7606_ADDRESS_SETUP_TIME               5						//0
#define AD7606_ADDRESS_HOLD_TIME                0						//0
#define AD7606_DATA_SETUP_TIME                  7						//9
#define AD7606_BUS_TURN_AROUND_DURATION         1
#define AD7606_CLK_DIVISION                     0						//0
#define AD7606_DATA_LATENCY                     0						//0
#define AD7606_ACCESS_MODE                      FSMC_ACCESS_MODE_A

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
 * @author	kernel	<kernel_sa@163.com>  write time: 2020.06.02
 * @create time  : 2020.06.02
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
 * 2020.06.02		kernel       create this class
 *						
 */




/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* PROJECT_LH_DRIVERS_INCLUDE_DRIVERS_FSMC_AD7606_H_ */

