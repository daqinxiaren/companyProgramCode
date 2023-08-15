/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-23     balanceTWK   first version
 */

#include "drv_config.h"
#ifdef ENABLE_PULSE_ENCODER

//#define DRV_DEBUG
#define LOG_TAG             "drv.pulse_encoder"
#include <drv_log.h>

#if !defined(ENABLE_PULSE_ENCODER1) && !defined(ENABLE_PULSE_ENCODER2) && !defined(ENABLE_PULSE_ENCODER3) \
    && !defined(ENABLE_PULSE_ENCODER4) && !defined(ENABLE_PULSE_ENCODER5) && !defined(ENABLE_PULSE_ENCODER6)
    #error "Please define at least one ENABLE_PULSE_ENCODERx"
    /* this driver can be disabled at menuconfig → RT-Thread Components → Device Drivers */
#endif

#define AUTO_RELOAD_VALUE 0x7FFF

enum
{
#ifdef ENABLE_PULSE_ENCODER1
    PULSE_ENCODER1_INDEX,
#endif
#ifdef ENABLE_PULSE_ENCODER2
    PULSE_ENCODER2_INDEX,
#endif
#ifdef ENABLE_PULSE_ENCODER3
    PULSE_ENCODER3_INDEX,
#endif
#ifdef ENABLE_PULSE_ENCODER4
    PULSE_ENCODER4_INDEX,
#endif
#ifdef ENABLE_PULSE_ENCODER5
    PULSE_ENCODER5_INDEX,
#endif
#ifdef ENABLE_PULSE_ENCODER6
    PULSE_ENCODER6_INDEX,
#endif
};

struct stm32_pulse_encoder_device
{
    struct rt_pulse_encoder_device pulse_encoder;
    TIM_HandleTypeDef tim_handler;
    IRQn_Type encoder_irqn;
    rt_int32_t over_under_flowcount;
    char *name;
};

static struct stm32_pulse_encoder_device stm32_pulse_encoder_obj[] =
{
#ifdef ENABLE_PULSE_ENCODER1
    PULSE_ENCODER1_CONFIG,
#endif
#ifdef ENABLE_PULSE_ENCODER2
    PULSE_ENCODER2_CONFIG,
#endif
#ifdef ENABLE_PULSE_ENCODER3
    PULSE_ENCODER3_CONFIG,
#endif
#ifdef ENABLE_PULSE_ENCODER4
    PULSE_ENCODER4_CONFIG,
#endif
#ifdef ENABLE_PULSE_ENCODER5
    PULSE_ENCODER5_CONFIG,
#endif
#ifdef ENABLE_PULSE_ENCODER6
    PULSE_ENCODER6_CONFIG,
#endif
};

rt_err_t pulse_encoder_init(struct rt_pulse_encoder_device *pulse_encoder)
{
    TIM_Encoder_InitTypeDef sConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    struct stm32_pulse_encoder_device *stm32_device;
    stm32_device = (struct stm32_pulse_encoder_device*)pulse_encoder;

    stm32_device->tim_handler.Init.Prescaler = 0;
    stm32_device->tim_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    stm32_device->tim_handler.Init.Period = AUTO_RELOAD_VALUE;
    stm32_device->tim_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    stm32_device->tim_handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 3;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 3;

    if (HAL_TIM_Encoder_Init(&stm32_device->tim_handler, &sConfig) != HAL_OK)
    {
        LOG_E("pulse_encoder init failed");
        return -OS_ERROR;
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&stm32_device->tim_handler, &sMasterConfig))
    {
        LOG_E("TIMx master config failed");
        return -OS_ERROR;
    }
    else
    {
        HAL_NVIC_SetPriority(stm32_device->encoder_irqn, 3, 0);

        /* enable the TIMx global Interrupt */
        HAL_NVIC_EnableIRQ(stm32_device->encoder_irqn);

        /* clear update flag */
        __HAL_TIM_CLEAR_FLAG(&stm32_device->tim_handler, TIM_FLAG_UPDATE);
        /* enable update request source */
        __HAL_TIM_URS_ENABLE(&stm32_device->tim_handler);
    }

    return OS_EOK;
}

rt_err_t pulse_encoder_clear_count(struct rt_pulse_encoder_device *pulse_encoder)
{
    struct stm32_pulse_encoder_device *stm32_device;
    stm32_device = (struct stm32_pulse_encoder_device*)pulse_encoder;
    stm32_device->over_under_flowcount = 0;
    __HAL_TIM_SET_COUNTER(&stm32_device->tim_handler, 0);
    return OS_EOK;
}

rt_int32_t pulse_encoder_get_count(struct rt_pulse_encoder_device *pulse_encoder)
{
    struct stm32_pulse_encoder_device *stm32_device;
    stm32_device = (struct stm32_pulse_encoder_device*)pulse_encoder;
    return (rt_int32_t)((rt_int16_t)__HAL_TIM_GET_COUNTER(&stm32_device->tim_handler) + stm32_device->over_under_flowcount * AUTO_RELOAD_VALUE);
}

