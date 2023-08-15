/**
 * @file
 * @details
 * @author
 * @date    Public information protocol
 * @version
**/

#ifndef __CLB_RECORD_PROTOCOL_H__
#define __CLB_RECORD_PROTOCOL_H__

/* type declaration */
typedef struct
{
	uint8_t v1_voltage_fault  :1;
	uint8_t v2_voltage_fault  :1;
	uint8_t sd_save_fault     :1;
	uint8_t flash_save_fault  :1;
	uint8_t reserve           :4;
}sys_dev_state_t,* p_sys_dev_state_t;
typedef struct
{
	uint8_t reserve           :3;
	uint8_t deal_board_fault  :1;
	uint8_t front_dev4_fault  :1;
	uint8_t front_dev3_fault  :1;
	uint8_t front_dev2_fault  :1;
	uint8_t front_dev1_fault  :1;
}front_dev_fault_t, *p_front_dev_fault_t;
typedef struct
{
	uint8_t bearing_sensor_8_fault :1;
	uint8_t bearing_sensor_7_fault :1;
	uint8_t bearing_sensor_6_fault :1;
	uint8_t bearing_sensor_5_fault :1;
	uint8_t bearing_sensor_4_fault :1;
	uint8_t bearing_sensor_3_fault :1;
	uint8_t bearing_sensor_2_fault :1;
	uint8_t bearing_sensor_1_fault :1;
}bearing_sensorn_t, *p_bearing_sensorn;
typedef struct
{
	uint8_t bearing_warning		:1; //预警
	uint8_t bearing_1_alram		:1; //一级报警
	uint8_t bearing_2_alarm		:1; //二级报警
	uint8_t tread_warning		:1; //预警
	uint8_t tread_1_alram		:1; //一级报警
	uint8_t tread_2_alarm		:1; //二级报警
	uint8_t temp_warning			:1; //预警
	uint8_t temp_alram			:1; //报警
}bearingn_alarm_t,*p_bearing_alarm;
typedef struct
{
	uint8_t reserve               :4;
	uint8_t motor_sensor_4_fault  :1;
	uint8_t motor_sensor_3_fault  :1;
	uint8_t motor_sensor_2_fault  :1;
	uint8_t motor_sensor_1_fault  :1;
}motor_sensorn_t, *p_motor_sensorn;
typedef struct
{
	uint8_t gear_warning		:1;
	uint8_t gear_1_alram		:1;
	uint8_t gear_2_alarm		:1;
	uint8_t bear_warning		:1;
	uint8_t bear_1_alram		:1;
	uint8_t bear_2_alarm		:1;
	uint8_t temp_warning		:1;
	uint8_t temp_alarm		:1;
}gearn_motor_fault_t, *p_gearn_motor_fault;
typedef struct
{
	uint8_t reserve                  :4;
	uint8_t gear_bear_sensor_4_fault :1;
	uint8_t gear_bear_sensor_3_fault :1;
	uint8_t gear_bear_sensor_2_fault :1;
	uint8_t gear_bear_sensor_1_fault :1;
}gear_bear_sensorn_t, *p_gear_bear_sensor;
typedef struct
{
	uint8_t gear_warning		:1;
	uint8_t gear_1_alram		:1;
	uint8_t gear_2_alarm		:1;
	uint8_t bear_warning		:1;
	uint8_t bear_1_alram		:1;
	uint8_t bear_2_alarm		:1;
	uint8_t temp_warning		:1;
	uint8_t temp_alarm		:1;
}gearn_bear_fault_t, *p_gear_bear_fault;
typedef union
{
	struct
	{
		uint8_t bear_serson4 :2;
		uint8_t bear_serson3 :2;
		uint8_t bear_serson2	:2;
		uint8_t bear_serson1	:2;

		uint8_t bear_serson8	:2;
		uint8_t bear_serson7 :2;
		uint8_t bear_serson6 :2;
		uint8_t bear_serson5 :2;

		uint8_t motor_serson4 :2;
		uint8_t motor_serson3 :2;
		uint8_t motor_serson2 :2;
		uint8_t motor_serson1 :2;

		uint8_t gear_serson4 :2;
		uint8_t gear_serson3 :2;
		uint8_t gear_serson2 :2;
		uint8_t gear_serson1 :2;
	}bits;
	uint8_t byte[4];
}serson_detail_fault_t;
typedef struct
{
	uint8_t               bearing_soft_version[2];      //Byte 0-1
	uint8_t               bearing_hard_version[2];      //Byte 2-3
	uint8_t               bearing_upgrade_time[4];      //Byte 4-7
	uint8_t               front_dev1_soft_version[2];   //Byte 8-9
	uint8_t               front_dev1_hard_version[2];   //Byte10-11
	uint8_t               front_dev1_upgrade_time[4];   //Byte12-15
	uint8_t               front_dev2_soft_version[2];   //Byte16-17
	uint8_t               front_dev2_hard_version[2];   //Byte18-19
	uint8_t               front_dev2_upgrade_time[4];   //Byte20-23
	uint8_t               front_dev3_soft_version[2];   //Byte24-25
	uint8_t               front_dev3_hard_version[2];   //Byte26-27
	uint8_t               front_dev3_upgrade_time[4];   //Byte28-31
	uint8_t               front_dev4_soft_version[2];   //Byte32-33
	uint8_t               front_dev4_hard_version[2];   //Byte34-35
	uint8_t               front_dev4_upgrade_time[4];   //Byte36-39
	sys_dev_state_t       sys_dev_state;                //Byte40
	front_dev_fault_t     front_dev_fault;              //Byte41
	bearing_sensorn_t     bearing_sensorn;              //Byte42
	bearingn_alarm_t      bearing_alarm[8];             //Byte43-50
	motor_sensorn_t       motor_sensorn;                //Byte51
	gearn_motor_fault_t   gear_motor_fault[4];          //Byte52-55
	gear_bear_sensorn_t   gear_bear_sensor;             //Byte56
	gearn_bear_fault_t    gear_bear_fault[4];           //Byte57-60
	int8_t               bearing_temp[8];              //Byte61-68
	int8_t               motor_temp[4];                //Byte69-72
	int8_t               gear_temp[4];                 //Byte73-76
	int8_t               env_temp[4];            //Byte77-80
	serson_detail_fault_t serson_detail_fault;          //Byte81-84
	uint8_t               reserve[15];                  //Byte85-99
}clb_record_common_data_t, *pclb_record_common_data_t;

typedef struct
{
	uint8_t target_addr;
	uint8_t cmd_date;
}cmd_date_t, *pcmd_date_t;

typedef struct
{
	uint8_t data_head[2]; //byte 0-1
	uint8_t data_len[2];  //byte 2-3
	uint8_t factory_code; //byte 4
	uint8_t device_code;  //byte 5
	uint8_t life_signal[2];//byte 6-7
	uint8_t target_addr[2];//byte 8-9
	uint8_t resend_flag;//byte 10
	uint8_t answer_flag;//byte 11
	uint8_t udp_packet;//byte 12
	uint8_t group_num; ////byte 13 反馈给记录板当前的编组信息
	uint8_t reserve[12];
	uint8_t data_inform[sizeof(clb_record_common_data_t)];
	uint8_t sum_crc[2];
}clb_record_protocol_t, *pclb_record_protocol_t;

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
	uint8_t data_inform[sizeof(cmd_date_t)];
	uint8_t sum_crc[2];
}public_frame_t, *ppublic_frame_t;

#endif /*__CLB_RECORD_PROTOCOL_H__*/





