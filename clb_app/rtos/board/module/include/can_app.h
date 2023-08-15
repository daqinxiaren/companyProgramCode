/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file                : Header file for the ad7606_app1.h API or library.
 *
 * @features            :
 *
 * @author              : www
 * 
 * @version             : V 1.0.0
 *
 * @date                : 2020年6月8日
 *
 * @environment         : null
 *
 * @description         : null
 *
 * @module              : null
 *
 * \n <b>@note</b>      : null
 *
 * \b @warning          : null
 * 
 * *
 * Change Logs:
 * Date             author      notes
 * 2020年6月8日            www     create this file
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
 
#ifndef PROJECT_LH_BOARD_PROTOCOL_CAN_APP_CAN_APP_H_
#define PROJECT_LH_BOARD_PROTOCOL_CAN_APP_CAN_APP_H_

/**
 ***************** include file ***************
 */

#include <drv_config.h>
#include <rtthread.h>

#include "can_init.h"
#include "../../app/app_board_config/app_board_config.h"
/**
 * @addtogroup group name
 */

/**@{*/


/**@}*/

/** @formatter:off  */
/**
 ****************<Public macro>****************
 */

//#define XXX_VERSION           1           /**< major version number */


#define CAN_DEBUG
#ifdef CAN_DEBUG
#define CAN_PRINT   rt_kprintf 
#else
#define CAN_PRINT(...)
#endif

/**
 * macro define - description
 * @description     : null
 * @note            : null
 * @param   param   -   description.
 * @param   param   -   description.
 * @note            : null
 */
//#define desc_class(xxx, xxxx)
//        for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


#define QZQ_TOTAL                  DEF_QZQ_TOTAL


/** @formatter:on */

/**
 ***************<Public typedef>***************
 */

typedef struct temperature_sensor_status
{
		uint8_t temp_sensor1			:1;
		uint8_t temp_sensor2			:1;
		uint8_t temp_sensor3			:1;
		uint8_t temp_sensor4			:1;
		uint8_t reserve				:4;

}temp_sensor_status_t, * temp_sensor_status_p;

typedef struct vibration_sensor_status
{
		/* bear */
		uint8_t vib_sensor1			:1;
		/* bear */
		uint8_t vib_sensor2			:1;
		/* gear */
		uint8_t vib_sensor3			:1;
		/* motor */
		uint8_t vib_sensor4			:1;
		uint8_t reserve				:4;

}vib_sensor_status_t, * vib_sensor_status_p;
typedef struct version_msg
{
	uint8_t soft_version[2];
	uint8_t hard_version[2];
	uint8_t date[4];
}version_t;

typedef struct recv_data
{
        vib_sensor_status_t vib_sensor_status;
        temp_sensor_status_t temp_sensor_status;
        /* 0-right 1-error */
        uint8_t power_12V_status;
        /* 0-right 1-error */
        uint8_t power_5V_status;
        /* row1-row4-> sensor1_temp-sensor4_temp
         * last row is environment temperature */
        uint16_t temp_orignal[5][10];
        version_t version;
}qzq_data_t, * qzq_data_p;
typedef struct
{
    uint8_t getid[QZQ_TOTAL];
    uint8_t id_index;
}qzq_id_buff_t;
typedef struct
{
     uint16_t count;
     uint8_t *buffer;
}qzq_recv_t, * qzq_recv_p;
typedef struct
{
         uint8_t qzq_status[QZQ_TOTAL];
         uint8_t qzq_power_on[QZQ_TOTAL];
         qzq_data_t qzq_data[QZQ_TOTAL];
}qzq_info_t, * qzq_info_p;
typedef struct
{
    qzq_id_buff_t qzq_id; //存放获取的前置器ID
    qzq_recv_t recv_data[QZQ_TOTAL]; //存放从前置器接收到的数据
    qzq_info_t qzq_info; //存放解析后的前置器信息
    qzq_recv_t temporary_buff; //存放前置器id超出范围时接收的数据
}qzq_deal_t;

enum message_type
{
	SYSTEM_RESET_CMD = 0x00,  		//复位重启
	UPDATE_PRE_BOARD_CMD = 0x01, 	//升级前置处理器
	SYS_DATA_CMD = 0x05,  			//系统数据
    GET_ID_CMD = 0x06,
	SET_ID_CMD = 0X7 ,				//设置前置器地址（预留）
};

typedef struct proximitor_status
{
		uint8_t proximitor1_comm_status;
		uint8_t proximitor2_comm_status;
		uint8_t proximitor3_comm_status;
		uint8_t proximitor4_comm_status;
		uint8_t proximitor1_power_on;
		uint8_t proximitor2_power_on;
		uint8_t proximitor3_power_on;
		uint8_t proximitor4_power_on;

}proximitor_status_t, * proximitor_status_p;



typedef struct proximitor_data
{
		vib_sensor_status_t vib_sensor_status;
		temp_sensor_status_t temp_sensor_status;
		/* 0-right 1-error */
		uint8_t power_12V_status;
		/* 0-right 1-error */
		uint8_t power_5V_status;
		/* row1-row4-> sensor1_temp-sensor4_temp
		 * last row is environment temperature */
		uint16_t temp_orignal[5][10];
		version_t version;

}proximitor_data_t, * proximitor_data_p;

typedef struct
{
		proximitor_status_t proximitor_status;
		proximitor_data_t proximitor_data1;
		proximitor_data_t proximitor_data2;
		proximitor_data_t proximitor_data3;
		proximitor_data_t proximitor_data4;

}tfdsn_proximitor_t, * tfdsn_proximitor_p;
/**
 * @addtogroup group name
 */
extern tfdsn_proximitor_t proximitor_data;
/**@{*/


/**@}*/

/**
 **************<Public variables>**************
 */

/**
 **************<Public functions>**************
 */
/**@}*/
/**
 * @brief ad7606 data initialization
 * *
 * @author	www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * *
 * @return
 *			- xxx   ad7606_data_p ad7606 data point
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			init/destroy ad7606
 *
 * @warning
 *
 * Change Logs:
 * Date				author			notes
 * 2020年6月8日			www			create this class
 *
 */
tfdsn_proximitor_p proximitor_data_get();

/**
 * @brief development description of class
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2020年5月20日
 * @create time  : 2020年5月20日
 * *
 * Parameters
 * *
 * @param	param	new node to be inserted
 * 
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
 * @note 			can
 *							
 * @warning			description 
 *	
 * Change Logs:
 * Date           author       notes
 * 2020年5月20日		  kernel       create this class
 *						
 */

void can_lh_send_reset(uint8_t taget);

/*can数据发送 组包*/
void can_lh_send(uint8_t target, uint8_t id, uint8_t *data, uint32_t len);

qzq_id_buff_t * app_can_qzq_get_id(void);
void send_radio_getid(void);
qzq_id_buff_t * app_can_qzq_get_id(void);
void app_can_qzq_clean_id(void);
void config_qzq_id(uint8_t oldaddr,uint8_t newaddr);
/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* PROJECT_LH_BOARD_PROTOCOL_CAN_APP_CAN_APP_H_ */

