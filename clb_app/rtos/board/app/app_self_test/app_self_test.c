/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/
/* includes */
#include <tfdsn_io_config.h>
#include <drv_gpio.h>
//#include "app_timer.h"
#include "app_self_test.h"

/** macro */
#define PI  3.14159

/* type declaration */

/* variable declaration */
static rt_base_t app_POST_power_pin_list[2];
static sys_dev_state_t app_self_test_para;

/* function declaration */
/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/
sys_dev_state_t* app_get_self_test_para(void)
{
	return &app_self_test_para;
}

/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/
void app_self_test_power(sys_dev_state_t * device_para)
{
	if (OS_NULL == device_para)
	{
		rt_kprintf("Error: in bsp_internal_collect_init_version  frame_data_p is NULL!\n");
	}
	sys_dev_state_t* device_para_p = device_para;

	if(rt_pin_read(app_POST_power_pin_list[0]))
		device_para_p->v1_voltage_fault = 0;
	else
	{
		device_para_p->v1_voltage_fault = 1;
		rt_kprintf("Error: in app_self_test_power power 1 is error!!!\n");
	}

	if(rt_pin_read(app_POST_power_pin_list[1]))
		device_para_p->v2_voltage_fault = 0;
	else
	{
		device_para_p->v2_voltage_fault = 1;
		rt_kprintf("Error: in app_self_test_power power 2 is error!!!\n");
	}

	return;
}

/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/
/*
void app_self_test_get_version(bsp_speed_frame_data_t * frame_data)
{
	if (OS_NULL == frame_data)
	{
		rt_kprintf("Error: in bsp_internal_collect_init_version  frame_data_p is NULL!\n");
	}
	bsp_speed_frame_data_t* frame_data_p = frame_data;

	uint16_t version_soft = ZXB_SOFT_VERSION;
	uint32_t version_time = ZXB_UPDATE_TIME;

	frame_data_p->version.soft_version[0]=(uint8_t)(version_soft>>8);
	frame_data_p->version.soft_version[1]=(uint8_t)(version_soft);

	frame_data_p->version.hard_version[0]=ZXB_HARDWARE_VERSION_H;
	frame_data_p->version.hard_version[1]=ZXB_HARDWARE_VERSION_L;

	frame_data_p->version.date[0]=(uint8_t)(version_time>>24);
	frame_data_p->version.date[1]=(uint8_t)(version_time>>16);
	frame_data_p->version.date[2]=(uint8_t)(version_time>>8);
	frame_data_p->version.date[3]=(uint8_t)(version_time);

	return;
}
*/
/**
 * @brief  上点自检初始化
 * @param
 * @return if ok return 0 else fail
 * @note
**/
void app_self_test_init(void)
{
	memset(&app_self_test_para, 0, sizeof(sys_dev_state_t));

	//自检 pin init
	app_POST_power_pin_list[0] = BSP_POWER_24V_CH1_PIN;
	app_POST_power_pin_list[1] = BSP_POWER_24V_CH2_PIN;
	rt_pin_mode(app_POST_power_pin_list[0], PIN_MODE_INPUT);
	rt_pin_mode(app_POST_power_pin_list[1], PIN_MODE_INPUT);

	// 检测电源
	app_self_test_power(&app_self_test_para);

	//version init
	//app_self_test_get_version(frame_data);

	return;
}
