/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_DIAGNOSIS_H__
#define __APP_DIAGNOSIS_H__

/* include */
#include <rtthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "rtdevice.h"
#include "../algorithm_lib/initial_pro.h"
#include "../algorithm_lib/polygon_diag.h"
#include "app_board_config.h"
#include "app_send_record.h"
#include "clb_save_protocol.h"
#include "app_board_config.h"

/* macro */
#define APP_DIAG_STACK              DEF_DIAG_STACK
#define APP_DIAG_THREAD_PRO         DEF_DIAG_THREAD_PRO
#define APP_DIAG_AD7606_CH_MAX      DEF_AD7606_DATA_CH_MAX
#define APP_DIAG_SAMPLING_SIZE_MAX  (4096u)     //采样点最大值
#define APP_DIAG_SAVE_TIME          1        //特值数据保存时间（分钟）
#define APP_DIAG_RATE               (4096u)     //采样率

#define Diag_Num_ZXZC_Wheel         5
#define Diag_Num_DJ                 8
#define Diag_Num_CLX                12
#define Diag_NH_OFFSET              0
#define Diag_WH_OFFSET              1
#define Diag_GDT_OFFSET             2
#define Diag_BCJ_OFFSET             3
#define Diag_Num_COMMON             4
#define Diag_TM_PFFSET             4

/* type declaration */
typedef enum data_type ad_data_type_e;
typedef struct FFT_DIAG_SAVE diag_result_t;
typedef struct POLYGON_DIAG_SAVE polygon_save_t;
typedef struct
{
//	rt_list_t      list;
	bool           active_flag;
	ad_data_type_e ad_data_type;
	uint8_t        channel;
	uint16_t       speed;
	float          wheel;
	struct tm      time;
//	uint16_t       raw_data[APP_DIAG_SAMPLING_SIZE_MAX];
	uint16_t*      raw_data;
	float          acc_data[APP_DIAG_SAMPLING_SIZE_MAX];
	bool           acc_state;
	bool           diag_state;
//	bool           out_state;
	int            out_result;
	int            tm_result;
	diag_result_t  diag_result;
	polygon_save_t polygon_save;
	bool           remove_state;
}diag_data_node_t;

typedef struct
{
	uint16_t raw_buff[APP_DIAG_SAMPLING_SIZE_MAX];
}raw_data_t;

typedef struct
{
		uint8_t bearing_warning				:1;
		uint8_t bearing_1_alarm				:1;
		uint8_t bearing_2_alarm				:1;
		uint8_t tread_warning				:1;
		uint8_t tread_1_alarm				:1;
		uint8_t tread_2_alarm				:1;
}diag_bearing_alarm_t,* diag_bearing_alarm_p;
typedef struct
{
		uint8_t gear_warning					:1;//齿端电机齿轮报警
		uint8_t gear_1_alarm					:1;
		uint8_t gear_2_alarm					:1;
		uint8_t bear_warning					:1;//齿端电机轴承报警
		uint8_t bear_1_alarm					:1;
		uint8_t bear_2_alarm					:1;
}diag_bear_motor_fault_t,* diag_bear_motor_fault_p;

typedef struct
{
	diag_bearing_alarm_t bearing_alarm[8];  //轴箱轴承故障
	diag_bear_motor_fault_t gear_motor_alarm[4]; //齿轮电机故障
	diag_bear_motor_fault_t gear_bear_alarm[4];  //齿轮轴承故障
}diag_alarm_data_t,* diag_alarm_data_p;

/*------------save tz data type----------------------------------*/
#define PROC_CHANNEL_NUM  16
#define QZQ_TOTAL         DEF_QZQ_TOTAL
#define TZ_DATA_LEN       64
#define ZXZC_NUM          8
#define DJ_NUM            4
#define CXL_NUM           4
#define QZQ_HOST_NUM3      DEF_QZQ_HOST_NUM_3
#define QZQ_HOST_NUM2      DEF_QZQ_HOST_NUM_2

