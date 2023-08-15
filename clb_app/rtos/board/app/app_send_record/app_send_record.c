/**
 * @file
 * @details  System startup parameters. Other parameters are not 
             recommended to be saved in STM32 internal flash
 * @author   ren
 * @date     2021.09
 * @version  V1.0
**/

/* include */
#include "app_send_record.h"
#include "dev_config.h"
#include "app_multicast_thread.h"
#include "app_paras_save.h"
#include "app_self_test.h"
#include "app_board_config.h"
#include "app_diagnosis.h"

/* macro */
#define FRONT_TEMP_WORNING       DEF_TEMP_WORNING
#define FRONT_TEMP_ALARM         DEF_TEMP_ALARM

/* variable declaration */
app_send_record_env_t app_send_record_env;
#define env app_send_record_env

/* function declaration */
#include "front_device_inform.c"

/**
 * @brief  获取发送给记录板的车厢号
 * @param
 * @return
 * @note
**/
uint8_t app_send_get_train_num(void)
{
	return env.train_id;
}
/**
 * @brief  //获取主从标志
 * @param
 * @return
 * @note
**/
uint8_t app_send_get_host_flag(void)
{
	return env.host_flag;
}

/**
 * @brief
 * @param
 * @return
 * @note   协议头填充
**/
static inline void app_send_record_fill_head(void)
{
#define head_data app_send_record_env.clb_record_data

	static uint16_t clb_record_life;

	clb_record_life++;
	head_data.data_head[0] = 0xAA;
	head_data.data_head[1] = 0x50;
	head_data.data_len[0] = (uint8_t)(sizeof(clb_record_protocol_t) >> 8);
	head_data.data_len[1] = (uint8_t)(sizeof(clb_record_protocol_t));
	head_data.factory_code = DEVICE_FACTORY_CODE;
	head_data.device_code = BEARING_PROCESS_BOARD;
	head_data.life_signal[0] = (uint8_t)(clb_record_life >> 8);
	head_data.life_signal[1] = (uint8_t)(clb_record_life);
	head_data.target_addr[0] = (uint8_t)(CLB_RECORD_TARGET_ADDR >> 8);
	head_data.target_addr[1] = (uint8_t)(CLB_RECORD_TARGET_ADDR);
	head_data.udp_packet = 0x01;
	head_data.group_num = app_paras_save_env.group_num;
	//更新主机前置器数量
	env.host_qzq_num = app_paras_save_env.host_qzq_num;
	//更新主从机标识
	env.host_flag = app_paras_save_env.host_flag;
#undef head_data
}

/**
 * @brief
 * @param
 * @return
 * @note   版本信息填充
**/
static inline void app_send_record_fill_version(void)
{
	pclb_record_common_data_t version_data;
	version_data = (pclb_record_common_data_t)env.clb_record_data.data_inform;

	version_data->bearing_soft_version[0] = (uint8_t)(CLB_SOFT_VERSION >> 8);
	version_data->bearing_soft_version[1] = (uint8_t)(CLB_SOFT_VERSION);
	version_data->bearing_hard_version[0] = CLB_HARDWARE_VERSION_H;
	version_data->bearing_hard_version[1] = CLB_HARDWARE_VERSION_L;
	version_data->bearing_upgrade_time[0] = (uint8_t)(CLB_UPDATE_TIME >> 24);
	version_data->bearing_upgrade_time[1] = (uint8_t)(CLB_UPDATE_TIME >> 16);
	version_data->bearing_upgrade_time[2] = (uint8_t)(CLB_UPDATE_TIME >> 8);
	version_data->bearing_upgrade_time[3] = (uint8_t)(CLB_UPDATE_TIME);
	memcpy(version_data->front_dev1_soft_version, proximitor_data.proximitor_data1.version.soft_version, 2);
	memcpy(version_data->front_dev1_hard_version, proximitor_data.proximitor_data1.version.hard_version, 2);
	memcpy(version_data->front_dev1_upgrade_time, proximitor_data.proximitor_data1.version.date, 4);
	memcpy(version_data->front_dev2_soft_version, proximitor_data.proximitor_data2.version.soft_version, 2);
	memcpy(version_data->front_dev2_hard_version, proximitor_data.proximitor_data2.version.hard_version, 2);
	memcpy(version_data->front_dev2_upgrade_time, proximitor_data.proximitor_data2.version.date, 4);
	memcpy(version_data->front_dev3_soft_version, proximitor_data.proximitor_data3.version.soft_version, 2);
	memcpy(version_data->front_dev3_hard_version, proximitor_data.proximitor_data3.version.hard_version, 2);
	memcpy(version_data->front_dev3_upgrade_time, proximitor_data.proximitor_data3.version.date, 4);
	memcpy(version_data->front_dev4_soft_version, proximitor_data.proximitor_data4.version.soft_version, 2);
	memcpy(version_data->front_dev4_hard_version, proximitor_data.proximitor_data4.version.hard_version, 2);
	memcpy(version_data->front_dev4_upgrade_time, proximitor_data.proximitor_data4.version.date, 4);
}
/**
 * @brief
 * @param
 * @return
 * @note    系统设备状态信息填充
**/
static inline void app_send_record_fill_device_state(void)
{
	//获取状态
	sys_dev_state_t* device_state = app_get_self_test_para();

	pclb_record_common_data_t common_data;
	common_data = (pclb_record_common_data_t)env.clb_record_data.data_inform;

	common_data->sys_dev_state = *device_state;

}

