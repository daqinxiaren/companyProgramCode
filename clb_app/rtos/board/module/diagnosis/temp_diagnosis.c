/**
 *
 * Copyright (c) 2015-2020  LH  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the temp_diagnosis.c API or library.
 *
 * @features			:
 *
 * @author       		: www
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020年6月13日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b>@note</b> 		: null
 *
 * \b <b> @warning </b> : null
 * 
 * *
 * Change Logs:
 *	Date				author       notes
 *	2020年6月13日				www         create this file
 *	2020年6月13日
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
 

/**
 ***************** include file ***************
 */
#include <drv_config.h>
#if defined(HAL_DIAGNOSIS_ENABLED)
#include <temp_diagnosis.h>
#include <net_protocol.h>
#include <net_app.h>
#include <ad7606_app.h>
#include <speed_deal.h>
#if 1

/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/
/**@}*/

/** @formatter:off */
/**
 ***************<Private macro>****************
 */

/**
 * @brief macro define	- description 
 * @param	param	- description.
 * @param	param	- description.
 */
//#define desc_class(xxx, xxxx) 									\
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/** @formatter:on */

/**
 ***************<Private define>****************
 */
CCMRAM static struct rt_thread diagnosis_temp1_thread;
CCMRAM static struct rt_thread diagnosis_temp2_thread;
CCMRAM static rt_uint8_t diagnosis_temp1_thread_stack[1024*2];
CCMRAM static rt_uint8_t diagnosis_temp2_thread_stack[1024*2];


static RTC_REAL_TIME sys_time;

struct RUNNING_GEAR_TEMP_DIAG_VAL tz_running_gear_temp;
struct RUNNING_GEAR_TEMP_DIAG_VAL tz_running_gear_temp2;
struct ENV_TEMP_DIAG_VAL tz_env_temp;
struct ENV_TEMP_DIAG_VAL tz_env_temp2;

/**
 **************<Public variables>**************
 */
struct ENV_TEMP env_temp;
struct TEMP_SAVE temp_save_proc1;
struct TEMP_SAVE temp_save_proc2;

/**
 **************<Private functions>**************
 */
static void init_temp_diag_default_config(struct TEMP_DIAG_CONFIG_PARA *temp_para);
static void init_running_gear_temp_diag_val(struct RUNNING_GEAR_TEMP_DIAG_VAL *temp_diag_val);
static void init_env_temp_diag_val(struct ENV_TEMP_DIAG_VAL *temp_diag_val);
static void diag_env_temp(struct ENV_TEMP_DIAG_VAL *env_temp_diag_val);
static void diag_running_gear_temp(uint8_t ch, float env_1s_temp, struct RUNNING_GEAR_TEMP_DIAG_VAL *temp_val, const struct TEMP_DIAG_CONFIG_PARA temp_para);

static inline float convert_ad_temp(unsigned short ad);
static void update_mvb_proc1_temp_diag_status(uint8_t ch, int diag_res);
static void update_mvb_proc2_temp_diag_status(uint8_t ch, int diag_res);
static void update_mvb_proc1_temp_data(uint8_t flag);
static void update_mvb_proc2_temp_data(uint8_t flag);
static void diagnosis_temp1_thread_entry(void *paramter);
static void diagnosis_temp2_thread_entry(void *paramter);

/**
 **************<Public IMPL functions>**************
 */

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @brief development description of class
 * *
 * @author	www  write time: 2020年6月13日
 * @create time  : 2020年6月13日
 * *
 * Parameters
 * *
 * @param				param	new node to be inserted
 * @param [ in or out ]	param	new node to be inserted
 * * 
 * @return 
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
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
 * Date				author			notes
 * 2020年6月13日			www			create this class
 *						
 */


/**
 * 初始化温度算法配置参数
 */
static void init_temp_diag_default_config(struct TEMP_DIAG_CONFIG_PARA *temp_para)
{
    temp_para->temp_diff_warn_threshold = 40;         //温升差值的预警阈值 默认40
    temp_para->temp_abs_warn_threshold = 80;          //温度绝温预警阈值80
    temp_para->temp_diff_first_alarm_threshold = 60;  //温升差值一级报警阈值60
    temp_para->temp_abs_first_alarm_threshold = 100;  //温度绝温一级报警阈值100
    temp_para->temp_diff_second_alarm_threshold = 80; //温升差值二级报警阈值80
    temp_para->temp_abs_second_alarm_threshold = 120; //温度绝温二级报警阈值120
}


/**
 * 初始化温度算法特征值
 */
static void init_running_gear_temp_diag_val(struct RUNNING_GEAR_TEMP_DIAG_VAL *temp_diag_val)
{
    uint8_t ch;
    memset(temp_diag_val, 0, sizeof(struct RUNNING_GEAR_TEMP_DIAG_VAL));
    for (ch = 0; ch < 8; ch++)
    {
        temp_diag_val->kalman_running_gear_temp[ch].gain = 1;
        temp_diag_val->kalman_running_gear_temp[ch].covariance = 1;
        temp_diag_val->kalman_running_gear_temp[ch].temp_filter = 0;
    }
}

