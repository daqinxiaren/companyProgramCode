
 
/* include */
#include "app_multicast_thread.h"
#include "app_send_record.h"
#include "record_board_protocol.h"

/* type declaration */
typedef struct
{
	sockaddr_in_t send_record_addr;  //记录板组播地址
}app_multicast_thread_local_t;

/* variable declaration */
app_multicast_thread_env_t app_multicast_thread_env;
#define multicast_env app_multicast_thread_env

app_multicast_thread_local_t app_multicast_thread_local;
#define multicast_local app_multicast_thread_local

/* functions declaration */

/**
 * @brief
 * @param
 * @return
 * @note   测试任务调度时间
**/
static void print_current_time(void)
{
	time_t timer;
	struct tm * time_p = OS_NULL;

	timer = time(&timer);
	time_p = localtime(&timer);

	rt_kprintf("time_p: %d-%d-%d  %d:%d:%d\n",time_p->tm_year+1900,time_p->tm_mon+1,time_p->tm_mday,\
			time_p->tm_hour, time_p->tm_min, time_p->tm_sec);
}
/**
 * @brief
 * @param
 * @return
 * @note   multicast send
**/
void app_multicast_send(void *data, uint16_t size)
{
	if(multicast_env.send_handle < 0)
	{
		rt_kprintf("multicast_send_handle failed!\n");
		return;
	}
	sendto(multicast_env.send_handle, (char *)data, size, 0,
			 (const struct sockaddr *)&multicast_local.send_record_addr,
			 sizeof(sockaddr_in_t));
//	rt_kprintf("---test--- in app_multicast_send  addr: 0x%x  port: 0x%x\n", multicast_local.send_record_addr.sin_addr.s_addr, multicast_local.send_record_addr.sin_port);
}

