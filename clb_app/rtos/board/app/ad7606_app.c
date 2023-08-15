/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the ad7606_app1.c API or library.
 *
 * @features			:
 *
 * @author       		: www
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020年6月8日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: ad
 *
 * \n <b>@note</b> 		: null
 *
 * \b <b> @warning </b> : null
 * 
 * *
 * Change Logs:
 *	Date				author       notes
 *	2020年6月8日			www         create this file
 *	2020年6月8日
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
 
#if 0
/**
 ***************** include file ***************
 */
#include <drv_gpio.h>
#include <drv_config.h>
#if defined(ENABLE_FSMC_ADC7606)
#include <stdio.h>
#include <tfdsn_io_config.h>
#include "rtdevice.h"
#include "ad7606_app.h"
#include <ad7606_fsmc.h>
#include <ad7606_init.h>
#include "../../lh/itc/itc.h"
#include <switch_app.h>
#include "app_diagnosis.h"
/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/
/**@}*/

/** @formatter:off */
/**
 ***************<Private macro>****************
 */

/**
 * @brief macro define	- description 
 * @param	param	- description.
 * @param	param	- description.
 */
//#define desc_class(xxx, xxxx)
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/** @formatter:on */

/**
 ***************<Public variables>****************
 */


/**
 **************<Private variables>**************
 */
static struct ad7606_dev *device;
static struct filter_data filters={
		.filter_flag=1,
		.filter_length=0,
};
/**
 **************<Private functions>**************
 */

/**
 **************<Public IMPL functions>**************
 */


/**
 * @addtogroup group name 
 */

/**@{*/

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
ad7606_data_p ad7606_databuff_get()
{
	static ad7606_data_t ad7606_data;

	return &ad7606_data;
}

/**
 * @brief ad7606 data initialization
 * *
 * @author	www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param [ in or out ]	ad7606_data_p ad7606_data  ad7606 data point to initialization
 * *
 * @return
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
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
int ad7606_databuff_init(ad7606_data_p ad7606_data)
{
	int result;

	memset(ad7606_data,0,sizeof(ad7606_data_t));

	ad7606_data->ad7606_data_ch0.ad7606_data_buffer = rt_malloc(FS_AD7606 * sizeof(uint16_t));
	ad7606_data->ad7606_data_ch1.ad7606_data_buffer = rt_malloc(FS_AD7606 * sizeof(uint16_t));
	ad7606_data->ad7606_data_ch2.ad7606_data_buffer = rt_malloc(FS_AD7606 * sizeof(uint16_t));
	ad7606_data->ad7606_data_ch3.ad7606_data_buffer = rt_malloc(FS_AD7606 * sizeof(uint16_t));
	ad7606_data->ad7606_data_ch4.ad7606_data_buffer = rt_malloc(FS_AD7606 * sizeof(uint16_t));
	ad7606_data->ad7606_data_ch5.ad7606_data_buffer = rt_malloc(FS_AD7606 * sizeof(uint16_t));

	if((OS_NULL == ad7606_data->ad7606_data_ch0.ad7606_data_buffer) || (OS_NULL == ad7606_data->ad7606_data_ch1.ad7606_data_buffer) ||
			(OS_NULL == ad7606_data->ad7606_data_ch2.ad7606_data_buffer) || (OS_NULL == ad7606_data->ad7606_data_ch3.ad7606_data_buffer) ||
			(OS_NULL == ad7606_data->ad7606_data_ch4.ad7606_data_buffer))
	{
		result = -1;
	}
	else
	{
		result = 1;
	}

	return result;
}

/*采样数据转换电压*/
//static float calculate_to_voltage(uint16_t value)
//{
//    return (value - 32768.0) / 65536.0 * 20.0;
//}

/*采样数据补码转换*/
/**
 * @brief complement_to_source
 * *
 * @author	www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param - comp_data	ad value
 * *
 * @return
 *			- uint16_t 		behind of complement
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
static uint16_t complement_to_source(uint16_t comp_data)
{
    if (comp_data >= 32768)
    {
        return (comp_data - 32768);
    }
    else
    {
        return (comp_data + 32768);
    }
}

/**
 * @brief ad7606 data initialization
 * *
 * @author	www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param				param	new node to be inserted
 * @param [ in or out ]	param	new node to be inserted
 * * 
 * @return 
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
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
void ad7606_data_init(void)
{
	ad7606_data_p ad7606_data;
	struct ad7606_init_param init_param;

	/* initialization ad7606 data */
	ad7606_data = ad7606_databuff_get();
	ad7606_databuff_init(ad7606_data);

	//初始化AD7607
	init_param.device_id = ID_AD7606_6;
	init_param.gpio_reset = GET_PIN(RESET_GPIO,RESET_PIN);
	init_param.gpio_convst = GET_PIN(CONVST_GPIO,CONVST_PIN);
	init_param.gpio_busy = GET_PIN(BUSY_GPIO,BUSY_PIN);

	ad7606_init(&device,&init_param);
	rt_pin_write(device->gpio_reset,PIN_LOW);
