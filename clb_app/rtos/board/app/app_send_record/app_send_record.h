/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_SEND_RECORD_H__
#define __APP_SEND_RECORD_H__

/* include */
#include "app_utility.h"
#include "clb_record_protocol.h"
#include "system_config.h"
#include <can_app.h>
//#include <xfer_protocol.h>
#include "../app/include/fault_hand.h"

/* macro */
#define APP_SEND_RECORD_FRONT_TIMEOUT_CNT         60   //timeout times (60s)

/* type declaration */
union tmp_t{
	struct{
		uint8_t st_1 :2;
		uint8_t st_2 :2;
		uint8_t st_3 :2;
		uint8_t st_4 :2;
	}bits;
	uint8_t byte;
};

typedef enum
{
	record_board             = 0x11,
	communication_board      = 0x22,
	BEARING_PROCESS_BOARD    = 0x33,
	derailed_process_board1  = 0x44,
	derailed_process_board2  = 0x55,
	rotating_speed_bus_board = 0x66,
	ptu                      = 0xff,
}device_code_e;
typedef struct
{
	uint16_t zxzc_temp[4];/* 每个前置bear1 - bear4 temp */
	uint16_t dj_temp[2];/* motor1 - motor4 temp */
	uint16_t clx_temp[2];/* gear1 - gearr4 temp */
	uint16_t env_temp;
}temp_wait_t;
typedef struct
{
	uint8_t dev4_fault;
	uint8_t dev3_fault;
	uint8_t dev2_fault;
	uint8_t dev1_fault;
}send_record_front_state_t;
typedef struct
{
	uint8_t state;
	int8_t temp;
}send_device_t;
typedef struct
{
//	send_device_t dev8_inform;
//	send_device_t dev7_inform;
//	send_device_t dev6_inform;
//	send_device_t dev5_inform;
	send_device_t dev4_inform;
	send_device_t dev3_inform;
	send_device_t dev2_inform;
	send_device_t dev1_inform;
}send_zxzc_sensor_t;
typedef struct
{
//	send_device_t dev4_inform;
//	send_device_t dev3_inform;
	send_device_t dev2_inform;
	send_device_t dev1_inform;
}send_dj_sensor_t;
typedef struct
{
//	send_device_t dev4_inform;
//	send_device_t dev3_inform;
	send_device_t dev2_inform;
	send_device_t dev1_inform;
}send_clx_sensor_t;
typedef struct
{
	uint8_t            front_state;
	uint8_t            front_power_on;
	char               env_temp;       //环境温度
	send_zxzc_sensor_t bearing_inform; //轴箱传传感器状态
	send_dj_sensor_t   motor_inform;   //齿端电机轴承传感器状态
	send_clx_sensor_t  gear_inform;    //小齿轮箱车轮侧轴承传感器状态
}send_front_t;
typedef struct
{
	send_front_t front_dev1_inform;
	send_front_t front_dev2_inform;
	send_front_t front_dev3_inform;
	send_front_t front_dev4_inform;
}send_all_front_t;
typedef struct
{
	send_all_front_t all_front_inform;//前置器信息
	clb_record_protocol_t          clb_record_data; //处理板发送到记录板的信息
	uint8_t                        train_id; //车厢号:自组网模式，直接从flash中获取，非自组网从记录板公共信息中获取
	uint8_t                        host_flag;//主从标志位
	uint8_t                        host_qzq_num;//主机前置器数量
}app_send_record_env_t;

/* variable declaration */
extern app_send_record_env_t app_send_record_env;

/* function declaration */
void app_send_record_protocol_init(void);
void app_send_record_protocol(void);
uint8_t app_send_get_train_num(void);
/**
 * @brief
 * @param
 * @return
 * @note
**/

#endif /*__APP_SEND_RECORD_H__*/


