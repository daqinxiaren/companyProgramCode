/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the net_config.h API or library.
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
 
#ifndef NET_CONFIG_H_
#define NET_CONFIG_H_

/**
 ***************** include file ***************
 */
#include <api_config.h>

#if defined(ENABLE_WIFI)  ||  defined(ENABLE_PHY)     /**< ENABLE_RTOS module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */
/* Socket abstraction layer */

//#define ENABLE_SAL

/* protocol stack implement */
#ifdef ENABLE_SAL
	#define SAL_ENABLE_LWIP
	#define SAL_ENABLE_POSIX
	//#define SAL_ENABLE_TLS
	//#define SAL_ENABLE_AT
	#define SAL_SOCKETS_NUM 6

#endif

/* Network interface device */

#define ENABLE_NETDEV
#ifdef ENABLE_NETDEV
	#define NETDEV_ENABLE_IFCONFIG
	#define NETDEV_ENABLE_PING
	#define NETDEV_ENABLE_NETSTAT
	#define NETDEV_ENABLE_AUTO_DEFAULT
	//#define NETDEV_ENABLE_IPV6
	#define NETDEV_IPV4 1
	#define NETDEV_IPV6 0
	//#define NETDEV_IPV6_SCOPES
#endif

/* light weight TCP/IP stack */

#define ENABLE_LWIP
#ifdef ENABLE_LWIP
	#define ENABLE_LWIP210
	//#define ENABLE_LWIP_IPV6
	#define OS_LWIP_IGMP
	#define OS_LWIP_ICMP
	#define OS_LWIP_SNMP
	#define OS_LWIP_DNS
	//#define OS_LWIP_DHCP
	#define IP_SOF_BROADCAST 1
	#define IP_SOF_BROADCAST_RECV 1

	/* Static IPv4 Address */
#if    defined(LH_TFDSN_QZB)
	#define OS_LWIP_IPADDR "192.168.1.221"
#elif  defined(LH_TFDSN_CLB)
	#define OS_LWIP_IPADDR "192.168.1.13"
#elif  defined(LH_TFDSN_TCLB)
	#define OS_LWIP_IPADDR "192.168.1.223"
#elif  defined(LH_TFDSN_TXB)
	#define OS_LWIP_IPADDR "192.168.1.223"
#elif  defined(LH_TFDSN_ZXB)
	#define OS_LWIP_IPADDR "192.168.1.225"
#endif
	#define OS_LWIP_GWADDR "192.168.1.1"
	#define OS_LWIP_MSKADDR "255.255.0.0"
	#define OS_LWIP_UDP
	#define OS_LWIP_TCP
	#define OS_LWIP_RAW
	//#define OS_LWIP_PPP
	//#define OS_LWIP_PPPOE
	//#define OS_LWIP_PPPOS
	#define LWIP_MEMP_NUMBER_NETCONN 16
	#define OS_LWIP_PBUF_NUM 16
	#define OS_LWIP_RAW_PCB_NUM 4
	#define OS_LWIP_UDP_PCB_NUM 4
	#define OS_LWIP_TCP_PCB_NUM 4
	#define OS_LWIP_TCP_SEG_NUM 40
	#define OS_LWIP_TCP_SND_BUF 8196
	#define OS_LWIP_TCP_WND 8196
	#define OS_LWIP_TCPTHREAD_PRIORITY 10
	#define OS_LWIP_TCPTHREAD_MBOX_SIZE 32
	#define OS_LWIP_TCPTHREAD_STACKSIZE 10240
	//#define LWIP_NO_RX_THREAD
	//#define LWIP_NO_TX_THREAD
	#define OS_LWIP_ETHTHREAD_PRIORITY 12
	#define OS_LWIP_ETHTHREAD_STACKSIZE 10240
	#define OS_LWIP_ETHTHREAD_MBOX_SIZE 32
	//#define OS_LWIP_REASSEMBLY_FRAG
	#define LWIP_NETIF_STATUS_CALLBACK 1
	#define LWIP_NETIF_LINK_CALLBACK 1
	#define SO_REUSE 1
	#define LWIP_SO_RCVTIMEO 1
	#define LWIP_SO_SNDTIMEO 1
	#define LWIP_SO_RCVBUF 1
	//#define OS_LWIP_NETIF_LOOPBACK
	#define LWIP_NETIF_LOOPBACK 0
	#define OS_LWIP_STATS
	#define OS_LWIP_ENABLE_PING
	#define LWIP_ENABLE_APP_PING
	#define LWIP_ENABLE_APP_TFTP
	//#define OS_LWIP_DEBUG
