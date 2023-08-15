/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the multicast_xfer.c API or library.
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
#if 0
/**
 ***************** include file *******************
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "include/net_xfer.h"
#include "include/net.h"
#include "app_paras_config.h"

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
typedef struct net_xfer_private
{
	struct sockaddr_in server;
	struct sockaddr_in addr;
	char *ip_addr;
	uint16_t port;

} net_xfer_private_t, *net_xfer_private_p;

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
 * @brief multicast xfer create
 * *
 * Parameters
 * *
 * @param ip_addr		-multicast ip of send or receive
 * @param port			-multicast port of send or receive
 * *
 * @return
 *							-multicast_xfer_p
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
net_xfer_p net_xfer_create(const char *ip_addr, int port)
{
	int sock;
	net_xfer_p xfer = NULL;
	net_xfer_private_p _private = NULL;
	int mem_len;
	
	/* malloc connect object */
	mem_len = sizeof(net_xfer_t) + sizeof(net_xfer_private_t);
	xfer = malloc(mem_len);
	if (xfer == NULL)
	{
		net_printf("can't create multicast transfer!! exit\n");
		return NULL;
	}
	memset(xfer, 0, mem_len);
	_private = (net_xfer_private_p) &xfer [1];
	
	/* create socket */
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		net_printf("can't create socket!! exit\n");
		free(xfer);
		return NULL;
	}
//	printf("multicast ip_addr:%s sock = %d\n", _private->ip_addr, sock);

	/* Initialize private data */
	_private->ip_addr = strdup(ip_addr);
	_private->port = port;
	xfer->sock = sock;
	xfer->_private = _private;
	
	return xfer;
}

/**
 * @brief multicast xfer destroy
 * *
 * Parameters
 * *
 * @param xfer			-the point of multicast xfer struct
 * * 
 * @return 
 *							-none
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
void net_xfer_destroy(net_xfer_p xfer)
{
	net_xfer_private_p _private = NULL;
	
	/* free all mem */
	_private = xfer->_private;
	shutdown(xfer->sock, SHUT_RDWR);
	free(_private->ip_addr);
	free(xfer);
}

