/**
 * @file
 * @details  System startup parameters. Other parameters are not 
             recommended to be saved in STM32 internal flash
 * @author   ren
 * @date     2021.09
 * @version  V1.0
**/

/* include */
#include "zxb_record_protocol.h"
#include "txb_public_protocol.h"
#include "record_board_protocol.h"
#include "tftp.h"
#include "app_data_save.h"
#include "app_paras_save.h"
#include "can_app.h"

/* macro */
#define APP_PARAS_FACTORY_CODE    DEVICE_FACTORY_CODE
#define APP_PARAS_JLB_CODE        DEF_JLB_DEVICE_CODE
#define APP_PARAS_CLB_CODE        DEF_CLB_DEVICE_CODE
#define APP_PARAS_ZXB_CODE        DEF_ZXB_DEVICE_CODE
#define APP_PARAS_PTU_CODE        DEF_PTU_DEVICE_CODE
#define UDP_SEND_HEAD_LEN         DEF_BOARD_UDP_DATA_HEAD_BYTES

#define APP_RESERVE_CMD           DEF_RESERVE_CMD
#define APP_APP_UPDATE_CMD        DEF_APP_UPDATE_CMD
#define APP_SYSTEM_REBOOT_CMD     DEF_SYSTEM_REBOOT_CMD
#define APP_CONFIG_PARA_SET_CMD   DEF_CONFIG_PARA_SET_CMD
#define APP_CONFIG_PARA_READ_CMD  DEF_CONFIG_PARA_READ_CMD
#define APP_GET_QZQ_DEFAULT_ID_CMD DEF_GET_QZQ_DEFAULT_ID_CMD
#define APP_SET_QZQ_NEW_ID_CMD    DEF_SET_QZQ_NEW_ID_CMD

#define TFTP_CONFIG_FILE_IP       DEF_TFTP_HEAD_CONFIG_FILE_IP
#define REMOTE_PATH            	 DEF_CONFIG_REMOTE_PATH
#define LOCAL_PATH            	 DEF_LOCAL_CONFIG_PATH
#define REMOTE_FILE_NAME			 DEF_REMOTE_FILE_NAME
#define LOCAL_FILE_NAME				 DEF_LOCAL_FILE_NAME

/* type declaration */

/* variable declaration */

/* function declaration */

