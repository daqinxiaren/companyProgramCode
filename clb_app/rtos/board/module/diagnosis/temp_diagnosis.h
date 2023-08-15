/**
 *
 * Copyright (c) 2015-2020  LH  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the temp_diagnosis.h API or library.
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
 * \b @warning     		: null
 * 
 * *
 * Change Logs:
 * Date				author		notes
 * 2020年6月13日			www		create this file
 * 
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
 
#ifndef PROJECT_LH_BOARD_MODULE_DIAGNOSIS_TEMP_DIAGNOSIS_H_
#define PROJECT_LH_BOARD_MODULE_DIAGNOSIS_TEMP_DIAGNOSIS_H_

/**
 ***************** include file ***************
 */

#include <user_data.h>
/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/** @formatter:off  */
/**
 ****************<Public macro>****************
 */

//#define XXX_VERSION	 		1 			/**< major version number */


/**
 * macro define	- description 
 * @description		: null 
 * @note			: null 
 * @param	param	-	description.
 * @param	param	-	description.
 * @note			: null 
 */
//#define desc_class(xxx, xxxx) 									\
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


#define DIAG_TEMP_STEP FS_TEMP //温度算法处理量

//#define TEMP_DEBUG
#ifdef TEMP_DEBUG
#define TEMP_PRINT rt_kprintf
#else
#define TEMP_PRINT(...)
#endif

/** @formatter:on */


/**
 **************<Public variables>**************
 */
extern struct TEMP_SAVE temp_save_proc1;
extern struct TEMP_SAVE temp_save_proc2;

/**
 ***************<Public typedef>***************
 */
extern struct ENV_TEMP env_temp;

//环境温度结构体
struct ENV_TEMP
{
    char pro1_env_temp;             //前置板1环温
    char pro2_env_temp;             //前置板2环温
};


 /**
  * 卡尔曼滤波结果结构体
  */
 struct KALMAN_FILTER_RES
 {
     float temp_new; //新温度,初始化0
     float temp_filter; //滤波后的温度，初始化0
     float covariance;  //初始化3
     float gain;        //初始化0.1
 };


 /**
  * 温度算法存储的数据
  */
 struct TEMP_SAVE
 {
     struct AD_PUBLIC_INFO pub_info1;
     unsigned char env_temp_self_test; //温度自检结果 1:err 0:ok
     unsigned char res[3];
     unsigned char running_gear_temp_self_test[PROC_CHANNEL_NUM]; //温度自检结果 1:err 0:ok
     unsigned char diag_res[PROC_CHANNEL_NUM];                    //诊断结果RUNNING_OK = 0x00,TEMP_WARN = 0x01,TEMP_ALARM = 0x02,    ACC_ALARM_FIRST_GRADE = 0x03,ACC_ALARM_SECOND_GRADE = 0x04
     float speed;                                                 //当前的速度，只是作记录，温度算法与速度无关
     float env_1s_temp;                                           //每秒的卡尔曼滤波之后的平均环温温度
     float running_gear_1s_temp[PROC_CHANNEL_NUM];                //每秒的卡尔曼滤波之后的平均走行部温度
     float diff_env_running_gear_temp[PROC_CHANNEL_NUM];          //环温与走行部温升的差值
 };

 /**
  * 温度算法相关特征结构体
  */
 
 struct RUNNING_GEAR_TEMP_DIAG_VAL
 {
     float ad_running_gear_temp[FS_TEMP];                  //实际采集的走行部温度缓存，1s FS_TEMP个点
     struct KALMAN_FILTER_RES kalman_running_gear_temp[8]; //走行部卡尔曼滤波
     struct TEMP_SAVE temp_save;                           //温度特征数据的存储
 };
 
 /**
  * 环温算法相关
  */
 struct ENV_TEMP_DIAG_VAL
 {
     float ad_env_temp[FS_TEMP];
     float env_1s_temp;
     struct KALMAN_FILTER_RES kalman_env_temp; //环温卡尔曼滤波
 };



#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/


/**
 **************<Public functions>**************
 */

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
 *
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
 * Date           author       notes
 * 2020年6月13日		  www       create this class
 *						
 */
float get_bearing_diag_env_temp(uint8_t ch);
float get_bearing_diag_running_gear_temp(uint8_t ch);
float get_bearing_proc1_temp(uint8_t ch);
float get_bearing_proc2_temp(uint8_t ch);
int diagnosis_temp_thread_init( char *arg);
enum ALARM_TYPE get_temp_alarm_status(uint8_t ch);
float pt1000_convert_ad_temp(uint16_t std, uint16_t src );

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* PROJECT_LH_BOARD_MODULE_DIAGNOSIS_TEMP_DIAGNOSIS_H_ */
/* !< end file   */