/**
 * @brief set multicast xfer type
 * *
 * Parameters
 * *
 * @param xfer			-multicast xfer struct
 * @param type			-multicast type
 * *
 * @return
 *							-int
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
int net_xfer_type_set(net_xfer_p xfer, int type)
{
	struct ip_mreq ipmreq;
	const int on = 1;
	net_xfer_private_p _private = NULL;
	
	_private = xfer->_private;
	
//	printf("88888888888888888888888888888888888888888888888888888!\n");

	/* Setting initialization type */
	switch (type)
	{
		case MULTICAST_XFER_TYPE_RECEIVE:

			/* Check whether the type is set */
			if (xfer->type != MULTICAST_XFER_TYPE_RECEIVE)
			{
				/* send type has been set. return */
				if ((xfer->type == MULTICAST_XFER_TYPE_SEND) || (xfer->type == BROADCAST_XFER_TYPE_RECEIVE)
				        || (xfer->type == BROADCAST_XFER_TYPE_SEND) || (xfer->type == UDP_XFER_TYPE_RECEIVE) || (xfer->type == UDP_XFER_TYPE_SEND))
				{
					return -NET_EINVAL;
				}
				
				/* Initialize client connection */
				_private->server.sin_family = PF_INET;
				_private->server.sin_port = htons(_private->port);
				_private->server.sin_addr.s_addr = INADDR_ANY;
				
				/* Binding port */
				if (bind(xfer->sock, (struct sockaddr*) &_private->server, sizeof(struct sockaddr_in)) < 0)
				{
					net_printf("multicast server bind failed!! exit\n");
					return -NET_ESYS;
				}
				
				ipmreq.imr_interface.s_addr = INADDR_ANY;
				//	ipmreq.imr_multiaddr.s_addr = inet_addr("239.255.20.1");
				ipmreq.imr_multiaddr.s_addr = inet_addr(_private->ip_addr);
				printf("multicast ip_addr:%s sock = %d\n", _private->ip_addr, xfer->sock);
				if (setsockopt(xfer->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ipmreq, sizeof(ipmreq)) < 0)
				{
					net_printf("add multicast group failed!! exit\n");
					return -NET_ESYS;
				}

				xfer->type = MULTICAST_XFER_TYPE_RECEIVE;
			}
			
			break;
			
		case MULTICAST_XFER_TYPE_SEND:

			if (xfer->type != MULTICAST_XFER_TYPE_SEND)
			{
				if (xfer->type == MULTICAST_XFER_TYPE_RECEIVE || (xfer->type == BROADCAST_XFER_TYPE_RECEIVE)
				        || (xfer->type == BROADCAST_XFER_TYPE_SEND) || (xfer->type == UDP_XFER_TYPE_RECEIVE) || (xfer->type == UDP_XFER_TYPE_SEND))
				{
					return -NET_EINVAL;
				}
				_private->server.sin_family = AF_INET;
				_private->server.sin_port = htons(_private->port);
				_private->server.sin_addr.s_addr = inet_addr(_private->ip_addr);
				
				xfer->type = MULTICAST_XFER_TYPE_SEND;
			}
			
			break;
			
		case BROADCAST_XFER_TYPE_RECEIVE:

			if (xfer->type != BROADCAST_XFER_TYPE_RECEIVE)
			{
				if ((xfer->type == MULTICAST_XFER_TYPE_SEND) || (xfer->type == MULTICAST_XFER_TYPE_RECEIVE)
				        || (xfer->type == BROADCAST_XFER_TYPE_SEND) || (xfer->type == UDP_XFER_TYPE_RECEIVE) || (xfer->type == UDP_XFER_TYPE_SEND))
				{
					return -NET_EINVAL;
				}
				_private->server.sin_family = AF_INET;
				_private->server.sin_port = htons(_private->port);
				_private->server.sin_addr.s_addr = INADDR_ANY;
				
				/* Binding port */
				if (bind(xfer->sock, (struct sockaddr*) &_private->server, sizeof(struct sockaddr_in)) < 0)
				{
					net_printf("broadcast server bind failed!! exit\n");
					return -NET_ESYS;
				}
				
				xfer->type = BROADCAST_XFER_TYPE_RECEIVE;
			}
			
			break;
			
		case BROADCAST_XFER_TYPE_SEND:

			if (xfer->type != BROADCAST_XFER_TYPE_SEND)
			{
				if ((xfer->type == MULTICAST_XFER_TYPE_SEND) || (xfer->type == MULTICAST_XFER_TYPE_RECEIVE)
				        || (xfer->type == BROADCAST_XFER_TYPE_RECEIVE) || (xfer->type == UDP_XFER_TYPE_RECEIVE) || (xfer->type == UDP_XFER_TYPE_SEND))
				{
					return -NET_EINVAL;
				}
				_private->server.sin_family = AF_INET;
				_private->server.sin_port = htons(_private->port);
				_private->server.sin_addr.s_addr = inet_addr(_private->ip_addr);
				
				if (setsockopt(xfer->sock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
				{
					net_printf("add broadcast group failed!! exit\n");
					return -NET_ESYS;
				}
				
				xfer->type = BROADCAST_XFER_TYPE_SEND;
				
			}
			
			break;

		case UDP_XFER_TYPE_RECEIVE:

			//printf("1111111111111111111111111111111111111111111111111111111!\n");

			if (xfer->type != UDP_XFER_TYPE_RECEIVE)
			{
				if ((xfer->type == MULTICAST_XFER_TYPE_SEND) || (xfer->type == MULTICAST_XFER_TYPE_RECEIVE)
				        || (xfer->type == BROADCAST_XFER_TYPE_SEND) || (xfer->type == BROADCAST_XFER_TYPE_RECEIVE) || (xfer->type == UDP_XFER_TYPE_SEND))
				{
					return -NET_EINVAL;
				}

				//printf("1111111111111111111111111111111111111111111111111111111!\n");

				_private->server.sin_family = AF_INET;
				_private->server.sin_port = htons(_private->port);
				_private->server.sin_addr.s_addr = inet_addr(_private->ip_addr);;

				 printf("udp ip_addr:%s sock = %d\n",_private->ip_addr,xfer->sock);
				if (setsockopt(xfer->sock, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
				{
					net_printf("add broadcast group failed!! exit\n");
					return -NET_ESYS;
				}
				/* Binding port */
				if (bind(xfer->sock, (struct sockaddr*) &_private->server, sizeof(struct sockaddr_in)) < 0)
				{
					net_printf("broadcast server bind failed!! exit\n");
					return -NET_ESYS;
				}

				xfer->type = UDP_XFER_TYPE_RECEIVE;

			}

			break;

		case UDP_XFER_TYPE_SEND:

			if (xfer->type != UDP_XFER_TYPE_SEND)
			{
				if ((xfer->type == MULTICAST_XFER_TYPE_SEND) || (xfer->type == MULTICAST_XFER_TYPE_RECEIVE)
				        || (xfer->type == BROADCAST_XFER_TYPE_RECEIVE) || (xfer->type == BROADCAST_XFER_TYPE_SEND) || (xfer->type == UDP_XFER_TYPE_RECEIVE))
				{
					return -NET_EINVAL;
				}
				_private->server.sin_family = AF_INET;
				_private->server.sin_port = htons(_private->port);
				_private->server.sin_addr.s_addr = inet_addr(_private->ip_addr);

				xfer->type = UDP_XFER_TYPE_SEND;

			}

			break;
			
		default:

			return -NET_EINVAL;
			
			break;
	}
	
	return NET_OK;
}

/**
 * @brief set multicast write data
 * *
 * Parameters
 * *
 * @param xfer			-multicast xfer struct
 * @param buff			-the point of write data
 * @param len			-the length of write data
 * *
 * @return
 *							-int
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
int net_write_data(net_xfer_p xfer, void *buff, int len)
{
	net_xfer_private_p _private = NULL;
	int size;
	
	_private = xfer->_private;
	
	/* Send data */
	size = sendto(xfer->sock, buff, len, 0, (struct sockaddr*) &_private->server, sizeof(struct sockaddr_in));
	if (size < 0)
	{
		perror("net send");
		return -NET_EXFER;
	}
	
	return size;
}

/**
 * @brief set multicast read data
 * *
 * Parameters
 * *
 * @param xfer			-multicast xfer struct
 * @param buff			-the point of read data
 * @param len			-the length of read data
 * *
 * @return
 *							-int
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
int net_recv_data(net_xfer_p xfer, void *buff, int len)
{
	net_xfer_private_p _private = NULL;
	int sender_len = sizeof(struct sockaddr_in);
	int r_size = 0;
	
	/* net receive data */
	_private = xfer->_private;
	r_size = recvfrom(xfer->sock, buff, len, 0, (struct sockaddr*)&_private->addr, (socklen_t*) &sender_len);
	if (r_size < 0)
	{
		perror("net_recv");
		return -NET_EXFER;
	}
	return r_size;
}
#endif
/** @} */    //\defgroup module API
/** @} *///\addtogroup dev module
//#endif /**< end XXX module api major */
