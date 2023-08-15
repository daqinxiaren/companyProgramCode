///**
// *
// * Copyright (c) 2015-2020  xxxx  Development
// * All rights reserved.
// * *
// * This file is part of the <application> or operating system.
// * *
// *
// * @file         		: Header file for the system_para.h API or library.
// *
// * @features			:
// *
// * @author       		: kernel <kernel_sa@163.com>
// *
// * @version      		: V 1.0.0
// *
// * @date         		: 2021年2月28日
// *
// * @environment  		: null
// *
// * @description  		: null
// *
// * @module       		: null
// *
// * \n <b> @NOTE </b> 	: null
// *
// * \b @Warning      		: null
// *
// * *
// * Change Logs:
// * Date           author       notes
// * 2021年2月28日		  kernel       create this file
// *
// *
// */
//
///**
// * \addtogroup dev module
// * @{
// */
//
//
///**
// * \defgroup module API
// *
// * The xxx API module defines a set of functions that a xxx device
// * driver must implement.
// *
// * @{
// */
//
//#ifndef MODULE_CONFIG_DEV_CONIFG_H_
//#define MODULE_CONFIG_DEV_CONIFG_H_
//
///**
// ***************** include file ***************
// * @description		: null
// * @note			: null
// * @note			: null
// */
//
//#include <drv_config.h>
//#include <rtconfig.h>
//
////#ifdef ENABLE_INIPARSER
//
//#include <stdint.h>
//#include <stdbool.h>
//#include <string.h>
//#include <stdlib.h>
//
///**
// * @addtogroup group name
// */
//
///**@{*/
//
//
///**@}*/
//
///** @formatter:off  */
///**
// ****************<Public macro>****************
// * @description		: null
// * @note			: null
// * @note			: null
// */
//
////#define XXX_VERSION	 		1 			/**< major version number */
//
//
///**
// * error code
// */
//typedef enum {
//
//    CONFIG_SUCCESS = 0,                                      /**< success */
//    CONFIG_ERR_NOT_FOUND = 1,                                /**< not found or not supported */
//    CONFIG_ERR_WRITE = 2,                                    /**< write error */
//    CONFIG_ERR_READ = 3,                                     /**< read error */
//    CONFIG_ERR_TIMEOUT = 4,                                  /**< timeout error */
//
//} config_err;
//
///**
// * device ID
// */
//typedef enum {
//
//    TONG_XIN_BOARD = 0x01,
//    JI_LU_BOARD = 0x02,
//    CU_LI_BOADR = 0x03,
//    TUO_GUI_BOARD1 = 0x04,
//    TUO_GUI_BOARD2 = 0x05,
//    ZHUAN_SU_BOARD = 0x06,
//    QIAN_ZHI_BOARD1 = 0x11,
//    QIAN_ZHI_BOARD2 = 0x12,
//    QIAN_ZHI_BOARD3 = 0x13,
//    QIAN_ZHI_BOARD4 = 0x14,
//
//} dev_id;
//
///** @formatter:on */
//
///**
// ***************<Public typedef>*************** */
//
///*******************************system parameters*****************************/
///**
// * @description		: production_fixed
// * @note			: null
// * @note			: null
// */
//typedef struct production_fixed
//{
//	 char prod_number[16];      	/* Production serial number */
//	 char prod_timer[16];       	/* Production timer */
//	 char hard_version[16];     	/* hardware version */
//
//}prod_fixed_t, * prod_fixed_p;
//
// /**
//  * @description	: device parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct device_parameters
// {
// 	 char train_number[8];    		/* train number */
// 	 char car_num[4];					/* car number */
// 	 unsigned int  board_num;     /* board number */
// 	 char install_time[8];			/* installation time */
// 	 char old_soft_version[8];   	/* old software version */
// 	 char curr_soft_version[8];   /* current software version */
// 	 char upgr_time[16];        	/* upgrade time */
// 	 char conf_time[16];          /* configuration time */
// 	 char reset_flag;             /* self reset using*/
//
// }dev_para_t, * dev_para_p;
//
//
// /**
//  * @description	: system parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct system_parameters
// {
//		 prod_fixed_t prod_fixed;
//		 dev_para_t dev_para;
//
// }sys_para_t, * sys_para_p;
//
// extern sys_para_t sys_conifg;
//
// /*******************************communication parameters*****************************/
// /**
//  * @description	: communication parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct communication_parameters
// {
//	 uint8_t  train_id;
//	 char local_ip[16];
//	 char local_mask[16];
//	 char local_gateway[16];
//
// }comm_para_t, * comm_para_p;
//
// /*******************************Algorithm configuration parameters*****************************/
// /**
//  * @description	: communication parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct collect_parameters
// {
//	 uint32_t coll_freq;				/* Acquisition frequency */
//	 uint32_t coll_time_interval;	/* Collection time interval */
//
// }coll_para_t, * coll_para_p;
//
// /**
//  * @description	: polygon parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct polygon_parameters
// {
//	 float sensitivity;
//	 int wheel_diameter;
//	 uint8_t upper_boun_value;		/* Upper boundary value of order */
//	 uint8_t lower_boun_value;		/* lower boundary value of order */
//	 uint8_t cons_same_order;     /* Consecutive same order times */
//	 uint8_t reserve;
//
// }poly_para_t, * poly_para_p;
//
// /**
//  * @description	:  shaft end bearing parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct shaft_end_bearing_parameters
// {
//	 float sensitivity;
//	 int wheel_diameter;
//	 uint8_t lower_inner_ring;		/* lower bound of the ratio of inner ring characteristic frequency to vehicle speed */
//	 uint8_t upper_inner_ring;		/* upper bound of the ratio of inner ring characteristic frequency to vehicle speed */
//	 uint8_t lower_outer_ring;		/* Lower bound of the ratio of outer loop characteristic frequency to vehicle speed */
//	 uint8_t upper_outer_ring;		/* upper bound of the ratio of outer loop characteristic frequency to vehicle speed */
//	 uint8_t lower_rolling;			/* The lower bound of the ratio of the characteristic frequency of the rolling element to the speed */
//	 uint8_t upper_rolling;			/* The upper bound of the ratio of the characteristic frequency of the rolling element to the speed */
//	 uint8_t lower_cage_ring;		/* Lower limit value of the ratio of cage characteristic frequency to vehicle speed */
//	 uint8_t upper_cage_ring;		/* upper limit value of the ratio of cage characteristic frequency to vehicle speed */
//
// }shaft_end_bear_t, * shaft_end_bear_p;
//
// /**
//  * @description	:  gear bearing parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct gear_bearing_parameters
// {
//	 uint8_t lower_inner_ring;		/* lower bound of the ratio of inner ring characteristic frequency to vehicle speed */
//	 uint8_t upper_inner_ring;		/* upper bound of the ratio of inner ring characteristic frequency to vehicle speed */
//	 uint8_t lower_outer_ring;		/* Lower bound of the ratio of outer loop characteristic frequency to vehicle speed */
//	 uint8_t upper_outer_ring;		/* upper bound of the ratio of outer loop characteristic frequency to vehicle speed */
//	 uint8_t lower_rolling;			/* The lower bound of the ratio of the characteristic frequency of the rolling element to the speed */
//	 uint8_t upper_rolling;			/* The upper bound of the ratio of the characteristic frequency of the rolling element to the speed */
//	 uint8_t lower_cage_ring;		/* Lower limit value of the ratio of cage characteristic frequency to vehicle speed */
//	 uint8_t upper_cage_ring;		/* upper limit value of the ratio of cage characteristic frequency to vehicle speed */
//
// }gear_bear_t, * gear_bear_p;
//
// /**
//  * @description	:  derailment parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct derailment_parameters
// {
//
// }dera_para_t, * dera_para_p;
//
// /**
//  * @description	: Algorithm configuration parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct algo_config_parameters
// {
//		 coll_para_t coll_para;
//		 poly_para_t poly_para;
//		 shaft_end_bear_t shaft_end_bear;
//		 gear_bear_t gear_bear;
//		 dera_para_t dera_para;
//
// }algo_config_para_t, * algo_config_para_p;
//
// /*******************************Algorithm threshold parameters*****************************/
// /**
//  * @description	:  Axle box temperature threshold
//  * @note			: null
//  * @note			: null
//  */
// typedef struct axle_box_temp_parameters
// {
//	 uint8_t axle_diff_threshold_warn;	/* Temperature difference warning threshold */
//	 uint8_t axle_diff_threshold_alarm;	/* Temperature difference alarm threshold */
//	 uint8_t axle_abso_threshold_warn;	/* Absolute temperature warning threshold */
//	 uint8_t axle_abso_threshold_alarm;	/* Absolute temperature alarm threshold */
//
// }axle_box_temp_para_t, * axle_box_temp_para_p;
//
// /**
//  * @description	: Motor temperature threshold
//  * @note			: null
//  * @note			: null
//  */
// typedef struct motor_temp_parameters
// {
//	 uint8_t motor_diff_threshold_warn;	/* Temperature difference warning threshold */
//	 uint8_t motor_diff_threshold_alarm;	/* Temperature difference alarm threshold */
//	 uint8_t motor_abso_threshold_warn;	/* Absolute temperature warning threshold */
//	 uint8_t motor_abso_threshold_alarm;	/* Absolute temperature alarm threshold */
//
// }motor_temp_para_t, * motor_temp_para_p;
//
// /**
//  * @description	: gear temperature threshold
//  * @note			: null
//  * @note			: null
//  */
// typedef struct gear_temp_parameters
// {
//	 uint8_t gear_diff_threshold_warn;	/* Temperature difference warning threshold */
//	 uint8_t gear_diff_threshold_alarm;	/* Temperature difference alarm threshold */
//	 uint8_t gear_abso_threshold_warn;	/* Absolute temperature warning threshold */
//	 uint8_t gear_abso_threshold_alarm;	/* Absolute temperature alarm threshold */
//
// }gear_temp_para_t, * gear_temp_para_p;
//
// /**
//  * @description	: polygon threshold parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct polygon_threshold_parameters
// {
//	 uint8_t poly_pred_threshold;		/* Multi-deformation prediction threshold */
//	 uint8_t poly_warn_threshold;		/* Multi-deformation warning threshold */
//	 uint8_t poly_alrm_threshold;		/* Multi-deformation alarm threshold */
//	 uint8_t reserve;
//
// }poly_thre_para_t, * poly_thre_para_p;
//
// /**
//  * @description	:  shaft end bearing threshold parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct shaft_end_bearing_threshold_parameters
// {
//	 uint8_t end_inner_pred_threshold;	/* inner loop prediction threshold */
//	 uint8_t end_inner_warn_threshold;	/* inner loop warning threshold */
//	 uint8_t end_inner_alrm_threshold;	/* inner loop alarm threshold */
//	 uint8_t end_outer_pred_threshold;	/* outer loop prediction threshold */
//	 uint8_t end_outer_warn_threshold;	/* outer loop warning threshold */
//	 uint8_t end_outer_alrm_threshold;	/* outer loop alarm threshold */
//	 uint8_t end_rolling_pred_threshold;/* rolling element prediction threshold  */
//	 uint8_t end_rolling_warn_threshold;/* rolling element warning threshold */
//	 uint8_t end_rolling_alrm_threshold;/* rolling element alarm threshold */
//	 uint8_t end_cage_pred_threshold;	/* cage loop prediction threshold */
//	 uint8_t end_cage_warn_threshold;	/* cage loop warning threshold */
//	 uint8_t end_cage_alrm_threshold;	/* cage loop alarm threshold */
//
// }end_bear_thre_para_t, * end_bear_thre_para_p;
//
// /**
//  * @description	:  motor bearing threshold parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct motor_bearing_threshold_parameters
// {
//	 uint8_t motor_inner_pred_threshold;	/* inner loop prediction threshold */
//	 uint8_t motor_inner_warn_threshold;	/* inner loop warning threshold */
//	 uint8_t motor_inner_alrm_threshold;	/* inner loop alarm threshold */
//	 uint8_t motor_outer_pred_threshold;	/* outer loop prediction threshold */
//	 uint8_t motor_outer_warn_threshold;	/* outer loop warning threshold */
//	 uint8_t motor_outer_alrm_threshold;	/* outer loop alarm threshold */
//	 uint8_t motor_rolling_pred_threshold;	/* rolling element prediction threshold  */
//	 uint8_t motor_rolling_warn_threshold;	/* rolling element warning threshold */
//	 uint8_t motor_rolling_alrm_threshold;	/* rolling element alarm threshold */
//	 uint8_t motor_cage_pred_threshold;		/* cage loop prediction threshold */
//	 uint8_t motor_cage_warn_threshold;		/* cage loop warning threshold */
//	 uint8_t motor_cage_alrm_threshold;		/* cage loop alarm threshold */
//
// }motor_bear_thre_para_t, * motor_bear_thre_para_p;
//
// /**
//  * @description	: gear bearing threshold parameters
//  * @note			: null
//  * @note			: null
//  */
// typedef struct gear_bearing_threshold_parameters
// {
//	 uint8_t gear_inner_pred_threshold;		/* inner loop prediction threshold */
//	 uint8_t gear_inner_warn_threshold;		/* inner loop warning threshold */
//	 uint8_t gear_inner_alrm_threshold;		/* inner loop alarm threshold */
//	 uint8_t gear_outer_pred_threshold;		/* outer loop prediction threshold */
//	 uint8_t gear_outer_warn_threshold;		/* outer loop warning threshold */
//	 uint8_t gear_outer_alrm_threshold;		/* outer loop alarm threshold */
//	 uint8_t gear_rolling_pred_threshold;	/* rolling element prediction threshold */
//	 uint8_t gear_rolling_warn_threshold;	/* rolling element warning threshold */
//	 uint8_t gear_rolling_alrm_threshold;	/* rolling element alarm threshold */
//	 uint8_t gear_cage_pred_threshold;		/* cage loop prediction threshold */
//	 uint8_t gear_cage_warn_threshold;		/* cage loop warning threshold */
//	 uint8_t gear_cage_alrm_threshold;		/* cage loop alarm threshold */
//
// }gear_bear_thre_para_t, * gear_bear_thre_para_p;
//
//
// /*******************************Self-check parameters*****************************/
// /**
//  * @description	: Self-test threshold
//  * @note			: null
//  * @note			: null
//  */
// typedef struct self_test_threshold_parameters
// {
//	 uint16_t save_threshold;
//	 uint16_t net_timeout;                  /* Network timeout */
//	 float sensor_check_volt_min;    		/* Minimum value of sensor self-test voltage */
//	 float sensor_check_volt_max;    		/* Max value of sensor self-test voltage */
//
// }self_test_thre_para_t, *self_test_thre_para_p;
//
//  /**
//  * @description	: Self-test threshold
//  * @note			: null
//  * @note			: null
//  */
// typedef struct self_test_result_parameters
// {
//	 uint8_t sd;
//	 uint8_t flash;
//	 uint8_t temperature;
//	 uint8_t sampling;
//	 uint8_t net;
//	 uint8_t voltage;
// }self_test_resu_para, * self_test_resu_para_p;
//
///**
// **************<Public variables>**************
// * @description		: null
// * @note			: null
// * @note			: null
// */
//
///**
// **************<Public functions>**************
// * @description		: null
// * @note			: null
// * @note			: null
// */
//
//
//#ifdef __cplusplus
//extern "C"
//{
//#endif
//
///**
// * @addtogroup group name
// */
//
///**@{*/
//
//
///**@}*/
//
//
///**
// **************<Public functions>**************
// * @description		: null
// * @note			: null
// * @note			: null
// */
//
///*****************************configuration file initialization***********************************/
///**
// * @brief determine whether the configuration file exists,if not,create and write default configuration,
// * if it exists,do not process
// * *
// * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
// * @create time  : 2021年2月28日
// * *
// * Parameters
// * *
// * @retval
// *				- 0   program execution completed normally
// *				- -1  Program execution error
// * *
// * @see  reference @file
// *
// * @note 			description
// *
// * @warning			description
// *
// * Change Logs:
// * Date           author       notes
// * 2021年2月28日		kernel       create this class
// *
// */
//int config_ini_file_init(void);
//
///*******************************system parameters*****************************/
///**
// * @brief Read and parse system parameters
// * *
// * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
// * @create time  : 2021年2月28日
// * *
// * Parameters
// * *
// * @param	sys_para_p p_sys_para[in] the pointer of the system parameter is used to obtain the key value
// *
// * @retval
// *				- 0   program execution completed normally
// *				- -1  Program execution error
// * *
// * @see  reference @file
// *
// * @note 			description
// *
// * @warning			description
// *
// * Change Logs:
// * Date           author       notes
// * 2021年2月28日		  kernel       create this class
// *
// */
//int read_system_ini(sys_para_p p_sys_para);
//
//int write_system_ini(sys_para_p p_sys_para);
//
//uint8_t update_sys_para();
//uint8_t save_sys_para();
//
///*******************************communication parameters*****************************/
///**
// * @brief read and parse communication parameters
// * *
// * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月28日
// * @create time  : 2021年2月28日
// * *
// * Parameters
// * *
// * @param	sys_para_p p_sys_para[in] the pointer of the communication parameter is used to obtain the key value
// *
// * @retval
// *				- 0   program execution completed normally
// *				- -1  Program execution error
// * *
// * @see  reference @file
// *
// * @note 			description
// *
// * @warning			description
// *
// * Change Logs:
// * Date           author       notes
// * 2021年2月28日		kernel       create this class
// *
// */
//int read_communication_ini(comm_para_p p_comm_para);
//int write_communi_ini(comm_para_p p_comm_para);
//
///** @} */ //\defgroup module API
///** @} */ //\addtogroup dev module
//
//#ifdef __cplusplus
//}
//#endif  /* end of __cplusplus */
//
//#endif /* ENABLE_INIPARSER */
//
////#endif
//
