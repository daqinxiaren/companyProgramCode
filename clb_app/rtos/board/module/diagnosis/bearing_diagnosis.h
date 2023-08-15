/**
 *
 * Copyright (c) 2015-2020  LH  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the bearing_diagnosis.h API or library.
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
 
#ifndef _BEARING_DIAGNOSIS_H_
#define _BEARING_DIAGNOSIS_H_

/**
 ***************** include file ***************
 */

#include <rtthread.h>
#include <stm32f4xx.h>

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


#define DIAG_ACC_STEP 4096//轴承算法处理数据量

//#define BEARING_DEBUG
#ifdef BEARING_DEBUG
#define BEARING_PRINT rt_kprintf
#else
#define BEARING_PRINT(...)
#endif

/** @formatter:on */


/**
 **************<Public variables>**************
 */


/**
 ***************<Public typedef>***************
 */
/**
 *  轴承诊断的结果
 */
struct BEAR_ALARM_VAL
{
    uint8_t alarm_inner_ring;   //内环故障 0：正常   1：一级报警   2：二级报警
    uint8_t alarm_outter_ring;  //外环故障 0：正常   1：一级报警   2：二级报警
    uint8_t alarm_rolling_body; //滚动体故障 0：正常   1：一级报警   2：二级报警
    uint8_t alarm_holder;       //保持架故障 0：正常   1：一级报警   2：二级报警
    uint32_t alarm_bearing;     //轴承故障   0：正常   1：一级报警   其余：二级报警
    enum ALARM_TYPE alarm_type; //轴承故障   1: 正常   4: 一级报警   5：二级报警
};


/**
 * 轴承算法存储的数据
 */
struct BEAR_SAVE
{
    struct AD_PUBLIC_INFO pub_info;
    unsigned char channel_id;        //通道编号0-15
    unsigned char self_test;         //传感器自检结果 1:err 0:ok
    unsigned char temp_alarm_status; //获取当时的温度预报警状态
    unsigned char speed_valid_flag;
    float diag_speed;                           //4096个加速度所对应速度的平均值
    float temp_env;                             //当时的环温，只做监测，与算法无关。环温测点是否会受到机械摩擦或者油温的影响
    float temp_running_gear_temp;               //当时的走行部温度，只做监测，与算法无关
    float amplitude_inner_ring;                 //内环故障特征数据
    float amplitude_outer_ring;                 //外环故障特征数据
    float amplitude_rolling_body;               //滚动体故障特征数据
    float amplitude_holder;                     //保持架故障特征数据
    float amplitude_wheel_rotation;             //轮对轴转频幅值
    float rms_envelope;                         //包络信号有效值
    struct BEAR_ALARM_VAL bearing_alarm_status; //轴承诊断的预报警结果
};

/**
 * 轴承算法相关特征结构体
 */
struct BEAR_DIAG_VAL
{
    float *acc_buff;
    float *speed_buff;
    int acc_buff_len;
    int speed_buff_len;
    struct BEAR_SAVE bear_save;
    struct SPEED_SAVE speed_save;
	uint16_t* bear_acc;
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

int diagnosis_bearing_thread_init( char *arg);

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* PROJECT_LH_BOARD_MODULE_DIAGNOSIS_BEARING_DIAGNOSIS_H_ */
/* !< end file   */