//	ad7606_convst(device,PIN_HIGH);
//	rt_thread_mdelay(1);
	ad7606_reset(device);
//	ad7606_convst(device,PIN_LOW);
//	rt_thread_mdelay(1);
//	ad7606_convst(device,PIN_HIGH);

//	ad7606_init();
//	ad7606_convst();
//	rt_kprintf("start ad7606!\n");
}

/**
 * @brief package ad7606 data
 * *
 * @author  www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param               param   new node to be inserted
 * @param [ in or out ] param   new node to be inserted
 * *
 * @return
 *          - xxx   description or none
 *          - xxx   description ,the operation status or on successful
 * @retval
 *          - xxx   description
 * *
 * @see  reference @file
 *
 * @note            开启/关闭 ad7606数据采集接口
 *
 * @warning         description
 *
 * Change Logs:
 * Date             author          notes
 * 2020年6月8日            www         create this class
 *
 */
void package_ad_data(uint16_t * data)
{
	static uint16_t cnt = 0;
	static uint16_t diag_cnt = 0;
	struct rt_event * ad7606_event;
	ad7606_data_p ad7606_data;
	extern int vib_save_flag;
//	static uint8_t test_cnt = 0;
//	rt_sem_t diag_sem;

	ad7606_event = ad7606_event_get();
	ad7606_data = ad7606_databuff_get();
//	diag_sem = app_diag_sem_get();

	if(filters.filter_flag)
	{
		filters.filter_length++;
		if(filters.filter_length>=FS_FILTER)
		{
			filters.filter_length=0;
			filters.filter_flag=0;
		}
	}
	else
	{
		ad7606_data->ad7606_data_ch0.ad7606_data_buffer[cnt] = complement_to_source(data[0]);
		app_diag_env->raw_data[0].raw_buff[diag_cnt] = complement_to_source(data[0]);
		ad7606_data->ad7606_data_ch1.ad7606_data_buffer[cnt] = complement_to_source(data[1]);
		app_diag_env->raw_data[1].raw_buff[diag_cnt] = complement_to_source(data[1]);
		ad7606_data->ad7606_data_ch2.ad7606_data_buffer[cnt] = complement_to_source(data[2]);
		app_diag_env->raw_data[2].raw_buff[diag_cnt] = complement_to_source(data[2]);
		ad7606_data->ad7606_data_ch3.ad7606_data_buffer[cnt] = complement_to_source(data[3]);
		app_diag_env->raw_data[3].raw_buff[diag_cnt] = complement_to_source(data[3]);
#ifndef TFDSN_WAVE
		ad7606_data->ad7606_data_ch4.ad7606_data_buffer[cnt] = complement_to_source(data[4]);
		app_diag_env.raw_data[4].raw_buff[diag_cnt] = complement_to_source(data[4]);
#endif //TFDSN_WAVE
		//ad7606_data->ad7606_data_ch5.ad7606_data_buffer[cnt] = complement_to_source(data[5]);
		//app_diag_env.raw_data[5].raw_buff[diag_cnt] = complement_to_source(data[5]);
		cnt++;
		diag_cnt++;
		//rt_sem_release(diag_sem);
		if(cnt >= FS_AD7606)
		{
			adg1608_switch_all();
			cnt = 0;
//			app_diag_raw_data_node_construct();
			rt_event_send(ad7606_event, AD7606_EVENT_SEND_SAVE_FLAG);
			filters.filter_flag=1;
			vib_save_flag = 0;
		}
		if(diag_cnt >= APP_DIAG_SAMPLING_SIZE_MAX)
		{
			diag_cnt = 0;
			//if (test_cnt < 5)
			{
			    app_diag_raw_data_node_construct();
//				test_cnt ++;
			}
		}
	}
}
/**
 * @brief development description of class
 * *
 * @author  www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param               param   new node to be inserted
 * @param [ in or out ] param   new node to be inserted
 * *
 * @return
 *          - xxx   description or none
 *          - xxx   description ,the operation status or on successful
 * @retval
 *          - xxx   description
 * *
 * @see  reference @ad7606.c
 *
 * @note            开启/关闭 ad7606数据采集接口
 *
 * @warning         description
 *
 * Change Logs:
 * Date             author          notes
 * 2020年6月8日            www         create this class
 *
 */
void timer2_interrupt_deal(void)
{
	static uint16_t ad7606_value[8];

	rt_interrupt_enter();

	ad7606_read(device,ad7606_value);

	package_ad_data(ad7606_value);

	rt_interrupt_leave();
}

/** @} */ //\defgroup module API

/** @} */ //\addtogroup dev module

#endif //\end module API
#endif //#if 0
/* !< end file   */
