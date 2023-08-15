/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author        Notes
 * 2018-12-04   balanceTWK    first version
 * 2020-10-14     Dozingfiretruck   Porting for stm32wbxx
 */

#include <rtconfig.h>
#include "drv_config.h"

#ifdef ENABLE_ONCHIP_RTC

#ifndef RTC_BKP_DR1
#define RTC_BKP_DR1 OS_NULL
#endif

//#define DRV_DEBUG
#define LOG_TAG             "drv.rtc"
#include <drv_log.h>

#define BKUP_REG_DATA 0xA5A5

static struct rt_device rtc;

static RTC_HandleTypeDef RTC_Handler;

OS_WEAK uint32_t HAL_RTCEx_BKUPRead(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister)
{
    return (~BKUP_REG_DATA);
}

OS_WEAK void HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister, uint32_t Data)
{
    return;
}

static time_t get_rtc_timestamp(void)
{
    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_DateTypeDef RTC_DateStruct = {0};
    struct tm tm_new = {0};

    HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);

    tm_new.tm_sec  = RTC_TimeStruct.Seconds;
    tm_new.tm_min  = RTC_TimeStruct.Minutes;
    tm_new.tm_hour = RTC_TimeStruct.Hours;
    tm_new.tm_mday = RTC_DateStruct.Date;
    tm_new.tm_mon  = RTC_DateStruct.Month - 1;
    tm_new.tm_year = RTC_DateStruct.Year + 100;

    LOG_D("get rtc time.");
    return mktime(&tm_new);
}

static rt_err_t set_rtc_time_stamp(time_t time_stamp)
{
    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_DateTypeDef RTC_DateStruct = {0};
    struct tm *p_tm;

    p_tm = localtime(&time_stamp);
    if (p_tm->tm_year < 100)
    {
        return -OS_ERROR;
    }

    RTC_TimeStruct.Seconds = p_tm->tm_sec ;
    RTC_TimeStruct.Minutes = p_tm->tm_min ;
    RTC_TimeStruct.Hours   = p_tm->tm_hour;
    RTC_DateStruct.Date    = p_tm->tm_mday;
    RTC_DateStruct.Month   = p_tm->tm_mon + 1 ;
    RTC_DateStruct.Year    = p_tm->tm_year - 100;
    RTC_DateStruct.WeekDay = p_tm->tm_wday + 1;

    if (HAL_RTC_SetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN) != HAL_OK)
    {
        return -OS_ERROR;
    }
    if (HAL_RTC_SetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN) != HAL_OK)
    {
        return -OS_ERROR;
    }

    LOG_D("set rtc time.");
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR1, BKUP_REG_DATA);

#ifdef STM32F1XX
    /* F1 series does't save year/month/date datas. so keep those datas to bkp reg */
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR2, RTC_DateStruct.Year);
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR3, RTC_DateStruct.Month);
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR4, RTC_DateStruct.Date);
    HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR5, RTC_DateStruct.WeekDay);
#endif

    return OS_EOK;
}

static void rt_rtc_init(void)
{
#if !defined(STM32H7XX) && !defined(SOC_SERIES_STM32WB)
    __HAL_RCC_PWR_CLK_ENABLE();
#endif

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
#ifdef BSP_RTC_ENABLE_LSI
#ifdef SOC_SERIES_STM32WB
RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI1;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
#else
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
#endif
#else
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
#endif
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

#ifdef STM32F1XX
/* update RTC_BKP_DRx*/
static void rt_rtc_f1_bkp_update(void)
{
    RTC_DateTypeDef RTC_DateStruct = {0};

    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BKP_CLK_ENABLE();

    RTC_DateStruct.Year    = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR2);
    RTC_DateStruct.Month   = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR3);
    RTC_DateStruct.Date    = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR4);
    RTC_DateStruct.WeekDay = HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR5);
    if (HAL_RTC_SetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
    if (HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR4) != RTC_DateStruct.Date)
    {
        HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR1, BKUP_REG_DATA);
        HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR2, RTC_DateStruct.Year);
        HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR3, RTC_DateStruct.Month);
        HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR4, RTC_DateStruct.Date);
        HAL_RTCEx_BKUPWrite(&RTC_Handler, RTC_BKP_DR5, RTC_DateStruct.WeekDay);
    }
}
#endif

