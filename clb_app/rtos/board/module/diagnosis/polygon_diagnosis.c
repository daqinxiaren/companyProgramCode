/**
 *
 * Copyright (c) 2015-2020  LH  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the polygon_diagnosis.c API or library.
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
#include <polygon_diagnosis.h>
#include <ad7606_app.h>
#include <can_app.h>
#include <speed_deal.h>
#include <math_app.h>
#include <net_protocol.h>
#include <net_app.h>
#include "sdcard_save.h"
#include <dfs_posix.h>
#include <switch_app.h>
#include <user_data.h>

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
#define POLYGON_DEBUG
#ifdef POLYGON_DEBUG
#define POLYGON_PRINT rt_kprintf
#else
#define POLYGON_PRINT(...)
#endif


/**
 **************<Private variables>**************
 */
CCMRAM struct rt_thread diagnosis_polygon_thread;
CCMRAM static rt_uint8_t diagnosis_polygon_thread_stack[1024*6];

static float FILTER_NUM_PARA[5] = {0.028, 0.053, 0.071, 0.053, 0.028};
static float FILTER_DEN_PARA[5] = {1.000, -2.026, 2.148, -1.159, 0.279};


/**
 **************<Private functions>**************
 */
static inline int check_bearing_box_channel(unsigned char ch);
static void init_polygon_default_config(struct POLYGON_DIAG_CONFIG_PARA *polygon_para);
static void init_polygon_diag(struct POLYGON_CALC *tz_polygon);
static void destroy_polygon_diag(struct POLYGON_CALC *tz_polygon);
static int judge_speed(float *speed_buff,uint32_t speed_buff_len,float *speed_max,float *min_speed,const struct POLYGON_DIAG_CONFIG_PARA speed_judge_para);
static void detrend(float *acc_buff, uint32_t acc_buff_len);
static void filter(float *acc_buff, uint32_t size);
static void interp1(float *src, uint32_t src_size, float *des, uint32_t des_size);
static void circle_integral(struct POLYGON_CALC *tz_polygon, struct POLYGON_DIAG_CONFIG_PARA polygon_para);
static void threshold_decision(struct POLYGON_CALC *tz_polygon, const struct POLYGON_DIAG_CONFIG_PARA polygon_para);
static int get_rotation_num(struct POLYGON_DIAG_CONFIG_PARA polygon_para);
static int get_circle_vector_size(struct POLYGON_DIAG_CONFIG_PARA polygon_para);
static float cal_db_val(float *src, int size, const float db_threshold);
static inline float convert_ad_polygon(unsigned short ad);
void updata_tread_mvb_data(uint8_t ch, int diag_res);
extern void update_mvb_bearing_diag_status(uint8_t ch, int diag_res);
static void diagnosis_polygon_thread_entry(void *paramter);

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
*判断是否进行多边形计算的通道
*/
static inline int check_bearing_box_channel(unsigned char ch)
{
    if(ch==0||ch==3||ch==4||ch==7||ch==8||ch==11||ch==12||ch==15)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * 初始化多边形算法默认配置
 */
static void init_polygon_default_config(struct POLYGON_DIAG_CONFIG_PARA *polygon_para)
{
    polygon_para->polygon_acc_sensitivity = 0.01;
    polygon_para->threshold_speed_delta = 6;
    polygon_para->threshold_speed_max = 80;
    polygon_para->wheel_diameter = 0.85;
    polygon_para->threshold_warn1 = 1;
    polygon_para->threshold_warn2 = 20;
    polygon_para->threshold_warn3 = 18;
    polygon_para->threshold_alarm1 = 3;
    polygon_para->threshold_alarm2 = 40;
    polygon_para->threshold_alarm1 = 24;
    polygon_para->calc_db1_para = 1500;
    polygon_para->calc_db2_para = 1.5;
}

/**
 * 初始化多边形算法的参数和缓存
 */
static void init_polygon_diag(struct POLYGON_CALC *tz_polygon)
{
    memset(tz_polygon, 0, sizeof(struct POLYGON_CALC));


    if (!tz_polygon->polygon_save)
    {
        tz_polygon->polygon_save = ( struct POLYGON_SAVE *)rt_malloc( sizeof( struct POLYGON_SAVE ));
        if (tz_polygon->polygon_save)
        {
            memset(tz_polygon->polygon_save, 0,sizeof( struct POLYGON_SAVE ) );
            //POLYGON_PRINT("tz_polygon->acc_buff ok\n");
        }
        else
        {
            POLYGON_PRINT("tz_polygon->polygon_save err\n");
        }

    }

    tz_polygon->acc_buff_len = FS_ACC * POLYGON_DIAG_STEP;

    if (!tz_polygon->acc_buff)
    {
        tz_polygon->acc_buff = (float *)rt_malloc(tz_polygon->acc_buff_len * sizeof(float));

        if (tz_polygon->acc_buff)
        {
            memset(tz_polygon->acc_buff, 0, tz_polygon->acc_buff_len * sizeof(float));
            //POLYGON_PRINT("tz_polygon->acc_buff ok\n");
        }
        else
        {
            POLYGON_PRINT("tz_polygon->acc_buff err\n");
        }
    }

    if (!tz_polygon->speed_buff)
    {
        tz_polygon->speed_buff = (float *)rt_malloc(FS_SPEED * POLYGON_DIAG_STEP * sizeof(float));
        if (tz_polygon->speed_buff)
        {
            memset(tz_polygon->speed_buff, 0, FS_SPEED * POLYGON_DIAG_STEP * sizeof(float));
            //POLYGON_PRINT("tz_polygon->speed_buff ok\n");
        }
        else
        {
            POLYGON_PRINT("tz_polygon->speed_buff err\n");
        }
    }
	
	if(!tz_polygon->polygon_acc)
	{
		
		tz_polygon->polygon_acc=(uint16_t*)rt_malloc(FS_ICP_VIBRA*2);
        if (tz_polygon->polygon_acc)
        {
            rt_memset(tz_polygon->polygon_acc, 0, FS_ICP_VIBRA*2);
           
        }
        else
        {
            rt_kprintf("tz_polygon->polygon_acc err\n");
        }
	
	}
	
}

/**
 * 释放多边形算法的参数和缓存
 */
static void destroy_polygon_diag(struct POLYGON_CALC *tz_polygon)
{
    if (tz_polygon->acc_buff)
    {
        rt_free(tz_polygon->acc_buff);
        tz_polygon->acc_buff = NULL;
    }

    if (tz_polygon->speed_buff)
    {
        rt_free(tz_polygon->speed_buff);
        tz_polygon->speed_buff = NULL;
    }
    memset(tz_polygon, 0, sizeof(struct POLYGON_CALC));
}

/**
 * 速度判断
 *
 */
int judge_speed(float *speed_buff,
                uint32_t speed_buff_len,
                float *speed_max,
                float *min_speed,
                const struct POLYGON_DIAG_CONFIG_PARA speed_judge_para)
{
    int i;
    float max = speed_buff[0];
    float min = speed_buff[0];
    float MAX_SPEED = speed_judge_para.threshold_speed_max + speed_judge_para.threshold_speed_delta;
    float MIN_SPEED = speed_judge_para.threshold_speed_max - speed_judge_para.threshold_speed_delta;
    //计算该段速度的最大值和最小值
    for (i = 0; i < speed_buff_len; i++)
    {
        if (max < speed_buff[i])
        {
            max = speed_buff[i];
        }
        else if (min > speed_buff[i])
        {
            min = speed_buff[i];
        }
    }
    *speed_max =  max;
    *min_speed =  min;
    //判断速度的最大和最小值是否在规定的范围内
    if ((max <= MAX_SPEED) && (min >= MIN_SPEED))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * 去除趋势项
 *
 */
static void detrend(float *acc_buff, uint32_t acc_buff_len)
{
    int i, j;
    if (acc_buff == NULL)
    {
        return;
    }
    float *acc_detrend_step_buff = (float *)rt_malloc(DETREND_STEP * sizeof(float));
    float *index = (float *)rt_malloc(DETREND_STEP * sizeof(float));
    int size = DETREND_STEP;

    if (acc_detrend_step_buff == NULL)
    {
        POLYGON_PRINT("acc_detrend_step_buff==NULL\n");
    }

    if (index == NULL)
    {
        POLYGON_PRINT("index==NULL\n");
    }

    for (i = 0; i < DETREND_STEP; i++)
    {
        index[i] = i;
    }

    if (acc_detrend_step_buff && index)
    {
        //去均值
        float mean_acc = float_sum(acc_buff, acc_buff_len) / acc_buff_len;
        for (i = 0; i < acc_buff_len; i++)
        {
            acc_buff[i] = acc_buff[i] - mean_acc;
        }

        //去趋势项
        for (i = 0; i < acc_buff_len / size; i++)
        {
            memcpy(acc_detrend_step_buff, &acc_buff[i * DETREND_STEP], sizeof(float) * size);
            float *x = index;
            float *y = acc_detrend_step_buff;

            float m = (size * float_sum_x2(x, size) - float_sum(x, size) * float_sum(x, size));
            float k = (size * float_sum_xy(x, y, size) - float_sum(x, size) * float_sum(y, size)) / m;

            float b = (float_sum_x2(x, size) * float_sum(y, size) - float_sum(x, size) * float_sum_xy(x, y, size)) / m;

            for (j = 0; j < size; j++)
            {
                acc_buff[i * size + j] = acc_buff[i * size + j] - (k * x[j] + b); //y_new=y-y1
            }
        }

        rt_free(acc_detrend_step_buff);
        acc_detrend_step_buff = NULL;
        rt_free(index);
        index = NULL;
    }
}

/**
 * 滤波
 *
 */
static void filter(float *acc_buff, uint32_t size)
{
    int i, q;
    int filter_order = sizeof(FILTER_NUM_PARA) / sizeof(FILTER_NUM_PARA[0]);
    int acc_buff_len = size;
    float *num = FILTER_NUM_PARA;
    float *den = FILTER_DEN_PARA;
    float *filter_in = (float *)rt_malloc((filter_order + acc_buff_len - 1) * sizeof(float));
    float *filter_out = (float *)rt_malloc((filter_order + acc_buff_len - 1) * sizeof(float));

    if (filter_in == NULL)
    {
        POLYGON_PRINT("filter_in==NULL\n");
    }

    if (filter_out == NULL)
    {
        POLYGON_PRINT("filter_out==NULL\n");
    }

    if (filter_in && filter_out && num && den)
    {
        int n_Den = sizeof(FILTER_DEN_PARA) / sizeof(FILTER_DEN_PARA[0]);
        int n_Num = sizeof(FILTER_NUM_PARA) / sizeof(FILTER_NUM_PARA[0]);

        //镜像消除滤波器端点效应
        for (i = 0; i < filter_order - 1; i++)
        {
            filter_in[i] = 0;
        }
        for (i = filter_order - 1; i < filter_order + acc_buff_len - 1; i++)
        {
            filter_in[i] = acc_buff[i];
        }

        memset(filter_out, 0, (filter_order + acc_buff_len - 1) * sizeof(float));

        for (q = filter_order - 1; q < (acc_buff_len + filter_order - 1); q++) //改地方数组越界，一共申请了（acc_buff_len + filter_order-1），那下标的最大值就是acc_buff_len + filter_order-2
        {
            float y_result_temp = 0;
            float x_result_temp = 0;
            for (i = 1; i < n_Den; i++)
            {
                y_result_temp = den[i] * filter_out[q - i] + y_result_temp;
            }
            for (i = 0; i < n_Num; i++)
            {
                x_result_temp = num[i] * filter_in[q - i] + x_result_temp;
            }
            filter_out[q] = (x_result_temp - y_result_temp) / den[0];
        }

        for (i = 0; i < acc_buff_len; i++)
        {
            acc_buff[i] = filter_out[i];
        }

        if (filter_out)
        {
            rt_free(filter_out);
            filter_out = NULL;
        }

        if (filter_in)
        {
            rt_free(filter_in);
            filter_in = NULL;
        }
    }
}

/**
 * 一维插值函数:/y'=(y2-y1)*(x'-x1)/(x2-x1)+y1
 *
 */
static void interp1(float *src, uint32_t src_size, float *des, uint32_t des_size)
{
    if (src_size == 0 || des_size == 0)
    {
        return;
    }
    int i;
    int x_floor, x_ceil;
    float delta = (src_size - 1) * 1.0 / des_size;
    float *x_out = (float *)rt_malloc(des_size * sizeof(float));
    if (x_out == NULL)
    {
        POLYGON_PRINT("x_out==NULL\n");
        return;
    }

    x_out[0] = 0;
    for (i = 1; i < des_size; i++)
    {
        x_out[i] = x_out[i - 1] + delta; //          x'
    }

    for (i = 0; i < des_size; i++)
    {
        x_floor = (int)x_out[i];                                                                            //向下取整  x1
        x_ceil = (int)(x_out[i] + 1);                                                                       //向上取整  x2
        des[i] = ((src[x_ceil] - src[x_floor]) * (x_out[i] - x_floor)) / (x_ceil - x_floor) + src[x_floor]; //y'=(y2-y1)*(x'-x1)/(x2-x1)+y1
    }

    if (x_out)
    {
        rt_free(x_out);
        x_out = NULL;
    }
}

/**
 * description:polygon arithmetic
 * author:
 * date:2018/08/27
 */
static void circle_integral(struct POLYGON_CALC *tz_polygon, struct POLYGON_DIAG_CONFIG_PARA polygon_para)
{
    int i, j;
    int speed_interp_size = FS_SPEED_INTERP * POLYGON_DIAG_STEP;   //速度插值后的长度
    int rotation_num = get_rotation_num(polygon_para);             // circles矩阵的行
    int circle_vector_size = get_circle_vector_size(polygon_para); //circles矩阵的列
    float *speed_interp = (float *)rt_malloc(speed_interp_size * sizeof(float));
    int *circle_index_acc = (int *)rt_malloc((rotation_num + 1) * sizeof(int));
    float **circles_interp = (float **)rt_malloc(rotation_num * sizeof(float *));

    for (i = 0; i < rotation_num; i++)
    {
        circles_interp[i] = (float *)rt_malloc(circle_vector_size * sizeof(float));
    }

    if (speed_interp == NULL)
    {
        POLYGON_PRINT("speed_interp==NULL\n");
    }

    if (circle_index_acc == NULL)
    {
        POLYGON_PRINT("circle_index_acc==NULL\n");
    }

    if (speed_interp && circle_index_acc && circles_interp)
    {
        int N1 = (int)(FS_ACC / FS_SPEED_INTERP); //1个速度值对应N1个加速度值 这个N1的理想状态是1，默认值是10，越接近1分析结果越精确
        int N_circle = 0;
        float Li = 0;
        circle_index_acc[0] = 0;
        interp1(tz_polygon->speed_buff, tz_polygon->speed_buff_len, speed_interp, speed_interp_size);
        for (i = 0; i < speed_interp_size; i++)
        {
            Li += speed_interp[i] / 3.6 / FS_SPEED_INTERP;

            if (Li >= (polygon_para.wheel_diameter * PI - (polygon_para.threshold_speed_max - polygon_para.threshold_speed_delta) / 3.6 / FS_SPEED_INTERP / 2))
            {
                Li = 0;
                N_circle++;
                circle_index_acc[N_circle] = i * N1;
                if (N_circle >= rotation_num)
                {
                    break;
                }
            }
        }
        //逐行插值
        for (i = 0; i < rotation_num; i++)
        {
            interp1(&tz_polygon->acc_buff[circle_index_acc[i]], circle_index_acc[i + 1] - circle_index_acc[i], circles_interp[i], circle_vector_size);
        }
        //struct TZ_POLYGON tz_polygon;
        //逐列计算均值
//        for (i = 0; i < circle_vector_size; i++)
//        {
//            tz_polygon->polygon_save->TZ4_vector[i] = 0;
//            for (j = 0; j < rotation_num; j++)
//            {
//                tz_polygon->polygon_save->TZ4_vector[i] += circles_interp[j][i];
//            }
//            tz_polygon->polygon_save->TZ4_vector[i] = tz_polygon->polygon_save->TZ4_vector[i] / rotation_num;
//        }
//
//        //计算TZ1：TZ4 vector的均方根
//        tz_polygon->polygon_save->TZ1_scalar = float_std_rms(tz_polygon->polygon_save->TZ4_vector, circle_vector_size);

        if (speed_interp)
        {
            rt_free(speed_interp);
            speed_interp = NULL;
        }

        if (circle_index_acc)
        {
            rt_free(circle_index_acc);
            circle_index_acc = NULL;
        }

        if (circles_interp)
        {
            for (i = 0; i < rotation_num; i++)
            {
                rt_free(circles_interp[i]);
                circles_interp[i] = NULL;
            }
            rt_free(circles_interp);
            circles_interp = NULL;
        }
    }
}

/**
 * 诊断决策
 *
 */
static void threshold_decision(struct POLYGON_CALC *tz_polygon, const struct POLYGON_DIAG_CONFIG_PARA polygon_para)
{
    if (tz_polygon->polygon_save->TZ1_scalar >= polygon_para.threshold_alarm1 || tz_polygon->polygon_save->TZ2_scalar >= polygon_para.threshold_alarm2 ||
        tz_polygon->polygon_save->TZ3_scalar >= polygon_para.threshold_alarm3)
    {
        tz_polygon->polygon_save->diag_res = ACC_ALARM_SECOND_GRADE;
    }
    else if (tz_polygon->polygon_save->TZ1_scalar >= polygon_para.threshold_warn1 || tz_polygon->polygon_save->TZ2_scalar >= polygon_para.threshold_warn2 || tz_polygon->polygon_save->TZ3_scalar >= polygon_para.threshold_warn3)
    {
        tz_polygon->polygon_save->diag_res = ACC_ALARM_FIRST_GRADE;
    }
    else
    {
        tz_polygon->polygon_save->diag_res = RUNNING_OK;
    }
}

/**
 * 计算1min车轮转数,计算circle矩阵的行
 *
 */
static int get_rotation_num(struct POLYGON_DIAG_CONFIG_PARA polygon_para)
{
    float threshold_speed_max = polygon_para.threshold_speed_max;
    float threshold_speed_delta = polygon_para.threshold_speed_delta;
    float wheel_diameter = polygon_para.wheel_diameter;
    int diag_step_time = POLYGON_DIAG_STEP;
    int rotation_num = (int)(diag_step_time * (threshold_speed_max - threshold_speed_delta) / 3.6 / PI / wheel_diameter);
    return rotation_num;
}

/**
 * 计算每圈加速度数据,计算circle矩阵的列
 *
 */
static int get_circle_vector_size(struct POLYGON_DIAG_CONFIG_PARA polygon_para)
{
    int rotation_num = get_rotation_num(polygon_para);
    int acc_size = FS_ACC * POLYGON_DIAG_STEP;
    int circle_vector_size = acc_size / rotation_num;
    //向上取一百
    int hundred = circle_vector_size / 100 + 1;
    return hundred * 100;
}

/**
 * 计算DB值
 */
static float cal_db_val(float *src, int size, const float db_threshold)
{
    float max_val = float_get_max(src, size);
    float min_val = float_get_min(src, size);
    float rms = float_std_rms(src, size);
    float val = 20 * log10(rms * (max_val - min_val) / db_threshold);
    if (val > 0)
    {
        return val;
    }
    else
    {
        return 0;
    }
}

/**
*判断是否进行多边形计算的通道
*/

 struct CPLX_p
{
	double real;
	double imag;
};

void fft_polygon(float *p, int N)
{
	int m = 0;
	int N_fft = 0;
	int t0 = 0;
	int t1 = 0;
	int t2 = 0;
	int t3 = 0;
	int t4 = 0;
	float Nfft_float = 0;
	struct CPLX_p u, v;
	//FFT
	m = log2(N);	   //m是log2(N)
	N_fft = pow(2, m); //做fft的数据总个数
	Nfft_float = (float)(N_fft);
	float *c = (float *)rt_malloc((2 * N_fft + 10) * sizeof(float));		   //new float[2 * N_fft + 10];
	float *z = (float *)rt_malloc((2 * N_fft + 10) * sizeof(float));		   //new float[2 * N_fft + 10];
	float *d = (float *)rt_malloc((2 * N_fft + 10) * sizeof(float));		   //new float[2 * N_fft + 10];
	float *fft_complex = (float *)rt_malloc((2 * N_fft + 10) * sizeof(float)); //new float[2 * N_fft + 10];
	//float *fft_result = (float *)rt_malloc((N_fft + 10) * sizeof(float));	  //new float[N_fft + 10];
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
	for (int i = 0; i < N_fft/2 ; i++)
	{
		p[i] = sqrt(*(fft_complex + i) * (*(fft_complex + i)) + *(fft_complex + i + N_fft) * (*(fft_complex + i + N_fft))) * 2 / Nfft_float;
		
	}
	rt_free(c);
	rt_free(z);
	rt_free(d);

	rt_free(fft_complex);
	c=NULL;
	z=NULL;
	d=NULL;

	fft_complex=NULL;
}


void diag_polygon(float *acc_buff, uint32_t acc_buff_size, struct POLYGON_DIAG_CONFIG_PARA polygon_para, struct POLYGON_CALC *tz_polygon)
{

    static	int order_5[15][5]={0};
    static uint32_t cnt_i=0;
    static 	uint32_t cnt=0;
    char debug[16];
    uint32_t same_flag=0;
    uint32_t zero_flag=0;
    uint8_t ch=0;
    float speed=0;
    float fault_frequency=0;//转频
    float fft_max=0;
    float fft_hz=0;
    float begin=0;
    float end=0;
			
    ch=tz_polygon->polygon_save->channel_id;
    speed=get_polygon_diag_speed1(&tz_polygon->speed_save);//获得平均速度
    tz_polygon->polygon_save->speed_max=speed;
    tz_polygon->polygon_save->speed_min=speed;
    //sprintf(debug_buff, "%f", speed);
    POLYGON_PRINT("speed:%d\n",(int)speed);
    fault_frequency=speed/3.6f / PI / polygon_para.wheel_diameter;  //轮对转频
    begin=15*8192/3000;
    end=40*8192/3000;
    float_sub_mean(acc_buff,acc_buff_size);
    filter(acc_buff, acc_buff_size);
    tz_polygon->polygon_save->TZ2_scalar = float_std_rms(acc_buff, acc_buff_size);
    fft_polygon(acc_buff,8192);
    polygon_get_fft_max(acc_buff,begin, end, fault_frequency, &fft_max, &fft_hz);
    sprintf(debug, "%f", fft_max);
    POLYGON_PRINT("fft_max=%s\n",debug);
    sprintf(debug, "%f", fft_hz);
    POLYGON_PRINT("fft_hz=%s\n",debug);
    order_5[ch][cnt_i]=(int)(fft_hz/fault_frequency+0.5);
    tz_polygon->polygon_save->TZ1_scalar=order_5[ch][cnt_i];
    tz_polygon->polygon_save->TZ3_scalar=round(0.5*(20*(log10(fft_max/0.01)+log10(tz_polygon->polygon_save->TZ2_scalar/0.01))));
    if(tz_polygon->polygon_save->TZ3_scalar<0)
    {
    tz_polygon->polygon_save->TZ3_scalar=0;
    }
    sprintf(debug, "%f", tz_polygon->polygon_save->TZ3_scalar);
    POLYGON_PRINT("TZ3=%s\n",debug);
    for (cnt=0;cnt<5;cnt++)
    {
        if(order_5[ch][cnt]!=order_5[ch][cnt_i])
        {
                same_flag++;
        }
        if(order_5[ch][cnt]==0)
        {
            zero_flag++;
        }
        //rt_kprintf("order_5=%d\n",order_5[cnt]);
    }

#ifdef DEV_ALARM_EN
    if(same_flag==0&&zero_flag==0)
    {
        if(tz_polygon->polygon_save->TZ3_scalar>27)
        {
        tz_polygon->polygon_save->diag_res = ACC_ALARM_SECOND_GRADE;
        }
        else if((tz_polygon->polygon_save->TZ3_scalar<=27)&&(tz_polygon->polygon_save->TZ3_scalar>22))
        {
            tz_polygon->polygon_save->diag_res =ACC_ALARM_FIRST_GRADE;
        }
        else
        {
        tz_polygon->polygon_save->diag_res =RUNNING_OK;
        }
    }
    else
    {
        tz_polygon->polygon_save->diag_res =RUNNING_OK;
    }
#endif

    cnt_i++;
		
    if(cnt_i>4)
    {
        cnt_i=0;
        same_flag=0;
        zero_flag=0;
    }



  		
}


/**
*多边形数据诊断线程
*
*/

static void diagnosis_polygon_thread_entry(void *paramter)
{
    rt_uint32_t e;
    uint32_t cnt;
    char debug_buff[32];
    int switch_status = -1;
    uint8_t *polygon_send_buff = NULL;
	struct POLYGON_CALC tz_polygon;  
    init_polygon_diag(&tz_polygon);
    static uint8_t currest_deal_ch=0;

#if 1
    uint16_t bear_max,bear_min;
    uint16_t bear_abs[16];
    uint8_t alarm2_cnt[16]={0,0,0,0,0,0,0,0};
    uint8_t alarm1_cnt[16]={0,0,0,0,0,0,0,0};
    uint8_t tm1_cnt[16]={0,0,0,0,0,0,0,0};
    uint8_t tm2_cnt[16]={0,0,0,0,0,0,0,0};
    int dbx_res,cj_res;
    uint8_t curr_ch;
	uint16_t *testbuf=RT_NULL;
	testbuf= (uint16_t *)rt_malloc(9000*2);
#define FIRST_TM_MIN 850
#define FIRST_TM_MAX 1450
#define SECOND_TM_MIN 2850
#define SECOND_TM_MAX 3450
#define FIRST_BREAR_MIN 1950
#define FIRST_BREAR_MAX 2550
#define SECOND_BREAR_MIN 3550
#define SECOND_BREAR_MAX 4000
#endif


#ifdef CLB_TEST
	uint32_t outc_5k_next_ch=0;
    uint32_t out34_next_ch=0;
    while (1)
    {		
		if(rt_event_recv(&switchs.out34_event,(1 << 3), Event_Option, RT_WAITING_FOREVER, &e)== RT_EOK)  //
		{								
			int data_cnt,write_res;
			char test_buff[16];	
		    if(Get_Sdio_Stat() != SDIO_RUN_ING )
		        continue ;
			
			for(data_cnt = 0;data_cnt < 9000;data_cnt ++)
			{				
				sprintf(test_buff,"%f",(float)out5k_t[0]->buffer[data_cnt]);
				test_buff[5] = '\n';
				write(vibr_file.test1.fd,test_buff,6);				
				memset(test_buff,0,sizeof(test_buff));					
			}
			rt_kprintf("outC1:%d\n",data_cnt);
			
			sync(vibr_file.test1.fd);//
			for(data_cnt = 0;data_cnt < 9000;data_cnt ++)
			{				
				sprintf(test_buff,"%f",(float)out5k_t[1]->buffer[data_cnt]);
				test_buff[5] = '\n';
				write(vibr_file.test2.fd,test_buff,6);				
				memset(test_buff,0,sizeof(test_buff));					
			}
			
			rt_kprintf("outC2:%d\n",data_cnt);
			sync(vibr_file.test2.fd);
			rt_memset(out5k_t[0]->buffer,0,18000);
			rt_memset(out5k_t[1]->buffer,0,18000);
			
			continue ;
		}																	
	}
#endif
		
	while (1)
    {	
        rt_thread_mdelay(1000);//500ms
        currest_deal_ch++;
        if(currest_deal_ch>=17)
            currest_deal_ch=1;
		
		//rt_kprintf("currest_deal_ch--------------------->%d \n",currest_deal_ch);
        if(!wr_ops[OUT5K_OPS][currest_deal_ch-1].write_ops_flag)
        {
			if(!wr_ops[OUT5K_OPS][currest_deal_ch-1].acc_valid_flag)
				continue;
			
			rt_memmove(tz_polygon.polygon_acc,out5k_t[currest_deal_ch-1]->buffer,FS_ICP_VIBRA*2);
			rt_memmove(&tz_polygon.polygon_save->pub_info, &out5k_t[currest_deal_ch-1]->info,sizeof( struct AD_PUBLIC_INFO));				
			wr_ops[OUT5K_OPS][currest_deal_ch-1].acc_valid_flag=0;	

            tz_polygon.polygon_save->channel_id = currest_deal_ch-1;
            tz_polygon.polygon_save->pub_info.train_id = clb_config_para.train_net_addr.train_id;
            if (tz_polygon.polygon_save->channel_id < 4)
            {
                tz_polygon.polygon_save->pub_info.proc_id = 1;
            }
            else if(tz_polygon.polygon_save->channel_id < 8)
            {
                tz_polygon.polygon_save->pub_info.proc_id = 2;
            }
            else if(tz_polygon.polygon_save->channel_id < 12)
            {
                tz_polygon.polygon_save->pub_info.proc_id = 3;
            }
            else
                tz_polygon.polygon_save->pub_info.proc_id = 4;

            //获取轴承自检状态
            tz_polygon.polygon_save->self_test = (ad_acc_polygon.self_test.bytes & (1 << tz_polygon.polygon_save->channel_id))>>(tz_polygon.polygon_save->channel_id);

            POLYGON_PRINT("<diag_polygon_ch%d>\r\n", tz_polygon.polygon_save->channel_id);

#if 0

            bear_max=0;
            bear_min=0;
            
            memmove(testbuf,tz_polygon.polygon_acc,9000*2);
            curr_ch= tz_polygon.polygon_save->channel_id;
            //填充加速度缓存
            for (cnt = 2000; cnt < 9000; cnt++)
            {

                if(bear_max<testbuf[cnt])
                    bear_max=testbuf[cnt];

                if(cnt==2000)
                    bear_min=testbuf[cnt];
                else if(bear_min>testbuf[cnt])
                {
                    bear_min=testbuf[cnt];
                }
            }

            bear_abs[curr_ch]=bear_max-bear_min;
            POLYGON_PRINT("bear_max-min:%d-%d=%d\n",bear_max,bear_min,bear_abs[curr_ch]);

            if((bear_abs[curr_ch]>SECOND_BREAR_MIN)&&(bear_abs[curr_ch]<SECOND_BREAR_MAX))
            {

                if(bear_min<39500)
                {
                    alarm2_cnt[curr_ch]++;
                    if(alarm2_cnt[curr_ch]>=2)
                        cj_res = ACC_ALARM_SECOND_GRADE;
                }

            }
            else if((bear_abs[curr_ch]>FIRST_BREAR_MIN)&&(bear_abs[curr_ch]<FIRST_BREAR_MAX))
            {
                if(bear_min<39500)
                {
                    alarm1_cnt[curr_ch]++;
                    if(alarm1_cnt[curr_ch]>=2)
                        cj_res = ACC_ALARM_FIRST_GRADE;
                }
            }
            else
            {
                alarm2_cnt[curr_ch]=0;
                alarm1_cnt[curr_ch]=0;
                cj_res = RUNNING_OK;
            }

            update_mvb_bearing_diag_status(curr_ch,cj_res);

            if (cj_res == RUNNING_OK)
            {
                POLYGON_PRINT("bear: ch%d diag ok...\n", curr_ch);
            }
            else if (cj_res == ACC_ALARM_FIRST_GRADE)
            {
                POLYGON_PRINT("bear: ch%d diag acc_alarm_first_grade!!!!!\n", curr_ch);
            }
            else
            {
                POLYGON_PRINT("bear: ch%d diag acc_alarm_second_grade!!!!!!!!\n", curr_ch);
            }

            if((bear_abs[curr_ch]>SECOND_TM_MIN)&&(bear_abs[curr_ch]<SECOND_TM_MAX))
            {

                if(bear_min<39500)
                {
                    tm2_cnt[curr_ch]++;
                    if(tm2_cnt[curr_ch]>=2)
                        dbx_res = ACC_ALARM_SECOND_GRADE;
                }

            }
            else if((bear_abs[curr_ch]>FIRST_TM_MIN)&&(bear_abs[curr_ch]<FIRST_TM_MAX))
            {
                if(bear_min<39500)
                {
                    tm1_cnt[curr_ch]++;
                    if(tm1_cnt[curr_ch]>=2)
                        dbx_res = ACC_ALARM_FIRST_GRADE;
                }
            }
            else
            {
                tm1_cnt[curr_ch]=0;
                tm2_cnt[curr_ch]=0;
                dbx_res = RUNNING_OK;
            }


            updata_tread_mvb_data(curr_ch,dbx_res);

            if (dbx_res == RUNNING_OK)
            {
                POLYGON_PRINT("polygon: ch%d diag ok...\n", curr_ch);
            }
            else if (dbx_res == ACC_ALARM_FIRST_GRADE)
            {
                POLYGON_PRINT("polygon: ch%d diag acc_alarm_first_grade!!!!!\n", curr_ch);
            }
            else
            {
                POLYGON_PRINT("polygon: ch%d diag acc_alarm_second_grade!!!!!!!!\n", curr_ch);
            }

#else
            if(check_bearing_box_channel(tz_polygon.polygon_save->channel_id))
            {
                //填充加速度缓存
                for (cnt = 0; cnt < tz_polygon.acc_buff_len; cnt++)
                {
                    tz_polygon.acc_buff[cnt] = convert_ad_polygon(tz_polygon.polygon_acc[cnt]);
                }
				//POLYGON_PRINT("diag_polygon-----------1\r\n");
				//diag_polygon(tz_polygon.acc_buff, tz_polygon.acc_buff_len,clb_config_para.polygon_diag_config_para,&tz_polygon);
				//rt_kprintf("diag_polygon-----------end\r\n");
                updata_tread_mvb_data(tz_polygon.polygon_save->channel_id, tz_polygon.polygon_save->diag_res);

                if (tz_polygon.polygon_save->diag_res == RUNNING_OK)
                {
                    POLYGON_PRINT("polygon:ch%d diag ok\n", tz_polygon.polygon_save->channel_id);
                }
                else if (tz_polygon.polygon_save->diag_res == ACC_ALARM_FIRST_GRADE)
                {
                    POLYGON_PRINT("polygon:ch%d diag acc alarm first grade!!!!!\n", tz_polygon.polygon_save->channel_id);
                }
                else
                {
                    POLYGON_PRINT("polygon:ch%d diag acc alarm second grade!!!!!!!!\n", tz_polygon.polygon_save->channel_id);
                }
            }
            else
            {
                rt_thread_delay(300);
            }
#endif

            if (!polygon_send_buff)
            {
                uint32_t send_len;
                send_len = FS_ICP_VIBRA * 2 + sizeof(struct POLYGON_SAVE) + sizeof(struct SPEED_SAVE);
                polygon_send_buff = (uint8_t *)rt_malloc(send_len);
                if (polygon_send_buff)
                {
                    //发送多边形特征数据
                    memmove(polygon_send_buff, tz_polygon.polygon_save, sizeof(struct POLYGON_SAVE));
                    if(check_bearing_box_channel(tz_polygon.polygon_save->channel_id))
                        net_send(clb_config_para.train_net_addr.train_id, 1, POLYGON_TZ_DATA_TYPE, polygon_send_buff, sizeof(struct POLYGON_SAVE),0);

                    //发送多边形特征数据+原始数据+速度原始数据
                    memmove(&polygon_send_buff[sizeof(struct POLYGON_SAVE)], tz_polygon.polygon_acc, FS_ICP_VIBRA * 2);
                    memmove(&polygon_send_buff[sizeof(struct POLYGON_SAVE) + FS_ICP_VIBRA* 2], &tz_polygon.speed_save, sizeof(struct SPEED_SAVE));
                    net_send(clb_config_para.train_net_addr.train_id, 1, POLYGON_RAW_DATA_TYPE, &polygon_send_buff[0], send_len,0);
										                   
                    rt_free(polygon_send_buff);
                    polygon_send_buff = NULL;
                }
            }
			memset(tz_polygon.polygon_save, 0, sizeof(struct POLYGON_SAVE));
            memset(&tz_polygon.speed_save, 0, sizeof(struct SPEED_SAVE));
			
        }
    }
}

/**
 * 多边形AD转加速度公式
*/
static inline float convert_ad_polygon(unsigned short ad)
{
    return (ad - 32768) / 65536.0 * 20 / clb_config_para.polygon_diag_config_para.polygon_acc_sensitivity/0.416;//电路放大0.416倍;
}

/*
*更新发送到MVB的踏面故障
*/
void updata_tread_mvb_data(uint8_t ch, int diag_res)
{
    uint8_t first_alarm, second_alarm = 0;
    if (diag_res == 0)
    {
        first_alarm = 0;
        second_alarm = 0;
    }
    else if (diag_res == 3)
    {
        first_alarm = 1;
        second_alarm = 0;
    }
    else if (diag_res == 4)
    {
        first_alarm = 0;
        second_alarm = 1;
    }


    if (ch == 0)
    {
        send_mvb_data.sensor_alarm_data.bits.side1_shaft_end_tread_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side1_shaft_end_tread_second_alarm = second_alarm;
    }
    else if (ch == 1)
    {
        send_mvb_data.sensor_alarm_data.bits.side2_shaft_end_tread_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side2_shaft_end_tread_second_alarm = second_alarm;
    }
    else if (ch == 2)
    {
        send_mvb_data.sensor_alarm_data.bits.side3_shaft_end_tread_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side3_shaft_end_tread_second_alarm = second_alarm;
    }
    else if (ch ==3)
    {
        send_mvb_data.sensor_alarm_data.bits.side4_shaft_end_tread_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side4_shaft_end_tread_second_alarm = second_alarm;
    }
    else if (ch == 4)
    {
        send_mvb_data.sensor_alarm_data.bits.side5_shaft_end_tread_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side5_shaft_end_tread_second_alarm = second_alarm;
    }
    else if (ch == 5)
    {
        send_mvb_data.sensor_alarm_data.bits.side6_shaft_end_tread_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side6_shaft_end_tread_second_alarm = second_alarm;
    }
    else if (ch == 6)
    {
        send_mvb_data.sensor_alarm_data.bits.side7_shaft_end_tread_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side7_shaft_end_tread_second_alarm = second_alarm;
    }
    else if (ch == 7)
    {
        send_mvb_data.sensor_alarm_data.bits.side8_shaft_end_tread_first_alarm = first_alarm;
        send_mvb_data.sensor_alarm_data.bits.side8_shaft_end_tread_second_alarm = second_alarm;
    }
}

/*polygon 线程*/
int diagnosis_polygon_thread_init(char *arg)
{
    rt_err_t result;
	

    /*自检线程*/
    result = rt_thread_init(&diagnosis_polygon_thread,"diag_polygon",
                                diagnosis_polygon_thread_entry, RT_NULL,
                                diagnosis_polygon_thread_stack, 1024*6, 25, 100);
    if (result == RT_EOK)
        rt_thread_startup(&diagnosis_polygon_thread);
    else
        rt_kprintf("sec_thread_init err\n");

    return result;
}

/** @} */ //\defgroup module API

/** @} */ //\addtogroup dev module

#endif //\end module API
#endif
/* !< end file   */
