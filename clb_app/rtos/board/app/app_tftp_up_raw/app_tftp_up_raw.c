/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* includes */
#include <rtthread.h>
#include <app_tftp_up_raw.h>
#include "tftp.h"
#include "../../lh/itc/itc.h"
#include <dfs_posix.h>
#include "app_data_save.h"
#include "app_send_record.h"

/** macro */
#define APP_TFTP_STACK             DEF_TFTP_STACK
#define APP_TFTP_PRO               DEF_TFTP_PRO

/* type declaration */

/* variable declaration */
app_tftp_env_t app_tftp_env;
#define env app_tftp_env

/* function declaration */

/**
 * @brief
 * @param
 * @return
 * @note
**/
/*
static void app_tftp_send_thread_entry(void * parameter)
{
	save_file_link_p link_list_head = OS_NULL,link_list_node = OS_NULL,link_list_tmp = OS_NULL;
	struct rt_semaphore* up_sem;
	struct tftp_client *client;
	int file_size;
	char remote_name[64];
	char time_name[16];
	char remote_up_path[20];
	uint8_t train_num = app_send_get_train_num();
	memset(remote_name,0,sizeof(remote_name));
	memset(remote_up_path,0,sizeof(remote_up_path));

	//link_list_head = up_list_head_get();
	link_list_head = app_data_save_file_head_get();
	rt_kprintf("link_list_head:%x\n",link_list_head->next);
	//up_sem = up_sem_get();
	up_sem = app_data_save_get_sem();

	while(1)
	{
		if(rt_sem_take(up_sem,WAITING_FOREVER) != OS_EOK)
		{
			continue;
		}
		else
		{
			rt_kprintf("up clb data!\n");
			link_list_node = link_list_head;
			while(link_list_node->next)
			{
				link_list_node = link_list_node->next;
				link_list_tmp = link_list_node;
				//rt_kprintf("up node_addr:%x\n",link_list_node);
				file_size = 0;
				memset(time_name,0,16);
				memcpy(time_name,&link_list_node->file_name[4],8);
				//sprintf(remote_up_path, "%s%d/%s", PATH_HDD, train_num, PATH_RAW);
				sprintf(remote_name,"%s%d/%s/%s/%s",PATH_HDD, train_num, PATH_RAW, time_name, &link_list_node->file_name[4]);
				client = tftp_client_create("192.168.1.11", 69);
				file_size = tftp_client_push(client,&link_list_node->file_name[1],remote_name);
				tftp_client_destroy(client);
				rt_kprintf("file_name:%s\n",link_list_node->file_name);
				rt_kprintf("remote_name:%s\n",remote_name);
				rt_kprintf("file_size:%d!\n",file_size);
				client = tftp_client_create("192.168.4.11", 69);
				file_size = tftp_client_push(client,&link_list_node->file_name[1],remote_name);
				rt_kprintf("file_name:%s\n",link_list_node->file_name);
				rt_kprintf("remote_name:%s\n",remote_name);
				rt_kprintf("file_size:%d!\n",file_size);
				tftp_client_destroy(client);
				memset(remote_name,0,sizeof(remote_name));
				{
					unlink(link_list_node->file_name);
				}

				//rt_kprintf("link_list_node->prev:%x\n",link_list_node->prev);
				//rt_kprintf("link_list_node->next:%x\n",link_list_node->next);
				link_list_node->prev->next = link_list_node->next;
				link_list_node->next->prev = link_list_node->prev;

				//rt_kprintf("link_list_head->next:%x\n",link_list_head->next);

				rt_free(link_list_tmp);
				link_list_tmp = OS_NULL;
			}
		}
	}
}
*/
static void app_tftp_send_thread_entry(void * parameter)
{
	//save_file_link_p link_list_head = OS_NULL;
	save_file_link_p link_list_node = OS_NULL;
	save_file_link_p link_list_tmp = OS_NULL;
	//struct rt_semaphore* up_sem;
	struct tftp_client *client;
	int file_size_head;
	int file_size_tail;
	char remote_name[64];
	char time_name[16];
	char remote_up_path[20];
	uint8_t train_num = app_send_get_train_num();
	memset(remote_name,0,sizeof(remote_name));
	memset(remote_up_path,0,sizeof(remote_up_path));
//	int test_cont = 0;

	while(1)
	{
		if(rt_sem_take(env.up_sem, WAITING_FOREVER) != OS_EOK)
		{
			continue;
		}
		else
		{
			//test_cont++;
//			rt_kprintf("up clb data! \n");
			//link_list_node = env.save_list_head;

			while(env.save_list_head->next)
			{
				link_list_node = env.save_list_head->next;
				file_size_head = 0;
				file_size_tail = 0;
//				memset(time_name,0,16);
//				memcpy(time_name,&link_list_node->file_name[4],12);
//				if(link_list_node->file_name[17] == 'd')
//				{//特征数据存到tz文件夹，自组网直接存到wts文件夹
//					memset(time_name,0,16);
//					memcpy(time_name,&link_list_node->file_name[4],12);
//					sprintf(remote_name,"%s/%s00/train%d/%s",PATH_WTS, time_name, train_num, &link_list_node->file_name[4]);
//				}
//				else
				{//原始数据存到raw文件夹
					memset(time_name,0,16);
					memcpy(time_name,&link_list_node->file_name[4],8);
					sprintf(remote_name,"%s%d/%s/%s/%s",PATH_HDD, train_num, PATH_RAW, time_name, &link_list_node->file_name[4]);
				}
				//向头车存
				client = tftp_client_create(TFTP_UP_HEAD_IP, 69);
				file_size_head = tftp_client_push(client,&link_list_node->file_name[1],remote_name);
				tftp_client_destroy(client);
				rt_kprintf("ftp send to %s: file_name:%s\n",TFTP_UP_HEAD_IP,link_list_node->file_name);
				rt_kprintf("remote_name:%s\n",remote_name);
				rt_kprintf("file_size_head:%d!\n",file_size_head);
				rt_thread_mdelay(100);
				if (file_size_head<=0)
				{
					//向尾车存
					client = tftp_client_create(TFTP_UP_TAIL_IP, 69);
					file_size_tail = tftp_client_push(client,&link_list_node->file_name[1],remote_name);
					tftp_client_destroy(client);
					rt_kprintf("ftp send to %s: file_name:%s\n",TFTP_UP_TAIL_IP,link_list_node->file_name);
					rt_kprintf("remote_name:%s\n",remote_name);
					rt_kprintf("file_size_tail:%d!\n",file_size_tail);
				}
				memset(remote_name,0,sizeof(remote_name));

				//if (file_size_head>=0 && file_size_tail>=0)
				{//去除链表中的已存文件，并删除文件
					unlink(link_list_node->file_name);
					link_list_tmp = link_list_node;
					link_list_tmp->prev->next = link_list_node->next;
					if (link_list_node->next)
					{
						link_list_tmp->next->prev = link_list_node->prev;
					}
					rt_free(link_list_tmp);
					link_list_tmp = OS_NULL;
					link_list_node = OS_NULL;
				}
			}
	    }
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   temp data save thread create
**/
void app_tftp_thread_create(void)
{
	/* 创建数据接收线程 */
	env.tftp_thread = rt_thread_create("tftp_send", app_tftp_send_thread_entry, OS_NULL, APP_TFTP_STACK, APP_TFTP_PRO, 30);
	if (env.tftp_thread != OS_NULL)
	{
		rt_thread_startup(env.tftp_thread);
	}
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_tftp_thread_init(void)
{
	rt_err_t result;

	env.save_list_head = app_data_save_file_head_get();
	env.up_sem = app_data_save_get_sem();
}


/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

//#endif /**< end XXX module api major */
