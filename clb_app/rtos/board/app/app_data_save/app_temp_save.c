/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* includes */
#include <stdio.h>
#include "dfs_posix.h"
#include "unistd.h"
#include "clb_save_protocol.h"
#include "rtthread.h"
#include "app_self_test.h"
#include "app_temp_save.h"
#include "clb_record_protocol.h"
#include "app_send_record.h"
#include "app_iwdg.h"

/** macro */
#define TEMP_MAX_SIZE                DEF_TEMP_MAX_SIZE //4个前置器的温度总数据大小
#define TEMP_ONE_SIZE                DEF_TEMP_ONE_SIZE //1个前置器的温度数据大小，（4传感器+1环温）*2字节*每秒10个温度值
/* type declaration */

/* variable declaration */
app_temp_save_env_t app_temp_save_env;
#define env app_temp_save_env

/* function declaration */

/**
 * @brief  temp_save_thread_entry
 * @param
 * @return
 * @note  存储温度信息到文件
**/
static void temp_save_thread_entry(void * parameter)
{
	clb_save_data_head_t head = {0};
	FILE * temp_ini = OS_NULL;
	uint16_t save_len;
	save_time_t save_time;
	struct rt_semaphore* up_sem = app_data_save_get_sem();
	uint8_t train_id = app_send_get_train_num();
	uint8_t host_flag = app_send_get_host_flag();

	char file_name_buff[32];

	get_current_time(&save_time.old);
	//开机生成存储文件
	memset(file_name_buff,0,sizeof(file_name_buff));
	temp_ini = app_create_save_file(file_name_buff, save_time.old, save_temp);
	while(1)
	{
		get_current_time(&save_time.new);
		//每分钟创建一个文件
		if((save_time.new.tm_min != save_time.old.tm_min) && ((save_time.new.tm_sec>=40) && (save_time.new.tm_sec<50)))
		{
			//关闭之前文件
			if(OS_NULL != temp_ini)
			{
				rt_kprintf("---%s  size: %d\n",file_name_buff,ftell(temp_ini));
				fclose(temp_ini);
				temp_ini = OS_NULL;
			}
			add_file_to_link(file_name_buff);
			save_time.old = save_time.new;
			memset(file_name_buff,0,sizeof(file_name_buff));
			//创建新文件
			temp_ini = app_create_save_file(file_name_buff, save_time.old, save_temp);
			rt_sem_release(up_sem);
//			rt_kprintf("---create temp file: %s\n",file_name_buff);
		}

		if(OS_NULL != temp_ini)
		{
			static rt_uint16_t pack_cnt = 0;

			// 填充头信息
			head.save_data_head_h = 0x66;
			head.save_data_head_l = 0xbb;
			head.year = save_time.new.tm_year-100;
			head.month = save_time.new.tm_mon+1;
			head.day = save_time.new.tm_mday;
			head.hour = save_time.new.tm_hour;
			head.min = save_time.new.tm_min;
			head.sec = save_time.new.tm_sec;
			head.pack_count_h = pack_cnt & 0xff;
			head.pack_count_l = (pack_cnt >> 8) & 0xff;
			pack_cnt++;
			head.host_slave_flag = host_flag;
			head.save_data_type = qzq_temp;

			head.train_id = train_id;
			save_len = sizeof(clb_save_data_head_t) + TEMP_MAX_SIZE;
			head.data_len[0] = (uint8_t)(save_len>>8);
			head.data_len[1] = (uint8_t)(save_len & 0x00ff);
			//写入采集数据
			fwrite(&head,sizeof(clb_save_data_head_t), 1, temp_ini);
			fwrite(proximitor_data.proximitor_data1.temp_orignal, TEMP_ONE_SIZE, 1, temp_ini);
			fwrite(proximitor_data.proximitor_data2.temp_orignal, TEMP_ONE_SIZE, 1, temp_ini);
			fwrite(proximitor_data.proximitor_data3.temp_orignal, TEMP_ONE_SIZE, 1, temp_ini);
			fwrite(proximitor_data.proximitor_data4.temp_orignal, TEMP_ONE_SIZE, 1, temp_ini);
		}
		/* feed dog */
//		app_feed_dog();
		rt_thread_mdelay(1000);
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   temp data save thread create
**/
void app_temp_save_thread_create(void)
{
	env.temp_thread = rt_thread_create("temp_save", temp_save_thread_entry, OS_NULL, \
										   APP_TEMP_SAVE_STACK, APP_TEMP_SAVE_PRO, 30);
	if(env.temp_thread != OS_NULL)
	{
		rt_thread_startup(env.temp_thread);
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
void app_temp_save_thread_init(void)
{
}



