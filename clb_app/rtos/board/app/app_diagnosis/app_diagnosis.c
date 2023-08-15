/**
 * @file
 * @details  
 * @author   
 * @date     
	诊断逻辑：1） 通过构造函数 app_diag_raw_data_node_construct() 获取到原始数据到链表，数据使用完成必须删除节点
			   2)  通过函数 app_diag_raw_to_acc() 将原始数据转换为加速度
			   3)  通过函数 app_diag_wheel_tread() 获取踏面诊断的结果，使用轴箱轴承冲击数据
			   4)  轴箱轴承的诊断结果，使用轴箱轴承的冲击数据，使用步骤3）的结果
			   5)  通过函数 app_diag_motor_bearing() 获取齿端电机轴承诊断结果，使用电机冲击数据
			   6)  通过函数 app_diag_motor_gear() 获取齿端电机齿轮诊断结果(根据相关的沟通，没有找到实现的依据，算法中心建议不关心该项点)
			   7） 通过函数 app_diag_wheel_bearing() 获取小齿轮箱车轮侧轴承诊断结果，使用齿轮冲击数据
			   8)  通过函数 app_diag_wheel_gear() 获取小齿轮箱车轮侧齿轮诊断结果，使用齿轮振动数据
			   9)  通过函数 app_diag_polygon() 是获取多边形诊断结果，使用轴箱轴承振动数据(在脱轨处理板处理，可不用关心)
			   10) 通过函数 app_diag_motor_vibrate() 该函数直接设置电机振动数据为删除状态（必须保留该函数）
			   11）通过函数 app_diag_state_out() 将诊断的结果，判断是否符合预警、报警，然后输出出去
			   12）通过函数 app_diag_result_save() 将诊断的结果保存
			   13）通过函数 app_diag_data_node_remove() 将使用完成的节点删除
 * @version  
**/

/* include */
#include "switch_app.h"
#include "time.h"
#include "app_test_data.h"
#include "../lh/itc/itc.h"
#include "fft_diag.h"
#include "initial_pro.h"
#include "ZXZC_Wheel_diag.h"#include "CLX_diag.h"#include "DJ_diag.h"#include "gear_diag.h"#include "polygon_diag.h"#include "corrugation_diag.h"#include "IIR_polygon.h"
#include "app_diagnosis.h"
#include "app_data_save.h"
#include "app_paras_save.h"
#include "config.h"
#include "txb_public_protocol.h"
#include "app_channel_switch.h"
#include "app_ad7606_data.h"

/* macro */
#define DIAG_WIN_LENS    3  //保存的秒数
#define DIAG_AXLE_NUM    8  //轴箱通道数
#define DIAG_MOTOR_BEAR_NUM   4  //电机与齿轮通道数

#define DIAG_DEBUG //算法诊断结果打印开关

/* type declaration */
typedef struct
{
	FILE      *f_hd;
	time_t    timer;
	struct tm *time_p, time_old, time_new;
	uint8_t   tread_status_out;
	uint8_t   zxzc_status_out;
	uint8_t   dj_zc_status_out;
	uint8_t   clx_zc_status_out;
	uint8_t   clx_cl_status_out;
	app_diag_save_head_t diag_save_head;
	zxzc_stack_t zxzc_stack[8];
	dj_clx_stack_t dj_zc_stack[4];
	dj_clx_stack_t clx_zc_stack[4];
	int clx_cl_stack[4][3];//4个齿轮数据通道，每个通道需要3s的stack
	polygon_diag_stack_t polygon_stack[8];
}app_diag_local_t;

/* variable declaration */
bool wts_tz_flag = 0;
static uint8_t diag_test_g = 0;
static diag_data_node_t* diag_data_node_buff[APP_DIAG_AD7606_CH_MAX];

app_diag_env_t* app_diag_env = NULL;
#define env app_diag_env
app_diag_local_t app_diag_local;
#define local app_diag_local
diag_alarm_data_t alarm_data_buff;
#define alarm alarm_data_buff
int host_flag_g;

extern txb_MVB_public_t app_save_public; //该结构体保存公共信息，随特征数据一同存储
/* function declaration */
/**
 * @brief
 * @param
 * @return
 * @note
**/
void print_float(float data, char ch)
{
	if(data>=6553.6||data<=-6553.6)
	{
		rt_kprintf(" %d",(int)data);
	}
	else
	{
		rt_kprintf(" %d.%d%d%d",
		(int)(data),
		(int)(data * 10) % 10,
		(int)(data * 100) % 10,
		(int)(data * 1000) % 10);
	}
	rt_kprintf("%c",ch);
}