rt_err_t pulse_encoder_control(struct rt_pulse_encoder_device *pulse_encoder, rt_uint32_t cmd, void *args)
{
    rt_err_t result;
    struct stm32_pulse_encoder_device *stm32_device;
    stm32_device = (struct stm32_pulse_encoder_device*)pulse_encoder;

    result = OS_EOK;

    switch (cmd)
    {
    case PULSE_ENCODER_CMD_ENABLE:
        HAL_TIM_Encoder_Start(&stm32_device->tim_handler, TIM_CHANNEL_ALL);
        HAL_TIM_Encoder_Start_IT(&stm32_device->tim_handler, TIM_CHANNEL_ALL);
        break;
    case PULSE_ENCODER_CMD_DISABLE:
        HAL_TIM_Encoder_Stop(&stm32_device->tim_handler, TIM_CHANNEL_ALL);
        HAL_TIM_Encoder_Stop_IT(&stm32_device->tim_handler, TIM_CHANNEL_ALL);
        break;
    default:
        result = -OS_ENOSYS;
        break;
    }

    return result;
}

void pulse_encoder_update_isr(struct stm32_pulse_encoder_device *device)
{
    /* TIM Update event */
    if (__HAL_TIM_GET_FLAG(&device->tim_handler, TIM_FLAG_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&device->tim_handler, TIM_IT_UPDATE);
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&device->tim_handler))
        {
            device->over_under_flowcount--;
        }
        else
        {
            device->over_under_flowcount++;
        }
    }
    /* Capture compare 1 event */
    if (__HAL_TIM_GET_FLAG(&device->tim_handler, TIM_FLAG_CC1) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&device->tim_handler, TIM_IT_CC1);
    }
    /* Capture compare 2 event */
    if (__HAL_TIM_GET_FLAG(&device->tim_handler, TIM_FLAG_CC2) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&device->tim_handler, TIM_IT_CC2);
    }
    /* Capture compare 3 event */
    if (__HAL_TIM_GET_FLAG(&device->tim_handler, TIM_FLAG_CC3) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&device->tim_handler, TIM_IT_CC3);
    }
    /* Capture compare 4 event */
    if (__HAL_TIM_GET_FLAG(&device->tim_handler, TIM_FLAG_CC4) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&device->tim_handler, TIM_IT_CC4);
    }
    /* TIM Break input event */
    if (__HAL_TIM_GET_FLAG(&device->tim_handler, TIM_FLAG_BREAK) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&device->tim_handler, TIM_IT_BREAK);
    }
    /* TIM Trigger detection event */
    if (__HAL_TIM_GET_FLAG(&device->tim_handler, TIM_FLAG_TRIGGER) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&device->tim_handler, TIM_IT_TRIGGER);
    }
    /* TIM commutation event */
    if (__HAL_TIM_GET_FLAG(&device->tim_handler, TIM_FLAG_COM) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&device->tim_handler, TIM_FLAG_COM);
    }
}

#ifdef ENABLE_PULSE_ENCODER1
#if defined(STM32F4XX)
void TIM1_UP_TIM10_IRQHandler(void)
#elif defined(STM32F1XX)
void TIM1_UP_IRQHandler(void)
#else
    #error "Please check TIM1's IRQHandler"
#endif
{
    /* enter interrupt */
    rt_interrupt_enter();
    pulse_encoder_update_isr(&stm32_pulse_encoder_obj[PULSE_ENCODER1_INDEX]);
    /* leave interrupt */
    rt_interrupt_leave();
}
#endif
#ifdef ENABLE_PULSE_ENCODER2
void TIM2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    pulse_encoder_update_isr(&stm32_pulse_encoder_obj[PULSE_ENCODER2_INDEX]);
    /* leave interrupt */
    rt_interrupt_leave();
}
#endif
#ifdef ENABLE_PULSE_ENCODER3
void TIM3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    pulse_encoder_update_isr(&stm32_pulse_encoder_obj[PULSE_ENCODER3_INDEX]);
    /* leave interrupt */
    rt_interrupt_leave();
}
#endif
#ifdef ENABLE_PULSE_ENCODER4
void TIM4_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    pulse_encoder_update_isr(&stm32_pulse_encoder_obj[PULSE_ENCODER4_INDEX]);
    /* leave interrupt */
    rt_interrupt_leave();
}
#endif

static const struct rt_pulse_encoder_ops _ops =
{
    .init = pulse_encoder_init,
    .get_count = pulse_encoder_get_count,
    .clear_count = pulse_encoder_clear_count,
    .control = pulse_encoder_control,
};

int hw_pulse_encoder_init(void)
{
    int i;
    int result;

    result = OS_EOK;
    for (i = 0; i < sizeof(stm32_pulse_encoder_obj) / sizeof(stm32_pulse_encoder_obj[0]); i++)
    {
        stm32_pulse_encoder_obj[i].pulse_encoder.type = AB_PHASE_PULSE_ENCODER;
        stm32_pulse_encoder_obj[i].pulse_encoder.ops = &_ops;

        if (rt_device_pulse_encoder_register(&stm32_pulse_encoder_obj[i].pulse_encoder, stm32_pulse_encoder_obj[i].name, OS_NULL) != OS_EOK)
        {
            LOG_E("%s register failed", stm32_pulse_encoder_obj[i].name);
            result = -OS_ERROR;
        }
    }

    return result;
}
INIT_BOARD_EXPORT(hw_pulse_encoder_init);

#endif
