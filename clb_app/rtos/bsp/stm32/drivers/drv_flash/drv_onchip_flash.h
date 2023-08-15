/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the drv_onchip_flash.h API or library.
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
 
#ifndef _DRV_ONCHIP_FLASH_H_
#define _DRV_ONCHIP_FLASH_H_

/**
 ***************** include file ***************
 */
#include <rtthread.h>
#if defined(STM32F1XX)
#include <stm32f1xx.h>
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_conf.h>
#elif  defined(STM32F4XX)
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_conf.h>
#endif



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
#if defined(ENABLE_ON_CHIP_FLASH) && defined(STM32F4XX)

#define STM32_SRAM_SIZE        			(128)
#define STM32_SRAM_END         			(0x20000000 + STM32_SRAM_SIZE * 1024)

#define STM32_FLASH_START_ADRESS     	((uint32_t)0x08000000)
#define STM32_FLASH_SIZE             	(1024 * 1024)
#define STM32_FLASH_END_ADDRESS      	((uint32_t)(STM32_FLASH_START_ADRESS + STM32_FLASH_SIZE))

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      					((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      					(__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      					((void *)&__bss_end)
#endif

#define HEAP_END        					STM32_SRAM_END

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

#endif /* STM32_LIBRARIES_HAL_DRIVERS_DRV_ONCHIP_FLASH_H_ */