#endif

/* AT commands */

//#define ENABLE_AT
#ifdef ENABLE_AT
	#define AT_DEBUG
	#define AT_ENABLE_SERVER
	#define AT_SERVER_DEVICE "uart3"
	#define AT_SERVER_RECV_BUFF_LEN 256
	#define AT_CMD_END_MARK_CRLF
	#define AT_ENABLE_CLIENT
	#define AT_CLIENT_NUM_MAX 1
	#define AT_ENABLE_SOCKET
	#define AT_ENABLE_CLI
	#define AT_PRINT_RAW_CMD
	#define AT_CMD_MAX_LEN 128
	#define AT_SW_VERSION_NUM 0x10300
	#define LWIP_ENABLE_DHCPD
	#define DHCPD_SERVER_IP "192.168.169.1"
	#define DHCPD_ENABLE_ROUTER
	#define LWIP_ENABLE_CUSTOMER_DNS_SERVER
	#define DHCP_DNS_SERVER_IP "1.1.1.1"
#endif






#ifdef ENABLE_WIFI
	#define WLAN_DEVICE_STA_NAME 			"wlan0"
	#define WLAN_DEVICE_AP_NAME 			"wlan1"
	#define WLAN_SSID_MAX_LENGTH 			32
	#define WLAN_PASSWORD_MAX_LENGTH 	32
	#define WLAN_DEV_EVENT_NUM 			2
	#define WLAN_MANAGE_ENABLE
	#define WLAN_SCAN_WAIT_MS 				10000
	#define WLAN_CONNECT_WAIT_MS 			10000
	#define WLAN_SCAN_SORT
	#define WLAN_MSH_CMD_ENABLE
	#define WLAN_AUTO_CONNECT_ENABLE
	#define AUTO_CONNECTION_PERIOD_MS 	2000
	#define WLAN_CFG_ENABLE
	#define WLAN_CFG_INFO_MAX 				3
	#define WLAN_PROT_ENABLE
	#define WLAN_PROT_NAME_LEN 			8
	#define WLAN_PROT_MAX 					2
	#define WLAN_DEFAULT_PROT 				"lwip"
	#define WLAN_PROT_LWIP_ENABLE
	#define WLAN_PROT_LWIP_NAME 			"lwip"
	#define WLAN_PROT_LWIP_PBUF_FORCE
	#define WLAN_WORK_THREAD_ENABLE
	#define WLAN_WORKQUEUE_THREAD_NAME 	"wlan"
	#define WLAN_WORKQUEUE_THREAD_SIZE 	2048
	#define WLAN_WORKQUEUE_THREAD_PRIO 	15
	#define WLAN_DEBUG
#endif


#define ENABLE_NET_UTILS

#ifdef ENABLE_NET_UTILS
	//#define ENABLE_NET_IPERF
	//#define ENABLE_NET_IO
	#define ENABLE_NET_NTP
	#ifndef ENABLE_NETDEV
		#define ENABLE_NET_PING
		//#define ENABLE_NET_TCPDUMP
	#endif
	#define ENABLE_NET_TELNET
	#define ENABLE_NET_TFTP
	#ifdef ENABLE_NET_TFTP
		#define ENABLE_NET_TFTP_SERVER
		#define ENABLE_NET_TFTP_CLIENT
	#endif
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

#endif  /**< end XXX module api major */

#endif /* NET_CONFIG_H_ */

