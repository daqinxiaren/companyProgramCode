/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the debug_config.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 10, 2021
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
 * Mar 10, 2021		  kernel       create this file
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
 
#ifndef DEBUG_CONFIG_H_
#define DEBUG_CONFIG_H_

/**
 ***************** include file ***************
 */
#include <kernel_config.h>
#include <net_config.h>

//#ifdef  ENABLE_RTOS    /**< ENABLE_RTOS module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */
#ifdef ENABLE_DEBUG_FUN
#define ENABLE_DEBUG_COLOR
	#define ENABLE_DEBUG_INIT_CONFIG
	#define ENABLE_DEBUG_INIT 					1
	#define ENABLE_DEBUG_THREAD_CONFIG
	#define ENABLE_DEBUG_THREAD 				1
	#define ENABLE_DEBUG_SCHEDULER_CONFIG
	#define ENABLE_DEBUG_SCHEDULER 			1
	#define ENABLE_DEBUG_IPC_CONFIG
	#define ENABLE_DEBUG_IPC 					1
	#define ENABLE_DEBUG_TIMER_CONFIG
	#define ENABLE_DEBUG_TIMER 				1
	#define ENABLE_DEBUG_IRQ_CONFIG
	#define ENABLE_DEBUG_IRQ 					1
	#define ENABLE_DEBUG_MEM_CONFIG
	#define ENABLE_DEBUG_MEM 					1
	#define ENABLE_DEBUG_SLAB_CONFIG
	#define ENABLE_DEBUG_SLAB 					1
	#define ENABLE_DEBUG_MEMHEAP_CONFIG
	#define ENABLE_DEBUG_MEMHEAP 				1
	#define ENABLE_DEBUG_MODULE_CONFIG
	#define ENABLE_DEBUG_MODULE 				1
#endif


#ifdef OS_LWIP_DEBUG
   #define OS_LWIP_SYS_DEBUG
	#define OS_LWIP_ETHARP_DEBUG
	#define OS_LWIP_PPP_DEBUG
	#define OS_LWIP_MEM_DEBUG
	#define OS_LWIP_MEMP_DEBUG
	#define OS_LWIP_PBUF_DEBUG
	#define OS_LWIP_API_LIB_DEBUG
	#define OS_LWIP_API_MSG_DEBUG
	#define OS_LWIP_TCPIP_DEBUG
	#define OS_LWIP_NETIF_DEBUG
	#define OS_LWIP_SOCKETS_DEBUG
	#define OS_LWIP_DNS_DEBUG
	#define OS_LWIP_AUTOIP_DEBUG
	#define OS_LWIP_DHCP_DEBUG
	#define OS_LWIP_IP_DEBUG
	#define OS_LWIP_IP_REASS_DEBUG
	#define OS_LWIP_ICMP_DEBUG
	#define OS_LWIP_IGMP_DEBUG
	#define OS_LWIP_UDP_DEBUG
	#define OS_LWIP_TCP_DEBUG
	#define OS_LWIP_TCP_INPUT_DEBUG
	#define OS_LWIP_TCP_OUTPUT_DEBUG
	#define OS_LWIP_TCP_RTO_DEBUG
	#define OS_LWIP_TCP_CWND_DEBUG
	#define OS_LWIP_TCP_WND_DEBUG
	#define OS_LWIP_TCP_FR_DEBUG
	#define OS_LWIP_TCP_QLEN_DEBUG
	#define OS_LWIP_TCP_RST_DEBUG
#endif


#ifdef WLAN_DEBUG
	#define WLAN_CMD_DEBUG
	#define WLAN_MGNT_DEBUG
	#define WLAN_DEV_DEBUG
	#define WLAN_PROT_DEBUG
	#define WLAN_CFG_DEBUG
	#define WLAN_LWIP_DEBUG
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
 * Mar 10, 2021		  kernel       create this class
 *						
 */




/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

//#endif  /**< end XXX module api major */

#endif /* DEBUG_CONFIG_H_ */