/**
 * @brief
 * @param
 * @return
 * @note   multicast send thread entry
**/
static void multicast_send_thread_entry(void * parameter)
{
	uint8_t start_time = 0;
	multicast_env.send_handle = -1;
	if((multicast_env.send_handle = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		rt_kprintf("multicast_send_handle failed!\n");
		return;
	}
	//rt_kprintf("--- multicast_send_thread_entry create socket = %d\n", multicast_env.send_handle);
	memset(&multicast_local.send_record_addr, 0, sizeof(sockaddr_in_t));
	multicast_local.send_record_addr.sin_family = AF_INET;
	multicast_local.send_record_addr.sin_port = htons(APP_MULTICAST_SEND_PORT);
	multicast_local.send_record_addr.sin_addr.s_addr = inet_addr(APP_MULTICAST_SEND_IP);

	while(1)
	{
		if (start_time < 40)
		{
			start_time++;
			rt_thread_mdelay(1000);
			continue;
		}
		app_send_record_protocol();
		rt_thread_mdelay(1000);
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   multicast send thread
**/
static void app_multicast_send_thread_create(void)
{
	multicast_env.send_thread = rt_thread_create("send_multicast", multicast_send_thread_entry, OS_NULL, \
												    	      APP_MULTICAST_SEND_STACK, APP_MULTICAST_SEND_PRO, 30);
	if(multicast_env.send_thread != OS_NULL)
	{
		rt_thread_startup(multicast_env.send_thread);
		rt_kprintf("multicast_send_thread create success!\n");
	}
	else {
		rt_kprintf("multicast_send_thread create failed!\n");
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   multicast send init
**/
static void app_multicast_send_thread_init(void)
{
	app_send_record_protocol_init();
}

///**
// * @brief
// * @param
// * @return
// * @note   multicast receive thread entry
//**/
static void multicast_recv_thread_entry(void * parameter)
{
	struct ip_mreq ipmreq;
	int fromlen = sizeof(sockaddr_in_t);
	int recv_size = 0;

	multicast_env.recv_handle = -1;
	if((multicast_env.recv_handle = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		rt_kprintf("multicast_recv_handle failed!\n");
		return ;
	}
	memset(&multicast_env.recv_addr, 0, sizeof(sockaddr_in_t));
	multicast_env.recv_addr.sin_family = AF_INET;
	multicast_env.recv_addr.sin_port = htons(APP_MULTICAST_RECV_PORT);
	multicast_env.recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(multicast_env.recv_handle, (struct sockaddr*)&multicast_env.recv_addr, sizeof(sockaddr_in_t)) < 0)
	{
		rt_kprintf("multicast recv bind failed!\n");
		return ;
	}
	//rt_kprintf(" app_multicast_thread.c -- bind success  s_addr : 0x%x  port: 0x%x\n", multicast_env.recv_addr.sin_addr.s_addr,multicast_env.recv_addr.sin_port);
    /*设置回环许可:当接收者加入到一个多播组以后，再向这个多播组发送数据，这个字段的设置是否允许再返回到本身*/
    int loop = 0;
    if(setsockopt(multicast_env.recv_handle, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) < 0)
    {
        perror("setsockopt():IP_MULTICAST_LOOP");
        return;
    }

    /**默认情况下，多播报文的ＴＴＬ被设置成了１，也就是说到这个报文在网络传送的时候，它只能在自己所在的网络传送，当要向外发送的时候，路由器把ＴＴＬ减１以后变成了０，这个报文就已经被Discard了*/
    unsigned char ttl = 1;
    if (setsockopt(multicast_env.recv_handle, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0)
     {
        perror("setsockopt():IP_MULTICAST_TTL");
        return;
    }
	ipmreq.imr_interface.s_addr = INADDR_ANY;
	ipmreq.imr_multiaddr.s_addr = inet_addr(APP_MULTICAST_RECV_IP);
	if(setsockopt(multicast_env.recv_handle, IPPROTO_IP, IP_ADD_MEMBERSHIP, &ipmreq, sizeof(ipmreq)) < 0)
	{
		rt_kprintf("add multicast group failed!\n");
		return ;
	}
//	rt_kprintf(" app_multicast_thread.c -- ADD_MEMBERSHIP success  s_addr : 0x%x\n", ipmreq.imr_multiaddr.s_addr);

	while(1)
	{
		recv_size = recvfrom(multicast_env.recv_handle, multicast_env.recv_buff, APP_MULTICAST_RECV_BUFF_MAX, 0,\
							      (struct sockaddr*)&multicast_env.recv_addr, (socklen_t*) &fromlen);
//		rt_kprintf("app_multicast_thread.c -- recvfrom ret  : %d\n", recv_size);
		if(recv_size > 0)
		{
//			rt_kprintf("app_multicast_thread.c -- multicast_recv_ public: recv_size: %d\n", recv_size);
//			for(int i = 0; i < recv_size; i++)
//			{
//				rt_kprintf("0x%02X ",multicast_env.recv_buff[i]);
//			}
//			rt_kprintf("\n");

			app_protocol_paras_ptu_data(multicast_env.recv_buff, recv_size, multicast_env.recv_addr);
		}

	}
}

///**
// * @brief
// * @param
// * @return
// * @note   multicast receive thread
//**/
static void app_multicast_recv_thread_create(void)
{
	multicast_env.recv_thread = rt_thread_create("recv_multicast", multicast_recv_thread_entry, OS_NULL, \
												  	  	  	   APP_MULTICAST_RECV_STACK, APP_MULTICAST_RECV_PRO, 30);
	if(multicast_env.recv_thread != OS_NULL) {
		rt_thread_startup(multicast_env.recv_thread);
	}
	else {
		rt_kprintf("multicast_recv_thread create failed!\n");
	}
}

///**
// * @brief
// * @param
// * @return
// * @note   multicast recv init
//**/
static void app_multicast_recv_thread_init(void)
{
	multicast_env.recv_buff = rt_malloc(APP_MULTICAST_RECV_BUFF_MAX);
	if(NULL == multicast_env.recv_buff)
	{
		rt_kprintf("multicast recv_buff malloc failed!\n");
		return ;
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   multicast thread create
**/
void app_multicast_thread_create(void)
{
	app_multicast_send_thread_create();
	app_multicast_recv_thread_create();
}

/**
 * @brief
 * @param
 * @return
 * @note   multicast thread init
**/
void app_multicast_thread_init(void)
{
	app_multicast_send_thread_init();
	app_multicast_recv_thread_init();
}