/**
 * 初始化环温算法特征值 、
 */
static void init_env_temp_diag_val(struct ENV_TEMP_DIAG_VAL *temp_diag_val)
{
    memset(temp_diag_val, 0, sizeof(struct ENV_TEMP_DIAG_VAL));
    temp_diag_val->kalman_env_temp.gain = 1;
    temp_diag_val->kalman_env_temp.covariance = 1;
    temp_diag_val->kalman_env_temp.temp_filter = 0;
}

/**
 * 环温计算
*/
static void diag_env_temp(struct ENV_TEMP_DIAG_VAL *env_temp_diag_val)
{
    uint8_t i;
    env_temp_diag_val->env_1s_temp = 0;
    for (i = 1; i < FS_TEMP-1; i++)
    {
        env_temp_diag_val->kalman_env_temp.temp_new = env_temp_diag_val->ad_env_temp[i];
        env_temp_diag_val->env_1s_temp += env_temp_diag_val->kalman_env_temp.temp_new;
    }
    env_temp_diag_val->env_1s_temp = env_temp_diag_val->env_1s_temp / (FS_TEMP-2);
}


/**
 * PT1000 rad_temp
*/



/**
 * PT1000 ad_temp
*/
float pt1000_convert_ad_temp(uint16_t std, uint16_t src )
{
	float t;
	uint16_t r= ((float)src)/((float)std)*1000;
	
	if(r>839 && r < 859)
	{
		r -= 10;
	}
	else if(r>859 && r < 996)
	{
			r -= 8;
	}
	else if(r>1000 && r <= 1200)
	{
		r -= 4;
	}
//	else if(r>1200 && r <= 1530)
//	{
//		r += 6;
//	}
	else if(r > 1535 && r <= 1680)
	{
		r+=9;
	}
	else if(r > 1680 && r <= 1690)
	{
		r+=15;
	}
	else if(r > 1690 && r <= 1700)
	{
		r+=18;
	}
	else if(r > 1700)
	{
		//r += 23;
		r += 10;
	}
	else
	{;}

	t=(r-1000)/3.85;

	if(r >1700 && r<= 1703)
	{
		t = 185;
	}
	else if(r>1703 && r<= 1706)
	{
		t = 186;
	}
	else if(r>1706 && r<= 1710)
	{
		t = 187;
	}
	else if(r>1710 && r<= 1714)
	{
		t = 188;
	}
	else if(r>1714 && r<= 1718)
	{
		t = 189;
	}
	else if(r>1718 && r<= 1721)
	{
		t = 190;
	}
	else if(r>1721 && r<= 1725)
	{
		t = 191;
	}
	else if(r>1725 && r<= 1729)
	{
		t = 192;
	}
	else if(r>1729 && r<= 1732)
	{
		t = 193;
	}
	else if(r>1732 && r<= 1736)
	{
		t = 194;
	}
	else if(r>1736 && r<= 1740)
	{
		t = 195;
	}
	else if(r>1740 && r<= 1743)
	{
		t = 196;
	}
	else if(r>1743 && r<= 1747)
	{
		t = 197;
	}
	else if(r>1747 && r<= 1751)
	{
		t = 198;
	}
	else if(r>1751 && r<= 1754)
	{
		t = 199;
	}
	else if(r>1754 && r<= 1758)
	{
		t = 200;
	}
	else if(r>1758 && r<= 1762)
	{
		t = 201;
	}
	else if(r>1762 && r<= 1765)
	{
		t = 202;
	}
	else
	{;}
	//t=(r-1000)/3.9;		//XUZHAO


//		if(r < 827)
//		{
//			t = -47 ;
//		}

	if(r > 803 && r <= 807 )
	{
		t = -49;
	}
	else if(r > 807 && r <= 811 )
	{
		t = -48;
	}
	else if(r > 811 && r <= 814 )
	{
		t = -47;
	}
	else if(r > 814 && r <= 818 )
	{
		t = -46;
	}
	else if(r > 818 && r <= 822 )
	{
		t = -45;
	}
	else if(r > 822 && r <= 826 )
	{
		t = -44;
	}
	else if(r > 826 && r <= 830 )
	{
		t = -43;
	}
	else if(r > 830 && r <= 834 )
	{
		t = -42;
	}
	else if(r > 834 && r <= 838 )
	{
		t = -41;
	}
	else if(r > 838 && r <= 842 )
	{
		t = -40;
	}


	if((t > 30) && (t - 30 >1) && (t - 30 < 5))
	{
		t = t-1;
	}

	if((t > 60) && (t - 60 >1) && (t - 60 < 5))
	{
		t = t-1;
	}

	if((t > 90) && (t - 90 >1) && (t - 90 < 5))
	{
		t = t-1;
	}
//	if(ad < 58)
//	{
//		t = -50;
//	}
 
	return t;
}

