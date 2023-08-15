/**
 *
 * Copyright (c) 2015-2020  LH  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the polygon_diagnosis.h API or library.
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
 
#ifndef PROJECT_LH_BOARD_MODULE_DIAGNOSIS_POLYGON_DIAGNOSIS_H_
#define PROJECT_LH_BOARD_MODULE_DIAGNOSIS_POLYGON_DIAGNOSIS_H_

/**
 ***************** include file ***************
 */

#include <user_data.h>
#include <speed_deal.h>
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

#define TZ_POLYGON_VECTOR_MAX_SIZE 600
#define POLYGON_DIAG_STEP 3
#define DETREND_STEP FS_ACC
#define FS_SPEED_INTERP 300 //1个速度对应10个加速度


/** @formatter:on */


/**
 **************<Public variables>**************
 */


/**
 ***************<Public typedef>***************
 */
/**
 * 多边形算法计算完之后的特征数据
 */
struct POLYGON_SAVE
{
    struct AD_PUBLIC_INFO pub_info;
    unsigned char channel_id;                     //通道编号0-15
    unsigned char self_test;                      //传感器自检结果:1:OK  0:ERR
    unsigned char diag_res;                       //诊断的结果 RUNNING_OK=0x00,TEMP_WARN=0x01,TEMP_ALARM=0x02,ACC_ALARM_FIRST_GRADE=0x03,ACC_ALARM_SECOND_GRADE=0x04
    unsigned char speed_valid_flag;               //速度是否有效的标志 1:有效 0:无效
    float speed_max;                              //本段时间内的最大速度 km/h
    float speed_min;                              //本段时间内的最小速度 km/h
    float TZ1_scalar;                             //特征数据1  特征向量的均方根
    float TZ2_scalar;                             //特征数据2  加速度均方根
    float TZ3_scalar;                             //特征数据3  DB值
    int TZ4_vector_length;                        //特征数据4的长度
    //float TZ4_vector[TZ_POLYGON_VECTOR_MAX_SIZE]; //特征数据4  特征向量
};

/**
 * 多边形计算过程
 */
struct POLYGON_CALC
{
    float *acc_buff;
    float *speed_buff;
    int acc_buff_len;
    int speed_buff_len;
    struct POLYGON_SAVE *polygon_save;
    struct SPEED_SAVE speed_save;
	uint16_t* polygon_acc;
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

int diagnosis_polygon_thread_init(char *arg);


/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* PROJECT_LH_BOARD_MODULE_DIAGNOSIS_POLYGON_DIAGNOSIS_H_ */
/* !< end file   */
