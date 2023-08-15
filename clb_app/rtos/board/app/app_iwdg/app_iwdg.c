/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* include */

#include <rtconfig.h>
#if defined(ENABLE_IWDG)
//#include <iwdg_app.h>
#include "rtdevice.h"
#include "app_iwdg.h"

/* macro */
#define APP_IWDG_DEV_NAME   "wdt"     /* 名称 */

/* variable declaration */
static rt_device_t app_iwdg_dev;   /* 设备句柄 */

/* function delcaration */
/**
 * @brief  初始化,设置超时时间
 * @param
 * @return if ok return 0 else fail
 * @note
**/
int app_iwdg_init(rt_uint32_t timeout_s)
{
    rt_err_t ret = OS_EOK;
    /* 查找定时器设备 */
    app_iwdg_dev = rt_device_find(APP_IWDG_DEV_NAME);
    if (app_iwdg_dev == OS_NULL)
    {
        rt_kprintf("iwdg_dev sample run failed! can't find %s device!\n", APP_IWDG_DEV_NAME);
        return OS_ERROR;
    }

	ret = rt_device_init(app_iwdg_dev);
    if (ret != OS_EOK)
    {
        rt_kprintf("initialize %s failed!\n", APP_IWDG_DEV_NAME);
        return OS_ERROR;
    }


    /* 设置超时 */
    if(timeout_s>32)
    {
        timeout_s=32;
    }
    ret = rt_device_control(app_iwdg_dev, DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout_s);
    if (ret != OS_EOK)
    {
        rt_kprintf("set mode failed! ret is :%d\n", ret);
        return ret;
    }

    /* 开启看门狗 */
    ret = rt_device_control(app_iwdg_dev, DEVICE_CTRL_WDT_START, OS_NULL);
    if (ret != OS_EOK)
    {
        rt_kprintf("set mode failed! ret is :%d\n", ret);
        return ret;
    }
    rt_kprintf("start iwdg!\n");
    return ret;
}



/*feed dog*/
void app_feed_dog(void)
{
    rt_err_t ret = OS_EOK;
    if (app_iwdg_dev != OS_NULL)
    {
        ret = rt_device_control(app_iwdg_dev, DEVICE_CTRL_WDT_KEEPALIVE, OS_NULL);
        if (ret != OS_EOK)
        {
            rt_kprintf("set mode failed! ret is :%d\n", ret);

        }
    }
	else
		rt_kprintf("iwdg_dev is NULL\n");

}

#if 0
/*Check if the system has resumed from IWDG reset*/
int get_system_resumed(void)
{
    int ret=0;
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
    {
      /* IWDGRST flag set*/
        ret=1;
      /* Clear reset flags */
      __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    return ret;
}
#endif

#endif