//温度数据滤波
static void temp_sort(float *buff,uint32_t len)
{
    float temp_stor =0;

    for(int i = 0; i < len; i++)
    {
        for(int j = i; j < len-1; j++)
        {
            if(buff[i] > buff[j+1])
            {
                temp_stor = buff[i];
                buff[i] = buff[j+1];
                buff[j+1] = temp_stor;
            }
        }
    }
}

/**
 * 温度算法
 * 1、轴承温升比环境温度高40 摄氏度或绝对温度达到80 摄氏度，轴温升高预警并进行记录。
 * 2、轴承温升比环境温度高60 摄氏度或绝对温度达到100 摄氏度，轴温升高1级报警并进行记录。
 * 3、轴承温升比环境温度高80 摄氏度或绝对温度达到120 摄氏度，轴温升高2级报警并进行记录。
 */
static void diag_running_gear_temp(uint8_t ch, float env_1s_temp, struct RUNNING_GEAR_TEMP_DIAG_VAL *temp_val, const struct TEMP_DIAG_CONFIG_PARA temp_para)
{
    int i;
    float temp = 0.0f;

    temp_sort(temp_val->ad_running_gear_temp,FS_TEMP);

    for (i = 1; i < FS_TEMP-1; i++)
    {
        temp_val->kalman_running_gear_temp[ch].temp_new = temp_val->ad_running_gear_temp[i];
        temp += temp_val->kalman_running_gear_temp[ch].temp_new;
    }

    temp_val->temp_save.env_1s_temp = env_1s_temp;

    temp_val->temp_save.running_gear_1s_temp[ch] = temp / (FS_TEMP-2);

    if (temp_val->temp_save.env_temp_self_test == 0)
    {
        temp_val->temp_save.diff_env_running_gear_temp[ch] =( temp_val->temp_save.running_gear_1s_temp[ch] - temp_val->temp_save.env_1s_temp);
    }
    else
    {
        temp_val->temp_save.diff_env_running_gear_temp[ch] = 0;
    }

    if (temp_val->temp_save.running_gear_temp_self_test[ch] == 1)
    {
        temp_val->temp_save.diag_res[ch] = RUNNING_OK;
        return;
    }

    if ((temp_val->temp_save.diff_env_running_gear_temp[ch] >= temp_para.temp_diff_second_alarm_threshold) || (temp_val->temp_save.running_gear_1s_temp[ch] >= temp_para.temp_abs_second_alarm_threshold))
    {
        temp_val->temp_save.diag_res[ch] = TEMP_ALARM;
    }
    else if ((temp_val->temp_save.diff_env_running_gear_temp[ch] >= temp_para.temp_diff_first_alarm_threshold) || (temp_val->temp_save.running_gear_1s_temp[ch] >= temp_para.temp_abs_first_alarm_threshold))
    {
        temp_val->temp_save.diag_res[ch] = TEMP_ALARM;
    }
    else if ((temp_val->temp_save.diff_env_running_gear_temp[ch] >= temp_para.temp_diff_warn_threshold )|| (temp_val->temp_save.running_gear_1s_temp[ch] >= temp_para.temp_abs_warn_threshold))
    {
        temp_val->temp_save.diag_res[ch] = TEMP_WARN;
    }
    else
    {
        temp_val->temp_save.diag_res[ch] = RUNNING_OK;
    }
}

/**
 * 获取温度报警状态---对应位置，最近一次的诊断结果
 */
enum ALARM_TYPE get_temp_alarm_status(uint8_t ch)
{
    if (ch < PROC_CHANNEL_NUM)
    {
        return temp_save_proc1.diag_res[ch];
    }
    else
    {
        return temp_save_proc2.diag_res[ch - PROC_CHANNEL_NUM];
    }
}

/**
 * 获取轴承诊断的环温
 */
float get_bearing_diag_env_temp(uint8_t ch)
{
    if (ch < PROC_CHANNEL_NUM)
    {
        return temp_save_proc1.env_1s_temp;
    }
    else
    {
        return temp_save_proc2.env_1s_temp;
    }
}

/**
 * 获取轴承诊断的走行部温度
 */
float get_bearing_diag_running_gear_temp(uint8_t ch)
{
    if (ch < PROC_CHANNEL_NUM)
    {
        return temp_save_proc1.running_gear_1s_temp[ch];
    }
    else
    {
        return temp_save_proc2.running_gear_1s_temp[ch - PROC_CHANNEL_NUM];
    }
}

/**
 * 获取轴承诊断的走行部温度
 */
float get_bearing_proc1_temp(uint8_t ch)
{
    return temp_save_proc1.running_gear_1s_temp[ch];
}
/**
 * 获取轴承诊断的走行部温度
 */
float get_bearing_proc2_temp(uint8_t ch)
{
    return temp_save_proc2.running_gear_1s_temp[ch];
}


