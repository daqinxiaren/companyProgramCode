/**
 * @file
 * @details
 * @author
 * @date    单播发送和接收线程
 * @version
**/

#ifndef __APP_SINGLECAST_THREAD__
#define __APP_SINGLECAST_THREAD__

/* include */
#include "app_utility.h"

/* macro */
#define APP_SINGLECAST_RECV_STACK       DEF_SINGLECAST_RECV_STACK
#define APP_SINGLECAST_RECV_PRO         DEF_SINGLECAST_RECV_PRO

#define APP_SINGLECAST_SERVER_IP        DEF_SINGLECAST_SERVER_IP
#define APP_SINGLECAST_SERVER_PORT      DEF_SINGLECAST_SERVER_PORT
#define APP_SINGLECAST_LOCAL_PORT       DEF_SINGLECAST_LOCAL_PORT
#define APP_SINGLECAST_RECV_BUFF_MAX    DEF_SINGLECAST_RECV_BUFF_MAX

/* type declaration */
typedef struct sockaddr_in sockaddr_in_t;
typedef struct
{
	rt_thread_t   recv_thread;
	int           recv_handle;
	char          local_ip[16];
	sockaddr_in_t recv_addr;
}app_singlecast_thread_env_t;

/* variable declaration */
extern app_singlecast_thread_env_t app_singlecast_thread_env;

/* function declaration */
void app_singlecast_thread_init(void);
void app_singlecast_thread_create(void);
void app_singlecast_send(void *data, uint16_t size);

#endif /* __APP_SINGLECAST_THREAD__ */
