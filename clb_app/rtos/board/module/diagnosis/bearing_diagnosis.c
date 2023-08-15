/**
 *
 * Copyright (c) 2015-2020  LH  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the bearing_diagnosis.c API or library.
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
#include <bearing_diagnosis.h>
#include <stm32f407xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MCU_ARM
#ifdef MCU_ARM 
#include "arm_math.h"
#endif
#include <ad7606_app.h>
#include "net_protocol.h"
#include <net_app.h>
#include <can_app.h>
#include <math_app.h>
#include "temp_diagnosis.h"
#include "sdcard_save.h"
#include <dfs_posix.h>
#include <switch_app.h>
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
#ifdef MCU_ARM
#else
struct CPLX
{
    double real;
    double imag;
};
#endif


/**
 **************<Private variables>**************
 */
 CCMRAM static rt_uint8_t diagnosis_bearing_thread_stack[1024*6];
 CCMRAM struct rt_thread diagnosis_bearing_thread;

const float KALMAN_FILTER_PARA_OB = 0.1; //卡尔曼滤波系数观测噪声 默认 0.1
const float KALMAN_FILTER_PARA_TS = 6;   //卡尔曼滤波系数测量噪声 默认 6

/**
 **************<Private functions>**************
 */

static void init_bearing_diag_default_config(struct BEAR_DIAG_CONFIG_PARA *bear_para);
static void init_bearing_diag_val(struct BEAR_DIAG_VAL *bear_diag_val);
static void destroy_bearing_diag_val(struct BEAR_DIAG_VAL *bear_diag_val);
static void diag_bearing(float *acc_buff, uint32_t acc_buff_size, const struct BEAR_DIAG_CONFIG_PARA bear_para, struct BEAR_DIAG_VAL *bear_val);
static void fft(float32_t *buff, int size);
static void get_correct_index(int *index);
static inline float convert_ad_bearing(unsigned short ad);
void update_mvb_bearing_diag_status(uint8_t ch, int diag_res);
static void diagnosis_bearing_thread_entry(void *paramter);

extern void updata_tread_mvb_data(uint8_t ch, int diag_res);
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
 * 初始化轴承算法配置参数
 */
static void init_bearing_diag_default_config(struct BEAR_DIAG_CONFIG_PARA *bear_para)
{
    bear_para->wheel_diameter = 0.85f;                        //车轮直径，默认0.85m，从通信协议获取
    bear_para->bearing_roller_num = 13;                       //轴承滚动体的个数,默认13,从地铁、长客获取
    bear_para->para_inner_ring = 10.0f;                       //内环故障频率与车速的比例 默认7.5075,从地铁、长客获取  HYX: change to 10   original is 7.5075
    bear_para->para_outer_ring = 7.0f;                        //外环故障频率与车速的比例 默认3.4743,从地铁、长客获取  HYX: change to 7    original is 3.4743
    bear_para->para_rolling_body = 4.0f;                      //滚动体故障频率与车速的比例 默认5.3465   意义是滚动体的公转频率*滚动体的个数,从地铁、长客获取    HYX: change to 4  original is 5.3465
    bear_para->para_holder = 1.0f;                            //保持架故障频率与车速的比例 默认0.4225,从地铁、长客获取 HYX: change to 1  original is 0.4225
    bear_para->threshold_value_inner_ring_alarm1 = 0.6067f;   //内环故障1级报警阈值  默认100km/h 0.6067
    bear_para->threshold_value_inner_ring_alarm2 = 0.91f;    //内环故障2级报警阈值  默认100km/h 0.91
    bear_para->threshold_value_outer_ring_alarm1 = 0.6067f;   //外环故障1级报警阈值  默认100km/h 0.6067
    bear_para->threshold_value_outer_ring_alarm2 = 0.91f;    //外环故障2级报警阈值  默认100km/h 0.91
    bear_para->threshold_value_rolling_body_alarm1 = 3.7696f; //滚动体故障1级报警阈值  默认100km/h 3.7696
    bear_para->threshold_value_rolling_body_alarm2 = 4.08f;   //滚动体故障2级报警阈值  默认100km/h 4.08
    bear_para->threshold_value_holder_alarm1 = 0.4061f;       //滚动体故障1级报警阈值  默认100km/h 0.4061
    bear_para->threshold_value_holder_alarm2 = 0.5414f;       //滚动体故障2级报警阈值  默认100km/h 0.5414
}