/**
 * 前置处理器1温度诊断线程
*/

static void diagnosis_temp1_thread_entry(void *paramter)
{
    uint8_t *temp_send_buff = NULL;
    rt_uint32_t e;
    uint8_t ch_num, cnt_i;
   // float temp_data = 0;
    char debug_buff[32];
    uint8_t temp_deal_flag=0;
    uint8_t ch_start,ch_end;

    init_running_gear_temp_diag_val(&tz_running_gear_temp);
    init_env_temp_diag_val(&tz_env_temp);
    while (1)
    {
        if (rt_event_recv(&AD_tempevent, diag_temp1_event, Event_Option, RT_WAITING_FOREVER, &e) != RT_EOK)
            continue;
        {
            //1、环温算法
            //填充环温原始数据
            temp_deal_flag=sys_ok_st.temp_deal_type12;
            if(!temp_deal_flag)
                continue;

            for (cnt_i = 0; cnt_i < FS_TEMP; cnt_i++)
            {
                tz_env_temp.ad_env_temp[cnt_i] = convert_ad_temp(diag_ad_temp1.ad_temp.buff[8][cnt_i]);
            }

            //清除特征存储数据
            memset(&tz_running_gear_temp.temp_save, 0, sizeof(tz_running_gear_temp.temp_save));

            //更新ad公共信息
            if(temp_deal_flag==1)
                tz_running_gear_temp.temp_save.pub_info1.proc_id = 1;
            else
                tz_running_gear_temp.temp_save.pub_info1.proc_id = 2;

            tz_running_gear_temp.temp_save.pub_info1.train_id = clb_config_para.train_net_addr.train_id;
            get_cur_time(&sys_time);
            tz_running_gear_temp.temp_save.pub_info1.year = sys_time.year%2000;
            tz_running_gear_temp.temp_save.pub_info1.mon = sys_time.mon;
            tz_running_gear_temp.temp_save.pub_info1.day = sys_time.day;
            tz_running_gear_temp.temp_save.pub_info1.hour = sys_time.hour;
            tz_running_gear_temp.temp_save.pub_info1.min = sys_time.min;
            tz_running_gear_temp.temp_save.pub_info1.sec = sys_time.sec;
            tz_running_gear_temp.temp_save.pub_info1.curr_station_id = mvb_speed_time_data.curr_station_id;
            tz_running_gear_temp.temp_save.pub_info1.next_station_id = mvb_speed_time_data.next_station_id;
            tz_running_gear_temp .temp_save .pub_info1 .train_number = mvb_speed_time_data .train_num ;
            tz_running_gear_temp.temp_save.pub_info1.ad_serial_num=diag_ad_temp1.pub_info.ad_serial_num;
            tz_running_gear_temp.temp_save.pub_info1.wheel_diameter=clb_config_para.bear_diag_config_para.wheel_diameter;
            //获取环温自检状态
            tz_running_gear_temp.temp_save.env_temp_self_test = diag_ad_temp1.envirment_temp_self_status;

            //TEMP_PRINT("proc1-2:sn=%d\n",tz_running_gear_temp.temp_save.pub_info.ad_serial_num);
            //环温卡尔曼滤波，得到1s的一个环温

            diag_env_temp(&tz_env_temp);
            env_temp.pro1_env_temp = (int)tz_env_temp.env_1s_temp;

            tz_running_gear_temp.temp_save.env_1s_temp=env_temp.pro1_env_temp;
            if(temp_deal_flag==1)
            {
                ch_start=0;
                ch_end=4;
            }
            else
            {
                ch_start=4;
                ch_end=8;
            }

            for (ch_num = ch_start; ch_num < ch_end; ch_num++)
            {
				if(!(((diag_ad_temp1.temp_self_status)&(0x01 << ch_num))>>ch_num))
				{
					//填充走行部温度原始数据
					for (cnt_i = 1; cnt_i < FS_TEMP; cnt_i++)
					{
						tz_running_gear_temp.ad_running_gear_temp[cnt_i] = pt1000_convert_ad_temp(diag_ad_temp1.ad_temp.buff[ch_num][0],diag_ad_temp1.ad_temp.buff[ch_num][cnt_i]);
						diag_ad_temp1.ad_temp.buff[ch_num][cnt_i]=((uint16_t)(tz_running_gear_temp.ad_running_gear_temp[cnt_i]*10))+1270;
					}
					diag_ad_temp1.ad_temp.buff[ch_num][0]=((uint16_t)(tz_running_gear_temp.ad_running_gear_temp[1]*10))+1270;
					tz_running_gear_temp.ad_running_gear_temp[0]=tz_running_gear_temp.ad_running_gear_temp[1];

					tz_running_gear_temp.temp_save.speed = get_mean_speed();
					tz_running_gear_temp.temp_save.running_gear_temp_self_test[ch_num] =0;

					//走行部温度算法诊断
					diag_running_gear_temp(ch_num, tz_env_temp.env_1s_temp, &tz_running_gear_temp, clb_config_para.temp_diag_config_para);					
					//打印预报警信息
					if (tz_running_gear_temp.temp_save.diag_res[ch_num] == RUNNING_OK)
					{
						TEMP_PRINT("proc1:temp_ch%d diag ok...\n", ch_num);
					}
					else if (tz_running_gear_temp.temp_save.diag_res[ch_num] == TEMP_WARN)
					{
						TEMP_PRINT("proc1:temp_ch%d diag warn!!!!!\n", ch_num);
					}
					else
					{
						TEMP_PRINT("proc1:temp_ch%d diag alarm!!!!!!!!\n", ch_num);
					}

//				    if(ch_num<4)
//				        rt_kprintf("qzq1_ch%d: %d\n", ch_num,(char)tz_running_gear_temp.temp_save.running_gear_1s_temp[ch_num]);
//				    else
//				        rt_kprintf("qzq2_ch%d: %d\n", ch_num,(char)tz_running_gear_temp.temp_save.running_gear_1s_temp[ch_num]);
					//更新发给MVB的相关数据

					update_mvb_proc1_temp_diag_status(ch_num, tz_running_gear_temp.temp_save.diag_res[ch_num]);
				}
				else
				{
					rt_memset(diag_ad_temp1.ad_temp.buff[ch_num],0,20);
					tz_running_gear_temp.temp_save.running_gear_1s_temp[ch_num]=-127;
				}									
            }

			//填充该通道温度诊断结果缓存
			memmove(&temp_save_proc1, &tz_running_gear_temp.temp_save, sizeof(struct TEMP_SAVE));
            update_mvb_proc1_temp_data(temp_deal_flag);
            if (!temp_send_buff)
            {
                unsigned int net_send_len =  100+ sizeof(struct TEMP_SAVE);
                temp_send_buff = rt_malloc(net_send_len);
                if (temp_send_buff)
                {
                    memmove(temp_send_buff, &temp_save_proc1, sizeof(struct TEMP_SAVE));
                    memmove(temp_send_buff + sizeof(struct TEMP_SAVE), &diag_ad_temp1.ad_temp.buff[8], 20);

                    if(temp_deal_flag==1)
                        memmove(temp_send_buff + sizeof(struct TEMP_SAVE)+20, &diag_ad_temp1.ad_temp.buff[0], 80);
                    else
                        memmove(temp_send_buff + sizeof(struct TEMP_SAVE)+20, &diag_ad_temp1.ad_temp.buff[4], 80);
                    //合并且发送温度原始数据和特征数据
                    net_send(clb_config_para.train_net_addr.train_id, 1, TEMP_RAW_DATA_TYPE, temp_send_buff, net_send_len,0);
                    rt_free(temp_send_buff);
                    temp_send_buff = NULL;
                }
            }
            //rt_kprintf("qzq1-deal-end\n");
        }
    }
}

