/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the multicast_recv.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2021年3月31日
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
 * 2021年3月31日		kernel       create this file
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
 
#ifndef MODULE_MULTICAST_MULTICAST_H_
#define MODULE_MULTICAST_MULTICAST_H_

/**
 ***************** include file *******************
 */

//#ifdef  XXX    /**< XXX module api major */

/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/

/**@}*/

/**
 * @formatter:off
 ****************<Private macro>********************
 */

/**
 * xxx  macro define	
 *                    - description 
 * @param  	xxx      :- description.
 */
//#define desc_class(xxx, xxxx) 									"\"
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)

/**
 * @formatter:on 
 ***************<Private typedef>*******************
 */
typedef struct net
{
    int err;
    void * _private;
}net_t, * net_p;

/**
 * xxx  typedef define	
 * @description		: null 
 * @note			      : null 
 */


/**
 ***************<Private define>********************
 */

/**
 * xxx  typedef define	
 * @description		: null 
 * @note			      : null 
 */


/**
 **************<Private variables>******************
 */



/**
 **************<Private functions>******************
 */



/**
 **************<Public IMPL functions>**************
 */

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @brief development description of class
 * *
 * Parameters
 * *
 * @param  xxx	               new node to be inserted
 * @param [ in or out ]	xxx	new node to be inserted
 * * 
 * @return 
 *			- xxx   description  or none ,the operation status or on successful
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
 * 2021年3月31日		  kernel       create this class
 *						
 */

net_p net_create(const char * ip_addr,int port);

void net_destroy(net_p net);

int net_init(net_p net,int type);

int net_read(net_p net,void *buff,int len);

int net_write(net_p net,void *buff,int len);

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#endif /**< end XXX module api major */
