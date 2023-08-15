/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/**
 ***************** include file ***************
 */
#include <rtconfig.h>
#include <drv_config.h>
#include <rtthread.h>
#include <stm32f4xx.h>
#include "drivers/pwm.h"
#include "rtdevice.h"

/* macro */
#if defined(ENABLE_PWM1)
#define APP_PWM_DEVICE      "pwm1"     /* 定时器名称 */
#define APP_PWM_CH          1
#endif

/* variable declaration */

/* function delcaration */
/**
 * @brief  pwm初始化
 * @param
 * @return if ok return 0 else fail
 * @note
**/
int app_pwm_init(uint32_t period,uint32_t pulse)
{
    rt_err_t ret = OS_EOK;
    rt_device_t pwm_dev = OS_NULL;   /* 设备句柄 */

    struct rt_pwm_configuration configuration =
    {         .channel = APP_PWM_CH,
              .period  = period,    /* */
              .pulse   = pulse     /* 50% duty */
    };

#if defined(BSP_ENABLE_PWM1)
    configuration.period = period * 2;//timer1/timer8
#endif
    /* 查找设备 */
    pwm_dev = rt_device_find(APP_PWM_DEVICE);
    if (pwm_dev == OS_NULL)
    {
        rt_kprintf(" can't find %s device!\n", APP_PWM_DEVICE);
        return OS_ERROR;
    }

    /* 以读写方式打开设备 */
    ret = rt_device_open(pwm_dev, DEVICE_OFLAG_RDWR);
    if (ret != OS_EOK)
    {
        rt_kprintf("open %s device failed!\n", APP_PWM_DEVICE);
        return ret;
    }

    ret=rt_device_write(pwm_dev, APP_PWM_CH, &configuration, sizeof(struct rt_pwm_configuration));
    if (ret <0 )
    {
        rt_kprintf("write pwm channel %d: faild! \n", APP_PWM_CH);
        return ret;
    }

    ret = rt_device_control(pwm_dev, PWM_CMD_SET, &configuration);
    if (ret != OS_EOK)
    {
        rt_kprintf("rt_device_control PWM_CMD_SET channel %d: faild! \n", APP_PWM_CH);
        return ret;
    }

    ret = rt_device_control(pwm_dev, PWM_CMD_ENABLE, &configuration);
    if (ret != OS_EOK)
    {
        rt_kprintf("rt_device_control PWM_CMD_ENABLE channel %d: faild! \n", APP_PWM_CH);
        return ret;
    }

    rt_kprintf("start %s: channel = %d, period = %d, pulse = %d \n", APP_PWM_DEVICE, APP_PWM_CH, period, pulse);
	
    return ret;
}

/* !< end file   */