/**
 * @brief   set time
 * @param
 * @return
 * @note
**/
static void app_set_local_time(zxb_collect_time_t *timebuf)
{
    if (OS_NULL == timebuf)
    {
    	rt_kprintf("Error: in app_udp_set_local_time  timebuf is NULL!\n");
    	return;
    }
    set_date(timebuf->year+2000,timebuf->mon,timebuf->day);
    set_time(timebuf->hour,timebuf->min,timebuf->sec);
}
/**
 * @brief   get time
 * @param
 * @return
 * @note
**/
static void app_get_local_time(zxb_collect_time_t *timebuf)
{
    time_t times;
    struct tm *rtc_time;
    times=time(NULL);
    rtc_time=localtime(&times);

    if (OS_NULL == timebuf)
    {
    	rt_kprintf("Error: in app_udp_set_local_time  timebuf is NULL!\n");
    	return;
    }
    if (rtc_time)
    {
    timebuf->year = rtc_time->tm_year+1900-2000;
    timebuf->mon = rtc_time->tm_mon+1;
    timebuf->day = rtc_time->tm_mday;
    timebuf->hour = rtc_time->tm_hour;
    timebuf->min = rtc_time->tm_min;
    timebuf->sec = rtc_time->tm_sec;
	//rt_kprintf("time:%d-%d-%d-%d:%d:%d\n",timebuf->year,timebuf->mon,timebuf->day,timebuf->hour,timebuf->min ,timebuf->sec);
    }
    else
    {
    	rt_kprintf("Error: in app_udp_get_local_time  rtc_time is NULL!\n");
    }
}
/**
 * @brief  update time
 * @param
 * @return
 * @note
**/
static int app_time_msg_valid_deal(uint8_t *timebuf)
{
    static uint8_t time_cnt=0;
    static uint8_t set_time_flag=1;
    zxb_collect_time_t time_t,loctime;
    int ret,return_t=0;

    if(set_time_flag)
    {
    	time_t.year= timebuf[0];
    	time_t.mon = timebuf[1];
    	time_t.day = timebuf[2];
    	time_t.hour= timebuf[3];
    	time_t.min = timebuf[4];
    	time_t.sec = timebuf[5];

    	app_set_local_time(&time_t);
		printf("$$$$sync mvb time.....\n");
		set_time_flag=0;
        return_t=1;
    }
   else if(time_cnt==51)
    {
	   app_get_local_time(&loctime);
		time_t.year=timebuf[0];
		time_t.mon=timebuf[1];
		time_t.day=timebuf[2];
		time_t.hour=timebuf[3];
		time_t.min=timebuf[4];
		time_t.sec=timebuf[5];

        ret=memcmp(&time_t,&loctime,4);

        if(ret!=0)
		{
		   printf("$$$$time reset....\n");
		   app_set_local_time(&time_t);
		}
      time_cnt=0;
    }
    time_cnt++;
    return return_t;
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
static int set_config_file()
{
	int file_size;
	struct tftp_client *client;
	char remote_name[64];
	char local_name[64];

	client = tftp_client_create(TFTP_CONFIG_FILE_IP, 69);
	sprintf(remote_name,"%s%s",REMOTE_PATH,REMOTE_FILE_NAME);
	sprintf(local_name,"%s%s",LOCAL_PATH,LOCAL_FILE_NAME);
	file_size = tftp_client_push(client,local_name,remote_name);
	tftp_client_destroy(client);

	return file_size;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static int read_config_file()
{
	int file_size;
	struct tftp_client *client;
	char remote_name[64];
	char local_name[64];

	client = tftp_client_create(TFTP_CONFIG_FILE_IP, 69);
	sprintf(remote_name,"%s%s",REMOTE_PATH,REMOTE_FILE_NAME);
	sprintf(local_name,"%s%s",LOCAL_PATH,LOCAL_FILE_NAME);
	file_size = tftp_client_pull(client,remote_name,local_name);
	tftp_client_destroy(client);

	return file_size;
}

/**
 * @brief
 * @param
 * @return
 * @note   clb重起数据处理
**/
int clb_reboot_deal(uint8_t add_cmd)
{
	if (add_cmd>=1 && add_cmd<=4)//1-4是对前置器的操作
	{
		printf("reboot qzq%d ...\n",add_cmd);
		can_lh_send_reset(add_cmd);
	}
	else
	{
		printf("reboot clb ...\n");
		rt_hw_cpu_reset();
	}
}

/**
 * @brief  应答UDP命令
 * @param  uint8_t *data：UDP收到的数据
 *         uint16_t size：收到数据的长度
 *         struct sockaddr_in recv_addr：接收到的地址
 * @return
 * @note   解析并处理通过内部组网（UDP）收到的信息
**/
void udp_para_ack(uint8_t cmd,uint8_t *buff,int buffsize)
{
//    static uint16_t counter=0;
    uint16_t len=UDP_SEND_HEAD_LEN+buffsize;
    uint8_t sendbuf[len];
    send_data_head_t heads = {0};
    memset(sendbuf, 0, len);
    heads.head[0]=0xaa;
    heads.head[1]=0x50;
    heads.len[0]=(uint8_t)(len>>8);
    heads.len[1]=(uint8_t)(len);

    heads.company_id = 0x4;
    heads.board_id = APP_PARAS_CLB_CODE;
    heads.cmd = cmd;

//    counter++;
//    heads.life_signal[0]= (uint8_t)(counter>>8);
//    heads.life_signal[1]= (uint8_t)(counter);

    memmove(sendbuf,&heads,sizeof(send_data_head_t));
    memmove(&sendbuf[sizeof(send_data_head_t)], buff, buffsize);
    uint16_t check_sum=sum_check_16(sendbuf,len-2);

    sendbuf[len-2]=(uint8_t)(check_sum>>8);
    sendbuf[len-1]=(uint8_t)(check_sum);

    app_singlecast_send(sendbuf, len);
}

/**
 * @brief  app_paras_config_get_data
 * @param  uint8_t *data：UDP收到的数据
 *         uint16_t size：收到数据的长度
 *         struct sockaddr_in recv_addr：接收到的地址
 * @return
 * @note   解析并处理通过内部组网（UDP）收到的信息
**/
void app_protocol_paras_ptu_data(uint8_t *data, uint16_t size, struct sockaddr_in recv_addr)
{
	record_unicast_protocol_t para_config_data;

	txb_MVB_public_t app_paras_public;

	memcpy(&para_config_data, data, UDP_SEND_HEAD_LEN);
	//para_config_data_p = (pnet_config_protocol_t)data;
	
	if((*(uint16_t*)&para_config_data.data_head != htons(0xAA50))) {
		return;
	}

//	para_config_data.sum_crc[0] = data[size - 2];
//	para_config_data.sum_crc[1] = data[size - 1];
	// 校验数据是否接收完整
//	uint16_t host_sum = sum_check_16(data, size - 2);
//	uint16_t net_sum = (para_config_data.sum_crc[0] << 8) + para_config_data.sum_crc[1];
//	if(host_sum != net_sum)
//	{
//		rt_kprintf("recv msg crc failed \n");
//		return;
//	}
//	else
//	{
		if(para_config_data.factory_code != APP_PARAS_FACTORY_CODE)
		{
     		rt_kprintf(" UDP recv factory is err:  0x%x\n", para_config_data.factory_code);
			return ;
		}

		if (para_config_data.device_code == APP_PARAS_JLB_CODE) //处理记录板发送的信息
		{
		    switch (para_config_data.cmd)
		    {
				case APP_RESERVE_CMD:  //命令位为预留的0时，为多播信息，即为公共信息
					memcpy(&app_paras_public, &data[26], sizeof(txb_MVB_public_t));
					app_save_public_info((uint8_t*)&app_paras_public);//保存公共信息到存储buffer，一并存到原始数据或特征文件中
					if(app_paras_public.valid.bits.time_valid)
					{
						app_time_msg_valid_deal(app_paras_public.time);
					}
					app_paras_save_env.temp_flag = para_config_data.temp_flag;
					app_paras_save_env.clb_ckdy = para_config_data.clb_ckdy;
					app_paras_save_env.clb_reboot = para_config_data.clb_reboot;
					//编组信息有变更时，同步编组信息
					if((4==para_config_data.group_num||6==para_config_data.group_num) && (app_paras_save_env.group_num!=para_config_data.group_num))
					{
						app_paras_save_env.group_num = para_config_data.group_num;
						paras_t *paras = app_paras_get();
						paras->group_num = para_config_data.group_num;
						update_sys_para();
						rt_kprintf("update group num: %d, reboot clb\n",app_paras_save_env.group_num);
						rt_hw_cpu_reset();
					}
//					rt_kprintf("record_public_data_parse  clb_ckdy=%d reboot=%d group_num=%d temp_flag=%d\n",\
//							app_paras_save_env.clb_ckdy,app_paras_save_env.clb_reboot,para_config_data.group_num,app_paras_save_env.temp_flag);
//					rt_kprintf("record_public_data_parse  train_id=%d\n",\
//							app_paras_save_env.train_num);
					break;

				case APP_APP_UPDATE_CMD: 		 //APP_APP_UPDATE_CMD APP_SYSTEM_REBOOT_CMD 功能相同
					break;
				case APP_SYSTEM_REBOOT_CMD:
					   rt_kprintf(" UDP recv APP_APP_UPDATE_CMD--clb\n");
					   clb_reboot_deal(para_config_data.add_data[0]);
					break;

				//case APP_SYSTEM_REBOOT_CMD:
						//rt_kprintf(" UDP recv APP_UDP_SYSTEM_REBOOT_CMD--clb\n");
						//rt_hw_cpu_reset();
				//	break;

				case APP_CONFIG_PARA_SET_CMD:
					set_config_file();
					break;
				case APP_CONFIG_PARA_READ_CMD:
					read_config_file();
					   break;
				//前置器命令处理
				case APP_GET_QZQ_DEFAULT_ID_CMD:
					app_scast_send_setaddr(recv_addr);
					rt_kprintf(" UDP get qzq id ...\n");
					send_radio_getid();
					rt_thread_mdelay(2000);
					qzq_id_buff_t* qzq_id = app_can_qzq_get_id();
					udp_para_ack(APP_GET_QZQ_DEFAULT_ID_CMD, qzq_id->getid, 4);
					app_can_qzq_clean_id();
					break;

				case APP_SET_QZQ_NEW_ID_CMD:
					app_scast_send_setaddr(recv_addr);
					config_qzq_id(para_config_data.add_data[0], para_config_data.add_data[1]);
					rt_kprintf(" UDP set qzq id old:%d   new:%d\n", para_config_data.add_data[0], para_config_data.add_data[1]);
					break;
				default:
					break;
	    	}
		}
		else  //非记录板信息
		{
          switch (para_config_data.device_code)
             {
        	     case APP_PARAS_ZXB_CODE:
        	    	 break;
        	     default:
        	    	 break;
             }
		}
//	}
}

