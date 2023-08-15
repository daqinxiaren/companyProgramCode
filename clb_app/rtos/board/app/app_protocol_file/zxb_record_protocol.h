/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __ZXB_RECORD_PROTOCOL_H__
#define __ZXB_RECORD_PROTOCOL_H__

/* include */
#include <stm32f4xx.h>
#include <rtthread.h>
#include "app_board_config.h"

/* macro */

/* type declaration */

//总线板错误信息表
typedef union
{
	struct
	{
		uint8_t power1_err :1;         //24v_1 电压故障
		uint8_t power2_err :1;         //24v_2 电压故障
		uint8_t save_err :1;           //存储设备故障
		uint8_t speed_valid_flag :1;   //速度有效 1：有效 0：无效
		uint8_t nc :4;
	} bits;
	uint8_t byte;
}zxb_collect_zxb_err_st_u;

typedef struct
{
	uint8_t soft_version[2];
	uint8_t hard_version[2];
	uint8_t update_time[4];
}zxb_speed_version_t;

// zxb转速信息表
typedef struct
{
	zxb_speed_version_t version;       // 版本信息
	zxb_collect_zxb_err_st_u zxb_err;  // 转速板故障信息
	uint8_t time[6];
	uint8_t capture[2];                // 脉冲数/s
	uint8_t speed[2];                  // 1秒平均速度
	unsigned char nc [11];             // 预留
}zxb_info_data_t;

typedef struct
{
	uint8_t data_head[2];
	uint8_t data_len[2];
	uint8_t factory_code;
	uint8_t device_code;
	uint8_t life_signal[2];
	uint8_t target_addr[2];
	uint8_t resend_flag;
	uint8_t answer_flag;
	uint8_t udp_packet;
	uint8_t reserve[13];
	uint8_t data_inform[sizeof(zxb_info_data_t)];
	uint8_t sum_crc[2];
}zxb_record_protocol_t, *pzxb_record_protocol_t;

//自定义时间结构体
typedef struct
{
	uint8_t year;
	uint8_t mon;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
}zxb_collect_time_t;

#endif /*__ZXB_RECORD_PROTOCOL_H__*/
