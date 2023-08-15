/**
 * @file
 * @details
 * @author
 * @date    record send clb txb tclb protocol
 * @version
**/

#ifndef __TXB_PUBLIC_PROTOCOL_H__
#define __TXB_PUBLIC_PROTOCOL_H__

/* include */
#include "dev_config.h"

/* type declaration */
//保存列车运行状态
typedef union
{
	struct
	{
		uint8_t rcc:4;//
		uint8_t down:1;//列车下行
		uint8_t up:1;//列车上行
		uint8_t TC1_active:1;//TC1司机室激活
		uint8_t TC2_active:1;//TC1司机室激活
	}bits;
	uint8_t byte;
}train_state_t;
/**
 * MVB的公共信息
 */
typedef struct
{
	uint8_t nc[10];
	union VALID
	{
		struct BITS
		{
			uint8_t time_valid :1;
			uint8_t wheel_valid :1;
			uint8_t speed_valid :1;
			uint8_t ncc :5;
		} bits;
		uint8_t byte ;
	} valid;
	uint8_t time[6];
	uint8_t line[2];
	uint8_t train_num[2];
	uint8_t wheel[16];
	uint8_t speed[2];
	uint8_t curr_id[2];    // 当前站ID
	uint8_t next_id[2];
	uint8_t dev_fault;
	uint8_t total_mileage[4]; //总里程
	uint8_t kilometer_post[2];//公里标
	uint8_t start_id[2]; //起点站
	uint8_t end_id[2]; //终点站
	train_state_t train_state; //列车运行状态
	uint8_t reserve[9];
	uint8_t check_sum_h;
	uint8_t check_sum_l;
	uint8_t exter1;
	uint8_t exter2;
}txb_MVB_public_t, *ptxb_MVB_public_t;

#endif /*__TXB_PUBLIC_PROTOCOL_H__*/