//每个报文的长度中包含报文头的长度
//#define TZ_HEAD_LEN       64
#define TZ_TEMP_LEN       128
#define TZ_ZC_LEN         80
#define TZ_DBX_LEN        144
#define TZ_TM_LEN         80
#define TZ_CL_LEN         80

//报警类型枚举
enum ALARM_TYPE
{
	RUNNING_OK = 0x00,
	TEMP_WARN = 0x01,
	TEMP_ALARM = 0x02,
	ACC_ALARM_FIRST_GRADE = 0x03,
	ACC_ALARM_SECOND_GRADE = 0x04
};

//保存特征数据类型枚举
typedef enum TZ_TYPE
{
	type_temp = 0x00,            //温度
	type_zx_zc = 0x01,           //轴箱轴承
	type_dj_zc = 0x02,           //电机轴承
	type_clx_zc = 0x03,          //齿轮箱轴承
	type_polygon = 0x04,         //多边形+波磨
	type_wave_grinding = 0x05,   //波磨
	type_tread = 0x06,           //踏面
	type_clx_cl = 0x07,          //齿轮箱齿轮
}tz_type_e;

typedef struct 
{
	bearing_sensorn_t bearing_sensor_status;
	motor_sensorn_t motor_sensor_status;
	gear_bear_sensorn_t gear_sensor_status;
}sensor_state_t;

//用于存储特征数据的每个的公共部分
typedef struct //62 byte,加上校验位64
{
	uint8_t data_head_h;   //报文头H: 0x66 byte0
	uint8_t data_head_l;   //报文头L: 0xcc byte1
	uint8_t data_len_h;    //报文总长度H byte2
	uint8_t data_len_l;    //报文总长度L byte3
	tz_type_e data_type;   //byte4 存储的原始数据类型
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;           //byte 10
	uint8_t pack_count_h;  //包计数H，顺序计数 byte 11
	uint8_t pack_count_l;  //包计数L，顺序计数 byte 12
	uint8_t curr_station_id_h; //byte 13
	uint8_t curr_station_id_l; //byte 14
	uint8_t next_station_id_h; //byte 15
	uint8_t next_station_id_l; //byte 16
	uint8_t speed_h;           //byte 17
	uint8_t speed_l;           //byte 18
	uint8_t wheel_h;           //byte 19
	uint8_t wheel_l;           //byte 20
	uint8_t chnnel;            //byte 21 振动采集的通道号，1-8
	uint8_t host_flag;         //byte 22 主从机标识（不一定使用）
	uint8_t train_id;          //byte 23 车厢号
	uint8_t total_km[4];       //byte 24-27 总里程
	uint8_t km_post[2];        //byte 28-29 公里标
	uint8_t start_station[2];  //byte 30-31 起点站
	uint8_t end_station[2];    //byte 32-33 终点站	
	uint8_t running_state;     //byte 34 从公共信息中取得列车运行状态，包括：上行、下行、TC1司机室激活，TC2司机室激活
	sensor_state_t sensor_state; //byte 35-37 所有传感器的状态
	uint8_t train_number[2];    //byte 38-39 列车号
    uint8_t reserve[22];
}app_diag_save_head_t;

//轴承报警枚举
//typedef struct         //5 bytes
//{
//	uint8_t alarm_inner_ring;   //内环故障 	0：正常   1：预警   2：一级报警   3:二级报警
//	uint8_t alarm_outter_ring;  //外环故障   	0：正常   1：预警   2：一级报警   3:二级报警
//	uint8_t alarm_rolling_body; //滚动体故障  	0：正常   1：预警   2：一级报警   3:二级报警
//	uint8_t alarm_holder;		//保持架故障 	0：正常   1：预警   2：一级报警   3:二级报警
//	uint8_t alarm_bearing;		//轴承故障  	0：正常   1：预警   2：一级报警   3:二级报警
//}save_bear_alarm_t;

