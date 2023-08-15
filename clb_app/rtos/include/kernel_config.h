/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the kernel_config.h API or library.
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
 
#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

/**
 ***************** include file ***************
 */
#include <system_config.h>


#ifdef  ENABLE_RTOS    /**< ENABLE_RTOS module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */
/* RT-Thread Kernel */

#define OS_NAME_MAX 								8
#define OS_ALIGN_SIZE 							4
#define THREAD_PRIORITY_32
#define THREAD_PRIORITY_MAX 					32
#define TICK_PER_SECOND 						1000
#define ENABLE_OVERFLOW_CHECK
#define ENABLE_HOOK
#define ENABLE_IDLE_HOOK
#define IDLE_HOOK_LIST_SIZE 					4
#define IDLE_THREAD_STACK_SIZE 				1024
#define ENABLE_TIMER_SOFT
#define TIMER_THREAD_PRIO 						4
#define TIMER_THREAD_STACK_SIZE 				512
#define ENABLE_DEBUG


/* Inter-Thread communication */

#define ENABLE_SEMAPHORE
#define ENABLE_MUTEX
#define ENABLE_EVENT
#define ENABLE_MAIL_BOX
#define ENABLE_MESSAGE_QUEUE

/* Memory Management */

//#define ENABLE_MEMORY_POOL
#define ENABLE_MEMHEAP
#define ENABLE_MEMHEAP_AS_HEAP
#define ENABLE_MEMORY_HEAP

/* Kernel Device Object */

#define ENABLE_DEVICE
#define ENABLE_CONSOLE
#define CONSOLE_BUFFER_SIZE 					256
#define CONSOLE_DEVICE_NAME 					"uart1"
#define HEX_VERSION_NUMBER 					0x40003
#define ARCH_ARM
#define ENABLE_CPU_FFS
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4

/* RT-Thread Components */

#define ENABLE_COMPONENTS_INIT
#define ENABLE_USER_MAIN
#define MAIN_THREAD_STACK_SIZE 				2048
#define MAIN_THREAD_PRIORITY 					10


/* Command shell */

#define ENABLE_FINSH
#ifdef ENABLE_FINSH
#define FINSH_THREAD_NAME 						"shell"
#define FINSH_ENABLE_HISTORY
#define FINSH_HISTORY_LINES 					5
#define FINSH_ENABLE_SYMTAB
#define FINSH_ENABLE_DESCRIPTION
#define FINSH_THREAD_PRIORITY 				20
#define FINSH_THREAD_STACK_SIZE 				4096
#define FINSH_CMD_SIZE 							80
#define FINSH_ENABLE_MSH
#define FINSH_ENABLE_MSH_DEFAULT
#define FINSH_ENABLE_MSH_ONLY
#define FINSH_ARG_MAX 							10
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

#endif  /**< end ENABLE_RTOS module api major */

#endif /* KERNEL_CONFIG_H_ */