/**
 * @brief  获取存放wts数据的buffer地址
 * @param
 * @return
 * @note
**/
wts_data_t* app_diag_get_wts_buff(void)
{
	return &env->wts_data;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
diag_alarm_data_p app_diag_get_buff(void)
{
	return &alarm_data_buff;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_diag_raw_data_node_construct(void)
{
	diag_data_node_t *diag_data_node = OS_NULL;
	struct tm *time_p;
	time_t timer;

	timer = time(&timer);
	time_p = localtime(&timer);
	app_select_num_p switch_channel = app_channel_adg1608_get_select();
	for(uint8_t i = 0; i < DEF_CLB_DATA_CH_MAX; i++)
	{
//		diag_data_node = (diag_data_node_t *)rt_malloc(sizeof(diag_data_node_t));
//		if(diag_data_node == OS_NULL)
//		{
//			rt_kprintf("diag_raw_data_node malloc failed\n");
//			rt_kprintf("reboot clb ...\n");
////			save_sys_para();
//			rt_hw_cpu_reset();//reset clb
//			continue ;
//		}
//		rt_kprintf("molloc node buff %d, \n", i);
		memset(diag_data_node_buff[i], 0, sizeof(diag_data_node_t));
		diag_data_node = diag_data_node_buff[i];
		//速度生成
		uint8_t tmp_h = app_save_public.speed[0];
		uint8_t tmp_l = app_save_public.speed[1];
		diag_data_node->speed = ((tmp_h << 8) | tmp_l) / 100;
		//轮径生成
		tmp_h = app_save_public.wheel[(env->train_id-1)*2];
		tmp_l = app_save_public.wheel[(env->train_id-1)*2+1];
		diag_data_node->wheel = (float)((tmp_h << 8) | tmp_l) / 1000;
		if (diag_test_g == 1)
		{
			diag_data_node->speed = 100;
			diag_data_node->wheel = POLYGON_wheel_diameter;
        }

//		rt_kprintf("construct node %p, speed %d\n", diag_data_node, diag_data_node->speed);
		rt_memcpy(&diag_data_node->time, time_p, sizeof(struct tm));
		diag_data_node->acc_state = false;
		diag_data_node->diag_state = false;
//		diag_data_node->out_state = false;
		diag_data_node->remove_state = false;
		diag_data_node->raw_data = env->raw_data[i].raw_buff;

//		if(i == 0)
		switch(i)
		{
			case AD7606_V1:
			{
				//if (!env->host_flag ||)
				{
					diag_data_node->channel = switch_channel->save_motor_gear_num;
					if(diag_data_node->channel % 2) {
						diag_data_node->ad_data_type = gear_impact;  //齿轮冲击
					}
					else {
						diag_data_node->ad_data_type = motor_impact; //电机冲击
					}
					diag_data_node->active_flag = true;
				}
				break;
			}
			case AD7606_V2:
			{
				//if (env->train_id!=APP_BOARD_TRAIN_HEAD && env->train_id!=APP_BOARD_TRAIN_TAIL)
				{
					diag_data_node->channel = switch_channel->save_motor_gear_num;
					if(diag_data_node->channel % 2) {
						diag_data_node->ad_data_type = gear_vibration; //齿轮震动 4096
						diag_data_node->active_flag = true;
					}
					else {
						diag_data_node->ad_data_type = motor_vibration; //电机震动 未使用
					}
				}
				break;
			}
			case AD7606_V3:
			{
				diag_data_node->ad_data_type = bearing_impact;  //轴承冲击
				diag_data_node->channel = switch_channel->save_axle_box_num;
				diag_data_node->active_flag = true;
				break;
			}
			case AD7606_V4:// z7、xa10有脱轨板处理，处理板不诊断
			{
				diag_data_node->ad_data_type = bearing_vibration; //轴承震动，多边形使用，在脱轨板，处理板不使用
				diag_data_node->channel = switch_channel->save_axle_box_num;
				//diag_data_node->active_flag = true;
				break;
			}
			case AD7606_V5:// 脱轨板处理，处理板不诊断
			{
				diag_data_node->ad_data_type = bearing_vibration; //轴承震动，多边形使用
				diag_data_node->channel = switch_channel->save_wave_num*2-1;
				break;
			}
			case AD7606_V6:// 脱轨板处理，处理板不诊断
			{
				diag_data_node->ad_data_type = bearing_vibration; //轴承震动，多边形使用
				diag_data_node->channel = switch_channel->save_wave_num*2;
				break;
			}
			default:
				break;
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   原始数据转换为加速度   (20mv/g)加速度 = (((AD-32768)/65536)*20/0.02)/1.25(g)
**/
static void app_diag_raw_to_acc(void)
{
	diag_data_node_t *diag_data_node = OS_NULL;
	int j;
	
//	rt_list_for_each_entry(diag_data_node, &env->diag_list, list)
	for(int i=0; i<APP_DIAG_AD7606_CH_MAX; i++)
	{
		diag_data_node = diag_data_node_buff[i];
		if (diag_data_node->active_flag && (diag_data_node->acc_state == false))
		{
			for(uint16_t i = 0; i < APP_DIAG_SAMPLING_SIZE_MAX; i++)
			{
				diag_data_node->acc_data[i] = ((diag_data_node->raw_data[i]-32768)/65536.0*20/0.02)/1.25;
			}
//	    	rt_kprintf(" channel: %d  diag data type:    %d------acc:------------\n",diag_data_node->channel,diag_data_node->ad_data_type);

			if (diag_test_g == 1)
			{
			    if (diag_data_node->ad_data_type == bearing_impact && diag_data_node->channel==3)
			    {
					for (j=0; j<APP_DIAG_SAMPLING_SIZE_MAX; j++)
					{
						diag_data_node->acc_data[j] = prewarning0[j];
					}
			    }
			}
			diag_data_node->acc_state = true;
		}
	}
}

/**
 * @brief  踏面与轴箱轴承诊断
 * @param
 * @return
 * @note   轴端踏面诊断,使用轴箱轴承冲击数据
**/
static void app_diag_wheel_tread(void)
{
	diag_data_node_t *diag_data_node;
//	struct FFT_DIAG_SAVE fft_save = {0};
	struct FFT_DIAG_PARA fft_diag_paras = {0};
	ZXZC_status_out_t* status_bearing_wheel = NULL;
//	int i;
//	rt_list_for_each_entry(diag_data_node, &env->diag_list, list)
	for(int i=0; i<APP_DIAG_AD7606_CH_MAX; i++)
	{
		diag_data_node = diag_data_node_buff[i];
		if(diag_data_node->active_flag && (diag_data_node->ad_data_type == bearing_impact) && (diag_data_node->acc_state == true) && (diag_data_node->diag_state == false))
		{
			//必须要先调用fft_diag_ZXZC_Wheel_init()函数对fft_diag_env->fft_diag_paras参数初始化
			fft_diag_paras.Sample_rate = APP_DIAG_RATE;
			fft_diag_paras.diag_speed = diag_data_node->speed;
			fft_diag_paras.wheel_diameter = diag_data_node->wheel;
			fft_diag_ZXZC_Wheel_init(&fft_diag_paras);
			status_bearing_wheel=ZXZC_Wheel_diag(diag_data_node->acc_data,
					                           fft_diag_paras,
					                           &diag_data_node->diag_result,
					                           &local.zxzc_stack[diag_data_node->channel-1]);
			local.zxzc_status_out = status_bearing_wheel->status_bearing_comprehensive;			local.tread_status_out = status_bearing_wheel->status_Wheel;
#ifdef DIAG_DEBUG
			if(!app_paras_save_env.clb_ckdy)
			{
				rt_kprintf("==> ZXZC_%d: n=%d   w=%d  g=%d  b=%d  tm=%d\n",diag_data_node->channel,diag_data_node->diag_result.DB[0],
						diag_data_node->diag_result.DB[1],diag_data_node->diag_result.DB[2],diag_data_node->diag_result.DB[3],
								   diag_data_node->diag_result.DB[4]);
				rt_kprintf("==> ZXZC_%d__result: zxzc = %d tm = %d\n",diag_data_node->channel,
						local.zxzc_status_out,local.tread_status_out);
			}
#endif
			diag_data_node->diag_state = true;
			//踏面报警结果填充
			switch(local.tread_status_out)
			{
				case 0://正常
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_warning = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_1_alarm = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_2_alarm = 0;
					break;
				case 1://预警，算法中为预判
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_warning = 1;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_1_alarm = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_2_alarm = 0;
					break;
				case 2://一级报警，算法中为预警
				    alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_warning = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_1_alarm = 1;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_2_alarm = 0;
					break;
				case 3://二级报警，算法中为报警
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_warning = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_1_alarm = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].tread_2_alarm = 1;
					break;
			}
			//轴箱报警轴承结果填充
			switch(local.zxzc_status_out)
			{
				case 0://正常
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_warning = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_1_alarm = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_2_alarm = 0;
					break;
				case 1://预警，算法中为预判
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_warning = 1;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_1_alarm = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_2_alarm = 0;
					break;
				case 2://一级报警，算法中为预警
				    alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_warning = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_1_alarm = 1;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_2_alarm = 0;
					break;
				case 3://二级报警，算法中为报警
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_warning = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_1_alarm = 0;
					alarm_data_buff.bearing_alarm[diag_data_node->channel-1].bearing_2_alarm = 1;
					break;
			}
//			for (int t=0; t<8; t++)
//			    rt_kprintf(" %02x", alarm_data_buff.bearing_alarm[t]);
//			rt_kprintf("\n");
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   齿端电机轴承诊断结果，使用电机冲击数据
**/
static void app_diag_motor_bearing(void)
{
	diag_data_node_t *diag_data_node;
//	struct FFT_DIAG_SAVE fft_save;
	struct FFT_DIAG_PARA fft_diag_paras = {0};

//	rt_list_for_each_entry(diag_data_node, &env->diag_list, list)
	for(int i=0; i<APP_DIAG_AD7606_CH_MAX; i++)
	{
		diag_data_node = diag_data_node_buff[i];

		if(diag_data_node->active_flag && (diag_data_node->ad_data_type == motor_impact) && (diag_data_node->diag_state == false))
		{

			//必须要先调用fft_diag_DJ_init()函数对fft_diag_env->fft_diag_paras参数初始化
			fft_diag_paras.Sample_rate = APP_DIAG_RATE;
			fft_diag_paras.diag_speed = diag_data_node->speed;
			fft_diag_paras.wheel_diameter = diag_data_node->wheel;
			fft_diag_DJ_init(&fft_diag_paras);
			//fft_diag()函数第一个参数为AD采集到的加速度数据，单位为G，第二个参数为数据长度为4096
			local.dj_zc_status_out = DJ_diag(diag_data_node->acc_data,
					                            fft_diag_paras,
					                            &diag_data_node->diag_result,
					                            &local.dj_zc_stack[diag_data_node->channel/2-1]);
			diag_data_node->diag_state = true;
#ifdef DIAG_DEBUG
			if(!app_paras_save_env.clb_ckdy)
			{
				rt_kprintf("==> DJZC_%d:  DB: n=%d   w=%d  g=%d  b=%d | n=%d   w=%d  g=%d  b=%d\n",diag_data_node->channel/2, diag_data_node->diag_result.DB[0],
						diag_data_node->diag_result.DB[1],diag_data_node->diag_result.DB[2],diag_data_node->diag_result.DB[3],
						diag_data_node->diag_result.DB[4],diag_data_node->diag_result.DB[5],diag_data_node->diag_result.DB[6],
						diag_data_node->diag_result.DB[7]);
				rt_kprintf("==> DJZC_%d__result: statue = %d\n",diag_data_node->channel/2, local.dj_zc_status_out);
			}
#endif

			//电机轴承报警结果填充
			switch(local.dj_zc_status_out)
			{
				case 0://正常
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_warning = 0;
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_1_alarm = 0;
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_2_alarm = 0;
					break;
				case 1://预警，算法中为预判
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_warning = 1;
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_1_alarm = 0;
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_2_alarm = 0;
					break;
				case 2://一级报警，算法中为预警
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_warning = 0;
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_1_alarm = 1;
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_2_alarm = 0;
					break;
				case 3://二级报警，算法中为报警
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_warning = 0;
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_1_alarm = 0;
					alarm_data_buff.gear_motor_alarm[(diag_data_node->channel-1)/2].bear_2_alarm = 1;
					break;
			}
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   齿端电机齿轮诊断结果(根据相关的沟通，没有找到实现的依据，算法中心建议不关心该项点)
**/
//static void app_diag_motor_gear(void)
//{
//	;
//}

/**
 * @brief
 * @param
 * @return
 * @note   小齿轮箱车轮侧轴承诊断结果，使用齿轮冲击数据
**/
static void app_diag_wheel_bearing(void)
{
	diag_data_node_t *diag_data_node;
//	struct FFT_DIAG_SAVE fft_save;
	struct FFT_DIAG_PARA fft_diag_paras = {0};

//	rt_list_for_each_entry(diag_data_node, &env->diag_list, list)
	for(int i=0; i<APP_DIAG_AD7606_CH_MAX; i++)
	{
		diag_data_node = diag_data_node_buff[i];
		if(diag_data_node->active_flag && (diag_data_node->ad_data_type == gear_impact) && (diag_data_node->acc_state == true) && (diag_data_node->diag_state == false))
		{
			//必须要先调用fft_diag_CLX_init()函数对fft_diag_env->fft_diag_paras参数初始化
			fft_diag_paras.Sample_rate = APP_DIAG_RATE;
			fft_diag_paras.diag_speed = diag_data_node->speed;
			fft_diag_paras.wheel_diameter = diag_data_node->wheel;
			fft_diag_CLX_init(&fft_diag_paras);
			//fft_diag()函数第一个参数为AD采集到的加速度数据，单位为G，第二个参数为数据长度为4096
			local.clx_zc_status_out = CLX_diag(diag_data_node->acc_data,
					                            fft_diag_paras,
					                            &diag_data_node->diag_result,
					                            &local.clx_zc_stack[diag_data_node->channel/2]);
			diag_data_node->diag_state = true;
#ifdef DIAG_DEBUG
			if(!app_paras_save_env.clb_ckdy)
			{
				rt_kprintf("==> CLXZC_%d: n=%d   w=%d  g=%d  b=%d | n=%d   w=%d  g=%d  b=%d\n",(diag_data_node->channel+1)/2,diag_data_node->diag_result.DB[0],
						diag_data_node->diag_result.DB[1],diag_data_node->diag_result.DB[2],diag_data_node->diag_result.DB[3],
						diag_data_node->diag_result.DB[4],diag_data_node->diag_result.DB[5],diag_data_node->diag_result.DB[6],
						diag_data_node->diag_result.DB[7]);
				rt_kprintf("==> CLXZC_%d__result: statue = %d\n",(diag_data_node->channel+1)/2,local.clx_zc_status_out);
			}
#endif

			//齿轮箱轴承报警结果填充
			switch(local.clx_zc_status_out)
			{
				case 0://正常
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_warning = 0;
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_1_alarm = 0;
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_2_alarm = 0;
					break;
				case 1://预警，算法中为预判
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_warning = 1;
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_1_alarm = 0;
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_2_alarm = 0;
					break;
				case 2://一级报警，算法中为预警
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_warning = 0;
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_1_alarm = 1;
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_2_alarm = 0;
					break;
				case 3://二级报警，算法中为报警
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_warning = 0;
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_1_alarm = 0;
					alarm_data_buff.gear_bear_alarm[(diag_data_node->channel-1)/2].bear_2_alarm = 1;
					break;
			}
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   小齿轮箱车轮侧齿轮诊断结果，使用齿轮振动数据     4096
**/
static void app_diag_wheel_gear(void)
{
	diag_data_node_t *diag_data_node;
//	struct FFT_DIAG_SAVE fft_save;
	struct FFT_DIAG_PARA fft_diag_paras = {0};

//	rt_list_for_each_entry(diag_data_node, &env->diag_list, list)
	for(int i=0; i<APP_DIAG_AD7606_CH_MAX; i++)
	{
		diag_data_node = diag_data_node_buff[i];
		if(diag_data_node->active_flag && (diag_data_node->ad_data_type == gear_vibration) && (diag_data_node->acc_state == true) && (diag_data_node->diag_state == false))
		{
			//必须要先调用fft_diag_gear_init()函数对fft_diag_env->fft_diag_paras参数初始化
			fft_diag_paras.Sample_rate = APP_DIAG_RATE;
			fft_diag_paras.diag_speed = diag_data_node->speed;
			fft_diag_paras.wheel_diameter = diag_data_node->wheel;
			fft_diag_gear_init(&fft_diag_paras);
			//fft_diag()函数第一个参数为AD采集到的加速度数据，单位为G，第二个参数为数据长度为4096
			local.clx_cl_status_out = gear_diag(diag_data_node->acc_data,
					                            fft_diag_paras,
					                            &diag_data_node->diag_result,
					                            local.clx_cl_stack[diag_data_node->channel/2]);
			diag_data_node->diag_state = true;
#ifdef DIAG_DEBUG
			if(!app_paras_save_env.clb_ckdy)
			{
				rt_kprintf("==> CL_%d: db = %d  speed=%d  wheel=%d\n",(diag_data_node->channel+1)/2,diag_data_node->diag_result.DB[0],diag_data_node->speed,(int)(diag_data_node->wheel*1000));
				rt_kprintf("==> CL_%d__result: clx_cl = %d\n",(diag_data_node->channel+1)/2,local.clx_cl_status_out);
			}
#endif
			//小齿轮箱侧齿轮报警结果填充
			switch(local.clx_cl_status_out)
			{
				case 0://正常
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_warning = 0;
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_1_alarm = 0;
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_2_alarm = 0;
					break;
				case 1://预警，算法中为预判
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_warning = 1;
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_1_alarm = 0;
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_2_alarm = 0;
					break;
				case 2://一级报警，算法中为预警
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_warning = 0;
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_1_alarm = 1;
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_2_alarm = 0;
					break;
				case 3://二级报警，算法中为报警
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_warning = 0;
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_1_alarm = 0;
					alarm_data_buff.gear_bear_alarm[diag_data_node->channel/2].gear_2_alarm = 1;
					break;
			}
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note   多边形诊断使用轴箱轴承振动数据
**/
static void app_diag_polygon(void)
{
	diag_data_node_t *diag_data_node;
//	struct DERAIL_CALC calc;
	struct POLYGON_DIAG_PARA polygon_para = {0};
	static float x[9] = {0};
	static float y[9] = {0};

	for(int i=0; i<APP_DIAG_AD7606_CH_MAX; i++)
	{
		diag_data_node = diag_data_node_buff[i];
		if(diag_data_node->active_flag && (diag_data_node->ad_data_type == bearing_vibration) && (diag_data_node->diag_state == false))
		{
			//多边形诊断参数配置
			polygon_para.wheel_diameter = diag_data_node->wheel;//单位 m    轮径，默认是0.805
			polygon_para.Sample_rate = APP_DIAG_RATE;			//采样频率
			polygon_para.polygon_diag_speed = diag_data_node->speed;   // km/h 车辆运行的速度
			polygon_para.threshold_prognosis = POLYGON_threshold_prognosis;
			polygon_para.threshold_prewarning = POLYGON_threshold_prewarning;
			polygon_para.threshold_warning = POLYGON_threshold_warning;
			polygon_para.time_length = POLYGON_time_length;

//			for(int t=0;t<DIAG_CHANNEL_NUM;t++) //能实现8个通道同时采集时，需要8个一起计算
			{
				uint8_t index_ch = diag_data_node->channel-1;
				memset(x, 0, sizeof(float)*9);
				memset(y, 0, sizeof(float)*9);

				for(int cnt_i = 0;cnt_i < APP_DIAG_RATE;cnt_i++)
				{
					//将采集数据转换为加速度并滤波后存到准备计算的buff中
					env->IIR_buf[cnt_i] =IIR_filter_polygon(diag_data_node->acc_data[cnt_i],x,y);
				}
				polygon_diag(env->IIR_buf,polygon_para,&diag_data_node->polygon_save,&local.polygon_stack[index_ch]);
				diag_data_node->diag_state = true;
#ifdef DIAG_DEBUG
				if(!app_paras_save_env.clb_ckdy)
				{
					rt_kprintf("==> DBX_%d: order=%d, rms=%d, DB=%d, status=%d, speed=%d\n",
							diag_data_node->channel,
							diag_data_node->polygon_save.order_result,(int)(diag_data_node->polygon_save.rms_result*100),
							(int)(diag_data_node->polygon_save.DB_result*100),diag_data_node->polygon_save.status_polygon,(int)diag_data_node->polygon_save.speed);
					rt_kprintf("polygon_para---> wheel=%d, speed=%d\n",
							(int)(polygon_para.wheel_diameter*1000),(int)polygon_para.polygon_diag_speed);
				}
#endif
			}
		}
	}


}

/**
 * @brief
 * @param
 * @return
 * @note   电机振动数据没有使用
**/
//static void app_diag_motor_vibrate(void)
//{
//	diag_data_node_t *diag_data_node;
//
//	rt_list_for_each_entry(diag_data_node, &env->diag_list, list)
//	{
//		if(diag_data_node->ad_data_type == motor_vibration)
//		{
//			diag_data_node->diag_state = true;
//		}
//	}
//}


/**
 * @brief
 * @param
 * @return
 * @note   保存温度数据
**/
void diag_fill_temperature(void)
{
//	extern int host_flag_g;
//	extern data_clb_to_record_t data_clb_to_record;
	extern app_send_record_env_t app_send_record_env;
	clb_record_common_data_t * data_clb_to_record = (clb_record_common_data_t*)&app_send_record_env.clb_record_data.data_inform;
	save_temp_t save_temp = {0};

	save_temp.env_temp_self_test = 0;

	//成19温度取值范围为0-255，实际温度偏移55，实际代表温度值为-55-200
	uint8_t* temp_buff_p = (uint8_t*)data_clb_to_record->bearing_temp;
	uint8_t* save_buff_p = (uint8_t*)save_temp.running_1s_temp.ZXZC_temp;
	uint8_t* diag_buff_p = (uint8_t*)save_temp.diag_res.ZXZC_temp;
	for (int i=0; i<PROC_CHANNEL_NUM; i++)
	{
		//数据保存时，按照 轴箱温度8个，电机温度4个，齿轮箱温度4个的顺序来保存
		save_buff_p[i] = temp_buff_p[i];
		if (save_buff_p[i]>=DEF_TEMP_WORNING && save_buff_p[i]<DEF_TEMP_ALARM)
		{
			diag_buff_p[i] = 1;
		}
		else if (save_buff_p[i] >= DEF_TEMP_ALARM)
		{
			diag_buff_p[i] = 2;
		}
		else
		{
			diag_buff_p[i] = 0;
		}
		//保存环境温度
		if (i < QZQ_TOTAL)
		{
			save_temp.running_env_temp[i] = data_clb_to_record->env_temp[i];
		}
	}
	//温差计算
	if (!env->host_flag)
	{//从机区分前置器数量，再分别对应
		for (int i=0; i<ZXZC_NUM; i++)
		{
			save_temp.diff_temp.ZXZC_temp[i] = save_temp.running_1s_temp.ZXZC_temp[i] - save_temp.running_env_temp[i/2];
			if (i < DJ_NUM)
			{
				save_temp.diff_temp.DJ_temp[i] = save_temp.running_1s_temp.DJ_temp[i] - save_temp.running_env_temp[i];
				save_temp.diff_temp.CLX_temp[i] = save_temp.running_1s_temp.CLX_temp[i] - save_temp.running_env_temp[i];
			}
		}
	}
	else
	{//主机区分前置器数量，再分别对应
		if (QZQ_HOST_NUM3 == env->host_qzq_num)//适用于郑许，成都19、西安10
		{
			for (int i=0; i<ZXZC_NUM; i++)
			{
				//轴箱轴承
				if (i < 4)
				{
					save_temp.diff_temp.ZXZC_temp[i] = save_temp.running_1s_temp.ZXZC_temp[i] - save_temp.running_env_temp[0];
				}
				else if (i < 6)
				{
					save_temp.diff_temp.ZXZC_temp[i] = save_temp.running_1s_temp.ZXZC_temp[i] - save_temp.running_env_temp[1];
				}
				else
				{
					save_temp.diff_temp.ZXZC_temp[i] = save_temp.running_1s_temp.ZXZC_temp[i] - save_temp.running_env_temp[2];
				}
				//电机与齿轮

				save_temp.diff_temp.DJ_temp[2] = save_temp.running_1s_temp.DJ_temp[2] - save_temp.running_env_temp[1];
				save_temp.diff_temp.CLX_temp[2] = save_temp.running_1s_temp.CLX_temp[2] - save_temp.running_env_temp[1];
				save_temp.diff_temp.DJ_temp[3] = save_temp.running_1s_temp.DJ_temp[3] - save_temp.running_env_temp[2];
				save_temp.diff_temp.CLX_temp[3] = save_temp.running_1s_temp.CLX_temp[3] - save_temp.running_env_temp[2];

			}
		}
		else if (QZQ_HOST_NUM2 == env->host_qzq_num)
		{
			for (int i=0; i<ZXZC_NUM; i++)//主机
			{
				if (i < 4)
				{
					save_temp.diff_temp.ZXZC_temp[i] = save_temp.running_1s_temp.ZXZC_temp[i] - save_temp.running_env_temp[0];
				}
				else
				{
					save_temp.diff_temp.ZXZC_temp[i] = save_temp.running_1s_temp.ZXZC_temp[i] - save_temp.running_env_temp[1];
				}
			}
		}
	}

	memcpy(&env->wts_data.temperature, &save_temp, sizeof(save_temp_t));
//	rt_kprintf("-----send  len_h  %x %x\n", env.wts_data.head.data_len_h,env.wts_data.head.data_len_l);
//	rt_kprintf("-----send  train_id:  %x\n", env.wts_data.head.train_id);
}
/**
 * @brief
 * @param
 * @return
 * @note   填充保存齿轮箱数据
**/
static void diag_fill_clx_zc(save_dj_clx_t* save_zc, diag_data_node_t* diag_data_node)
{
	int DB_result[Diag_Num_CLX];

	for(int i = 0; i < Diag_Num_CLX; i++)
	{
		//直接取整
		DB_result[i] = (int)diag_data_node->diag_result.DB[i]; //DB值
	}
	//填充齿轮箱轴承诊断数据
	save_zc->channel_id = diag_data_node->channel;
	save_zc->speed_valid_flag = 1;// xxx 默认速度有效
	//齿轮箱输入电机侧
	save_zc->bearing_tz1.amplitude_inner_ring = (uint8_t)DB_result[Diag_NH_OFFSET];
	save_zc->bearing_tz1.amplitude_outer_ring = (uint8_t)DB_result[Diag_WH_OFFSET];
	save_zc->bearing_tz1.amplitude_rolling_body = (uint8_t)DB_result[Diag_GDT_OFFSET];
	save_zc->bearing_tz1.amplitude_holder = (uint8_t)DB_result[Diag_BCJ_OFFSET];
	//齿轮箱输入车轮侧
	save_zc->bearing_tz2.amplitude_inner_ring = (uint8_t)DB_result[Diag_NH_OFFSET+Diag_Num_COMMON];
	save_zc->bearing_tz2.amplitude_outer_ring = (uint8_t)DB_result[Diag_WH_OFFSET+Diag_Num_COMMON];
	save_zc->bearing_tz2.amplitude_rolling_body = (uint8_t)DB_result[Diag_GDT_OFFSET+Diag_Num_COMMON];
	save_zc->bearing_tz2.amplitude_holder = (uint8_t)DB_result[Diag_BCJ_OFFSET+Diag_Num_COMMON];
	//报警状态
	save_zc->alarm_bearing = local.clx_zc_status_out;
	//发送报文信息填充
	memcpy(&env->wts_data.dj_clx_zc, save_zc, sizeof(save_dj_clx_t));

}
/**
 * @brief
 * @param
 * @return
 * @note   填充保存电机诊断数据
**/
static void diag_fill_dj(save_dj_clx_t* save_zc, diag_data_node_t* diag_data_node)
{

	int DB_result[Diag_Num_DJ];

	for(int i = 0; i < Diag_Num_DJ; i++)
	{
		//直接取整
		DB_result[i] = (int)diag_data_node->diag_result.DB[i]; //DB值
	}
	//填充齿轮箱轴承诊断数据
	save_zc->channel_id = diag_data_node->channel;
	save_zc->speed_valid_flag = 1;// xxx 默认速度有效
	//电机驱动端存储数据
	save_zc->bearing_tz1.amplitude_inner_ring = (uint8_t)DB_result[Diag_NH_OFFSET];
	save_zc->bearing_tz1.amplitude_outer_ring = (uint8_t)DB_result[Diag_WH_OFFSET];
	save_zc->bearing_tz1.amplitude_rolling_body = (uint8_t)DB_result[Diag_GDT_OFFSET];
	save_zc->bearing_tz1.amplitude_holder = (uint8_t)DB_result[Diag_BCJ_OFFSET];
	//电机非驱动端存储数据
	save_zc->bearing_tz2.amplitude_inner_ring = (uint8_t)DB_result[Diag_NH_OFFSET+Diag_Num_COMMON];
	save_zc->bearing_tz2.amplitude_outer_ring = (uint8_t)DB_result[Diag_WH_OFFSET+Diag_Num_COMMON];
	save_zc->bearing_tz2.amplitude_rolling_body = (uint8_t)DB_result[Diag_GDT_OFFSET+Diag_Num_COMMON];
	save_zc->bearing_tz2.amplitude_holder = (uint8_t)DB_result[Diag_BCJ_OFFSET+Diag_Num_COMMON];
	//报警状态

	save_zc->alarm_bearing = local.dj_zc_status_out;
	//发送报文信息填充
	memcpy(&env->wts_data.dj_clx_zc, save_zc, sizeof(save_dj_clx_t));
}
/**
 * @brief
 * @param
 * @return
 * @note   填充保存轴箱轴承诊断数据
**/
static void diag_fill_zxzc(save_zxzc_t* save_zc, diag_data_node_t* diag_data_node)
{

	int DB_result[Diag_Num_ZXZC_Wheel];

	for(int i = 0; i < Diag_Num_ZXZC_Wheel; i++)
	{
		//直接取整
		DB_result[i] = (int)diag_data_node->diag_result.DB[i]; //DB值
	}
	//填充齿轮箱轴承诊断数据
	save_zc->channel_id = diag_data_node->channel;
//	save_zc->speed_valid_flag = 1;// xxx 默认速度有效
	//轴箱轴承存储数据
	save_zc->bearing_tz.amplitude_inner_ring = (uint8_t)DB_result[Diag_NH_OFFSET];
	save_zc->bearing_tz.amplitude_outer_ring = (uint8_t)DB_result[Diag_WH_OFFSET];
	save_zc->bearing_tz.amplitude_rolling_body = (uint8_t)DB_result[Diag_GDT_OFFSET];
	save_zc->bearing_tz.amplitude_holder = (uint8_t)DB_result[Diag_BCJ_OFFSET];
	save_zc->tm_db = (uint8_t)DB_result[Diag_TM_PFFSET];
	//报警状态
	save_zc->alarm_bearing = local.zxzc_status_out;
	save_zc->alarm_tm = local.tread_status_out;
	//发送报文信息填充
	memcpy(&env->wts_data.zx_zc, save_zc, sizeof(save_zxzc_t));
}
/**
 * @brief
 * @param
 * @return
 * @note   填充保存踏面诊断数据
**/
//static void diag_fill_tm(save_tread_t* save_tm, diag_data_node_t* diag_data_node, save_temp_t* save_temp)
//{
//
////	int DB_result[Diag_Num_ZXZC_Wheel];
////
////	for(int i = 0; i < Diag_Num_ZXZC_Wheel; i++)
////	{
////		//直接取整
////		DB_result[i] = (int)diag_data_node->diag_result.DB[i]; //DB值
////	}
//	//填充齿轮箱轴承诊断数据
//	save_tm->channel_id = diag_data_node->channel;
////	save_tm->self_test = 0;
//	save_tm->speed_valid_flag = 1;// xxx 默认速度有效
////	save_tm->diag_speed_h = (diag_data_node->speed >> 8) & 0xff;
////	save_tm->diag_speed_h = diag_data_node->speed & 0xff;
//	save_tm->temp_env = save_temp->running_env_temp[0]; //xxx 使用前置1的环境温度
//	save_tm->amplitude_tread = local.tread_status_out;
//	//发送报文信息填充
//	memcpy(&env.wts_data.tm, save_tm, sizeof(save_tread_t));
//}
/**
 * @brief
 * @param
 * @return
 * @note   填充保存齿轮诊断数据（使用齿轮振动数据计算）
**/
static void diag_fill_cl(save_gear_t* save_cl, diag_data_node_t* diag_data_node)
{

	int DB_result;
	int amp_result;
	int fre_result;

	//直接取整
	DB_result = (int)(diag_data_node->diag_result.DB[0]*100); //DB值
	amp_result = (int)(diag_data_node->diag_result.amp[0]*100); //DB值
	fre_result = (int)(diag_data_node->diag_result.fre[0]*100); //DB值

	//填充齿轮箱轴承诊断数据
	save_cl->channel_id = diag_data_node->channel;
	save_cl->gear_DB_h = (DB_result>>8) & 0x000000ff;
	save_cl->gear_DB_l = DB_result & 0x000000ff;
	save_cl->gear_amp_h = ((amp_result*100)>>8) & 0x000000ff;
	save_cl->gear_amp_l = (amp_result*100) & 0x000000ff;
	save_cl->gear_fre_h = ((fre_result*100)>>8) & 0x000000ff;
	save_cl->gear_fre_l = (fre_result*100) & 0x000000ff;
	save_cl->alarm_status = local.clx_cl_status_out;
	//发送报文信息填充
	memcpy(&env->wts_data.cl, save_cl, sizeof(save_gear_t));
}
/**
 * @brief
 * @param
 * @return
 * @note   填充保存多边形数据
**/
static void diag_fill_dbx(polygon_tz_t* save_dbx, diag_data_node_t* diag_data_node)
{
	//填充多边形诊断数据
	save_dbx->channel_id = diag_data_node->channel;
	save_dbx->diag_res = diag_data_node->polygon_save.status_polygon & 0x000000ff;// 多边形状态
	save_dbx->TZ1_order_h = (diag_data_node->polygon_save.order_result>>8) & 0x000000ff;
	save_dbx->TZ1_order_l = diag_data_node->polygon_save.order_result & 0x000000ff;
	save_dbx->TZ2_rms_h = (int)(diag_data_node->polygon_save.rms_result*100) >> 8 & 0x000000ff;
	save_dbx->TZ2_rms_l = (int)(diag_data_node->polygon_save.rms_result*100) & 0x000000ff;
	save_dbx->TZ3_DB_h = (int)(diag_data_node->polygon_save.DB_result*100) >> 8 & 0x000000ff;
	save_dbx->TZ3_DB_l = (int)(diag_data_node->polygon_save.DB_result*100) & 0x000000ff;
	//发送报文信息填充
	memcpy(&env->wts_data.dbx, save_dbx, sizeof(polygon_tz_t));
}

/**
 * @brief
 * @param
 * @return
 * @note   诊断结果进行保存，每5分钟保存一次，浮点型数据放大1000倍存储，文件大小103K
**/
static void app_diag_result_save(void)
{
//	extern comm_pub_data_t comm_pub_data;
//	extern txb_MVB_public_t app_save_public; //该结构体保存公共信息，随原始数据一同存储

	diag_data_node_t *diag_data_node;

	env->wts_data.head[0] = 0x66;
	env->wts_data.head[1] = 0xdd;
	env->wts_data.len[0] = 0;
	env->wts_data.len[1] = sizeof(wts_data_t);
	env->wts_data.train_id = env->train_id;

	for(int i=0; i<APP_DIAG_AD7606_CH_MAX; i++)
	{
		diag_data_node = diag_data_node_buff[i];
		if(diag_data_node->active_flag && (diag_data_node->diag_state == true))
		{
			save_zxzc_t save_zc = {0};
			save_dj_clx_t save_dj_clx = {0}; //踏面数据
			save_gear_t save_cl = {0};
			polygon_tz_t save_dbx = {0}; //多边形数据

			//根据采集数据类型保存
			switch (diag_data_node->ad_data_type)
			{
				case gear_impact:  //齿轮冲击-app_diag_wheel_bearing()小齿轮箱车轮侧轴承诊断
					diag_fill_clx_zc(&save_dj_clx, diag_data_node);
					break;

				case motor_impact: //齿端电机轴承诊断结果，使用电机冲击数据
					diag_fill_dj(&save_dj_clx, diag_data_node);
					break;

				case gear_vibration: //小齿轮箱车轮侧齿轮诊断结果，使用齿轮振动数据
					diag_fill_cl(&save_cl, diag_data_node);
					break;

				case bearing_impact: //轴端踏面诊断使用轴箱轴承冲击数据(轴箱轴承)
					diag_fill_zxzc(&save_zc, diag_data_node);
					break;

				case bearing_vibration: //多边形诊断使用轴箱轴承振动数据
					diag_fill_dbx(&save_dbx, diag_data_node);
					break;
				default:
					break;
			}
			diag_data_node->remove_state = true;
		}
	}
	wts_tz_flag = 1;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_diag_data_node_remove(void)
{
	diag_data_node_t *diag_data_node;
//	rt_list_for_each_entry_safe(diag_data_node, next_diag_data_node, &env->diag_list, list)
	for(int i=0; i<APP_DIAG_AD7606_CH_MAX; i++)
	{
		diag_data_node = diag_data_node_buff[i];
//		rt_kprintf("remove--------  node:0x%p----  0x%p,0x%p,0x%p\n",diag_data_node, diag_data_node_buff[0],diag_data_node_buff[1],diag_data_node_buff[2]);
		if(diag_data_node->active_flag)
		{
			diag_data_node->active_flag = 0;
		}
	}
}

static void app_diag_thread_entry(void *params)
{
	extern int vib_save_flag;//用于标志当前采集原始数据是否存储完成，存完再开始算法处理
	uint8_t wait_cnt = 0;
	rt_thread_delay(5000);//增加延时，防止太快启动造成的死机问题
	while(1)
	{
		if(wait_cnt > 200)
		{
			rt_kprintf("-----error:  wait vib save >20s, reboot clb.....\n");
			rt_hw_cpu_reset();//reboot clb
		}
		if(diag_data_node_buff[2]->active_flag && vib_save_flag)
		{
			wait_cnt = 0;
			env->host_flag = app_paras_save_env.host_flag;
			env->host_qzq_num = app_paras_save_env.host_qzq_num;
			app_diag_raw_to_acc();
			app_diag_wheel_tread();
			app_diag_motor_bearing();
//			app_diag_motor_gear();//电机的齿轮不诊断
			app_diag_wheel_bearing();
			app_diag_wheel_gear();
//			app_diag_polygon();
//			app_diag_motor_vibrate();//电机振动不参与计算
			app_diag_result_save();
			app_diag_data_node_remove();
		}
		wait_cnt ++;
		rt_thread_delay(100);
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_diag_thread(void)
{
	env->diag_thread = rt_thread_create("diag_thread", app_diag_thread_entry, OS_NULL,
										  	     APP_DIAG_STACK, APP_DIAG_THREAD_PRO, 20);
    if(env->diag_thread != OS_NULL) {
        rt_thread_startup(env->diag_thread);
    }
    else {
        rt_kprintf("diag_thread create fail !\n");
    }
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_diag_init(void)
{
	paras_t *paras = app_paras_get();
	app_diag_env = (app_diag_env_t*)rt_malloc(sizeof(app_diag_env_t));
	if(app_diag_env == OS_NULL)
	{
		rt_kprintf("app_diag_env malloc failed!!!!!!!!!!!\n");
		rt_kprintf("reboot clb ...\n");
		rt_hw_cpu_reset();//reset clb
	}
	env->train_id = paras->train_num;
	env->host_flag = 0;

	for(uint8_t i = 0; i < APP_DIAG_AD7606_CH_MAX; i++)
	{
		rt_memset(env->raw_data[i].raw_buff, 0, APP_DIAG_SAMPLING_SIZE_MAX);
		diag_data_node_buff[i] = (diag_data_node_t *)rt_malloc(sizeof(diag_data_node_t));
		if(diag_data_node_buff[i] == OS_NULL)
		{
			rt_kprintf("diag_raw_data_node malloc failed!!!!!!!!!!!\n");
			rt_kprintf("reboot clb ...\n");
			rt_hw_cpu_reset();//reset clb
		}
	}
	env->IIR_buf = rt_malloc(APP_DIAG_SAMPLING_SIZE_MAX * sizeof(float));
	memset(env->IIR_buf, 0, APP_DIAG_SAMPLING_SIZE_MAX*sizeof(float));
	local.f_hd = OS_NULL;

	rt_memset(local.zxzc_stack, 0, sizeof(zxzc_stack_t));
	rt_memset(local.dj_zc_stack, 0, sizeof(dj_clx_stack_t));
	rt_memset(local.clx_zc_stack, 0, sizeof(dj_clx_stack_t));
	rt_memset(local.clx_cl_stack, 0, 3);
	rt_memset(&local.diag_save_head, 0, sizeof(app_diag_save_head_t));
	rt_memset(&alarm, 0, sizeof(diag_alarm_data_t));
}


