/**
 * 初始化轴承算法特征值
 */
static void init_bearing_diag_val(struct BEAR_DIAG_VAL *bear_diag_val)
{
    rt_memset(bear_diag_val, 0, sizeof(struct BEAR_ALARM_VAL));
    if (!bear_diag_val->acc_buff)
    {
        bear_diag_val->acc_buff = (float *)rt_malloc(DIAG_ACC_STEP * sizeof(float));
        if (bear_diag_val->acc_buff)
        {
            rt_memset(bear_diag_val->acc_buff, 0, DIAG_ACC_STEP * sizeof(float));
            //BEARING_PRINT("bear_diag_val->acc_buff ok\n");
        }
        else
        {
            BEARING_PRINT("bear_diag_val->acc_buff err\n");
        }
    }

    if (!bear_diag_val->speed_buff)
    {
        bear_diag_val->speed_buff = (float *)rt_malloc(FS_SPEED * 2 * sizeof(float));
        if (bear_diag_val->speed_buff)
        {
            rt_memset(bear_diag_val->speed_buff, 0, FS_SPEED * 2 * sizeof(float));
           // BEARING_PRINT("bear_diag_val->speed_buff ok\n");
        }
        else
        {
            BEARING_PRINT("bear_diag_val->speed_buff err\n");
        }
    }

	{
		
		bear_diag_val->bear_acc=(uint16_t*)rt_malloc(FS_ICP_BEAR*2);
        if (bear_diag_val->bear_acc)
        {
            rt_memset(bear_diag_val->bear_acc, 0, FS_ICP_BEAR*2);
            //BEARING_PRINT("bear_diag_val->bear_acc ok\n");
        }
        else
        {
            rt_kprintf("bear_diag_val->bear_acc err\n");
        }
	
	}

    bear_diag_val->acc_buff_len = DIAG_ACC_STEP;
}

/**
 * 释放轴承算法缓存
 */
static void destroy_bearing_diag_val(struct BEAR_DIAG_VAL *bear_diag_val)
{
    if (bear_diag_val->acc_buff)
    {
        rt_free(bear_diag_val->acc_buff);
        bear_diag_val->acc_buff = NULL;
    }

    if (bear_diag_val->speed_buff)
    {
        rt_free(bear_diag_val->speed_buff);
        bear_diag_val->speed_buff = NULL;
    }

    memset(bear_diag_val, 0, sizeof(struct BEAR_DIAG_VAL));
}


/**
 * 轴承诊断
 */
