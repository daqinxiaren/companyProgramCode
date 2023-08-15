/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the system_config.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 12, 2021
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
 * Mar 12, 2021		  kernel       create this file
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
 
#ifndef SYSTEM_CONFIG_H_
#define SYSTEM_CONFIG_H_

/**
 ***************** include file ***************
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cconfig.h"

//#ifdef  XXX    /**< XXX module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */

/**
 * current support list
 *
 *STM32F10X			BOARD_STM32F1XX
 *STM32F4XX			BOARD_STM32F4XX
 *STM32L4XX			BOARD_STM32L4XX
 *
 */
//#define BOARD_STM32F1XX
#define BOARD_STM32F4XX
//#define BOARD_STM32F7XX

/*!< TODO: Open F1's BSP development kit.*/
#if  defined(BOARD_STM32F1XX)
	#define STM32F1XX				/*!< NOTE: Can't edit here, Open F1's BSP development kit.*/

	/*!< NOTE: F1's BSP development kit. Support board: */
	#define LH_TFDSN
	#ifdef LH_TFDSN
		#define LH_TFDSN_QZB										/*!< qianzhiqi 					*/
		#if  defined(LH_TFDSN_QZB)
			#define HW_VERSION      		(0.1)				/*!< hardware  version number 		*/
			#define SW_VERSION      		(0.1)				/*!< software  version number 		*/
		#else
			#error "Please enable mcu hardware board! @see arm_config.h"
		#endif
	#endif //LH_TFDSN

	#define ENABLE_RTOS										/*!< enable rtos 	 						*/
	#define ENABLE_DEVICE_MANAGE							/*!< enable rtos device manager api */
	#define ENABLE_DEBUG										/*!< enable debug module				*/

/*!< TODO: Open F4's BSP development kit.*/
#elif  defined(BOARD_STM32F4XX)
	#define STM32F4XX				/*!< NOTE: Can't edit here, Open F1's BSP development kit.*/
	#if !defined(STM32F407xx)
		#define STM32F407xx
	#endif
	/*!< NOTE: F4's BSP development kit. Support board: */
	#define LH_TFDSN

	#ifdef LH_TFDSN
		#define LH_TFDSN_CLB											/*!< chu li ban 				*/
			#if defined(LH_TFDSN_CLB)
//			#define TFDSN_CLB_SLAVE// SLAVE
			#endif
		//#define LH_TFDSN_TCLB										/*!< tuo gui chu li ban 		*/
		//#define LH_TFDSN_TXB										/*!< tong xun ban 				*/
		//#define LH_TFDSN_ZXB										/*!< zhuan su ban 				*/

			#if    defined(LH_TFDSN_QZB)
				#define HW_VERSION      		(0.1)				/*!< hardware  version number 		*/
				#define SW_VERSION      		(0.1)				/*!< software  version number 		*/
			#elif  defined(LH_TFDSN_CLB)
				#define HW_VERSION      		(0.1)				/*!< hardware  version number 		*/
				#define SW_VERSION      		(0.1)				/*!< software  version number 		*/
			#elif  defined(LH_TFDSN_TCLB)
				#define HW_VERSION      		(0.1)				/*!< hardware  version number 		*/
				#define SW_VERSION      		(0.1)				/*!< software  version number 		*/
			#elif  defined(LH_TFDSN_TXB)
				#define HW_VERSION      		(0.1)				/*!< hardware  version number 		*/
				#define SW_VERSION      		(0.1)				/*!< software  version number 		*/
			#elif  defined(LH_TFDSN_ZXB)
				#define HW_VERSION      		(0.1)				/*!< hardware  version number 		*/
				#define SW_VERSION      		(0.1)				/*!< software  version number 		*/
			#else
				#error "Please enable mcu hardware board! @see arm_config.h"
			#endif
		#endif

	#define ENABLE_RTOS					1					/*!< enable rtos 	 						*/
	#define ENABLE_DEVICE_MANAGE							/*!< enable rtos device manager api */
	#define ENABLE_DEBUG										/*!< enable debug module				*/

#elif  defined(BOARD_STM32F7XX)



#endif




#ifdef ENABLE_DEVICE_MANAGE
	//#define ENABLE_DEVICE_OPS
#endif





/**
 * xxx  macro define	
 *                    - description 
 * @param  	xxx      :- description.
 */
//#define desc_class(xxx, xxxx) 									"\"
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/**
 *  @formatter:on 
 ***************<Public typedef>***************
 */


/**
 **************<Public variables>**************
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
 **************<inline functions>**************
 */

/**
 * xxx  functions define	
 *                    - description 
 * @param  	xxx      :- description.
 */



/**
 **************<Public functions>**************
 */

/**
 * @brief development description of class
 * *
 * Parameters
 * *
 * @param  xxx               -	new node to be inserted
 * @param [ in or out ]	xxx  -	new node to be inserted
 *
 * * 
 * @return 
 *			- xxx   description or none,the operation status or on successful
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
 * Mar 12, 2021		  kernel       create this class
 *						
 */




/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

//#endif  /**< end XXX module api major */

#endif /* SYSTEM_CONFIG_H_ */

