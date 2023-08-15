/**
 * @file
 * @details  cominform = communication information
 * @author
 * @date     2021.09
 * @version  V1.0
**/

/* include */
#include <drv_config.h>
#include <rtconfig.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <finsh.h>
#include "ezini.h"
#include "rtthread.h"
#include "app_cominform_config.h"
#include "util.h"
#include "app_paras_save.h"

/* macro */
#define SYSTEM_PARA_FILE 					"/config/system.ini"
#define COMM_PARA_FILE 						"/config/communi.ini"
#define ALGO_CONF_PARA_FILE 				"/config/algoconf.ini"
#define ALGO_THRE_PARA_FILE 				"/config/algothre.ini"
#define SELF_TEST_PARA_FILE 				"/config/selftest.ini"

#define PROD_FIXED_SECTION_NAME    	 	"prod_fixed"
#define DEV_PARA_SECTION_NAME     		"dev_para"
#define COMM_PARA_SECTION_NAME     		"comm_para"
#define COLL_PARA_SECTION_NAME     		"coll_para"
#define POLY_PARA_SECTION_NAME     		"poly_para"
#define END_BEAR_SECTION_NAME      		"end_bear"
#define GEAR_BEAR_SECTION_NAME      	"gear_bear"
#define DERA_PARA_SECTION_NAME      	"dera_para"
#define AXLE_BOX_SECTION_NAME      		"axle_box_temp"
#define MOTOR_TEMP_SECTION_NAME     	"motor_temp"
#define GEAR_TEMP_SECTION_NAME     		"gear_temp"
#define POLY_THRE_SECTION_NAME     		"poly_thre"
#define END_BEAR_THRE_SECTION_NAME  	"end_bear_thre"
#define MOTOR_BEAR_THRE_SECTION_NAME	"motor_bear_thre"
#define GEAR_BEAR_THRE_SECTION_NAME		"gear_bear_thre"
#define SELF_TEST_THRE_SECTION_NAME		"self_test_thre"
#define SELF_TEST_RESU_SECTION_NAME		"self_test_resu"

#define KEY_PROD_FIXED_NUM_NAME        "prod_number"
#define KEY_PROD_FIXED_TIME_NAME       "prod_timer"
#define KEY_HARD_VERSION_NAME       	"hard_version"

#define KEY_DEV_TRAIN_NUM_NAME       	"train_number"
#define KEY_DEV_CAR_NUM_NAME       		"car_number"
#define KEY_DEV_BOARD_NUM_NAME       	"board_number"
#define KEY_DEV_INSTALL_TIME_NAME      "install_time"
#define KEY_DEV_OLD_SOFT_VER_NAME      "old_soft_version"
#define KEY_DEV_CURR_SOFT_VER_NAME     "curr_soft_version"
#define KEY_UPGRADE_TIME_NAME     		"upgrade_time"
#define KEY_CONFIG_TIME_NAME     		"config_time"
#define KEY_CONFIG_RESET_NAME     		"reset_flag"

#define KEY_COMM_LOCAL_TRAIN_ID_NAME   "train_id"
#define KEY_COMM_LOCAL_IP_NAME       	"local_ip"
#define KEY_COMM_LOCAL_MASK_NAME       "local_mask"
#define KEY_COMM_LOCAL_GATE_NAME       "local_gate"

#define KEY_COLL_FREQ_NAME       		"coll_freq"
#define KEY_COLL_TIME_INTE_NAME      	"coll_time_inte"

#define KEY_POLY_SENSITIVITY_NAME      "sensitivity"
#define KEY_POLY_WHEEL_DIAM_NAME      	"wheel_diameter"
#define KEY_POLY_UPPER_BOUN_VALUE_NAME "upper_boun_value"
#define KEY_POLY_LOWER_BOUN_VALUE_NAME "lower_boun_value"
#define KEY_POLY_CONS_SAME_ORDER_NAME 	"cons_same_order"