void diag_bearing(float *acc_buff, uint32_t acc_buff_size, const struct BEAR_DIAG_CONFIG_PARA bear_para, struct BEAR_DIAG_VAL *bear_val)
{
    int i;
    char debug_buff[32];
    int bearing_roller_num = bear_para.bearing_roller_num; //轴承滚动体的个数
    bear_val->bear_save.diag_speed = get_bearing_diag_speed(&bear_val->speed_save);//100;//
    bear_val->bear_save.temp_env = get_bearing_diag_env_temp(bear_val->bear_save.channel_id);
    bear_val->bear_save.temp_running_gear_temp = get_bearing_diag_running_gear_temp(bear_val->bear_save.channel_id);
    bear_val->bear_save.temp_alarm_status=get_temp_alarm_status(bear_val->bear_save.channel_id);
    float wheel_rotation_fre =bear_val->bear_save.diag_speed/ 3.6f / PI /bear_para.wheel_diameter; //轮对轴转频

    float rolling_body_revolution_fre = wheel_rotation_fre * bear_para.para_holder;                   //滚动体的公转频率,与保持架的旋转频率相同
    float fft_resolution = FS_ACC * 1.0f / acc_buff_size;                                             //频率分辨率

    float f_inner_ring = wheel_rotation_fre *bear_para.para_inner_ring;  //内环故障频率
    float f_outer_ring = wheel_rotation_fre *bear_para.para_outer_ring;  //外环的故障频率 //
    float f_rolling_body = wheel_rotation_fre * bear_para.para_rolling_body; //滚动体的故障频率//
    float f_holder = rolling_body_revolution_fre;                    //保持架的故障频率
//  sprintf(debug_buff, "%f", f_holder);
//  BEARING_PRINT("f_holder=%s\n", debug_buff);
//  sprintf(debug_buff, "%f", f_inner_ring);
//  BEARING_PRINT("f_inner_ring=%s\n", debug_buff);
//  sprintf(debug_buff, "%f", f_outer_ring);
//  BEARING_PRINT("f_outer_ring=%s\n", debug_buff);
//  sprintf(debug_buff, "%f", f_rolling_body);
//  BEARING_PRINT("f_rolling_body=%s\n", debug_buff);

    int begin_index = 0;
    int end_index = 0;

    float outter_1order_fre_amp = 0;                                                        //外环故障幅值
    float inner_1order_fre_amp = 0;                                                         //内环故障频率
    float roller_1order_fre_amp = 0;                                                        //滚动体故障频率
    float holder_1order_fre_amp = 0;                                                        //保持架故障频率

    float outter_1order_fre_hz = 0;
    float inner_1order_fre_hz = 0;
    float roller_1order_fre_hz = 0;
    float holder_1order_fre_hz = 0;

    //去均值
    //  float_sub_mean(acc_buff, acc_buff_size);

    //包络有效值计算

    bear_val->bear_save.rms_envelope = float_std_rms(acc_buff, acc_buff_size);
    //进行fft运算

    fft(acc_buff, acc_buff_size);


#if 0
    float max_amp = 0;
    for (i = 0; i < 4096; i++)
    {
    //  sprintf(debug_buff, "%f", acc_buff[i]);
    //  BEARING_PRINT("    %s\n", debug_buff);
    }
    max_amp = float_get_max(acc_buff + 1, 2047);
    sprintf(debug_buff, "%f", max_amp);
    BEARING_PRINT(" max_amp=%s\n", debug_buff);
#endif


    //内环诊断
    fre_get_fft_max(acc_buff,f_inner_ring - 5,f_inner_ring + 5,fft_resolution,&inner_1order_fre_amp,&inner_1order_fre_hz);
    bear_val->bear_save.amplitude_inner_ring= inner_1order_fre_amp;

    //外环诊断
    fre_get_fft_max(acc_buff,f_outer_ring - 5,f_outer_ring + 5,fft_resolution,&outter_1order_fre_amp,&outter_1order_fre_hz);
    bear_val->bear_save.amplitude_outer_ring=outter_1order_fre_amp;


    //保持架诊断
    fre_get_fft_max(acc_buff,f_holder - 5,f_holder + 5,fft_resolution,&holder_1order_fre_amp,&holder_1order_fre_hz);
    bear_val->bear_save.amplitude_holder=holder_1order_fre_amp;

    //滚动体诊断
    fre_get_fft_max(acc_buff,f_rolling_body - 5,f_rolling_body + 5,fft_resolution,&roller_1order_fre_amp,&roller_1order_fre_hz);
    bear_val->bear_save.amplitude_rolling_body=roller_1order_fre_amp;

	sprintf(debug_buff, "%f", inner_1order_fre_hz);
	BEARING_PRINT("inner_1order_fre_hz=%s\n", debug_buff);

	sprintf(debug_buff, "%f", outter_1order_fre_hz);
	BEARING_PRINT("outter_1order_fre_hz=%s\n", debug_buff);

	sprintf(debug_buff, "%f", holder_1order_fre_hz);
	BEARING_PRINT("holder_1order_fre_hz=%s\n",debug_buff);

	sprintf(debug_buff, "%f", roller_1order_fre_hz);
	BEARING_PRINT("roller_1order_fre_hz=%s\n", debug_buff);

#ifdef DEV_ALARM_EN
    //外环
    if (bear_val->bear_save.amplitude_outer_ring > bear_para.threshold_value_outer_ring_alarm2)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_outter_ring = 2;
    }
    else if (bear_val->bear_save.amplitude_outer_ring > bear_para.threshold_value_outer_ring_alarm1)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_outter_ring = 1;
    }
    else
    {
        bear_val->bear_save.bearing_alarm_status.alarm_outter_ring = 0;
    }

    //内环
    if (bear_val->bear_save.amplitude_inner_ring > bear_para.threshold_value_inner_ring_alarm2)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_inner_ring = 2;
    }
    else if (bear_val->bear_save.amplitude_inner_ring > bear_para.threshold_value_inner_ring_alarm1)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_inner_ring = 1;
    }
    else
    {
        bear_val->bear_save.bearing_alarm_status.alarm_inner_ring = 0;
    }

    //滚动体
    if (bear_val->bear_save.amplitude_rolling_body > bear_para.threshold_value_rolling_body_alarm2)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_rolling_body = 2;
    }
    else if (bear_val->bear_save.amplitude_rolling_body > bear_para.threshold_value_rolling_body_alarm1)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_rolling_body = 1;
    }
    else
    {
        bear_val->bear_save.bearing_alarm_status.alarm_rolling_body = 0;
    }

    //保持架
    if (bear_val->bear_save.amplitude_holder > bear_para.threshold_value_holder_alarm2)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_holder = 2;
    }
    else if (bear_val->bear_save.amplitude_holder > bear_para.threshold_value_holder_alarm1)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_holder = 1;
    }
    else
    {
        bear_val->bear_save.bearing_alarm_status.alarm_holder = 0;
    }

    //综合诊断
    bear_val->bear_save.bearing_alarm_status.alarm_bearing = bear_val->bear_save.bearing_alarm_status.alarm_holder | bear_val->bear_save.bearing_alarm_status.alarm_inner_ring | bear_val->bear_save.bearing_alarm_status.alarm_outter_ring | bear_val->bear_save.bearing_alarm_status.alarm_rolling_body;

    if (bear_val->bear_save.bearing_alarm_status.alarm_bearing == 0)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_type = RUNNING_OK;
    }
    else if (bear_val->bear_save.bearing_alarm_status.alarm_bearing == 1)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_type = ACC_ALARM_FIRST_GRADE;
		rt_kprintf("bear_val->ACC_ALARM_FIRST_GRADE----\n");
    }
    else
    {
        bear_val->bear_save.bearing_alarm_status.alarm_type = ACC_ALARM_SECOND_GRADE;
		rt_kprintf("bear_val->ACC_ALARM_SECOND_GRADE----\n");
    }

    //这个时刻只需要拿温度的报警状态来二次判断，只要温度至少预警就可以上报振动预报警
    if (get_temp_alarm_status(bear_val->bear_save.channel_id) == RUNNING_OK)
    {
        bear_val->bear_save.bearing_alarm_status.alarm_type = RUNNING_OK;
        bear_val->bear_save.bearing_alarm_status.alarm_bearing=0;
        bear_val->bear_save.bearing_alarm_status.alarm_holder = 0;
        bear_val->bear_save.bearing_alarm_status.alarm_rolling_body = 0;
        bear_val->bear_save.bearing_alarm_status.alarm_inner_ring = 0;
        bear_val->bear_save.bearing_alarm_status.alarm_outter_ring = 0;
    }