/**
 * 前置处理器2温度诊断线程
*/
static void diagnosis_temp2_thread_entry(void *paramter)
{
    uint8_t *temp_send_buff = NULL;
    rt_uint32_t e;
    uint8_t ch_num, cnt_i;
    uint8_t temp2_deal_flag=0;
    uint8_t ch2_start,ch2_end;
   // float temp_data = 0;
    char debug_buff[32];
    init_running_gear_temp_diag_val(&tz_running_gear_temp2);
    init_env_temp_diag_val(&tz_env_temp2);
    while (1)
    {
        if (rt_event_recv(&AD_tempevent, diag_temp2_event, Event_Option, RT_WAITING_FOREVER, &e) != RT_EOK)
            continue;
        {

            //1、环温算法
            //填充环温原始数据
            temp2_deal_flag=sys_ok_st.temp_deal_type34;
            if(!temp2_deal_flag)
                continue;

            for (cnt_i = 0; cnt_i < FS_TEMP; cnt_i++)
            {
                    tz_env_temp2.ad_env_temp[cnt_i] = convert_ad_temp(diag_ad_temp2.ad_temp.buff[8][cnt_i]);
            }

            //清除特征存储
            memset(&tz_running_gear_temp2.temp_save, 0, sizeof(tz_running_gear_temp2.temp_save));

            //更新ad公共信息
            if(temp2_deal_flag==1)
                tz_running_gear_temp2.temp_save.pub_info1.proc_id = 3;
            else
                tz_running_gear_temp2.temp_save.pub_info1.proc_id = 4;
            tz_running_gear_temp2.temp_save.pub_info1.train_id = clb_config_para.train_net_addr.train_id;
            get_cur_time(&sys_time);
            tz_running_gear_temp2.temp_save.pub_info1.year = sys_time.year%2000;
            tz_running_gear_temp2.temp_save.pub_info1.mon = sys_time.mon;
            tz_running_gear_temp2.temp_save.pub_info1.day = sys_time.day;
            tz_running_gear_temp2.temp_save.pub_info1.hour = sys_time.hour;
            tz_running_gear_temp2.temp_save.pub_info1.min = sys_time.min;
            tz_running_gear_temp2.temp_save.pub_info1.sec = sys_time.sec;
            tz_running_gear_temp2.temp_save.pub_info1.curr_station_id = mvb_speed_time_data.curr_station_id;
            tz_running_gear_temp2.temp_save.pub_info1.next_station_id = mvb_speed_time_data.next_station_id;
            tz_running_gear_temp2.temp_save.pub_info1.train_number = mvb_speed_time_data.train_num;
            tz_running_gear_temp2.temp_save.pub_info1.ad_serial_num=diag_ad_temp2.pub_info.ad_serial_num;
            tz_running_gear_temp2.temp_save.pub_info1.wheel_diameter=clb_config_para.bear_diag_config_para.wheel_diameter;
            //获取环温自检状态
            tz_running_gear_temp2.temp_save.env_temp_self_test = diag_ad_temp2.envirment_temp_self_status;
            //rt_kprintf("proc2:sn=%d\n",tz_running_gear_temp2.temp_save.pub_info.ad_serial_num);

            //环温卡尔曼滤波，得到1s的一个环温
            diag_env_temp(&tz_env_temp2);
            env_temp.pro2_env_temp  = (int)tz_env_temp2.env_1s_temp;
            tz_running_gear_temp2.temp_save.env_1s_temp=env_temp.pro2_env_temp;
           // sprintf(debug_buff, "%f", tz_env_temp2.env_1s_temp);
            //rt_kprintf("qzq2:temp_env_1s=%s\n", debug_buff);
			
            if(temp2_deal_flag==1)
            {

                ch2_start=0;
                ch2_end=4;
            }
            else
            {
                ch2_start=4;
                ch2_end=8;
            }

			for (ch_num =ch2_start; ch_num < ch2_end; ch_num++)
			{
				if(!(((diag_ad_temp2.temp_self_status) &(0x01 << ch_num))>>ch_num))
				{					
					//填充走行部温度原始数据
					for (cnt_i = 1; cnt_i < FS_TEMP; cnt_i++)
					{
						tz_running_gear_temp2.ad_running_gear_temp[cnt_i] = pt1000_convert_ad_temp(diag_ad_temp2.ad_temp.buff[ch_num][0],diag_ad_temp2.ad_temp.buff[ch_num][cnt_i]);
						diag_ad_temp2.ad_temp.buff[ch_num][cnt_i]=((uint16_t)(tz_running_gear_temp2.ad_running_gear_temp[cnt_i]*10))+1270;
					}
					diag_ad_temp2.ad_temp.buff[ch_num][0]=((uint16_t)(tz_running_gear_temp2.ad_running_gear_temp[1]*10))+1270;
					tz_running_gear_temp2.ad_running_gear_temp[0]=tz_running_gear_temp2.ad_running_gear_temp[1];
					//rt_kprintf("qzq2-->pt1000_ch%d: %d\n", ch_num,diag_ad_temp2.ad_temp.buff[ch_num][0]);
					//填充数据存储相关的内容？？？
					tz_running_gear_temp2.temp_save.speed = get_mean_speed();

					tz_running_gear_temp2.temp_save.running_gear_temp_self_test[ch_num] = 0;

					//走行部温度算法诊断
					diag_running_gear_temp(ch_num, tz_env_temp2.env_1s_temp, &tz_running_gear_temp2, clb_config_para.temp_diag_config_para);

					if (tz_running_gear_temp2.temp_save.diag_res[ch_num] == RUNNING_OK)
					{
						TEMP_PRINT("qzq2:temp_ch%d diag ok...\n", ch_num);
					}
					else if (tz_running_gear_temp2.temp_save.diag_res[ch_num] == TEMP_WARN)
					{
						TEMP_PRINT("qzq2:temp_ch%d diag warn!!!!!\n", ch_num);
					}
					else
					{
						TEMP_PRINT("qzq2:temp_ch%d diag alarm!!!!!!!!\n", ch_num);
					}

					//更新发给MVB的相关数据？？？
					update_mvb_proc2_temp_diag_status(ch_num, tz_running_gear_temp2.temp_save.diag_res[ch_num]);
//                    if(ch_num<4)
//                        rt_kprintf("qzq3_ch%d: %d\n", ch_num,(char)tz_running_gear_temp2.temp_save.running_gear_1s_temp[ch_num]);
//                    else
//                        rt_kprintf("qzq4_ch%d: %d\n", ch_num,(char)tz_running_gear_temp2.temp_save.running_gear_1s_temp[ch_num]);
				}
			
				else
				{
					rt_memset(diag_ad_temp2.ad_temp.buff[ch_num],0,20);
					tz_running_gear_temp2.temp_save.running_gear_1s_temp[ch_num]=-127;
				}
			}
			//填充该通道温度诊断结果缓存
			memmove(&temp_save_proc2, &tz_running_gear_temp2.temp_save, sizeof(struct TEMP_SAVE));
            update_mvb_proc2_temp_data(temp2_deal_flag);
            if (!temp_send_buff)
            {
                unsigned int net_send_len = 100 + sizeof(struct TEMP_SAVE);
                temp_send_buff = rt_malloc(net_send_len);
                if (temp_send_buff)
                {
                    memmove(temp_send_buff, &temp_save_proc2, sizeof(struct TEMP_SAVE));
                    memmove(temp_send_buff + sizeof(struct TEMP_SAVE), &diag_ad_temp2.ad_temp.buff[8], 20);
                     if(temp2_deal_flag==1)
                        memmove(temp_send_buff + sizeof(struct TEMP_SAVE)+20, &diag_ad_temp2.ad_temp.buff[0], 80);
                    else
                        memmove(temp_send_buff + sizeof(struct TEMP_SAVE)+20, &diag_ad_temp2.ad_temp.buff[4], 80);
                    //合并且发送温度原始数据和特征数据
                    net_send(clb_config_para.train_net_addr.train_id, 1, TEMP_RAW_DATA_TYPE, temp_send_buff, net_send_len,0);
                    rt_free(temp_send_buff);
                    temp_send_buff = NULL;
                }

            }
			//rt_kprintf("qzq2-deal-end\n");
        }
    }
}