//轴承故障特征数据
typedef struct //4 bytes
{
	uint8_t amplitude_inner_ring;			//内环故障特征数据
	uint8_t amplitude_outer_ring;			//外环故障特征数据
	uint8_t amplitude_rolling_body;		    //滚动体故障特征数据
	uint8_t amplitude_holder;				//保持架故障特征数据
}bearing_tz_t;
//温度存放结构体
typedef struct //16 bytes
{
	int8_t ZXZC_temp[8];			//轴箱轴承温度
	int8_t DJ_temp[4];			    //电机温度
	int8_t CLX_temp[4];		    //齿轮箱温度
}temp_tz_t;

//温度数据
typedef struct //64 byte
{
	uint8_t env_temp_self_test;//温度自检结果 1:err 0:ok ??如何算err
	temp_tz_t diag_res;	//诊断结果（报警状态）RUNNING_OK=0x00,TEMP_WARN=0x01,TEMP_ALARM=0x02,ACC_ALARM_FIRST_GRADE=0x03,ACC_ALARM_SECOND_GRADE  0x04
	temp_tz_t running_1s_temp;	//每秒的卡尔曼滤波之后的平均走行部温度
	temp_tz_t diff_temp;//环温与走行部温升的差值
	uint8_t running_env_temp[QZQ_TOTAL];	//环境温度
	uint8_t reserve[11];
}save_temp_t;

//轴箱轴承数据
typedef struct //8 byte
{
	uint8_t channel_id;		 //通道编号1-8
	uint8_t tm_db;           //踏面DB值
	bearing_tz_t bearing_tz;  //type不同代表的位置不同 type_zx_zc（轴箱轴承）, type_dj_zc（电机驱动端存储数据），type_clx_zc（齿轮箱输入电机侧）
	uint8_t alarm_bearing;		//轴承故障  	0：正常   1：预警   2：一级报警   3:二级报警
	uint8_t alarm_tm;		//轴承故障  	0：正常   1：预警   2：一级报警   3:二级报警
}save_zxzc_t;
//齿轮箱或电机轴承数据
typedef struct //16 byte
{
	uint8_t channel_id;		 //通道编号1-8
	uint8_t speed_valid_flag;
	bearing_tz_t bearing_tz1;  //type不同代表的位置不同 type_zx_zc（轴箱轴承）, type_dj_zc（电机驱动端存储数据），type_clx_zc（齿轮箱输入电机侧）
	bearing_tz_t bearing_tz2;  //type不同代表的位置不同 type_zx_zc（轴箱轴承）, type_dj_zc（电机驱动端存储数据），type_clx_zc（齿轮箱输入电机侧）
	uint8_t alarm_bearing;		//轴承故障  	0：正常   1：预警   2：一级报警   3:二级报警
	uint8_t reserve[5];
}save_dj_clx_t;
//多边形数据
typedef struct //8 byte
{
	uint8_t channel_id;		//通道号1-8，对应轴箱轴承位编号
	uint8_t diag_res;	   //诊断结果（报警状态）		RUNNING_OK=0x00,TEMP_WARN=0x01,TEMP_ALARM=0x02,ACC_ALARM_FIRST_GRADE=0x03,ACC_ALARM_SECOND_GRADE=0x04
	uint8_t TZ1_order_h;	//特征数据1  多边形阶次
	uint8_t TZ1_order_l;
	uint8_t TZ2_rms_h;		//1=0.01特征数据2  加速度均方根(不存)
	uint8_t TZ2_rms_l;
	uint8_t TZ3_DB_h;		//1=0.01特征数据3  DB值
	uint8_t TZ3_DB_l;
}polygon_tz_t;
//波磨数据
typedef struct //8 byte
{
	uint8_t indi1357_result_h;       //1357位车轮对应钢轨的波磨指标
	uint8_t indi1357_result_l;
	uint8_t indi2468_result_h;       //2468位车轮对应钢轨的波磨指标
	uint8_t indi2468_result_l;
	uint8_t nameda1357_result_h;     //1357位车轮对应钢轨的波长指标
	uint8_t nameda1357_result_l;
	uint8_t nameda2468_result_h;     //2468位车轮对应钢轨的波长指标
	uint8_t nameda2468_result_l;
}wave_tz_t;
//需要保存的多边形及波磨特征数据的结构类型
typedef struct//80 byte
{
	polygon_tz_t polygon_tz[8]; //对应8位轴箱的多边形特征值
	wave_tz_t wave_tz;          //8个轴箱算出的波磨特征值
	uint8_t valid_channel_num;  //1-8标出有效数据对应的通道，若此字段为0，代表所有通道数据都有效
	uint8_t reserve[7];
}save_dbx_t;