#endif

}


/**
 * fft的C语言实现
 */
#ifdef MCU_ARM
static void fft(float32_t *buff, int size)
{
    static float32_t fft_temp[4096 * 2];
    uint16_t i;
    arm_cfft_radix4_instance_f32 scfft;
    memset(fft_temp, 0, sizeof(fft_temp));
    for (i = 0; i < size; i++)
    {
        fft_temp[2 * i] = buff[i];
    }
    arm_cfft_radix4_init_f32(&scfft, size, 0, 1);
    arm_cfft_radix4_f32(&scfft, fft_temp);
    arm_cmplx_mag_f32(fft_temp, buff, size);

    for (i = 0; i < size; i++)
    {
        buff[i] = buff[i] / (size / 2);
    }
}
#else
float *fft(float *p, int N)
{
    int m = 0;
    int N_fft = 0;
    int t0 = 0;
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    int t4 = 0;
    float Nfft_float = 0;
    struct CPLX u, v;
    //FFT
    m = log2(N);       //m是log2(N)
    N_fft = pow(2, m); //做fft的数据总个数
    Nfft_float = (float)(N_fft);
    float *c = (float *)rt_malloc((2 * N_fft + 10) * sizeof(float));           //new float[2 * N_fft + 10];
    float *z = (float *)rt_malloc((2 * N_fft + 10) * sizeof(float));           //new float[2 * N_fft + 10];
    float *d = (float *)rt_malloc((2 * N_fft + 10) * sizeof(float));           //new float[2 * N_fft + 10];
    float *fft_complex = (float *)rt_malloc((2 * N_fft + 10) * sizeof(float)); //new float[2 * N_fft + 10];
    float *fft_result = (float *)rt_malloc((N_fft + 10) * sizeof(float));     //new float[N_fft + 10];
    for (int i = 0; i < N_fft; i++)
    {
        c[i] = *(p + i);
        c[i + N_fft] = 0;
    }
    for (int i = 0; i < N_fft; i++)
    {
        z[i] = cos(2 * PI * i / N_fft);
        z[i + N_fft] = -sin(2 * PI * i / N_fft);
    }

    for (int n = 0; n < m; n++)
    {
        for (int k = 0; k <= pow(2, (m - n - 1)) - 1; k++)
        {
            for (int j = 0; j <= pow(2, n) - 1; j++)
            {
                t0 = pow(2, n) * k + j;
                t1 = j * pow(2, (m - n - 1));
                t2 = pow(2, n) * k + pow(2, (m - 1)) + j;
                t3 = pow(2, (n + 1)) * k + j;
                t4 = pow(2, (n + 1)) * k + j + pow(2, n);
                u.real = *(c + t0);
                u.imag = *(c + t0 + N_fft);
                v.real = z[t1] * c[t2] - z[t1 + N_fft] * c[t2 + N_fft];
                v.imag = z[t1 + N_fft] * c[t2] + z[t1] * c[t2 + N_fft];
                d[t3] = u.real + v.real;
                d[t3 + N_fft] = u.imag + v.imag;
                d[t4] = u.real - v.real;
                d[t4 + N_fft] = u.imag - v.imag;
            }
        }
        for (int i = 0; i < N_fft; i++)
        {
            *(c + i) = *(d + i);
            *(c + i + N_fft) = *(d + i + N_fft);
        }
    }
    for (int i = 0; i < N_fft; i++)
    {
        *(fft_complex + i) = *(c + i);
        *(fft_complex + i + N_fft) = *(c + i + N_fft);
    }
    for (int i = 0; i < N_fft / 2; i++)
    {
        *(fft_result + i) = sqrt(*(fft_complex + i) * (*(fft_complex + i)) + *(fft_complex + i + N_fft) * (*(fft_complex + i + N_fft))) * 2 / Nfft_float;
    }
    rt_free(c);
    rt_free(z);
    rt_free(d);
    return fft_result;
}
#endif

