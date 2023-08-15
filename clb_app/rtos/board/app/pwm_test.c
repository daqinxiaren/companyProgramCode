/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		   : file for the pwm_test.c API or library.
 *
 * @features			   : description
 *
 * @author       		   : kernel <kernel_sa@163.com> 
 * 
 * @version      		   : V 1.0.0
 *
 * @date         		   : May 11, 2021
 *
 * @environment  		   : null
 *
 * @description  		   : null 
 *
 * @module       		   : null
 *
 * \n <b> @NOTE </b> 	: null
 *
 * \b @Warning      		: null
 * 
 * *
 * Change Logs:
 * Date           author       notes
 * May 11, 2021		   kernel       create this file
 */

/**
 *************************< include file >***************************
 */
#include <rtthread.h>
#include <rtdevice.h>


//#ifdef  XXX    /**< XXX module api major */

/**
 * \defgroup module API
 *
 * The xxx API module defines a set of functions that a xxx device
 * driver must implement.
 * "@addtogroup group name"
 * 
 * @{
 */

/**
 * @formatter:off
 *************************< private macro >**************************
 */
/**
 * @name public variables
 * @{
 **/
#define PWM_DEV_NAME        "pwm1" /* PWM设 备 名 称 */
#define PWM_DEV_CHANNEL     1 << 0/* PWM通 道 */


struct rt_device_pwm *pwm_dev; /* PWM设 备 句 柄 */
/**
 * xxx  macro define	
 *                    - description 
 * @param  	xxx      :- description.
 * @return  			 - description.
 */
//#define desc_class(xxx, xxxx) 							           "\"
//		  	for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/**
 * @formatter:on 
 *************************< private typedef >***********************
 */




/**
 *************************< private variables >*********************
 */




/** 
 * @} //public variables
 **/   

/**
 *************************< private functions >*********************
 */





/**
 *************************< public functions >**********************
 * @defgroup xxx Utils 
 * @{
 */

/**
 * @brief development description of class
 * *
 * Parameters
 * *
 * @param  xxx	      	new node to be inserted
 * @param [ in or out ]	xxx	new node to be inserted
 * * 
 * @return 
 *			- xxx   description or none,the operation status or on successful.
 * @retval
 *			- xxx description
 *			- xxx description
 * *
 * @see  		reference @file
 * @example		eg:
 * 
 * *
 * @remarks		- description
 * 
 * @pre		   - description
 * 
 * @deprecated	- description
 * 
 * @note 		- description
 *							
 * @warning		- description 
 *	
 * Change Logs:
 * Date			   author		notes
 * May 11, 2021			kernel		create this class
 *						
 */

static void pwm_led_thread_entry(void *parameter)
{
    rt_uint32_t period, pulse, dir;
    period = 500000; /* 周 期 为0.5ms， 单 位 为 纳 秒ns */
    dir = 1; /* PWM脉 冲 宽 度 值 的 增 减 方 向 */
    pulse = 0; /* PWM脉 冲 宽 度 值， 单 位 为 纳秒ns */

    /* 查 找 设 备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == OS_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return;// OS_ERROR;
    }
    rt_kprintf("pwm sample run ! find %s device!\n", PWM_DEV_NAME);
    /* 设 置PWM周 期 和 脉 冲 宽 度 默 认 值 */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    /* 使 能 设 备 */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
    while (1)
    {
        rt_thread_mdelay(50);
        if (dir)
        {
            pulse += 5000; /* 从0值 开 始 每 次 增加5000ns */
        }
        else
        {
            pulse -= 5000; /* 从 最 大 值 开 始 每 次减 少5000ns */
        }
        if (pulse >= period)
        {
            dir = 0;
        }
        if (0 == pulse)
        {
            dir = 1;
        }
        /* 设 置PWM周 期 和 脉 冲 宽 度 */
        rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    }
}

static int pwm_led_init(void)
{
    static rt_thread_t tid_pwmled = OS_NULL;

    rt_err_t ret = OS_EOK;

/* 创建 serial 线程 */
    tid_pwmled = rt_thread_create("t_pwmled",pwm_led_thread_entry, OS_NULL, 512, 24, 10);
   /* 创建成功则启动线程 */
   if (tid_pwmled != OS_NULL)
   {
       rt_thread_startup(tid_pwmled);
   }
   else
   {
       ret = OS_ERROR;
   }

   return ret;

}


//INIT_APP_EXPORT(pwm_led_init);


/** 
 * @} //xxx Utils 
 **/   


/** 
 * end defgroup module API
 * @}  
 */ 

//#endif  /**< end XXX module api major */
