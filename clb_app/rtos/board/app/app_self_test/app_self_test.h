/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_SELF_TEST_H__
#define __APP_SELF_TEST_H__

/* includes */
#include "app_board_config.h"
#include "clb_record_protocol.h"

/* macro */
#define BSP_POWER_24V_CH1_PIN            DEF_POWER_24V_CH1_PIN
#define BSP_POWER_24V_CH2_PIN            DEF_POWER_24V_CH2_PIN

/* type declaration */
//typedef struct
//{
//		uint8_t v1_fault      : 1;  // 电源1: 0：正常，1：故障
//		uint8_t v2_fault      : 1;  // 电源2: 0：正常，1：故障
//		uint8_t sd_fault    	 : 1;  // 存储: 0：正常，1：故障
//		uint8_t flash_fault   : 1;  // 存储: 0：正常，1：故障
//		uint8_t reset_para    : 1;
//		uint8_t reserve       : 3;
//}system_device_para_t;

/* variable declaration */

/* function declaration */
void app_self_test_init(void);
sys_dev_state_t* app_get_self_test_para(void);

#endif /* __APP_SELF_TEST_H__ */