/**
 * 避免计算轴承特征值时，频率取值范围有负数部分
 */

static void get_correct_index(int *index)
{
    if (*index <= 0)
    {
        *index = 1;
    }
}

/**
*轴承算法诊断线程
*
**/
//struct BEAR_DIAG_VAL tz_bear;
extern int w1_index;
static void diagnosis_bearing_thread_entry(void *paramter)
{
    rt_uint32_t e;
    uint16_t cnt;
    char debug_buff[32];
    uint8_t *bearing_send_buff = NULL;
    struct BEAR_DIAG_VAL tz_bear;
    init_bearing_diag_val(&tz_bear);

#ifdef CLB_TEST
    uint32_t outc_5k_next_ch=0;
    uint32_t out34_next_ch=0;
    while (1)
    {		
		if(rt_event_recv(&switchs.out34_event,(1 << 4), Event_Option, RT_WAITING_FOREVER, &e)== RT_EOK)  //
		{								
			int data_cnt,write_res;
			char test_buff[16];	
		    if(Get_Sdio_Stat() != SDIO_RUN_ING )
		        continue ;
					
			for(data_cnt = 0;data_cnt < 9000;data_cnt ++)
			{				
				sprintf(test_buff,"%f",(float)out5k_t[2]->buffer[data_cnt]);
				test_buff[5] = '\n';
				write(vibr_file.test3.fd,test_buff,6);				
				memset(test_buff,0,sizeof(test_buff));					
			}
			rt_kprintf("outC3:%d\n",data_cnt);
			sync(vibr_file.test3.fd);
			
			for(data_cnt = 0;data_cnt < 9000;data_cnt ++)
			{				
				sprintf(test_buff,"%f",(float)out5k_t[3]->buffer[data_cnt]);
				test_buff[5] = '\n';
				write(vibr_file.test4.fd,test_buff,6);				
				memset(test_buff,0,sizeof(test_buff));					
			}
			rt_kprintf("outC4:%d\n",data_cnt);
			sync(vibr_file.test4.fd);
			
			rt_memset(out5k_t[2]->buffer,0,18000);
			rt_memset(out5k_t[3]->buffer,0,18000);
			
//			out34_next_ch=ad_acc_polygon.ch_num+1;
//			if(out34_next_ch>USE_CNT)
//            out34_next_ch=1;
//			rt_mb_send(&switchs.out34_mb,out34_next_ch);	
				w1_index=0;			
			continue ;
								
		}
	}
#endif	
	
 
    while (1)
    {

				
		if(rt_event_recv(&switchs.outc_event,(1 << 3), Event_Option, RT_WAITING_FOREVER, &e)== RT_EOK)  //
		{      


			rt_memmove(tz_bear.bear_acc,outc_t[0]->buffer,FS_ICP_BEAR*2);
			rt_memmove(&tz_bear.bear_save.pub_info, &outc_t[0]->info,sizeof( struct AD_PUBLIC_INFO));
           
            tz_bear.bear_save.channel_id =bear_ch_num - 1;
            BEARING_PRINT("\r\n<--------diag_bearing_ch%d----->\r\n", tz_bear.bear_save.channel_id + 1);
            tz_bear.bear_save.pub_info.train_id = clb_config_para.train_net_addr.train_id;
            if (tz_bear.bear_save.channel_id < 4)
            {
                tz_bear.bear_save.pub_info.proc_id = 1;
            }
            else if(tz_bear.bear_save.channel_id < 8)
            {
                tz_bear.bear_save.pub_info.proc_id = 2;
            }
            else if(tz_bear.bear_save.channel_id < 12)
            {
                tz_bear.bear_save.pub_info.proc_id = 3;
            }
            else
                tz_bear.bear_save.pub_info.proc_id = 4;


#if 0

#else
            for (cnt = 0; cnt < tz_bear.acc_buff_len; cnt++)
            {
                tz_bear.acc_buff[cnt] = convert_ad_bearing(tz_bear.bear_acc[DIAG_ACC_STEP+cnt ]);
            }
            //获取轴承自检状态
            tz_bear.bear_save.self_test=(ad_acc_bearing.self_test.bytes&((uint16_t)1<<tz_bear.bear_save.channel_id))>>(tz_bear.bear_save.channel_id);

            /*轴承算法*/
            diag_bearing(tz_bear.acc_buff, tz_bear.acc_buff_len, clb_config_para.bear_diag_config_para, &tz_bear);

            update_mvb_bearing_diag_status(tz_bear.bear_save.channel_id, tz_bear.bear_save.bearing_alarm_status.alarm_bearing);

//            if (tz_bear.bear_save.bearing_alarm_status.alarm_type == RUNNING_OK)
//            {
//               // BEARING_PRINT("bear: ch%d diag ok...\n", tz_bear.bear_save.channel_id);
//            }
//            else if (tz_bear.bear_save.bearing_alarm_status.alarm_type == ACC_ALARM_FIRST_GRADE)
//            {
//                BEARING_PRINT("bear: ch%d diag acc_alarm_first_grade!!!!!\n", tz_bear.bear_save.channel_id);
//            }
//            else
//            {
//                BEARING_PRINT("bear: ch%d diag acc_alarm_second_grade!!!!!!!!\n", tz_bear.bear_save.channel_id);
//            }
			
#endif
            if(!bearing_send_buff)
            {
                uint32_t send_len;
                send_len = sizeof(struct BEAR_SAVE) + FS_ICP_BEAR* 2 + sizeof(struct SPEED_SAVE); //原始数据字节数
                bearing_send_buff = (uint8_t *)rt_malloc(send_len);
                if(bearing_send_buff)
                {
                    
                    memmove(bearing_send_buff, &tz_bear.bear_save, sizeof(struct BEAR_SAVE));

                    //发送轴承特征数据
                    net_send(clb_config_para.train_net_addr.train_id, 1, BEARING_TZ_DATA_TYPE, bearing_send_buff, sizeof(struct BEAR_SAVE),0);

                    //发送轴承特征数据 + 原始数据 + 速度原始数据
                    memmove(&bearing_send_buff[sizeof(struct BEAR_SAVE)], tz_bear.bear_acc, FS_ICP_BEAR * 2);
                    memmove(&bearing_send_buff[sizeof(struct BEAR_SAVE) + FS_ICP_BEAR * 2], &tz_bear.speed_save, sizeof(struct SPEED_SAVE));

                    //rt_kprintf ("send_len:%d\n",send_len);
                    net_send(clb_config_para.train_net_addr.train_id, 1, BEARING_RAW_DATA_TYPE, &bearing_send_buff[0], send_len,0);
					              
                    rt_free(bearing_send_buff);
                    bearing_send_buff = NULL;
                }
            }
			memset(&tz_bear.bear_save, 0, sizeof(struct BEAR_SAVE));
            memset(&tz_bear.speed_save, 0, sizeof(struct SPEED_SAVE));
			//rt_kprintf("end bearing ---\n");

        }
    }
}


