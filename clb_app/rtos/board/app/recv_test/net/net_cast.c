/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the multicast_recv.c API or library.
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
 * 2021年3月31日		  kernel       create this file
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

#include "include/net_cast.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "include/net.h"
#include "include/net_xfer.h"

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
typedef struct net_private
{
	int timeout;
	net_xfer_p net_xfer;

} net_private_t, *net_private_p;


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
 * @brief multicast create
 * *
 * Parameters
 * *
 * @param ip_addr				-multicast addr
 * @param port					-multicast port
 * * 
 * @return 
 *									-the point of multicast struct
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
 * 2021年3月31日		kernel       create this class
 *						
 */
net_p net_create(const char *ip_addr, int port)
{
      net_private_p _private = NULL;
	net_p net = NULL;

	/* malloc multicast mem */
	net = malloc(sizeof(net_t) + sizeof(net_private_p));
	if (net == NULL)
	{
	    net_printf("create multicast failed! exit \n");
		return NULL;
	}
	/* Creating Private Data */
	_private = (net_private_p) &net [1];
	/* Create a client connection */
	_private->net_xfer = net_xfer_create(ip_addr, port);
	if (_private->net_xfer == NULL)
	{
		net_printf("net xfer create failed! exit\n");
		free(net);
		return NULL;
	}

	/* Initialization error number */
	net->err = NET_OK;
	/* Binding Private Data */
	net->_private = _private;

	return net;
}

/**
 * @brief multicast destroy
 * *
 * Parameters
 * *
 * @param multicast			-the point of multicast struct
 * *
 * @return
 *									-none
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
 * 2021年3月31日		kernel       create this class
 *
 */
void net_destroy(net_p net)
{
      net_private_p _private = NULL;

	_private = net->_private;
	/* Release connection objects */
	net_xfer_destroy(_private->net_xfer);
	/* Free memory */
	free(net);
}

/**
 * @brief multicast init
 * *
 * Parameters
 * *
 * @param multicast			-the point of multicast struct
 * @param typde				-multicast initialization type
 * *
 * @return
 *									-none
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
 * 2021年3月31日		kernel       create this class
 *
 */
int net_init(net_p net, int type)
{
	int res;
	net_private_p _private = NULL;

	_private = net->_private;

	res = net_xfer_type_set(_private->net_xfer,type);

	return res;
}

/**
 * @brief multicast init
 * *
 * Parameters
 * *
 * @param multicast			-the point of multicast struct
 * @param buff					-the point of read data
 * @param len					-the length of read data
 * *
 * @return
 *									-read size
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
 * 2021年3月31日		kernel       create this class
 *
 */
int net_read(net_p multicast, void *buff, int len)
{
	int r_size;
	net_private_p _private = NULL;

	_private = multicast->_private;

	r_size = net_recv_data(_private->net_xfer, buff, len);

	return r_size;
}

/**
 * @brief multicast init
 * *
 * Parameters
 * *
 * @param multicast			-the point of multicast struct
 * @param buff					-the point of read data
 * @param len					-the length of read data
 * *
 * @return
 *									-write size
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
 * 2021年3月31日		kernel       create this class
 *
 */
int net_write(net_p multicast, void *buff, int len)
{
	int w_size;
	net_private_p _private = NULL;

	_private = multicast->_private;

	w_size = net_write_data(_private->net_xfer, buff, len);

	return w_size;
}


/** @} */    //\defgroup module API
/** @} *///\addtogroup dev module
//#endif /**< end XXX module api major */
