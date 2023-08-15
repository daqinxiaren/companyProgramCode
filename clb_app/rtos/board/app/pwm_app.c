/**
 *
 * Copyright (c) 2015-2020  LH  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the pwm_app.c API or library.
 *
 * @features			:
 *
 * @author       		: www
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020年7月10日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b>@note</b> 		: null
 *
 * \b <b> @warning </b> : null
 * 
 * *
 * Change Logs:
 *	Date				author       notes
 *	2020年7月10日				www         create this file
 *	2020年7月10日
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
 

/**
 ***************** include file ***************
 */
#include <rtconfig.h>
#include <drv_config.h>
#include <rtthread.h>
#include <stm32f4xx.h>
#include "drivers/pwm.h"
#include "rtdevice.h"
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
 ***************<Private define>****************
 */

#if defined(ENABLE_PWM1)
#define PWM_DEVICE   "pwm1"     /* 定时器名称 */
#define PWM_CH    1
#endif

/**
 **************<Private variables>**************
 */



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
 * @brief development description of class
 * *
 * @author	www  write time: 2020年7月10日
 * @create time  : 2020年7月10日
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
 * @note 			description
 *							
 * @warning			description 
 *	
 * Change Logs:
 * Date				author			notes
 * 2020年7月10日			www			create this class
 *						
 */


/*pwm初始化*/
int pwm_init(uint32_t period,uint32_t pulse)
{
    rt_err_t ret = OS_EOK;
    rt_device_t pwm_dev = OS_NULL;   /* 设备句柄 */

    struct rt_pwm_configuration configuration =
    {         .channel = PWM_CH,
              .period  = period,    /* */
              .pulse   = pulse     /* 50% duty */
    };

#if defined(BSP_ENABLE_PWM1)
    configuration.period=period*2;//timer1/timer8
#endif
    /* 查找设备 */
    pwm_dev = rt_device_find(PWM_DEVICE);
    if (pwm_dev == OS_NULL)
    {
        rt_kprintf(" can't find %s device!\n", PWM_DEVICE);
        return OS_ERROR;
    }

    /* 以读写方式打开设备 */
    ret = rt_device_open(pwm_dev, DEVICE_OFLAG_RDWR);
    if (ret != OS_EOK)
    {
        rt_kprintf("open %s device failed!\n", PWM_DEVICE);
        return ret;
    }

    ret=rt_device_write(pwm_dev, PWM_CH, &configuration, sizeof(struct rt_pwm_configuration));
    if (ret <0 )
    {
        rt_kprintf("write pwm channel %d: faild! \n", PWM_CH);
        return ret;
    }

    ret = rt_device_control(pwm_dev, PWM_CMD_SET, &configuration);
    if (ret != OS_EOK)
    {
        rt_kprintf("rt_device_control PWM_CMD_SET channel %d: faild! \n", PWM_CH);
        return ret;
    }

    ret = rt_device_control(pwm_dev, PWM_CMD_ENABLE, &configuration);
    if (ret != OS_EOK)
    {
        rt_kprintf("rt_device_control PWM_CMD_ENABLE channel %d: faild! \n", PWM_CH);
        return ret;
    }

    rt_kprintf("start %s: channel = %d, period = %d, pulse = %d \n",PWM_DEVICE, PWM_CH, period,pulse);
	
    return ret;
}




/** @} */ //\defgroup module API

/** @} */ //\addtogroup dev module

/* !< end file   */