/**
 * 轴承AD转加速度公式
*/
static inline float convert_ad_bearing(unsigned short ad)
{
    return (ad - 32768) / 65536.0 * 20 / clb_config_para.bear_diag_config_para.bear_acc_sensitivity/0.416;//电路放大0.416倍

}

/**
*轴承MVB发送结果
*/
void update_mvb_bearing_diag_status(uint8_t ch, int diag_res)
{
    uint8_t first_alarm, second_alarm = 0;
    if (diag_res == 0) //正常结果
    {
        first_alarm = 0;
        second_alarm = 0;
    }
    else if (diag_res == 3) //1级报警
    {
        first_alarm = 1;
        second_alarm = 0;
    }

    else if(diag_res == 4) //2级报警
    {
        first_alarm = 0;
        second_alarm = 1;
    }

    //port1
    if (ch == 0)
    {
        send_mvb_data.sensor_alarm_data.bits.side2_shaft_end_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side2_shaft_end_bearing_second_alarm = second_alarm;
    }
    else if (ch == 1)
    {
       send_mvb_data.sensor_alarm_data.bits.shaft1_gear_box_bearing_first_alarm = first_alarm;
       send_mvb_data.sensor_alarm_data.bits.shaft1_gear_box_bearing_second_alarm = second_alarm;
       send_mvb_data.sensor_alarm_data.bits.shaft1_gear_box_gear_first_alarm = 0;
       send_mvb_data.sensor_alarm_data.bits.shaft1_gear_box_gear_second_alarm = 0;
    }
    else if (ch == 2)
    {
       send_mvb_data.sensor_alarm_data.bits.shaft1_motor_bearing_first_alarm = first_alarm;
       send_mvb_data.sensor_alarm_data.bits.shaft1_motor_bearing_second_alarm = second_alarm;
    }
    else if (ch == 3)
    {
        send_mvb_data.sensor_alarm_data.bits.side1_shaft_end_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side1_shaft_end_bearing_second_alarm = second_alarm;
    }

    //port2
    else if (ch == 4)
    {
        send_mvb_data.sensor_alarm_data.bits.side3_shaft_end_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side3_shaft_end_bearing_second_alarm = second_alarm;
    }
    else if (ch == 5)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft2_gear_box_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.shaft2_gear_box_bearing_second_alarm = second_alarm;
        send_mvb_data.sensor_alarm_data.bits.shaft2_gear_box_gear_first_alarm = 0;
        send_mvb_data.sensor_alarm_data.bits.shaft2_gear_box_gear_second_alarm = 0;
    }
    else if (ch == 6)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft2_motor_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.shaft2_motor_bearing_second_alarm = second_alarm;
    }
    else if (ch == 7)
    {
        send_mvb_data.sensor_alarm_data.bits.side4_shaft_end_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side4_shaft_end_bearing_second_alarm = second_alarm;
    }

    //port3
    else if (ch == 8)
    {
        send_mvb_data.sensor_alarm_data.bits.side7_shaft_end_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side7_shaft_end_bearing_second_alarm = second_alarm;
    }
    else if (ch == 9)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft4_gear_box_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.shaft4_gear_box_bearing_second_alarm = second_alarm;
        send_mvb_data.sensor_alarm_data.bits.shaft4_gear_box_gear_first_alarm = 0;
        send_mvb_data.sensor_alarm_data.bits.shaft4_gear_box_gear_second_alarm = 0;
    }
    else if (ch == 10)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft4_motor_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.shaft4_motor_bearing_second_alarm = second_alarm;
    }
    else if (ch == 11)
    {
        send_mvb_data.sensor_alarm_data.bits.side8_shaft_end_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side8_shaft_end_bearing_second_alarm = second_alarm;
    }

    //port4
    else if (ch == 12)
    {
        send_mvb_data.sensor_alarm_data.bits.side6_shaft_end_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side6_shaft_end_bearing_second_alarm = second_alarm;
    }
    else if (ch == 13)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft3_gear_box_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.shaft3_gear_box_bearing_second_alarm = second_alarm;
        send_mvb_data.sensor_alarm_data.bits.shaft3_gear_box_gear_first_alarm = 0;
        send_mvb_data.sensor_alarm_data.bits.shaft3_gear_box_gear_second_alarm = 0;
    }
    else if (ch == 14)
    {
        send_mvb_data.sensor_alarm_data.bits.shaft3_motor_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.shaft3_motor_bearing_second_alarm = second_alarm;
    }
    else if (ch == 15)
    {
        send_mvb_data.sensor_alarm_data.bits.side5_shaft_end_bearing_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side5_shaft_end_bearing_second_alarm = second_alarm;
    }

}


/**
*轴承线程
*/
int diagnosis_bearing_thread_init( char *arg)
{
    rt_err_t result;
//	if (!diagnosis_bearing_thread_stack)
//    {
//        diagnosis_bearing_thread_stack = (rt_uint8_t *)rt_malloc(1024*10);
//    }
//	
//	if(diagnosis_bearing_thread_stack==RT_NULL)
//	{
//		rt_kprintf("diagnosis_bearing_thread_stack rt_malloc err\n");
//		return -1;
//	}
	
    /*自检线程*/
    result = rt_thread_init(&diagnosis_bearing_thread, "diag_bearing",
                                diagnosis_bearing_thread_entry, RT_NULL,
                                diagnosis_bearing_thread_stack, 1024*6, 25, 70);
        if (result == RT_EOK)
            rt_thread_startup(&diagnosis_bearing_thread);
    else
        rt_kprintf("diagnosis_bearing_thread_init err\n");

    return result;
}


/** @} */ //\defgroup module API

/** @} */ //\addtogroup dev module
#endif

#endif //\end module API
/* !< end file   */
