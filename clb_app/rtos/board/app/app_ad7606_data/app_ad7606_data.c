/**
 * @file
 * @details
 * @author
 * @date    AD7606采集数据
 * @version
**/

/* include */
#include <rtthread.h>
#include "app_ad7606_data.h"
#include <drv_gpio.h>
#include <tfdsn_io_config.h>
#include "app_channel_switch.h"
#include "app_data_save.h"
#include "app_diagnosis.h"

/* type declaration */

/* variable declaration */
app_ad7606_data_env_t app_ad7606_data_env;
#define env app_ad7606_data_env

app_ad7606_data_local_t app_ad7606_data_local =
{
	.filter_data.state_flag = 1,
	.filter_data.length     = 0,
};
#define local app_ad7606_data_local

/* functions declaration */
/**
 * @brief  获取存放采集原始数据的buffer地址
 * @param
 * @return 返回7606内部结构体地址
 * @note
**/
app_ad7606_data_local_t * app_ad7606_data_getbuff(void)
{
	return &local;
}
/**
 * @brief  获取存放采集原始数据的buffer地址
 * @param
 * @return 返回7606内部结构体地址
 * @note
**/
struct rt_event * app_ad7606_event_get()
{
	return &env.ad7606_event;
}
/**
 * @brief  将采集到的电信号做原、补码转换
 * @param  uint16_t comp_data 原始电信号采集值
 * @return 补码转换后的值
 * @note
**/
static uint16_t complement_to_source(uint16_t comp_data)
{
   if(comp_data >= 32768) {
	   return (comp_data - 32768);
    }
   else {
      return (comp_data + 32768);
    }
}

/*
 * 补码转原码
 * */
uint16_t ad_complementary_to_original(uint16_t ad)
{
 int16_t ad_org = 0;
 uint16_t ad_org_u16 = 0;

 if(ad & 0x8000)
 {
  ad_org = -(int16_t)((ad ^ 0xffff) + 1);//int16_t用uint16_t保存,最高位不丢数
  ad_org_u16 = (uint16_t)ad_org;
 }
 else
 {
  ad_org_u16 = ad;
 }

 return ad_org_u16;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_ad7606_data_get(uint16_t * data)
{
	static uint16_t cnt = 0;
//	uint8_t mem_err = 0;
	uint8_t i = 0;
	extern int vib_save_flag; //用于标志当前采集原始数据是否存储完成，存完再开始算法处理

	if(local.filter_data.state_flag)
	{
		local.filter_data.length++;
		if(local.filter_data.length >= APP_AD7606_FILTER_SIZE)
		{
			local.filter_data.length = 0;
			local.filter_data.state_flag = 0;
		}
	}
	else
	{
		for(i=0; i<APP_AD7606_DATA_CH_MAX; i++)
		{
			local.ad7606_data.ch[i].data_buffer[cnt] = complement_to_source(data[i]);
			app_diag_env->raw_data[i].raw_buff[cnt] = complement_to_source(data[i]);
		}
		cnt++;

		if(cnt >= APP_AD7606_CH_BUFF_SIZE_MAX)
		{
			//采够4k数据后，切换通道，将原始数据保存，并添加数据到算法诊断中
			app_channel_adg1608_switch_all();
			vib_save_flag = 0;
			rt_event_send(&env.ad7606_event, APP_AD7606_DATA_SAVE_EVENT);
//		    app_data_save_vib(app_channel_adg1608_get_select(),local.ad7606_data);
		    app_diag_raw_data_node_construct();
			cnt = 0;
			local.filter_data.state_flag = 1;
		}
	}
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_ad7606_fill_data(void)
{
	//static uint16_t app_ad7606_value[APP_AD7606_CH_BUFF_SIZE_MAX];

	rt_interrupt_enter();

	bsp_internal_ad7606_read(local.device, local.read_data_buffer);

	app_ad7606_data_get(local.read_data_buffer);

	rt_interrupt_leave();
}
/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_ad7606_data_init(void)
{
	if (rt_event_init(&env.ad7606_event, "ad7606", IPC_FLAG_FIFO) != OS_EOK)
	{
		rt_kprintf("%s %d: ad7606 event init fail!!!\n", __FILE__, __LINE__);
	}
	memset(&local.ad7606_data, 0, sizeof(ad7606_data_t));
	for(uint8_t i = 0; i < APP_AD7606_DATA_CH_MAX; i++)
	{
		local.ad7606_data.ch[i].data_buffer = rt_malloc(APP_AD7606_CH_BUFF_SIZE_MAX * sizeof(uint16_t));
		if(local.ad7606_data.ch[i].data_buffer == OS_NULL) {
			rt_kprintf("app_ad7606_data.c init ch = %d, data buff malloc failed!\n", i);
		}
		//rt_kprintf("app_ad7606_data.c init ch = %d, data buff malloc success   %p!\n", i,local.ad7606_data.ch[i].data_buffer);
	}
	local.init_paras.device_id = APP_AD7606_DEVICE_ID;
	local.init_paras.gpio_reset = GET_PIN(RESET_GPIO, RESET_PIN);
	local.init_paras.gpio_convst = GET_PIN(CONVST_GPIO, CONVST_PIN);
	local.init_paras.gpio_busy = GET_PIN(BUSY_GPIO, BUSY_PIN);
	bsp_internal_ad7606_init(&local.device, &local.init_paras);
	rt_pin_write(local.device->gpio_reset, PIN_LOW);
	bsp_internal_ad7606_reset(local.device);
}