static rt_err_t rt_rtc_config(struct rt_device *dev)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    HAL_PWR_EnableBkUpAccess();
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
#ifdef BSP_RTC_ENABLE_LSI
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
#else
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
#endif
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    /* Enable RTC Clock */
    __HAL_RCC_RTC_ENABLE();

    RTC_Handler.Instance = RTC;
    if (HAL_RTCEx_BKUPRead(&RTC_Handler, RTC_BKP_DR1) != BKUP_REG_DATA)
    {
        LOG_I("RTC hasn't been configured, please use <date> command to config.");

#if defined(STM32F1XX)
        RTC_Handler.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
        RTC_Handler.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
#elif defined(STM32F0XX)

        /* set the frequency division */
#ifdef BSP_RTC_ENABLE_LSI
        RTC_Handler.Init.AsynchPrediv = 0XA0;
        RTC_Handler.Init.SynchPrediv = 0xFA;
#else
        RTC_Handler.Init.AsynchPrediv = 0X7F;
        RTC_Handler.Init.SynchPrediv = 0x0130;
#endif /* BSP_RTC_ENABLE_LSI */

        RTC_Handler.Init.HourFormat = RTC_HOURFORMAT_24;
        RTC_Handler.Init.OutPut = RTC_OUTPUT_DISABLE;
        RTC_Handler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
        RTC_Handler.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
#elif defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F7XX) || defined(STM32L4XX) || defined(STM32H7XX) || defined (SOC_SERIES_STM32WB)

        /* set the frequency division */
#ifdef BSP_RTC_ENABLE_LSI
        RTC_Handler.Init.AsynchPrediv = 0X7D;
#else
        RTC_Handler.Init.AsynchPrediv = 0X7F;
#endif /* BSP_RTC_ENABLE_LSI */
        RTC_Handler.Init.SynchPrediv = 0XFF;

        RTC_Handler.Init.HourFormat = RTC_HOURFORMAT_24;
        RTC_Handler.Init.OutPut = RTC_OUTPUT_DISABLE;
        RTC_Handler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
        RTC_Handler.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
#endif
        if (HAL_RTC_Init(&RTC_Handler) != HAL_OK)
        {
            return -OS_ERROR;
        }
        LOG_I("RTC BKUP_REG_DATA!.");
    }
#ifdef STM32F1XX
    else
    {
        /* F1 series need update by bkp reg datas */
        rt_rtc_f1_bkp_update();
    }
#endif

    return OS_EOK;
}

static rt_err_t rt_rtc_control(rt_device_t dev, int cmd, void *args)
{
    rt_err_t result = OS_EOK;
    OS_ASSERT(dev != OS_NULL);
    switch (cmd)
    {
    case DEVICE_CTRL_RTC_GET_TIME:
        *(rt_uint32_t *)args = get_rtc_timestamp();
        LOG_D("RTC: get rtc_time %x\n", *(rt_uint32_t *)args);
        //rt_kprintf("RTC: get rtc_time %x\n", *(rt_uint32_t *)args);
        break;

    case DEVICE_CTRL_RTC_SET_TIME:
        if (set_rtc_time_stamp(*(rt_uint32_t *)args))
        {
            result = -OS_ERROR;
        }
        LOG_D("RTC: set rtc_time %x\n", *(rt_uint32_t *)args);
        //rt_kprintf("RTC: set rtc_time %x\n", *(rt_uint32_t *)args);
        break;
    }

    return result;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops rtc_ops =
{
    OS_NULL,
    OS_NULL,
    OS_NULL,
    OS_NULL,
    OS_NULL,
    rt_rtc_control
};
#endif

//#ifdef ENABLE_HW_RTC
static rt_err_t rt_hw_rtc_register(rt_device_t device, const char *name, rt_uint32_t flag)
{
    OS_ASSERT(device != OS_NULL);
    rt_rtc_init();
    if (rt_rtc_config(device) != OS_EOK)
    {
        LOG_I("\tRTC config \t\t[error!]\r\n");
        return -OS_ERROR;
    }
#ifdef ENABLE_DEVICE_OPS
    device->ops         = &rtc_ops;
#else
    device->init        = OS_NULL;
    device->open        = OS_NULL;
    device->close       = OS_NULL;
    device->read        = OS_NULL;
    device->write       = OS_NULL;
    device->control     = rt_rtc_control;
#endif
    device->type        = DEVICE_CLASS_RTC;
    device->rx_indicate = OS_NULL;
    device->tx_complete = OS_NULL;
    device->user_data   = OS_NULL;

    /* register a character device */
    return rt_device_register(device, name, flag);
}

int rt_hw_rtc_init(void)
{
    rt_err_t result;
    result = rt_hw_rtc_register(&rtc, "rtc", DEVICE_FLAG_RDWR);
    if (result != OS_EOK)
    {
        LOG_E("hw rtc register err code: %d", result);
        return result;
    }

    LOG_I("\tRTC INIT \t\t[source!]\r\n");
    return OS_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_rtc_init);

//#endif

#endif /* ENABLE_ONCHIP_RTC */