#define KEY_BEAR_LOWER_INNER_RING_NAME	"lower_inner_ring"
#define KEY_BEAR_UPPER_INNER_RING_NAME "upper_inner_ring"
#define KEY_BEAR_LOWER_OUTER_RING_NAME "lower_outer_ring"
#define KEY_BEAR_UPPER_OUTER_RING_NAME "upper_outer_ring"
#define KEY_BEAR_LOWER_ROLLING_NAME 	"lower_rolling"
#define KEY_BEAR_UPPER_ROLLING_NAME 	"upper_rolling"
#define KEY_BEAR_LOWER_CAGE_RING_NAME 	"lower_cage_ring"
#define KEY_BEAR_UPPER_CAGE_RING_NAME 	"upper_cage_ring"

/* variable declaration */
sys_para_t sys_conifg;

/* function declaration */
/**
 * @brief
 * @param
 * @return
 * @note
**/
static int parse_prod_fixed(prod_fixed_p p_prod_fixed,ini_entry_t * entry)
{
	if(!strcmp(KEY_PROD_FIXED_NUM_NAME,entry->key))
	{
		memcpy(p_prod_fixed->prod_number,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_PROD_FIXED_TIME_NAME,entry->key))
	{
		memcpy(p_prod_fixed->prod_timer,entry->value,strlen(entry->value));
	}
	else if(!strcmp(KEY_HARD_VERSION_NAME,entry->key))
	{
		memcpy(p_prod_fixed->hard_version,entry->value,strlen(entry->value));
	}
	else
	{
		rt_kprintf("WARNING:%s should not contain %s\n",PROD_FIXED_SECTION_NAME,entry->key);
		return -1;
	}

	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static int write_prod_fixed(prod_fixed_p p_prod_fixed,char * key,char *value)
{
	if(!strcmp(KEY_PROD_FIXED_NUM_NAME,key))
	{
		memset(p_prod_fixed->prod_number,0,sizeof(p_prod_fixed->prod_number));
		memcpy(p_prod_fixed->prod_number,value,strlen(value));
	}
	else if(!strcmp(KEY_PROD_FIXED_TIME_NAME,key))
	{
		memset(p_prod_fixed->prod_timer,0,sizeof(p_prod_fixed->prod_timer));
		memcpy(p_prod_fixed->prod_timer,value,strlen(value));
	}
	else if(!strcmp(KEY_HARD_VERSION_NAME,key))
	{
		memset(p_prod_fixed->hard_version,0,sizeof(p_prod_fixed->hard_version));
		memcpy(p_prod_fixed->hard_version,value,strlen(value));
	}
	else
	{
		rt_kprintf("WARNING:%s should not contain %s\n",PROD_FIXED_SECTION_NAME,key);
		return -1;
	}



	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static void display_prod_fixed(prod_fixed_p p_prod_fixed)
{
	rt_kprintf("%s:\n",PROD_FIXED_SECTION_NAME);
	rt_kprintf("prod_number = %s\n",p_prod_fixed->prod_number);
	rt_kprintf("prod_timer = %s\n",p_prod_fixed->prod_timer);
	rt_kprintf("hard_version = %s\n",p_prod_fixed->hard_version);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
 static int parse_dev_para(dev_para_p p_dev_para,ini_entry_t * entry)
 {
 	if(!strcmp(KEY_DEV_TRAIN_NUM_NAME,entry->key))
 	{
 		memcpy(p_dev_para->train_number,entry->value,strlen(entry->value));
 	}
 	else if(!strcmp(KEY_DEV_CAR_NUM_NAME,entry->key))
 	{
 		memcpy(p_dev_para->car_num,entry->value,strlen(entry->value));
 	}
 	else if(!strcmp(KEY_DEV_BOARD_NUM_NAME,entry->key))
 	{
 		p_dev_para->board_num = srt_to_uint32(entry->value);
 	}
 	else if(!strcmp(KEY_DEV_INSTALL_TIME_NAME,entry->key))
 	{
 		memcpy(p_dev_para->install_time,entry->value,strlen(entry->value));
 	}
 	else if(!strcmp(KEY_DEV_OLD_SOFT_VER_NAME,entry->key))
 	{
 		memcpy(p_dev_para->old_soft_version,entry->value,strlen(entry->value));
 	}
 	else if(!strcmp(KEY_DEV_CURR_SOFT_VER_NAME,entry->key))
 	{
 		memcpy(p_dev_para->curr_soft_version,entry->value,strlen(entry->value));
 	}
 	else if(!strcmp(KEY_UPGRADE_TIME_NAME,entry->key))
 	{
 		memcpy(p_dev_para->upgr_time,entry->value,strlen(entry->value));
 	}
 	else if(!strcmp(KEY_CONFIG_TIME_NAME,entry->key))
 	{
 		memcpy(p_dev_para->conf_time,entry->value,strlen(entry->value));
 	}
 	else if(!strcmp(KEY_CONFIG_RESET_NAME,entry->key))
 	{
 		p_dev_para->reset_flag=srt_to_uint32(entry->value);
 	}
 	else
 	{
 		rt_kprintf("WARNING:%s should not contain %s\n",DEV_PARA_SECTION_NAME,entry->key);
 		return -1;
 	}

 	return 0;
}

 /**
  * @brief
  * @param
  * @return
  * @note
 **/
static int write_dev_para(dev_para_p p_dev_para,char * key,char *value)
{
	if(!strcmp(KEY_DEV_TRAIN_NUM_NAME,key))
	{
		memset(p_dev_para->train_number,0,sizeof(p_dev_para->train_number));
		memcpy(p_dev_para->train_number,value,strlen(value));
	}
	else if(!strcmp(KEY_DEV_CAR_NUM_NAME,key))
	{
		memset(p_dev_para->car_num,0,sizeof(p_dev_para->car_num));
		memcpy(p_dev_para->car_num,value,strlen(value));
	}
	else if(!strcmp(KEY_DEV_BOARD_NUM_NAME,key))
	{
		p_dev_para->board_num = srt_to_uint32(value);
	}
	else if(!strcmp(KEY_DEV_INSTALL_TIME_NAME,key))
	{
		memset(p_dev_para->install_time,0,sizeof(p_dev_para->install_time));
		memcpy(p_dev_para->install_time,value,strlen(value));
	}
	else if(!strcmp(KEY_DEV_OLD_SOFT_VER_NAME,key))
	{
		memset(p_dev_para->old_soft_version,0,sizeof(p_dev_para->old_soft_version));
		memcpy(p_dev_para->old_soft_version,value,strlen(value));
	}
	else if(!strcmp(KEY_DEV_CURR_SOFT_VER_NAME,key))
	{
		memset(p_dev_para->curr_soft_version,0,sizeof(p_dev_para->curr_soft_version));
		memcpy(p_dev_para->curr_soft_version,value,strlen(value));
	}
	else if(!strcmp(KEY_UPGRADE_TIME_NAME,key))
	{
		memset(p_dev_para->upgr_time,0,sizeof(p_dev_para->upgr_time));
		memcpy(p_dev_para->upgr_time,value,strlen(value));
	}
	else if(!strcmp(KEY_CONFIG_TIME_NAME,key))
	{
		memset(p_dev_para->conf_time,0,sizeof(p_dev_para->conf_time));
		memcpy(p_dev_para->conf_time,value,strlen(value));
	}
	else if(!strcmp(KEY_CONFIG_RESET_NAME,key))
	{
		p_dev_para->reset_flag=srt_to_uint32(value);
	}
	else
	{
		rt_kprintf("WARNING:%s should not contain %s\n",DEV_PARA_SECTION_NAME,key);
		return -1;
	}

	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static void display_dev_para(dev_para_p p_dev_para)
{
 	rt_kprintf("%s:\n",DEV_PARA_SECTION_NAME);
 	rt_kprintf("train_number = %s\n",p_dev_para->train_number);
 	rt_kprintf("car_num = %s\n",p_dev_para->car_num);
 	rt_kprintf("board_num = %u\n",p_dev_para->board_num);
 	rt_kprintf("install_time = %s\n",p_dev_para->install_time);
 	rt_kprintf("old_soft_version = V%s\n",p_dev_para->old_soft_version);
 	rt_kprintf("curr_soft_version = V%s\n",p_dev_para->curr_soft_version);
 	rt_kprintf("upgr_time = %s\n",p_dev_para->upgr_time);
 	rt_kprintf("upgr_time = %s\n",p_dev_para->upgr_time);
 	rt_kprintf("reset_flag = %d\n",p_dev_para->reset_flag);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
int read_system_ini(sys_para_p p_sys_para)
{
//	int result;
//	FILE *fp;
//	ini_entry_t entry;

	if(NULL == p_sys_para)
	{
		rt_kprintf("ERROR: input point is NULL!\n");
		return -1;
	}

//	fp = fopen(SYSTEM_PARA_FILE, "r");
//	if(fp == NULL)
//	{
//		rt_kprintf("ERROR: File open failed!\n");
//		return -1;
//	}
//
//	/* initialize entry structure before reading first entry */
//	entry.section = NULL;
//	entry.key = NULL;
//	entry.value = NULL;
//
//	/* read one entry at a time */
//	while((result = GetEntryFromFile(fp, &entry)) > 0)
//	{
//		if(!strcmp(PROD_FIXED_SECTION_NAME,entry.section))
//		{
//			parse_prod_fixed(&p_sys_para->prod_fixed,&entry);
//		}
//		else if(!strcmp(DEV_PARA_SECTION_NAME,entry.section))
//		{
//			parse_dev_para(&p_sys_para->dev_para,&entry);
//		}
//		else
//		{
//			rt_kprintf("WARNING:%s should not contain %s\n",SYSTEM_PARA_FILE,entry.section);
//		}
//	}
//
//	fclose(fp);
//	p_sys_para = app_paras_get();
	p_sys_para = &app_paras_save_env.paras_inform.paras.sys_para;
	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
int write_system_ini(sys_para_p p_sys_para)
{
	FILE * ini;

	if((ini = fopen(SYSTEM_PARA_FILE, "w"))==NULL) {
		rt_kprintf("ERROR: open %s failed!\n",SYSTEM_PARA_FILE);
		return -1;
	}

	fseek(ini,0,SEEK_SET);

	fprintf(ini,
		"#\n"
		"# This is a system of ini file\n"
		"#\n"
		"\n"
		"[%s]\n"
		"\n"
		"prod_number       = %s\n"
		"prod_timer        = %s\n"
		"hard_version      = %s\n"
		"\n"
		"\n"
		"[%s]\n"
		"\n"
		"train_number      = %s\n"
		"car_number        = %s\n"
		"board_number      = %u\n"
		"install_time      = %s\n"
		"old_soft_version  = %s\n"
		"curr_soft_version = %s\n"
		"upgrade_time      = %s\n"
		"config_time       = %s\n"
		"reset_flag        = %d\n"
		"\n",PROD_FIXED_SECTION_NAME,p_sys_para->prod_fixed.prod_number,p_sys_para->prod_fixed.prod_timer,p_sys_para->prod_fixed.hard_version,
		DEV_PARA_SECTION_NAME,p_sys_para->dev_para.train_number,p_sys_para->dev_para.car_num,p_sys_para->dev_para.board_num,
		p_sys_para->dev_para.install_time,p_sys_para->dev_para.old_soft_version,p_sys_para->dev_para.curr_soft_version,
		p_sys_para->dev_para.upgr_time,p_sys_para->dev_para.conf_time,p_sys_para->dev_para.reset_flag);

	fclose(ini);

	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static int write_default_sys_para()
{
	int result = 0;

	sys_para_t default_sys_para;

	memcpy(default_sys_para.prod_fixed.prod_number,"20210306",9);
	memcpy(default_sys_para.prod_fixed.prod_timer,"2021.03.06",11);
	memcpy(default_sys_para.prod_fixed.hard_version,"LH-TFDSN-CLB",13);

	memcpy(default_sys_para.dev_para.train_number,"2021",5);
	memcpy(default_sys_para.dev_para.car_num,"1",2);
	default_sys_para.dev_para.board_num = CU_LI_BOADR;
	memcpy(default_sys_para.dev_para.install_time,"2021.03.06",11);
	memcpy(default_sys_para.dev_para.old_soft_version,"1.1",4);
	memcpy(default_sys_para.dev_para.curr_soft_version,"1.2",4);
	memcpy(default_sys_para.dev_para.upgr_time,"2021.03.06",11);
	memcpy(default_sys_para.dev_para.conf_time,"2021.03.06",11);
	default_sys_para.dev_para.reset_flag=0;
//	result = write_system_ini(&default_sys_para);

	return result;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
uint8_t update_sys_para(void)
{
	uint8_t ret=0;
	paras_t *paras = app_paras_get();
   if(paras->sys_para.dev_para.reset_flag)
   {
	   paras->sys_para.dev_para.reset_flag=0;
	   ret=1;
   }
   app_paras_update();
   return ret;
}

uint8_t save_sys_para(void)
{
	paras_t *paras = app_paras_get();
	paras->sys_para.dev_para.reset_flag=1;
//	write_system_ini(&sys_conifg);
    app_paras_update();
	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static int parse_communi_para(comm_para_p p_comm_para,ini_entry_t * entry)
{
	if(!strcmp(KEY_COMM_LOCAL_TRAIN_ID_NAME,entry->key))
	{
		p_comm_para->train_id = srt_to_uint32(entry->value);
	}
	else if(!strcmp(KEY_COMM_LOCAL_IP_NAME,entry->key))
 	{
 		memcpy(p_comm_para->local_ip,entry->value,strlen(entry->value));
 	}
 	else if(!strcmp(KEY_COMM_LOCAL_MASK_NAME,entry->key))
 	{
 		memcpy(p_comm_para->local_mask,entry->value,strlen(entry->value));
 	}
 	else if(!strcmp(KEY_COMM_LOCAL_GATE_NAME,entry->key))
 	{
 		memcpy(p_comm_para->local_gateway,entry->value,strlen(entry->value));
 	}
 	else
 	{
 		rt_kprintf("WARNING:%s should not contain %s\n",DEV_PARA_SECTION_NAME,entry->key);
 		return -1;
 	}

 	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static int write_communi_para(comm_para_p p_comm_para,char * key,char * value)
{
	if(!strcmp(KEY_COMM_LOCAL_TRAIN_ID_NAME,key))
	{
		p_comm_para->train_id = srt_to_uint32(value);
	}
	else if(!strcmp(KEY_COMM_LOCAL_IP_NAME,key))
	{
		memset(p_comm_para->local_ip,0,sizeof(p_comm_para->local_ip));
		memcpy(p_comm_para->local_ip,value,strlen(value));
	}
	else if(!strcmp(KEY_COMM_LOCAL_MASK_NAME,key))
	{
		memset(p_comm_para->local_mask,0,sizeof(p_comm_para->local_mask));
		memcpy(p_comm_para->local_mask,value,strlen(value));
	}
	else if(!strcmp(KEY_COMM_LOCAL_GATE_NAME,key))
	{
		memset(p_comm_para->local_gateway,0,sizeof(p_comm_para->local_gateway));
		memcpy(p_comm_para->local_gateway,value,strlen(value));
	}
	else
	{
		rt_kprintf("WARNING:%s should not contain %s\n",DEV_PARA_SECTION_NAME,key);
		return -1;
	}

	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static void display_comm_para(comm_para_p p_comm_para)
{
 	rt_kprintf("%s:\n",COMM_PARA_SECTION_NAME);
 	rt_kprintf("train_id = %d\n",p_comm_para->train_id);
 	rt_kprintf("local_ip = %s\n",p_comm_para->local_ip);
 	rt_kprintf("local_mask = %s\n",p_comm_para->local_mask);
 	rt_kprintf("local_gateway = %s\n",p_comm_para->local_gateway);

 }

/**
 * @brief
 * @param
 * @return
 * @note
**/
int read_communication_ini(comm_para_p p_comm_para)
{
	int result;
	FILE *fp;
	ini_entry_t entry;

	if(NULL == p_comm_para)
	{
		rt_kprintf("ERROR: input point is NULL!\n");
		return -1;
	}

	fp = fopen(COMM_PARA_FILE, "r");
	if(fp == NULL)
	{
		rt_kprintf("ERROR: File open failed!\n");
		return -1;
	}

	/* initialize entry structure before reading first entry */
	entry.section = NULL;
	entry.key = NULL;
	entry.value = NULL;

	/* read one entry at a time */
	while((result = GetEntryFromFile(fp, &entry)) > 0)
	{
		if(!strcmp(COMM_PARA_SECTION_NAME,entry.section))
		{
			parse_communi_para(p_comm_para,&entry);
		}
		else
		{
			rt_kprintf("WARNING:%s should not contain %s\n",COMM_PARA_FILE,entry.section);
		}
	}

	fclose(fp);
	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
int write_communi_ini(comm_para_p p_comm_para)
{
	FILE * ini ;

	if((ini = fopen(COMM_PARA_FILE, "w"))==NULL) {
		rt_kprintf("ERROR: open %s failed!\n",COMM_PARA_FILE);
		return -1;
	}

	fseek(ini,0,SEEK_SET);

	fprintf(ini,
		"#\n"
		"# This is a system of ini file\n"
		"#\n"
		"\n"
		"[%s]\n"
		"\n"
		"train_id            = %d\n"
		"local_ip            = %s\n"
		"local_mask          = %s\n"
		"local_gate          = %s\n"
		"\n",COMM_PARA_SECTION_NAME,p_comm_para->train_id,p_comm_para->local_ip,p_comm_para->local_mask,p_comm_para->local_gateway);

	fclose(ini);

	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static int write_default_comm_para()
{
	int result;
	comm_para_t default_comm_para;
	default_comm_para.train_id=0;
	memcpy(default_comm_para.local_ip,"192.168.0.13",15);
	memcpy(default_comm_para.local_mask,"255.255.0.0",16);
	memcpy(default_comm_para.local_gateway,"192.168.0.1",13);

	result = write_communi_ini(&default_comm_para);

	return result;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static int parse_coll_para(coll_para_p p_coll_para,ini_entry_t * entry)
{
	if(!strcmp(KEY_COLL_FREQ_NAME,entry->key))
	{
		p_coll_para->coll_freq = srt_to_uint32(entry->value);
	}
	else if(!strcmp(KEY_COLL_TIME_INTE_NAME,entry->key))
	{
		p_coll_para->coll_time_interval = srt_to_uint32(entry->value);
	}
	else
	{
		rt_kprintf("WARNING:%s should not contain %s\n",DEV_PARA_SECTION_NAME,entry->key);
		return -1;
	}

	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
int read_algo_config_ini(algo_config_para_p p_algo_config_para)
{
	int result;
	FILE *fp;
	ini_entry_t entry;

	if(NULL == p_algo_config_para)
	{
		rt_kprintf("ERROR: input point is NULL!\n");
		return -1;
	}

	fp = fopen(ALGO_CONF_PARA_FILE, "r");
	if(fp == NULL)
	{
		rt_kprintf("ERROR: File open failed!\n");
		return -1;
	}

	/* initialize entry structure before reading first entry */
	entry.section = NULL;
	entry.key = NULL;
	entry.value = NULL;

	/* read one entry at a time */
	while((result = GetEntryFromFile(fp, &entry)) > 0)
	{
		if(!strcmp(COLL_PARA_SECTION_NAME,entry.section))
		{
			parse_coll_para(&p_algo_config_para->coll_para,&entry);
		}
		else
		{
			rt_kprintf("WARNING:%s should not contain %s\n",COMM_PARA_FILE,entry.section);
		}
	}

	fclose(fp);
	return -1;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
int config_ini_file_init()
{
	//int result;

	if(0 != access(SYSTEM_PARA_FILE, F_OK))
	{
		write_default_sys_para();
	}

	if(0 != access(COMM_PARA_FILE, F_OK))
	{
		write_default_comm_para();
	}

	return 0;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static void config(uint8_t argc, char **argv) {

#define __is_print(ch)                ((unsigned int)((ch) - ' ') < 127u - ' ')
#define HEXDUMP_WIDTH                 16

#define CMD_FORMAT_INDEX              0
#define CMD_PROD_FIXED_INDEX          1
#define CMD_DEV_PARA_INDEX            2
#define CMD_COMMUNI_PARA_INDEX        3

#define OPS_HELP                      "h"
#define OPS_READ                      "r"
#define OPS_WRITE                     "w"
#define OPS_SYNC                      "s"

	const char * config_help_info[] =
	{
		[CMD_FORMAT_INDEX]     			= "config [section] [ops] [key] [value]   - config format,ops have r(read),w(wrtie),s(sync),h(help)",
		[CMD_PROD_FIXED_INDEX]     	= "config prod_fixed                      - configuration production fixed parameters",
		[CMD_DEV_PARA_INDEX]     	 	= "config dev_para                        - configuration device parameters",
		[CMD_COMMUNI_PARA_INDEX]    	= "config comm_para                       - configuration communication parameters",
	};

	const char * prod_fixed_help_info[] =
	{
		[0]     								= "config prod_fixed prod_number    - configuration production serial number",
		[1]     	 							= "config prod_fixed prod_timer     - configuration production timer",
		[2]    								= "config prod_fixed hard_version   - configuration hardware version",
	};

	const char * dev_para_help_info[] =
	{
		[0]     								= "config dev_para train_number        - configuration device parameters train number",
		[1]     	 							= "config dev_para car_number          - configuration device parameters car number",
		[2]    								= "config dev_para board_number        - configuration device parameters hardware version",
		[3]     								= "config dev_para install_time        - configuration device parameters installation time ",
		[4]     	 							= "config dev_para old_soft_version    - configuration device parameters old software version",
		[5]    								= "config dev_para curr_soft_version   - configuration device parameters current software version",
		[6]     	 							= "config dev_para upgrade_time        - configuration device parameters upgrade time",
		[7]    								= "config dev_para config_time         - configuration device parameters configuration time",
	};

	const char * communi_para_help_info[] =
	{
		[0]     								= "config comm_para train_id          - configuration communication parameters train_id",
		[1]     								= "config comm_para local_ip           - configuration communication parameters local IP",
		[2]     	 							= "config comm_para local_mask         - configuration communication parameters local mask",
		[3]    								= "config comm_para local_gate      	- configuration communication parameters local gateway",
	};

	char * section = NULL;
	char * key = NULL;
	char * ops = NULL;
	char * value = NULL;
	int i = 0;

	sys_para_t sys_para_conifg;
	comm_para_t comm_para_config;
	paras_t *paras = app_paras_get();

	memset(&sys_para_conifg,0,sizeof(sys_para_conifg));
	memset(&comm_para_config,0,sizeof(comm_para_config));

	if(argc < 2)
	{
		rt_kprintf("Usage:\n");
		for (i = 0; i < sizeof(config_help_info) / sizeof(char*); i++)
		{
			rt_kprintf("%s\n", config_help_info[i]);
		}
		rt_kprintf("\n");
	}
	else if(argc < 3)
	{
		rt_kprintf("ERROR:number of parameter is error!\n");
	}
	else if(argc < 4)
	{
		rt_kprintf("Usage:\n");

		section = argv[1];
		ops = argv[2];

		if(!strcmp(PROD_FIXED_SECTION_NAME,section))
		{
			if(!strcmp(OPS_HELP,ops))
			{
				for(i = 0; i < sizeof(prod_fixed_help_info) / sizeof(char*); i++)
				{
					rt_kprintf("%s\n", prod_fixed_help_info[i]);
				}
				rt_kprintf("\n");
			}
			else if(!strcmp(OPS_READ,ops))
			{
//				read_system_ini(&sys_para_conifg);
				memcpy(&sys_para_conifg, &paras->sys_para, sizeof(sys_para_t));
				display_prod_fixed(&sys_para_conifg.prod_fixed);
			}
			else if(!strcmp(OPS_SYNC,ops))
			{
//				read_system_ini(&sys_para_conifg);
				memcpy(&sys_para_conifg, &paras->sys_para, sizeof(sys_para_t));
//				write_system_ini(&sys_para_conifg);
			}
			else
			{
				rt_kprintf("ERROR:ops parameter is error!\n");
			}
		}
		else if(!strcmp(DEV_PARA_SECTION_NAME,section))
		{
			if(!strcmp(OPS_HELP,ops))
			{
				for(i = 0; i < sizeof(dev_para_help_info) / sizeof(char*); i++)
				{
					rt_kprintf("%s\n", dev_para_help_info[i]);
				}
				rt_kprintf("\n");
			}
			else if(!strcmp(OPS_READ,ops))
			{
				memcpy(&sys_para_conifg, &paras->sys_para, sizeof(sys_para_t));
//				read_system_ini(&sys_para_conifg);
				display_dev_para(&sys_para_conifg.dev_para);
			}
			else
			{
				rt_kprintf("ERROR:ops parameter is error!\n");
			}
		}
		else if(!strcmp(COMM_PARA_SECTION_NAME,section))
		{
			if(!strcmp(OPS_HELP,ops))
			{
				for(i = 0; i < sizeof(communi_para_help_info) / sizeof(char*); i++)
				{
					rt_kprintf("%s\n", communi_para_help_info[i]);
				}
				rt_kprintf("\n");
			}
			else if(!strcmp(OPS_READ,ops))
			{
				read_communication_ini(&comm_para_config);
				display_comm_para(&comm_para_config);
			}
			else
			{
				rt_kprintf("ERROR:ops parameter is error!\n");
			}
		}
		else
		{
			rt_kprintf("ERROR:section parameter is error!\n");
		}
    }
	else if(argc < 5)
	{
		rt_kprintf("ERROR:number of parameter is error!\n");
	}
	else if(argc < 6)
	{
		section = argv[1];
		ops = argv[2];
		key = argv[3];
		value = argv[4];

		if(!strcmp(PROD_FIXED_SECTION_NAME,section))
		{
			if(!strcmp(OPS_WRITE,ops))
			{
				read_system_ini(&sys_para_conifg);
				write_prod_fixed(&sys_para_conifg.prod_fixed,key,value);
				rt_kprintf("train_number:%s",sys_para_conifg.dev_para.train_number);
				write_system_ini(&sys_para_conifg);
			}
			else
			{
				rt_kprintf("ERROR:ops parameter is error!\n");
			}
		}
		else if(!strcmp(DEV_PARA_SECTION_NAME,section))
		{
			if(!strcmp(OPS_WRITE,ops))
			{
				read_system_ini(&sys_para_conifg);
				write_dev_para(&sys_para_conifg.dev_para,key,value);
				write_system_ini(&sys_para_conifg);
			}
			else
			{
				rt_kprintf("ERROR:ops parameter is error!\n");
			}
		}
		else if(!strcmp(COMM_PARA_SECTION_NAME,section))
		{
			if(!strcmp(OPS_WRITE,ops))
			{
				read_communication_ini(&comm_para_config);
				write_communi_para(&comm_para_config,key,value);
				write_communi_ini(&comm_para_config);
			}
			else
			{
				rt_kprintf("ERROR:ops parameter is error!\n");
			}
		}
		else
		{
			rt_kprintf("ERROR:section parameter is error!\n");
		}
	}
	else
	{
		rt_kprintf("ERROR:number of parameter is error!\n");
	}

	return;
}
MSH_CMD_EXPORT(config, config parameters);

static void train_config(uint8_t argc, char **argv)
{
	uint8_t trian_id=0;
	comm_para_t comm_para_config;
	char temp[16];
	if(argc != 2)
	{
		rt_kprintf("ERROR:number of parameter is error---eg:train_config id!\n");
		return;
	}
	else
	{
		trian_id=srt_to_uint32(argv[1]);
		if((trian_id>0)&&(trian_id<7))
		{
			comm_para_config.train_id=trian_id;
			sprintf(temp,"%d.%d.%d.%d",192,168,trian_id,13);
			strcpy(comm_para_config.local_ip,temp);
			strcpy(comm_para_config.local_gateway,"192.168.0.1");
			strcpy(comm_para_config.local_mask,"255.255.0.0");
			write_communi_ini(&comm_para_config);
			memset(&comm_para_config,0,sizeof(comm_para_config));
			read_communication_ini(&comm_para_config);
			display_comm_para(&comm_para_config);
		}
		else
		{
			rt_kprintf("ERROR:number [1--6]!\n");
		}
	}
}
MSH_CMD_EXPORT(train_config, trian parameters);