//踏面数据
//typedef struct //16 byte
//{
//	uint8_t channel_id;		 //通道编号1-8
//	uint8_t temp_alarm_status; //获取当时的温度预报警状态
//	int8_t temp_env;	                //当时的环温，只做监测，与算法无关。环温测点是否会受到机械摩擦或者油温的影响
//	uint8_t speed_valid_flag;
//	uint8_t amplitude_tread;	    //踏面故障特征数据
//	uint8_t reserve[11];
//}save_tread_t;

//齿轮数据
typedef struct //8 byte 使用齿轮振动数据
{
	uint8_t channel_id;		 //通道编号1-8
	uint8_t gear_DB_h;
	uint8_t gear_DB_l;          //1=0.01
	uint8_t gear_amp_h;         //1=0.01
	uint8_t gear_amp_l;
	uint8_t gear_fre_h;         //1=0.01
	uint8_t gear_fre_l;
	uint8_t alarm_status;	    //齿轮故障特征数据
}save_gear_t;

//传感器数据
typedef struct //4 byte 使用齿轮振动数据
{
	front_dev_fault_t qzq_status; //前置器状态
	bearing_sensorn_t zxzc_cgq_status; //轴箱轴承传感器状态
	motor_sensorn_t dj_cgq_status; //电机轴承传感器状态
	gear_bear_sensorn_t clx_cgq_status; //齿轮箱轴承传感器状态
}save_cgq_t;

//wts落地报文
typedef struct
{
	uint8_t head[2];
	uint8_t len[2];
	uint8_t year;
	uint8_t mon;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t train_id;
	save_temp_t temperature;
	save_zxzc_t zx_zc;
	save_dj_clx_t dj_clx_zc;
	polygon_tz_t dbx;
	save_gear_t cl;
	save_cgq_t cgq;
	uint8_t reserve[20];
	uint8_t crc;//单字节和校验
}wts_data_t;

typedef struct
{
	rt_event_t  diag_event;
	rt_thread_t diag_thread;
	rt_list_t   diag_list;
//	struct rt_semaphore    diag_sem;
	raw_data_t  raw_data[APP_DIAG_AD7606_CH_MAX];
	float*      IIR_buf;//初始化时申请空间，用于存放多边形滤波后的数据
	uint8_t     train_id;
	uint8_t     host_flag;
	uint8_t     host_qzq_num;
	wts_data_t  wts_data;
}app_diag_env_t;
/* variable declaration */
extern app_diag_env_t* app_diag_env;

/* function declaration */
void app_diag_init(void);
void app_diag_thread(void);
void app_diag_raw_data_node_construct(void);
diag_alarm_data_p app_diag_get_buff(void);
diag_data_node_t* app_diag_get_node_buff(uint8_t channel);
wts_data_t* app_diag_get_wts_buff(void);
void diag_fill_temperature(void);

#endif /*__APP_DIAGNOSIS_H__*/


//#ifndef __APP_TEST_DATA_H__
//#define __APP_TEST_DATA_H__
//
//float prewarning[4096] = {
//
//};
//
//
//#endif /*__APP_TEST_DATA_H__*/







