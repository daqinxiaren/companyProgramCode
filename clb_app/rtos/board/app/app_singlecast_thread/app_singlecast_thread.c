/**
 * @file
 * @details
 * @author
 * @date    单播的发送和接收线程，单播接收记录板发送的命令信息和发送信息到记录板
 * @version
**/

/* include */
#include "app_singlecast_thread.h"
#include "app_paras_config.h"
//#include "app_cominform_config.h"
//#include "xfer_protocol.h"
#include "record_board_protocol.h"
#include "app_paras_save.h"

/* type declaration */
typedef struct
{
	sockaddr_in_t send_addr;
	sockaddr_in_t server_addr;   //单播发送到记录板的地址
	uint8_t       *recv_buff;    //单播接收的缓存空间
}app_singlecast_thread_local_t;

/* variable declaration */
app_singlecast_thread_env_t app_singlecast_thread_env;
#define singlecast_env app_singlecast_thread_env

app_singlecast_thread_local_t app_singlecast_thread_local;
#define singlecast_local app_singlecast_thread_local

/* functions declaration */

/**
 * @brief
 * @param
 * @return
 * @note   singlecast send
**/
void app_scast_send_setaddr(sockaddr_in_t send_addr)
{
	singlecast_local.server_addr.sin_addr.s_addr = send_addr.sin_addr.s_addr;
//	rt_kprintf("app_scast_send_setaddr: %x\n",singlecast_local.server_addr.sin_addr.s_addr);
}

/**
 * @brief
 * @param
 * @return
 * @note   singlecast send
**/
void app_singlecast_send(void *data, uint16_t size)
{
	if(singlecast_env.recv_handle < 0)
	{
		rt_kprintf("multicast_send_handle failed!\n");
		return;
	}
	sendto(singlecast_env.recv_handle, (char *)data, size, 0,
			 (const struct sockaddr *)&singlecast_local.server_addr,
			 sizeof(sockaddr_in_t));
}
/**
 * @brief
 * @param
 * @return
 * @note   singlecast send thread
**/
static void app_singlecast_send_thread_create(void)
{
	;
}

/**
 * @brief
 * @param
 * @return
 * @note   singlecast send init
**/
static void app_singlecast_send_thread_init(void)
{
	memset(&singlecast_local.server_addr, 0, sizeof(sockaddr_in_t));
	singlecast_local.server_addr.sin_family = AF_INET;
	singlecast_local.server_addr.sin_addr.s_addr = inet_addr(APP_SINGLECAST_SERVER_IP);
	singlecast_local.server_addr.sin_port = htons(APP_SINGLECAST_SERVER_PORT);
}

/**
 * @brief
 * @param
 * @return
 * @note   singlecast receive thread entry
**/
static void singlecast_recv_thread_entry(void * parameter)
{
	sockaddr_in_t sockaddr;
	const int on = 1;
	int recv_size = 0;
	int fromlen = sizeof(sockaddr_in_t);

	singlecast_env.recv_handle = -1;
	if((singlecast_env.recv_handle = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		rt_kprintf("singlecast_recv_handle failed!\n");
		return ;
	}
	memset(&sockaddr, 0, sizeof(sockaddr_in_t));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(APP_SINGLECAST_LOCAL_PORT);
	sockaddr.sin_addr.s_addr = inet_addr(singlecast_env.local_ip);
	if(setsockopt(singlecast_env.recv_handle, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
	{
		rt_kprintf("add broadcast group failed!\n");
		return ;
	}
	if(bind(singlecast_env.recv_handle, (struct sockaddr*)&sockaddr, sizeof(sockaddr_in_t)) < 0)
	{
		rt_kprintf("singlecast recv bind failed!\n");
		return ;
	}
	while(1)
	{
		recv_size = recvfrom(singlecast_env.recv_handle, singlecast_local.recv_buff, APP_SINGLECAST_RECV_BUFF_MAX, 0,\
							      (struct sockaddr*)&singlecast_env.recv_addr, (socklen_t*) &fromlen);
		rt_kprintf("app_singlecast_thread.c -- 1111   recvfrom ret  : %d\n", recv_size);

		if(recv_size > 0)
		{
			rt_kprintf("app_singlecast_thread.c -- singlecast_recv_thread_entry : \n");
			for(int i = 0; i < recv_size; i++)
			{
				rt_kprintf("0x%02X ",singlecast_local.recv_buff[i]);
			}
			rt_kprintf("\n");
			app_protocol_paras_ptu_data(singlecast_local.recv_buff, recv_size, singlecast_env.recv_addr);

			app_paras_config_get_data(singlecast_local.recv_buff, recv_size, singlecast_env.recv_addr);

			recv_size = 0;
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   singlecast receive thread
**/
static void app_singlecast_recv_thread_create(void)
{
	singlecast_env.recv_thread = rt_thread_create("recv_singlecast", singlecast_recv_thread_entry, OS_NULL, \
												  	  	  	    APP_SINGLECAST_RECV_STACK, APP_SINGLECAST_RECV_PRO, 30);
	if(singlecast_env.recv_thread != OS_NULL)
	{
		rt_thread_startup(singlecast_env.recv_thread);
		rt_kprintf("singlecast_recv_thread create success!\n");
	}
	else {
		rt_kprintf("singlecast_recv_thread create failed!\n");
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   singlecast receive init
**/
static void app_singlecast_recv_thread_init(void)
{
	//comm_para_t comm_para;
	paras_t* paras = app_paras_get();
	char temp[16] = {0};

	singlecast_local.recv_buff = rt_malloc(APP_SINGLECAST_RECV_BUFF_MAX);
	if(NULL == singlecast_local.recv_buff)
	{
		printf("singlecast recv_buff malloc failed!\n");
		return ;
	}
//	memset(&comm_para, 0,sizeof(comm_para_t));
//	read_communication_ini(&comm_para);
	sprintf(temp,"%d.%d.%d.%d", 192, 168, paras->train_num, 13);
//	memcpy(singlecast_env.local_ip, comm_para.local_ip, 16);
	memcpy(singlecast_env.local_ip, temp, 16);
}

/**
 * @brief
 * @param
 * @return
 * @note   singlecast thread create
**/
void app_singlecast_thread_create(void)
{
	app_singlecast_send_thread_create();
	app_singlecast_recv_thread_create();
}

/**
 * @brief
 * @param
 * @return
 * @note   singlecast thread init
**/
void app_singlecast_thread_init(void)
{
	app_singlecast_recv_thread_init();
	app_singlecast_send_thread_init();
}








