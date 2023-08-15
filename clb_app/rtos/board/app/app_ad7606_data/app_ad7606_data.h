/**
 * @file
 * @details
 * @author  ren
 * @date    组播发送和接收线程
 * @version
**/

#ifndef __APP_AD7606_DATA_H__
#define __APP_AD7606_DATA_H__

/* include */
#include "app_utility.h"
#include "bsp_internal_ad7606.h"
#include "app_board_config.h"

/* macro */
#define APP_AD7606_CH                DEF_CLB_DATA_CH_MAX
#define APP_AD7606_DATA_CH_MAX       DEF_AD7606_DATA_CH_MAX
#define APP_AD7606_CH_BUFF_SIZE_MAX  DEF_AD7606_CH_BUFF_SIZE_MAX
#define APP_AD7606_FILTER_SIZE       DEF_AD7606_FILTER_SIZE       //过滤数据
#define APP_AD7606_DEVICE_ID         DEF_AD7606_DEVICE_ID

#define APP_AD7606_DATA_SAVE_EVENT   1

enum AD7606_V
{
	AD7606_V1      = 0x00,   //7606采集通道
	AD7606_V2      = 0x01,   //轴箱轴承振动 用于计算多边形与波磨
	AD7606_V3      = 0x02,   //电机冲击 用于计算电机轴承
	AD7606_V4      = 0x03,   //电机振动
	AD7606_V5      = 0x04,   //齿轮冲击 用于计算齿轮箱轴承
	AD7606_V6      = 0x05,   //齿轮振动 用于计算齿轮箱齿轮
};

/* type declaration */
typedef struct
{
	struct rt_event ad7606_event;
}app_ad7606_data_env_t;

typedef struct
{
	uint16_t ad7606_pack_head;
	uint8_t  reserve[62];
}ad7606_data_head_t, *pad7606_data_head;
typedef struct
{
	//ad7606_data_head_t data_head;
	uint16_t           *data_buffer;
}ad7606_ch_data_t, *pad7606_ch_data;
typedef struct
{
	ad7606_ch_data_t ch[APP_AD7606_DATA_CH_MAX];
}ad7606_data_t, *pad7606_data;
typedef struct
{
	uint8_t  state_flag;
	uint16_t length;
}filter_data_t;

typedef struct
{
	bsp_ad7606_init_param_t  init_paras;
	bsp_ad7606_dev_t         *device;
	ad7606_data_t            ad7606_data;
	filter_data_t            filter_data;
	uint16_t                 read_data_buffer[APP_AD7606_DATA_CH_MAX];
}app_ad7606_data_local_t;

/* variable declaration */
extern app_ad7606_data_env_t app_ad7606_data_env;

/* function declaration */
void app_ad7606_data_init(void);
void app_ad7606_fill_data(void);
struct rt_event * app_ad7606_event_get(void);
app_ad7606_data_local_t * app_ad7606_data_getbuff(void);

#endif /* __APP_AD7606_DATA_H__ */