/**
 * @brief
 * @param
 * @return
 * @note   温度故障信息填充
**/
static inline void app_send_record_fill_temp_breakdown(void)
{
	int i = 0;
	pclb_record_common_data_t common_data;
	common_data = (pclb_record_common_data_t)env.clb_record_data.data_inform;

	for (i=0; i<8; i++)
	{
		if ((common_data->bearing_temp[i]>=FRONT_TEMP_WORNING) && (common_data->bearing_temp[i]<FRONT_TEMP_ALARM))
		{
			common_data->bearing_alarm[i].temp_warning = 1;
			common_data->bearing_alarm[i].temp_alram = 0;
		}
		else if (common_data->bearing_temp[i]>=FRONT_TEMP_ALARM)
		{
			common_data->bearing_alarm[i].temp_warning = 0;
			common_data->bearing_alarm[i].temp_alram = 1;
		}
		else
		{
			common_data->bearing_alarm[i].temp_warning = 0;
			common_data->bearing_alarm[i].temp_alram = 0;
		}
	}
	if(!env.host_flag) //中间车
	{
		for (i=0; i<4; i++)
		{
			if ((common_data->motor_temp[i]>=FRONT_TEMP_WORNING) && (common_data->motor_temp[i]<FRONT_TEMP_ALARM))
			{
				common_data->gear_motor_fault[i].temp_warning = 1;
				common_data->gear_motor_fault[i].temp_alarm = 0;
			}
			else if (common_data->motor_temp[i]>=FRONT_TEMP_ALARM)
			{
				common_data->gear_motor_fault[i].temp_warning = 0;
				common_data->gear_motor_fault[i].temp_alarm = 1;
			}
			else
			{
				common_data->gear_motor_fault[i].temp_warning = 0;
				common_data->gear_motor_fault[i].temp_alarm = 0;
			}
		}
		for (i=0; i<4; i++)
		{
			if ((common_data->gear_temp[i]>=FRONT_TEMP_WORNING) && (common_data->gear_temp[i]<FRONT_TEMP_ALARM))
			{
				common_data->gear_bear_fault[i].temp_warning = 1;
				common_data->gear_bear_fault[i].temp_alarm = 0;
			}
			else if (common_data->gear_temp[i]>=FRONT_TEMP_ALARM)
			{
				common_data->gear_bear_fault[i].temp_warning = 0;
				common_data->gear_bear_fault[i].temp_alarm = 1;
			}
			else
			{
				common_data->gear_bear_fault[i].temp_warning = 0;
				common_data->gear_bear_fault[i].temp_alarm = 0;
			}
		}
	}
	else // 头尾车
	{
		if (DEF_QZQ_HOST_NUM_3 == env.host_qzq_num)
		{
			for (i=2; i<4; i++) //主机3个前置时，只有电机3和电机4
			{
				if ((common_data->motor_temp[i]>FRONT_TEMP_WORNING) && (common_data->motor_temp[i]<=FRONT_TEMP_ALARM))
				{
					common_data->gear_motor_fault[i].temp_warning = 1;
					common_data->gear_motor_fault[i].temp_alarm = 0;
				}
				else if (common_data->motor_temp[i]>FRONT_TEMP_ALARM)
				{
					common_data->gear_motor_fault[i].temp_warning = 0;
					common_data->gear_motor_fault[i].temp_alarm = 1;
				}
				else
				{
					common_data->gear_motor_fault[i].temp_warning = 0;
					common_data->gear_motor_fault[i].temp_alarm = 0;
				}
			}
			for (i=2; i<4; i++) //主机3个前置时，只有齿轮箱3和齿轮箱4
			{
				if ((common_data->gear_temp[i]>FRONT_TEMP_WORNING) && (common_data->gear_temp[i]<=FRONT_TEMP_ALARM))
				{
					common_data->gear_bear_fault[i].temp_warning = 1;
					common_data->gear_bear_fault[i].temp_alarm = 0;
				}
				else if (common_data->gear_temp[i]>FRONT_TEMP_ALARM)
				{
					common_data->gear_bear_fault[i].temp_warning = 0;
					common_data->gear_bear_fault[i].temp_alarm = 1;
				}
				else
				{
					common_data->gear_bear_fault[i].temp_warning = 0;
					common_data->gear_bear_fault[i].temp_alarm = 0;
				}
			}
		}
	}

}
/**
 * @brief
 * @param
 * @return
 * @note   传感器故障信息填充
**/
static inline void app_send_record_fill_sensor_breakdown(void)
{
#define front_inform  env.all_front_inform

	pclb_record_common_data_t common_data;
	common_data = (pclb_record_common_data_t)env.clb_record_data.data_inform;

	//前置器故障信息
	common_data->front_dev_fault.front_dev1_fault = front_inform.front_dev1_inform.front_state;
	common_data->front_dev_fault.front_dev2_fault = front_inform.front_dev2_inform.front_state;
	if(!env.host_flag) //中间车
	{
	    common_data->front_dev_fault.front_dev3_fault = front_inform.front_dev3_inform.front_state;
	    common_data->front_dev_fault.front_dev4_fault = front_inform.front_dev4_inform.front_state;
	}
	else //头尾车
	{
		if (DEF_QZQ_HOST_NUM_3 == env.host_qzq_num)
		{
			common_data->front_dev_fault.front_dev3_fault = front_inform.front_dev3_inform.front_state;
		}
	}
	//传感器故障信息
	if(!env.host_flag) //中间车
	{
		//轴箱，电机，齿轮传感器故障信息
		/* QZQ 1 */
		common_data->bearing_sensorn.bearing_sensor_1_fault = front_inform.front_dev1_inform.bearing_inform.dev1_inform.state;
		common_data->bearing_sensorn.bearing_sensor_2_fault = front_inform.front_dev1_inform.bearing_inform.dev2_inform.state;
		common_data->motor_sensorn.motor_sensor_1_fault = front_inform.front_dev1_inform.motor_inform.dev1_inform.state;
		common_data->gear_bear_sensor.gear_bear_sensor_1_fault = front_inform.front_dev1_inform.gear_inform.dev1_inform.state;
		/* QZQ 2 */
		common_data->bearing_sensorn.bearing_sensor_3_fault = front_inform.front_dev2_inform.bearing_inform.dev1_inform.state;
		common_data->bearing_sensorn.bearing_sensor_4_fault = front_inform.front_dev2_inform.bearing_inform.dev2_inform.state;
		common_data->motor_sensorn.motor_sensor_2_fault = front_inform.front_dev2_inform.motor_inform.dev1_inform.state;
		common_data->gear_bear_sensor.gear_bear_sensor_2_fault = front_inform.front_dev2_inform.gear_inform.dev1_inform.state;
		/* QZQ 3 */
		common_data->bearing_sensorn.bearing_sensor_5_fault = front_inform.front_dev3_inform.bearing_inform.dev1_inform.state;
		common_data->bearing_sensorn.bearing_sensor_6_fault = front_inform.front_dev3_inform.bearing_inform.dev2_inform.state;
		common_data->motor_sensorn.motor_sensor_3_fault = front_inform.front_dev3_inform.motor_inform.dev1_inform.state;
		common_data->gear_bear_sensor.gear_bear_sensor_3_fault = front_inform.front_dev3_inform.gear_inform.dev1_inform.state;
		/* QZQ 4 */
		common_data->bearing_sensorn.bearing_sensor_7_fault = front_inform.front_dev4_inform.bearing_inform.dev1_inform.state;
		common_data->bearing_sensorn.bearing_sensor_8_fault = front_inform.front_dev4_inform.bearing_inform.dev2_inform.state;
		common_data->motor_sensorn.motor_sensor_4_fault = front_inform.front_dev4_inform.motor_inform.dev1_inform.state;
		common_data->gear_bear_sensor.gear_bear_sensor_4_fault = front_inform.front_dev4_inform.gear_inform.dev1_inform.state;
	}
	else
	{
		//轴箱传感器故障信息
		/* QZQ 1 */
		common_data->bearing_sensorn.bearing_sensor_1_fault = front_inform.front_dev1_inform.bearing_inform.dev1_inform.state;
		common_data->bearing_sensorn.bearing_sensor_2_fault = front_inform.front_dev1_inform.bearing_inform.dev2_inform.state;
		common_data->bearing_sensorn.bearing_sensor_3_fault = front_inform.front_dev1_inform.bearing_inform.dev3_inform.state;
		common_data->bearing_sensorn.bearing_sensor_4_fault = front_inform.front_dev1_inform.bearing_inform.dev4_inform.state;
		if (DEF_QZQ_HOST_NUM_2 == env.host_qzq_num)
		{
			common_data->bearing_sensorn.bearing_sensor_5_fault = front_inform.front_dev2_inform.bearing_inform.dev1_inform.state;
			common_data->bearing_sensorn.bearing_sensor_6_fault = front_inform.front_dev2_inform.bearing_inform.dev2_inform.state;
			common_data->bearing_sensorn.bearing_sensor_7_fault = front_inform.front_dev2_inform.bearing_inform.dev3_inform.state;
			common_data->bearing_sensorn.bearing_sensor_8_fault = front_inform.front_dev2_inform.bearing_inform.dev4_inform.state;
		}
		else if (DEF_QZQ_HOST_NUM_3 == env.host_qzq_num)
		{
			/* QZQ 2 */
			common_data->bearing_sensorn.bearing_sensor_5_fault = front_inform.front_dev2_inform.bearing_inform.dev1_inform.state;
			common_data->bearing_sensorn.bearing_sensor_6_fault = front_inform.front_dev2_inform.bearing_inform.dev2_inform.state;
			common_data->motor_sensorn.motor_sensor_3_fault = front_inform.front_dev2_inform.motor_inform.dev1_inform.state;
			common_data->gear_bear_sensor.gear_bear_sensor_3_fault = front_inform.front_dev2_inform.gear_inform.dev1_inform.state;
			/* QZQ 3 */
			common_data->bearing_sensorn.bearing_sensor_7_fault = front_inform.front_dev3_inform.bearing_inform.dev1_inform.state;
			common_data->bearing_sensorn.bearing_sensor_8_fault = front_inform.front_dev3_inform.bearing_inform.dev2_inform.state;
			common_data->motor_sensorn.motor_sensor_4_fault = front_inform.front_dev3_inform.motor_inform.dev1_inform.state;
			common_data->gear_bear_sensor.gear_bear_sensor_4_fault = front_inform.front_dev3_inform.gear_inform.dev1_inform.state;
		}
	}


#undef front_inform
}