/**
 * DS18B20前置处理器获取的值 温度转换公式
*/
static inline float convert_ad_temp(unsigned short ad)
{
    return (ad - 1270) / 10.0;
}

/**
*轴端温度MVB发送结果
*/
static void update_mvb_proc1_temp_diag_status(uint8_t ch, int diag_res)
{
    uint8_t temp_warn, temp_alarm = 0;
    if (diag_res == RUNNING_OK)
    {
        temp_warn = 0;
        temp_alarm = 0;
    }
    else if (diag_res == TEMP_WARN)
    {
        temp_warn = 1;
        temp_alarm = 0;
    }
    else if (diag_res == TEMP_ALARM)
    {
        temp_warn = 0;
        temp_alarm = 1;
    }

#ifdef ALARM_EN_DEBUG
    temp_warn =0;
    temp_alarm=0;
#endif

    if (ch == 0)
    {
        send_mvb_data.sensor_alarm_data.bits.side1_shaft_end_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.side1_shaft_end_temp_alarm = temp_alarm;
    }
    else if (ch == 1)
    {
        send_mvb_data.sensor_alarm_data.bits.side2_shaft_end_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.side2_shaft_end_temp_alarm = temp_alarm;
    }
    else if (ch == 2)
    {
        send_mvb_data.sensor_alarm_data.bits.side3_shaft_end_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.side3_shaft_end_temp_alarm = temp_alarm;
    }
    else if (ch == 3)
    {
        send_mvb_data.sensor_alarm_data.bits.side4_shaft_end_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.side4_shaft_end_temp_alarm = temp_alarm;
    }
    else if (ch == 4)
    {
        send_mvb_data.sensor_alarm_data.bits.side5_shaft_end_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.side5_shaft_end_temp_alarm = temp_alarm;
    }
    else if (ch == 5)
    {
        send_mvb_data.sensor_alarm_data.bits.side6_shaft_end_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.side6_shaft_end_temp_alarm = temp_alarm;
    }
    else if (ch == 6)
    {
        send_mvb_data.sensor_alarm_data.bits.side7_shaft_end_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.side7_shaft_end_temp_alarm = temp_alarm;
    }
    else if (ch == 7)
    {
        send_mvb_data.sensor_alarm_data.bits.side8_shaft_end_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.side8_shaft_end_temp_alarm = temp_alarm;
    }
}

