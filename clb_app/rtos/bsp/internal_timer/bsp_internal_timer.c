/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* include */

#include <drv_config.h>

#if defined(ENABLE_TIM)
#include <drivers/hwtimer.h>
#include <timer_app.h>
#include "ad7606_app.h"
#include "bsp_internal_timer.h"

/* macro */

/* variable declaration */

/* function delcaration */

/**
 * @brief  定时器初始化
 * @param
 * @return if ok return 0 else fail
 * @note
**/
int bsp_internal_timer_init(uint8_t sec, uint32_t usec, \
		                      rt_err_t (*timeout_cb)(rt_device_t dev, rt_size_t size), \
		                      void (*capture_cb)(void))
{
    rt_err_t ret = OS_EOK;
    rt_hwtimerval_t timeout_s;      /* 定时器超时值 */
    rt_device_t hw_dev = OS_NULL;   /* 定时器设备句柄 */
    rt_hwtimer_mode_t mode;         /* 定时器模式 */

    /* 查找定时器设备 */
    hw_dev = rt_device_find(BSP_HWTIMER_DEV_NAME);
    if (hw_dev == OS_NULL)
    {
        rt_kprintf("hwtimer sample run failed! can't find %s device!\n", BSP_HWTIMER_DEV_NAME);
        return OS_ERROR;
    }

    /* 以读写方式打开设备 */
    ret = rt_device_open(hw_dev, DEVICE_OFLAG_RDWR);
    if (ret != OS_EOK)
    {
        rt_kprintf("open %s device failed!\n", BSP_HWTIMER_DEV_NAME);
        return ret;
    }

    /* 设置超时回调函数 */
    rt_device_set_rx_indicate(hw_dev, timeout_cb);

    /* 设置模式为周期性定时器 */
    mode = HWTIMER_MODE_PERIOD;
    ret = rt_device_control(hw_dev, HWTIMER_CTRL_MODE_SET, &mode);
    if (ret != OS_EOK)
    {
        rt_kprintf("set mode failed! ret is :%d\n", ret);
        return ret;
    }

    /* 设置定时器超时值为并启动定时器 */
    timeout_s.sec = sec;      /* 秒 */
    timeout_s.usec = usec;     /* 微秒 */

    if (rt_device_write(hw_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(timeout_s))
    {
        rt_kprintf("set timeout value failed\n");
        return OS_ERROR;
    }

    /* 延时3500ms */
    rt_thread_mdelay(3500);

    /* 读取定时器当前值 */
    rt_device_read(hw_dev, 0, &timeout_s, sizeof(timeout_s));
    rt_kprintf("start  %s!\n", BSP_HWTIMER_DEV_NAME);

    return ret;
}

/**
 * @brief  使能定时器IT
 * @param
 * @return if ok return 0 else fail
 * @note
**/
void bsp_internal_enable_timx_it()
{
    TIM_HandleTypeDef time;
    time.Instance = BSP_HWTIMER_TIMX;
    HAL_TIM_Base_Start_IT(&time);
}

/**
 * @brief  关闭定时器IT
 * @param
 * @return if ok return 0 else fail
 * @note
**/
void bsp_internal_disable_timx_it()
{
    TIM_HandleTypeDef time;
    time.Instance = BSP_HWTIMER_TIMX;
    HAL_TIM_Base_Stop_IT(&time);
}

#endif