/**
 * @brief
 * @param
 * @return
 * @note   算法诊断结果填充
**/
static inline void app_send_record_fill_diag(void)
{
#define front_inform  env.all_front_inform
	diag_alarm_data_p diag_buff = app_diag_get_buff();
	pclb_record_common_data_t common_data;
	common_data = (pclb_record_common_data_t)env.clb_record_data.data_inform;

	memmove(common_data->bearing_alarm, diag_buff->bearing_alarm, 8);
	memmove(common_data->gear_motor_fault,diag_buff->gear_motor_alarm,4);
	memmove(common_data->gear_bear_fault,diag_buff->gear_bear_alarm,4);

}

/**
 * @brief
 * @param
 * @return
 * @note   温度信息填充
**/
static inline void app_send_record_fill_temp(void)
{
#define front_inform  env.all_front_inform

	pclb_record_common_data_t common_data;
	common_data = (pclb_record_common_data_t)env.clb_record_data.data_inform;

   //printf("++++++++ app_send_record_fill_temp  train num  %d\n",env.train_num);
	//填充温度信息
	if(!env.host_flag) //中间车
	{
		/* QZQ 1 */
		common_data->bearing_temp[0] = front_inform.front_dev1_inform.bearing_inform.dev1_inform.temp;
		common_data->bearing_temp[1] = front_inform.front_dev1_inform.bearing_inform.dev2_inform.temp;
		common_data->motor_temp[0] = front_inform.front_dev1_inform.motor_inform.dev1_inform.temp;
		common_data->gear_temp[0] = front_inform.front_dev1_inform.gear_inform.dev1_inform.temp;
		common_data->env_temp[0] = front_inform.front_dev1_inform.env_temp;
		/* QZQ 2 */
		common_data->bearing_temp[2] = front_inform.front_dev2_inform.bearing_inform.dev1_inform.temp;
		common_data->bearing_temp[3] = front_inform.front_dev2_inform.bearing_inform.dev2_inform.temp;
		common_data->motor_temp[1] = front_inform.front_dev2_inform.motor_inform.dev1_inform.temp;
		common_data->gear_temp[1] = front_inform.front_dev2_inform.gear_inform.dev1_inform.temp;
		common_data->env_temp[1] = front_inform.front_dev2_inform.env_temp;
		/* QZQ 3 */
		common_data->bearing_temp[4] = front_inform.front_dev3_inform.bearing_inform.dev1_inform.temp;
		common_data->bearing_temp[5] = front_inform.front_dev3_inform.bearing_inform.dev2_inform.temp;
		common_data->motor_temp[2] = front_inform.front_dev3_inform.motor_inform.dev1_inform.temp;
		common_data->gear_temp[2] = front_inform.front_dev3_inform.gear_inform.dev1_inform.temp;
		common_data->env_temp[2] = front_inform.front_dev3_inform.env_temp;
		/* QZQ 4 */
		common_data->bearing_temp[6] = front_inform.front_dev4_inform.bearing_inform.dev1_inform.temp;
		common_data->bearing_temp[7] = front_inform.front_dev4_inform.bearing_inform.dev2_inform.temp;
		common_data->motor_temp[3] = front_inform.front_dev4_inform.motor_inform.dev1_inform.temp;
		common_data->gear_temp[3] = front_inform.front_dev4_inform.gear_inform.dev1_inform.temp;
		common_data->env_temp[3] = front_inform.front_dev4_inform.env_temp;
	}
	else //头尾车
	{
		/* QZQ 1 */
		common_data->bearing_temp[0] = front_inform.front_dev1_inform.bearing_inform.dev1_inform.temp;
		common_data->bearing_temp[1] = front_inform.front_dev1_inform.bearing_inform.dev2_inform.temp;
		common_data->bearing_temp[2] = front_inform.front_dev1_inform.bearing_inform.dev3_inform.temp;
		common_data->bearing_temp[3] = front_inform.front_dev1_inform.bearing_inform.dev4_inform.temp;
		common_data->env_temp[0] = front_inform.front_dev1_inform.env_temp;
		if (DEF_QZQ_HOST_NUM_2 == env.host_qzq_num)
		{
			/* QZQ 2 */
			common_data->bearing_temp[4] = front_inform.front_dev2_inform.bearing_inform.dev1_inform.temp;
			common_data->bearing_temp[5] = front_inform.front_dev2_inform.bearing_inform.dev2_inform.temp;
			common_data->bearing_temp[6] = front_inform.front_dev2_inform.bearing_inform.dev3_inform.temp;
			common_data->bearing_temp[7] = front_inform.front_dev2_inform.bearing_inform.dev4_inform.temp;
			common_data->env_temp[1] = front_inform.front_dev2_inform.env_temp;
		}
		else if (DEF_QZQ_HOST_NUM_3 == env.host_qzq_num)
		{
			/* QZQ 2 */
			common_data->bearing_temp[4] = front_inform.front_dev2_inform.bearing_inform.dev1_inform.temp;
			common_data->bearing_temp[5] = front_inform.front_dev2_inform.bearing_inform.dev2_inform.temp;
			common_data->motor_temp[2] = front_inform.front_dev2_inform.motor_inform.dev1_inform.temp;
			common_data->gear_temp[2] = front_inform.front_dev2_inform.gear_inform.dev1_inform.temp;
			common_data->env_temp[1] = front_inform.front_dev2_inform.env_temp;
			/* QZQ 3 */
			common_data->bearing_temp[6] = front_inform.front_dev3_inform.bearing_inform.dev1_inform.temp;
			common_data->bearing_temp[7] = front_inform.front_dev3_inform.bearing_inform.dev2_inform.temp;
			common_data->motor_temp[3] = front_inform.front_dev3_inform.motor_inform.dev1_inform.temp;
			common_data->gear_temp[3] = front_inform.front_dev3_inform.gear_inform.dev1_inform.temp;
			common_data->env_temp[2] = front_inform.front_dev3_inform.env_temp;
		}
	}
//	rt_kprintf("---- -- fill_clb_record: temp\n");
//	uint8_t *send_data = (uint8_t *)&common_data->bearing_temp[0];
//	for(uint16_t i = 0; i < 20; i++)
//	{
//	   rt_kprintf("0x%02X ", *send_data);
//	   send_data++;
//	}
//   rt_kprintf("\n");
#undef front_inform
}

