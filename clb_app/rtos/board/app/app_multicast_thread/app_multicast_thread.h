/**
 * @file
 * @details
 * @author  ren
 * @date    组播发送和接收线程
 * @version
**/
 
#ifndef __APP_MULTICAST_THREAD_H__
#define __APP_MULTICAST_THREAD_H__

/* include */
#include "app_utility.h"

/* macro */
#define APP_MULTICAST_SEND_STACK       DEF_MULTICAST_SEND_STACK
#define APP_MULTICAST_SEND_PRO         DEF_MULTICAST_SEND_PRO
#define APP_MULTICAST_RECV_STACK       DEF_MULTICAST_RECV_STACK
#define APP_MULTICAST_RECV_PRO         DEF_MULTICAST_RECV_PRO

#define APP_MULTICAST_SEND_IP          DEF_MULTICAST_SEND_IP
#define APP_MULTICAST_SEND_PORT        DEF_MULTICAST_SEND_PORT
#define APP_MULTICAST_RECV_IP		   DEF_MULTICAST_RECV_IP
#define APP_MULTICAST_RECV_PORT        DEF_MULTICAST_RECV_PORT
#define APP_MULTICAST_RECV_BUFF_MAX    DEF_MULTICAST_RECV_BUFF_MAX

/* type declaration */
typedef struct sockaddr_in sockaddr_in_t;
typedef struct
{
	rt_thread_t   send_thread;
	int           send_handle;

	rt_thread_t   recv_thread;
	int           recv_handle;
	sockaddr_in_t recv_addr;
	uint8_t       *recv_buff;
}app_multicast_thread_env_t;

/* variable declaration */
extern app_multicast_thread_env_t app_multicast_thread_env;

/* function declaration */
void app_multicast_thread_init(void);
void app_multicast_thread_create(void);
void app_multicast_send(void *data, uint16_t size);

#endif /* __APP_MULTICAST_THREAD_H__ */

