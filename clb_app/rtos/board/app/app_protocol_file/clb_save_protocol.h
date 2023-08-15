/**
 * @file
 * @details
 * @author
 * @date    record send clb txb tclb protocol
 * @version
**/

#ifndef __CLB_SAVE_PROTOCOL_H__
#define __CLB_SAVE_PROTOCOL_H__

/* include */
#include <stdio.h>
#include <rtthread.h>
#include "app_ad7606_data.h"
#include "dev_config.h"

/* macro */
#define SAVE_TYPEERR						(4)
#define TEMP_RAW_CNT               	(200u)

/* type declaration */
enum save_type
{
	vibration                 = 0x01,
	temperature               = 0x02
};

enum data_type
{
	bearing_impact         = 0x01,   //轴箱轴承冲击 用于计算轴箱轴承与踏面
	bearing_vibration      = 0x02,   //轴箱轴承振动 用于计算多边形与波磨
	motor_impact           = 0x03,   //电机冲击 用于计算电机轴承
	motor_vibration        = 0x04,   //电机振动
	gear_impact            = 0x05,   //齿轮冲击 用于计算齿轮箱轴承
	gear_vibration         = 0x06,   //齿轮振动 用于计算齿轮箱齿轮
	qzq_temp               = 0x07,   //前置器温度
	wave_grinding          = 0x08,   //波磨
};

enum ad_channel_type
{
    motor_gear_impact_ch     = 0,
    motor_gear_vibration_ch,
    axle_impact_ch,
    axle_vibration_ch,
    wave_wear_ch1,
    wave_wear_ch2,
//    reserve1_ch,  //地铁项目只用到6通道采样
//    reserve2_ch,  //地铁项目只用到6通道采样
};

typedef struct save_data_head
{
		uint8_t save_data_head_h; //包头 H  byte0 66 (存储包头：66bb)
		uint8_t save_data_head_l; //包头 L  byte1 bb (存储包头：66bb)
		uint8_t data_len[2]; //数据包长度   byte2-3

		uint8_t save_data_type; //数据类型 byte4
		uint8_t year;  //年 byte5
		uint8_t month; //月 byte6
		uint8_t day;   //日 byte7
		uint8_t hour;  //时 byte8
		uint8_t min;   //分 byte9
		uint8_t sec;   //秒 byte10

		uint8_t pack_count_h; //数据包计数H byte11
		uint8_t pack_count_l; //数据包计数L byte12
		uint8_t current_id_h; //当前站ID H byte13
		uint8_t current_id_l; //当前站ID L byte14
		uint8_t next_id_h; //下一站ID H byte15

		uint8_t next_id_l; //下一站ID L byte16
		uint8_t speed_h; //速度 H byte17
		uint8_t speed_l; //速度 L byte18
		uint8_t wheel_diameter_h; //轮径 H byte19

		uint8_t wheel_diameter_l; //轮径 L byte20
		uint8_t channel; //adg1608片选模拟通道 byte21
		uint8_t host_slave_flag; //主从机标识 byte22 //适合主从机记录板自己记录的机型
		uint8_t train_id; //车厢号ID byte23 //适合主机记录板统一存所有车厢信息的机型
		uint8_t reserve[40];
}clb_save_data_head_t, * clb_save_data_head_p;

typedef struct save
{
		FILE * ini ;
		int flag;
		int type;
		struct tm time;
		char filename_new[32];
		char filename_old[32];
		void * _private;
}save_t, * save_p;


#endif /*__CLB_SAVE_PROTOCOL_H__*/