static inline void app_send_record_fill_wts(wts_data_t* send_tz)
{
	extern bool wts_tz_flag;
	extern clb_save_data_head_t save_data_head;
	wts_data_t* wts_data = app_diag_get_wts_buff();
	pclb_record_common_data_t common_data;
	common_data = (pclb_record_common_data_t)env.clb_record_data.data_inform;

	memcpy(&wts_data->cgq.qzq_status, &common_data->front_dev_fault, 1);
	memcpy(&wts_data->cgq.zxzc_cgq_status, &common_data->bearing_sensorn, 1);
	memcpy(&wts_data->cgq.dj_cgq_status, &common_data->motor_sensorn, 1);
	memcpy(&wts_data->cgq.clx_cgq_status, &common_data->gear_bear_sensor, 1);
	diag_fill_temperature();
	memcpy(&send_tz->head, &wts_data->head,sizeof(save_temp_t));
	wts_data->year = save_data_head.year;
	wts_data->mon = save_data_head.month;
	wts_data->day = save_data_head.day;
	wts_data->hour = save_data_head.hour;
	wts_data->min = save_data_head.min;
	wts_data->sec = save_data_head.sec;
	if (wts_tz_flag)
	{
		memcpy(send_tz, wts_data,sizeof(wts_data_t));
		wts_tz_flag = 0;
//			rt_kprintf("updata diag channel: %d\n",send_tz.zx_zc.channel_id);
	}
	else
	{
		memcpy(&send_tz->temperature, &wts_data->temperature,sizeof(save_temp_t));
	}
}
/**
 * @brief
 * @param
 * @return
 * @note   填充校验位
**/
static inline void app_send_record_fill_crc(void)
{
	uint16_t check_sum = sum_check_u16((uint8_t *)&env.clb_record_data,
												  sizeof(clb_record_protocol_t) - 2);
	env.clb_record_data.sum_crc[0] = (uint8_t)(check_sum >> 8);
	env.clb_record_data.sum_crc[1] = (uint8_t)(check_sum);
}

