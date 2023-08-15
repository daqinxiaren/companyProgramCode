/**
 * @file
 * @details
 * @author  ren
 * @date    振动/冲击采样通道切换
 * @version
**/

#ifndef __APP_CHANNEL_SWITCH_H__
#define __APP_CHANNEL_SWITCH_H__

/* include */
#include "app_utility.h"
#include "app_board_config.h"

/* macro */
#define APP_ADG1608_SELECT_CH_MAX     DEF_ADG1608_SELECT_CH_MAX
typedef enum
{
    channel_none = 0,
    channel_1,
    channel_2,
    channel_3,
    channel_4,
    channel_5,
    channel_6,
    channel_7,
    channel_8,
    channel_max_out
}app_adg1608_channel_table_e;

/* type declaration */
typedef struct adg1608_pin_configuration
{
	 rt_base_t adg1608_EN;
	 rt_base_t adg1608_A0;
	 rt_base_t adg1608_A1;
	 rt_base_t adg1608_A2;

}adg1608_pin_t, * adg1608_pin_p;
typedef struct adg1609_pin_configuration
{
	 rt_base_t adg1609_EN;
	 rt_base_t adg1609_A0;
	 rt_base_t adg1609_A1;
}adg1609_pin_t, * adg1609_pin_p;
typedef struct {
	uint8_t set_axle_box_num;       //轴箱通道
	uint8_t set_motor_gear_num;     //电机/齿轮通道
	uint8_t set_wave_num;           //多边形、波磨通道
	uint8_t save_axle_box_num;      //保存已采集数据的轴箱通道
	uint8_t save_motor_gear_num;    //保存已采集数据电机/齿轮通道
	uint8_t save_wave_num;          //保存已采集数据多边形、波磨通道
}app_select_num_t, * app_select_num_p;

typedef struct{
	adg1608_pin_t adg1608_axle_box;    //轴箱通道片选引脚配置(V3,V4)
	adg1608_pin_t adg1608_motor_gear;  //电机and齿轮通道片选引脚配置(V1,V2)
	adg1609_pin_t adg1609_wave;        //多边形+波磨采集通道片选引脚配置(V5,V6)
}app_select_pin_t, * app_select_pin_p;

/* variable declaration */


/* function declaration */
void app_channel_adg1608_init(void);
void app_channel_adg1608_switch_all(void);
app_select_num_p app_channel_adg1608_get_select(void);


#endif /* __APP_CHANNEL_SWITCH_H__ */

