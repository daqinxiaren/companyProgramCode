/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the multi_recv.c API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Apr 15, 2021
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
 * Apr 15, 2021		  kernel       create this file
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
 

/**
 ***************** include file *******************
 */
#include <stdio.h>
#include <rtthread.h>
#include <net.h>
#include <net_cast.h>
//#include <xfer_protocol.h>
//#include <save_protocol.h>

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
#define MULTICAST_RECV_IP										("239.255.10.1")
#define MULTICAST_RECV_PORT									(9001)

/**
 * @formatter:on 
 ***************<Private typedef>*******************
 */

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
/**@}*/
/**
 * @brief clb_to_record data send thread entry
 * *
 * Parameters
 * *
 * @param  parameter					-sec thread entry parameter
 * *
 * @return
 *			- xxx   description
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
 * 2021年3月15日		kernel       create this class
 *
 */
static void multicast_recv_thread_entry(void * parameter)
{
//	uint8_t * recv_buff = NULL, result = 0;
//	uint16_t rsize;
//	net_p net;
//
//	recv_buff = rt_malloc(sizeof(recv_record_public_t)-2);
//	if(NULL == recv_buff)
//	{
//		printf("multicast recv_buff malloc failed!\n");
//		return;
//	}
//
//	net = net_create(MULTICAST_RECV_IP,MULTICAST_RECV_PORT);
//	if(NULL == net)
//	{
//		return;
//	}
//
//	result = net_init(net,MULTICAST_XFER_TYPE_RECEIVE);
//	if(NET_OK != result)
//	{
//		printf("net_init failed!\n");
//		return;
//	}
//
//	while(1)
//	{
//		rsize = net_read(net,recv_buff,sizeof(recv_record_public_t)-2);
//
//		if(rsize != (sizeof(recv_record_public_t)-2))
//		{
//			rt_kprintf("multicast recvive length is error,len = %d\n",rsize);
//		}
//		else
//		{
//			rt_kprintf("test_recv.c -- multicast_recv_thread_entry : \n");
//			for(int i = 0; i < 78; i++)
//			{
//				rt_kprintf("0x%02X ",recv_buff[i]);
//			}
//			rt_kprintf("\n");
//			//rt_kprintf("multicast recvive length is error,len \n");
//
//			//record_public_data_parse((recv_record_public_p)recv_buff,rsize);
//		}
//	}
//
}

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
 * Apr 15, 2021		  kernel       create this class
 *						
 */
void multicast_recv_init()
{
	struct rt_thread * multicast_recv_thread;

	/* create sec thread */
	multicast_recv_thread = rt_thread_create("multicast_recv", multicast_recv_thread_entry, NULL, 4096, 23, 30);
	if (multicast_recv_thread != NULL)
	{
		rt_kprintf("init multicast_recv_thread thread!\n");
		rt_thread_startup(multicast_recv_thread);
	}
	else
	{
		rt_kprintf("multicast_recv_thread create failed!\n");
	}
}


/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

//#endif /**< end XXX module api major */