/**
 * @brief
 * @param
 * @return
 * @note   处理板到记录板协议填充
**/
void app_send_record_protocol(void)
{
	wts_data_t send_tz = {0};
	/* ----状态获取顺序不能改变---- */
	front_device_get_front_state();
	front_device_get_front_inform();
	/* -----------结束----------- */

	/* --协议填充的前后顺序不能改变-- */
	app_send_record_fill_head();
	app_send_record_fill_version();
	app_send_record_fill_device_state();
	app_send_record_fill_sensor_breakdown();
	app_send_record_fill_diag();
	app_send_record_fill_temp();
	app_send_record_fill_temp_breakdown();
	app_send_record_fill_crc();
	/* ------------结束------------ */
	app_multicast_send(&env.clb_record_data, sizeof(clb_record_protocol_t));

	//填充发往wts的报文
	app_send_record_fill_wts(&send_tz);
	//发送wts需要的特征信息
	app_multicast_send(&send_tz, sizeof(wts_data_t));
//	sendto(udp_socket,(char *) &send_tz,sizeof(wts_data_t),0,(const struct sockaddr *)&addr,sizeof(addr));//udp send

//	rt_kprintf("app_send_record.c -- fill_clb_record:\n");
//	uint8_t *send_data = (uint8_t *)&env.clb_record_data;
//	for(uint16_t i = 0; i < sizeof(clb_record_protocol_t); i++)
//	{
//		if (i!=0 && i%16==0) rt_kprintf("\n");
//		rt_kprintf("0x%02X ", send_data[i]);
//	}
//	rt_kprintf("\n");
}

/**
 * @brief
 * @param
 * @return
 * @note   协议相关的初始化
**/
void app_send_record_protocol_init(void)
{
	front_device_inform_init();
	env.train_id = app_paras_save_env.train_num;
	env.host_flag = 0;
	env.host_qzq_num = DEF_QZQ_HOST_NUM_3;
}
