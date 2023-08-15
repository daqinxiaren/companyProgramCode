/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* includes */
#include <stdio.h>
#include "app_data_save.h"
#include "app_ad7606_data.h"
#include "app_channel_switch.h"
#include "dfs_posix.h"
#include "unistd.h"
#include "clb_save_protocol.h"
#include "rtthread.h"
#include "app_self_test.h"
#include "clb_record_protocol.h"
#include "app_send_record.h"

/** macro */
#define APP_SAVE_CH_BUFF_SIZE_MAX    DEF_AD7606_CH_BUFF_SIZE_MAX
#define SAVE_VIB_ONE_SIZE            DEV_SAVE_VIB_ONE_SIZE

/* type declaration */

/* variable declaration */
app_data_save_env_t app_data_save_env;
#define env app_data_save_env
clb_save_data_head_t save_data_head;
#define head save_data_head
ad7606_data_t ad7606_buffer;
//static ad7606_data_t ad7606_buffer;
txb_MVB_public_t app_save_public; //该结构体保存公共信息，随原始数据一同存储
#define public app_save_public

int vib_save_flag = 0; //用于标志当前采集原始数据是否存储完成，存完再开始算法处理

/* function declaration */
/**
 * @brief
 * @param
 * @return
 * @note
**/
uint16_t save_sum_check_u16(uint8_t *data, uint16_t crclen, uint16_t check)
{
	int i;
	uint16_t check_sum = check;
	for(i=0;i<crclen;i++)
	{
		check_sum+=	data[i];
	}
	return check_sum;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
save_info_link_p info_link_create()
{
	save_info_link_p link_list_node = OS_NULL;

	link_list_node = rt_malloc(sizeof(save_info_link_t));
	if (OS_NULL != link_list_node)
	{
		memset(link_list_node,0,sizeof(save_info_link_t));
	}

	return link_list_node;
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
static save_info_link_p get_info_link_head(void)
{
	return &env.save_info_head;
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
void info_link_insert_of_end(save_info_link_p link_list_node)
{
	save_info_link_p link_list_tmp;
	if (OS_NULL == link_list_node)
	{
		rt_kprintf("%s line:%d time : save_list_insert_of_end link_list_node is NULL!!!\n", __FILE__, __LINE__);
		return ;
	}

	link_list_tmp = get_info_link_head();
	//link_list_tmp = &env.save_list_head;

	while(link_list_tmp->next)
	{
		link_list_tmp = link_list_tmp->next;
	}

	link_list_tmp->next = link_list_node;
	link_list_node->prev = link_list_tmp;

	return;
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
//void app_data_save_vib(app_select_num_p channel, ad7606_data_t data_buff)
//{
//	save_info_link_p link_info = OS_NULL;
//	ad7606_data_t* ad7606_buffer = OS_NULL;
//
//	link_info = info_link_create();
//	if (OS_NULL == link_info)
//	{
//		rt_kprintf("--- app_data_save_vib create link fail!!\n");
//		rt_kprintf("reboot clb ...\n");
////		save_sys_para();
//		rt_hw_cpu_reset();//reset clb
//		return;
//	}
//	link_info->file_type = save_vib;
//	link_info->vib_channel.save_axle_box_num = channel->save_axle_box_num;
//	link_info->vib_channel.save_motor_gear_num = channel->save_motor_gear_num;
//
//	ad7606_buffer = rt_malloc(sizeof(ad7606_data_t));
//	if (OS_NULL == ad7606_buffer)
//	{
//		rt_kprintf("--- app_data_save_vib   malloc 7606_buffer fail!!\n");
//		rt_kprintf("reboot clb ...\n");
//		save_sys_para();
//		rt_hw_cpu_reset();//reset clb
//		return;
//	}
//	//rt_kprintf("--- rt malloc heap 20 byte add is : %p\n",ad7606_buffer);
//	for (int i=0; i<APP_AD7606_DATA_CH_MAX; i++)
//	{
//	   ad7606_buffer->ch[i].data_buffer = data_buff.ch[i].data_buffer;
//	}
//
//	link_info->data_buff = (uint8_t*)ad7606_buffer;
//	info_link_insert_of_end(link_info);
//}
/**
 * @brief
 * @param
 * @return
 * @note
**/
//static void app_save_data_unlink(save_info_link_p link_info)
//{
//    //从链表中移除
//    link_info->prev->next = link_info->next;
//	if (link_info->next)
//	{
//	    link_info->next->prev = link_info->prev;
//	}
//	//释放存数据的buff
//	if (link_info->data_buff)
//	{
//	    rt_free(link_info->data_buff);
//	}
//	//释放当前链接空间
//	rt_free(link_info);
//}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_data_save_temp(void)
{
	save_info_link_p link_info = OS_NULL;

	link_info = info_link_create();
	link_info->file_type = save_temp;
	info_link_insert_of_end(link_info);
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_save_get_public_buf(txb_MVB_public_t* public_data)
{
	memcpy(public_data, &app_save_public, sizeof(txb_MVB_public_t));
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_save_public_info(uint8_t* public_data)
{
   if (public_data == NULL)
    {
	   rt_kprintf("app_save_public_info   public_data is NULL!!!\n");
    	return;
    }
   memcpy(&app_save_public, public_data, sizeof(txb_MVB_public_t));
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
static void sd_data_delete(void)
{
	 DIR *dir;
	 struct dirent *dirp;
	// struct stat buf;
	 char tag_path[128];
	 if(access("/sd/", F_OK)!=0)
		 return;

	 if((dir=opendir("sd"))==NULL)
		 return;

	while ((dirp = readdir(dir)) != NULL)
	{
		 if((strcmp(dirp->d_name,".")==0) || (strcmp(dirp->d_name,"..")==0))
			 continue;

		 memset(tag_path,'\0',sizeof(tag_path));
		 strcpy(tag_path,"/sd/");
		 strcat(tag_path,dirp->d_name);
		 printf("%s\n",tag_path);

		 unlink(tag_path);

	 }

	closedir(dir);

	printf("rm Successed end. . .\n");
 }

/**
 * @brief
 * @param
 * @return
 * @note
**/
void get_current_time(struct tm * time_buff)
{
	time_t timer;
	struct tm * time_p = OS_NULL;

	if (OS_NULL == time_buff)
	{
		rt_kprintf("%s line:%d time : time_buff is NULL!!!\n", __FILE__, __LINE__);
		return;
	}

	timer = time(&timer);
	time_p = localtime(&timer);
	*time_buff = * time_p;

}
/**
 * @brief
 * @param
 * @return
 * @note
**/
static save_file_link_p file_linke_create()
{
	save_file_link_p link_list_node = OS_NULL;

	link_list_node = rt_malloc(sizeof(save_file_link_t));
	if (link_list_node != OS_NULL)
	{
		memset(link_list_node,0,sizeof(save_file_link_t));
	}
	else
	{
		rt_kprintf("file_linke_create: rt_malloc is null\n");
	}

	return link_list_node;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void file_list_insert_of_end(save_file_link_p link_list_node)
{
	save_file_link_p link_list_tmp;
	if (OS_NULL == link_list_node)
	{
		rt_kprintf("%s line:%d time : save_list_insert_of_end link_list_node is NULL!!!\n", __FILE__, __LINE__);
		return ;
	}

//	link_list_tmp = up_list_head_get();
	link_list_tmp = &env.save_file_head;

	while(link_list_tmp->next)
	{
		link_list_tmp = link_list_tmp->next;
	}

	link_list_tmp->next = link_list_node;
	link_list_node->prev = link_list_tmp;

	return;
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
static FILE * creat_file(char* file_name)
{
	FILE * ini;
	sys_dev_state_t* system_para = app_get_self_test_para();

	if (OS_NULL == file_name)
	{
		rt_kprintf("%s line:%d time : creat_save_file time is NULL!!!\n", __FILE__, __LINE__);
		return OS_NULL;
	}

	if(0 != access(file_name, F_OK))
	{
		ini = fopen(file_name,"w+");
		if(OS_NULL == ini)
		{
			system_para->sd_save_fault = 1;
			rt_kprintf("create save file failed:%s!!! reboot\n",file_name);
			rt_hw_cpu_reset();//reset clb
		}
		else
		{
			system_para->sd_save_fault = 0;
		    fseek(ini,0,SEEK_SET);
		}
	}
	else
	{
		system_para->sd_save_fault = 0;
		ini = fopen(file_name,"w+");
		if(OS_NULL == ini)
		{
			rt_kprintf("open save file failed:%s\n",file_name);
		}
		else
		{
			fseek(ini,0,SEEK_SET);
		}
	}
	return ini;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static void save_data_fill_head(struct tm time)
{
	static rt_uint16_t pack_cnt = 0;
	uint8_t train_id = app_send_get_train_num();
	uint8_t host_flag = app_send_get_host_flag();
	//app_select_num_p select_channel_num = OS_NULL;

	head.save_data_head_h = 0x66;
	head.save_data_head_l = 0xbb;

	/* time */
	head.year = time.tm_year-100;
	head.month = time.tm_mon+1;
	head.day = time.tm_mday;
	head.hour = time.tm_hour;
	head.min = time.tm_min;
	head.sec = time.tm_sec;

	/* pack cnt */
	head.pack_count_h = pack_cnt & 0xff;
	head.pack_count_l = (pack_cnt >> 8) & 0xff;
	pack_cnt++;
	head.host_slave_flag = host_flag;

	//填充收到的公共信息
	/* current id */
	head.current_id_h = public.curr_id[0];
	head.current_id_l = public.curr_id[1];

	/* next id */
	head.next_id_h = public.next_id[0];
	head.next_id_l = public.next_id[1];

	/* speed */
	head.speed_h = public.speed[0];
	head.speed_l = public.speed[0];

	/* wheel diamter */
	head.wheel_diameter_h = public.wheel[2*(train_id-1)];
	head.wheel_diameter_l = public.wheel[2*(train_id-1)+1];

	head.train_id = train_id;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
FILE* app_create_save_file(char* file_name, struct tm file_time, uint8_t file_type)
{
	FILE* ini = OS_NULL;

	switch (file_type)
	{
		case save_temp:
			sprintf(file_name,"/sd/%04d%02d%02d%02d%02d-temp.dat",\
					file_time.tm_year+1900, file_time.tm_mon+1, file_time.tm_mday,\
					file_time.tm_hour, file_time.tm_min);
			break;

		case save_vib:
			sprintf(file_name,"/sd/%04d%02d%02d%02d%02d-vib.dat",\
					file_time.tm_year+1900, file_time.tm_mon+1, file_time.tm_mday,\
					file_time.tm_hour, file_time.tm_min);
			break;

		default:
			return OS_NULL;
			break;
	}

	ini = creat_file(file_name);
	return ini;

}

void add_file_to_link(char* file_name)
{
	save_file_link_p link_list_node = OS_NULL;
	link_list_node = file_linke_create();
	if(OS_NULL == link_list_node)
	{
		rt_kprintf("create link_list_node failed!\n");
	}
	else
	{
//		rt_kprintf("--------------link_list_add :  %s \n",file_name);
		memmove(link_list_node->file_name,file_name,sizeof(link_list_node->file_name));
		file_list_insert_of_end(link_list_node);
	}
}
//static void add_file_to_unlink(save_file_link_p link_list_node)
//{
//    link_list_node->prev->next = link_list_node->next;
//	if (link_list_node->next)
//	{
//	    link_list_node->next->prev = link_list_node->prev;
//	}
//	rt_free(link_list_node);
//}


/**
 * @brief
 * @param
 * @return
 * @note   data save thread entry
**/
static void data_save_thread_entry(void * parameter)
{
	rt_uint32_t e;
	FILE * vib_ini = OS_NULL;
	app_select_num_p select_channel_num = OS_NULL;
	save_time_t save_time;
	uint16_t save_len = 0;
	char file_name_buff[32];
	struct rt_event * ad7606_event;
	app_ad7606_data_local_t* ad7606_data;

	ad7606_data = app_ad7606_data_getbuff();
	ad7606_event = app_ad7606_event_get();
	get_current_time(&save_time.old);
	sd_data_delete();

	//开机生成存储文件
	memset(file_name_buff,0,sizeof(file_name_buff));
	vib_ini = app_create_save_file(file_name_buff, save_time.old, save_vib);

	while(1)
	{
		if(rt_event_recv(ad7606_event,APP_AD7606_DATA_SAVE_EVENT,EVENT_FLAG_OR | EVENT_FLAG_CLEAR,WAITING_FOREVER,&e) != OS_EOK)
		{
			continue;
		}
		else
		{
//			rt_kprintf("---channel: save:\n");
			get_current_time(&save_time.new);
			//每分钟创建一个文件
			if((save_time.new.tm_min != save_time.old.tm_min) && ((save_time.new.tm_sec>=0) && (save_time.new.tm_sec<10)))
			{
				//关闭之前文件
				if(OS_NULL != vib_ini)
				{
					rt_kprintf("---%s  size: %d\n",file_name_buff,ftell(vib_ini));
					fclose(vib_ini);
					vib_ini = OS_NULL;
				}
				add_file_to_link(file_name_buff);
				save_time.old = save_time.new;
				memset(file_name_buff,0,sizeof(file_name_buff));
				//创建新文件
				vib_ini = app_create_save_file(file_name_buff, save_time.old, save_vib);
				rt_sem_release(&env.save_sem);
//				rt_kprintf("---create vib file: %s\n",file_name_buff);
			}
			if(OS_NULL != vib_ini)
			{
				save_data_fill_head(save_time.new);
				save_len = sizeof(clb_save_data_head_t) + APP_SAVE_CH_BUFF_SIZE_MAX*2;
				head.data_len[0] = (uint8_t)(save_len>>8);
				head.data_len[1] = (uint8_t)(save_len & 0x00ff);
			    select_channel_num = app_channel_adg1608_get_select();
				for(int i = 0; i < DEF_CLB_DATA_CH_MAX; i++)
				{
					//填充adg1608片选模拟通道
					switch (i)
					{
						case motor_gear_impact_ch:
							/* typed */
							if(select_channel_num->save_motor_gear_num % 2)
							{
								head.save_data_type = gear_impact;
							}
							else
							{
								head.save_data_type = motor_impact;
							}
							head.channel = select_channel_num->save_motor_gear_num;
							break;

						case motor_gear_vibration_ch:
							if(select_channel_num->save_motor_gear_num % 2)
							{
								head.save_data_type = gear_vibration;
							}
							else
							{
								head.save_data_type = motor_vibration;
							}
							head.channel = select_channel_num->save_motor_gear_num;
							break;

						case axle_impact_ch:
							head.save_data_type = bearing_impact;
							head.channel = select_channel_num->save_axle_box_num;
							break;

						case axle_vibration_ch:
							head.save_data_type = bearing_vibration;
							head.channel = select_channel_num->save_axle_box_num;
							break;

//						case wave_wear_ch1:
//							head.save_data_type = wave_grinding;
//							head.channel = select_channel_num->save_wave_num*2-1;
//							break;
//						case wave_wear_ch2:
//							head.save_data_type = wave_grinding;
//							head.channel = select_channel_num->save_wave_num*2;
//							break;

						default:
							break;
					}
					//写入采集数据
					fwrite(&head,sizeof(clb_save_data_head_t), 1, vib_ini);
					fwrite(ad7606_data->ad7606_data.ch[i].data_buffer, APP_SAVE_CH_BUFF_SIZE_MAX*2, 1, vib_ini);
				}
			}
			vib_save_flag = 1;//写完之后算法可进行计算
		}
        rt_thread_mdelay(50);
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   data save thread create
**/
void app_data_save_thread_create(void)
{
	env.save_thread = rt_thread_create("data_save", data_save_thread_entry, OS_NULL, \
										   APP_DATA_SAVE_STACK, APP_DATA_SAVE_PRO, 30);
	if(env.save_thread != OS_NULL)
	{
		rt_thread_startup(env.save_thread);
		rt_kprintf("data save create success!\n");
	}
	else {
		rt_kprintf("data save create failed!\n");
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_data_save_thread_init(void)
{
	//信号量初始化
	if (rt_sem_init(&env.save_sem, "up_sem", 0, IPC_FLAG_FIFO) != OS_EOK)
	{
		rt_kprintf("%s line:%d time : save sem init failed !!!\n", __FILE__, __LINE__);
	}
    //链表初始化
	memset(&env.save_file_head,0,sizeof(save_file_link_t));
	memset(&env.save_info_head,0,sizeof(save_info_link_t));
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
struct rt_semaphore* app_data_save_get_sem(void)
{
	return &env.save_sem;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
save_file_link_p app_data_save_file_head_get(void)
{
	return &env.save_file_head;
}