/**
*轴温MVB发送结果
*/
static void update_mvb_proc2_temp_diag_status(uint8_t ch, int diag_res)
{
    uint8_t temp_warn, temp_alarm = 0;
    if (diag_res == RUNNING_OK)
    {
        temp_warn = 0;
        temp_alarm = 0;
    }
    else if (diag_res == TEMP_WARN)
    {
        temp_warn = 1;
        temp_alarm = 0;
    }
    else if (diag_res == TEMP_ALARM)
    {
        temp_warn = 0;
        temp_alarm = 1;
    }

#ifdef ALARM_EN_DEBUG
    temp_warn =0;
    temp_alarm=0;
#endif

    if (ch == 0)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft1_motor_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.shaft1_motor_temp_alarm = temp_alarm;
    }
    else if (ch == 1)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft2_motor_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.shaft2_motor_temp_alarm = temp_alarm;
    }
    else if (ch == 2)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft3_motor_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.shaft3_motor_temp_alarm = temp_alarm;
    }
    else if (ch == 3)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft4_motor_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.shaft4_motor_temp_alarm = temp_alarm;
    }
    else if (ch == 4)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft1_gear_box_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.shaft1_gear_box_temp_alarm = temp_alarm;
    }
    else if (ch == 5)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft2_gear_box_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.shaft2_gear_box_temp_alarm = temp_alarm;
    }
    else if (ch == 6)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft3_gear_box_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.shaft3_gear_box_temp_alarm = temp_alarm;
    }
    else if (ch == 7)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft4_gear_box_temp_warn = temp_warn;
        send_mvb_data.sensor_alarm_data.bits.shaft4_gear_box_temp_alarm = temp_alarm;
    }

}

/*
*更新温度数据
*/
static void update_mvb_proc1_temp_data(uint8_t flag)
{
//    send_mvb_data.sensor_temp_data.side1_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[0];

//    send_mvb_data.sensor_temp_data.side2_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[3];

//    send_mvb_data.sensor_temp_data.side3_shaft_end_temp =(int)temp_save_proc1.running_gear_1s_temp[4];

//    send_mvb_data.sensor_temp_data.side4_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[7];

//    send_mvb_data.sensor_temp_data.shaft2_motor_temp = (int)temp_save_proc1.running_gear_1s_temp[6];

//    send_mvb_data.sensor_temp_data.shaft2_gear_temp =(int)temp_save_proc1.running_gear_1s_temp[5];

//    send_mvb_data.sensor_temp_data.shaft1_motor_temp =(int)temp_save_proc1.running_gear_1s_temp[2];

//    send_mvb_data.sensor_temp_data.shaft1_gear_temp = (int)temp_save_proc1.running_gear_1s_temp[1];

    if(flag==1)
    {
        send_mvb_data.sensor_temp_data.side1_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[0];
        send_mvb_data.sensor_temp_data.side2_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[1];
        send_mvb_data.sensor_temp_data.side3_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[2];
        send_mvb_data.sensor_temp_data.side4_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[3];

        send_mvb_data.env_temp[0] =(int)tz_env_temp.env_1s_temp;
    }
    else if(flag==2)
    {
        send_mvb_data.sensor_temp_data.side5_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[4];
        send_mvb_data.sensor_temp_data.side6_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[5];
        send_mvb_data.sensor_temp_data.side7_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[6];
        send_mvb_data.sensor_temp_data.side8_shaft_end_temp = (int)temp_save_proc1.running_gear_1s_temp[7];

        send_mvb_data.env_temp[1] =(int)tz_env_temp.env_1s_temp;
    }
	
}

/*
*更新温度数据
*/
static void update_mvb_proc2_temp_data(uint8_t flag)
{

    if(flag==1)
    {
        send_mvb_data.sensor_temp_data.shaft1_motor_temp =(int)temp_save_proc2.running_gear_1s_temp[0];

        send_mvb_data.sensor_temp_data.shaft2_motor_temp =(int)temp_save_proc2.running_gear_1s_temp[1];

        send_mvb_data.sensor_temp_data.shaft3_motor_temp =(int)temp_save_proc2.running_gear_1s_temp[2];

        send_mvb_data.sensor_temp_data.shaft4_motor_temp =(int)temp_save_proc2.running_gear_1s_temp[3];

        send_mvb_data.env_temp[2] =(int)tz_env_temp2.env_1s_temp;

    }
    else if(flag==2)
    {

        send_mvb_data.sensor_temp_data.shaft1_gear_temp = (int)temp_save_proc2.running_gear_1s_temp[4];

        send_mvb_data.sensor_temp_data.shaft2_gear_temp =(int)temp_save_proc2.running_gear_1s_temp[5];

        send_mvb_data.sensor_temp_data.shaft3_gear_temp =(int)temp_save_proc2.running_gear_1s_temp[6];

        send_mvb_data.sensor_temp_data.shaft4_gear_temp =(int)temp_save_proc2.running_gear_1s_temp[7];

        send_mvb_data.env_temp[3] =(int)tz_env_temp2.env_1s_temp;
    }

}

/*温度 初始化线程*/
int diagnosis_temp_thread_init( char *arg)
{
    rt_err_t result;

    /*前置处理板1温度诊断线程*/
    result = rt_thread_init(&diagnosis_temp1_thread,"diag_temp1",
                            diagnosis_temp1_thread_entry, RT_NULL,
                            &diagnosis_temp1_thread_stack, sizeof(diagnosis_temp1_thread_stack), 24, 40);
    if (result == RT_EOK)
        rt_thread_startup(&diagnosis_temp1_thread);


    /*前置处理板2温度诊断线程*/
    result = rt_thread_init(&diagnosis_temp2_thread,"diag_temp2",
                            diagnosis_temp2_thread_entry, RT_NULL,
                            &diagnosis_temp2_thread_stack, sizeof(diagnosis_temp2_thread_stack), 24, 40);
    if (result == RT_EOK)
        rt_thread_startup(&diagnosis_temp2_thread);

    return result;
}

/** @} */ //\defgroup module API

/** @} */ //\addtogroup dev module

#endif //\end module API
#endif
/* !< end file   */
