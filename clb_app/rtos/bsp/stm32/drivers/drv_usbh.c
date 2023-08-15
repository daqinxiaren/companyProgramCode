/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2017-10-30     ZYH            the first version
 * 2019-12-19     tyustli           port to stm32 series
 */

#include <drv_config.h>
#include "drv_usbh.h"
#ifdef ENABLE_USBH
static HCD_HandleTypeDef stm32_hhcd_fs;
static struct rt_completion urb_completion;
static volatile rt_bool_t connect_status = OS_FALSE;

void OTG_FS_IRQHandler(void)
{
    rt_interrupt_enter();
    HAL_HCD_IRQHandler(&stm32_hhcd_fs);
    rt_interrupt_leave();
}

void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    uhcd_t hcd = (uhcd_t)hhcd->pData;
    if (!connect_status)
    {
        connect_status = OS_TRUE;
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("usb connected\n"));
        rt_usbh_root_hub_connect_handler(hcd, OTG_FS_PORT, OS_FALSE);
    }
}

void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
    uhcd_t hcd = (uhcd_t)hhcd->pData;
    if (connect_status)
    {
        connect_status = OS_FALSE;
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("usb disconnnect\n"));
        rt_usbh_root_hub_disconnect_handler(hcd, OTG_FS_PORT);
    }
}

void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
    rt_completion_done(&urb_completion);
}

static rt_err_t drv_reset_port(rt_uint8_t port)
{
    ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("reset port\n"));
    HAL_HCD_ResetPort(&stm32_hhcd_fs);
    return OS_EOK;
}

static int drv_pipe_xfer(upipe_t pipe, rt_uint8_t token, void *buffer, int nbytes, int timeouts)
{
    int timeout = timeouts;

    while (1)
    {
        if (!connect_status)
        {
            return -1;
        }
        rt_completion_init(&urb_completion);
        HAL_HCD_HC_SubmitRequest(&stm32_hhcd_fs,
                                 pipe->pipe_index,
                                 (pipe->ep.bEndpointAddress & 0x80) >> 7,
                                 pipe->ep.bmAttributes,
                                 token,
                                 buffer,
                                 nbytes,
                                 0);
        rt_completion_wait(&urb_completion, timeout);
        rt_thread_mdelay(1);
        if (HAL_HCD_HC_GetState(&stm32_hhcd_fs, pipe->pipe_index) == HC_NAK)
        {
            ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("nak\n"));
            if (pipe->ep.bmAttributes == USB_EP_ATTR_INT)
            {
                rt_thread_delay((pipe->ep.bInterval * TICK_PER_SECOND / 1000) > 0 ? (pipe->ep.bInterval * TICK_PER_SECOND / 1000) : 1);
            }
            HAL_HCD_HC_Halt(&stm32_hhcd_fs, pipe->pipe_index);
            HAL_HCD_HC_Init(&stm32_hhcd_fs,
                            pipe->pipe_index,
                            pipe->ep.bEndpointAddress,
                            pipe->inst->address,
                            USB_OTG_SPEED_FULL,
                            pipe->ep.bmAttributes,
                            pipe->ep.wMaxPacketSize);
            continue;
        }
        else if (HAL_HCD_HC_GetState(&stm32_hhcd_fs, pipe->pipe_index) == HC_STALL)
        {
            ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("stall\n"));
            pipe->status = UPIPE_STATUS_STALL;
            if (pipe->callback != OS_NULL)
            {
                pipe->callback(pipe);
            }
            return -1;
        }
        else if (HAL_HCD_HC_GetState(&stm32_hhcd_fs, pipe->pipe_index) == URB_ERROR)
        {
            ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("error\n"));
            pipe->status = UPIPE_STATUS_ERROR;
            if (pipe->callback != OS_NULL)
            {
                pipe->callback(pipe);
            }
            return -1;
        }
        else if(URB_DONE == HAL_HCD_HC_GetURBState(&stm32_hhcd_fs, pipe->pipe_index))
        {
            ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("ok\n"));
            pipe->status = UPIPE_STATUS_OK;
            if (pipe->callback != OS_NULL)
            {
                pipe->callback(pipe);
            }
            size_t size = HAL_HCD_HC_GetXferCount(&stm32_hhcd_fs, pipe->pipe_index);
            if (pipe->ep.bEndpointAddress & 0x80)
            {
                return size;
            }
            else if (pipe->ep.bEndpointAddress & 0x00)
            {
                return size;
            }
            return nbytes;
        }

        continue;
    }
}

static rt_uint16_t pipe_index = 0;
static rt_uint8_t  drv_get_free_pipe_index(void)
{
    rt_uint8_t idx;
    for (idx = 1; idx < 16; idx++)
    {
        if (!(pipe_index & (0x01 << idx)))
        {
            pipe_index |= (0x01 << idx);
            return idx;
        }
    }
    return 0xff;
}

static void drv_free_pipe_index(rt_uint8_t index)
{
    pipe_index &= ~(0x01 << index);
}

static rt_err_t drv_open_pipe(upipe_t pipe)
{
    pipe->pipe_index = drv_get_free_pipe_index();
    HAL_HCD_HC_Init(&stm32_hhcd_fs,
                    pipe->pipe_index,
                    pipe->ep.bEndpointAddress,
                    pipe->inst->address,
                    USB_OTG_SPEED_FULL,
                    pipe->ep.bmAttributes,
                    pipe->ep.wMaxPacketSize);
    /* Set DATA0 PID token*/
    if (stm32_hhcd_fs.hc[pipe->pipe_index].ep_is_in)
    {
        stm32_hhcd_fs.hc[pipe->pipe_index].toggle_in = 0;
    }
    else
    {
        stm32_hhcd_fs.hc[pipe->pipe_index].toggle_out = 0;
    }
    return OS_EOK;
}

static rt_err_t drv_close_pipe(upipe_t pipe)
{
    HAL_HCD_HC_Halt(&stm32_hhcd_fs, pipe->pipe_index);
    drv_free_pipe_index(pipe->pipe_index);
    return OS_EOK;
}

static struct uhcd_ops _uhcd_ops =
{
    drv_reset_port,
    drv_pipe_xfer,
    drv_open_pipe,
    drv_close_pipe,
};

static rt_err_t stm32_hcd_init(rt_device_t device)
{
    HCD_HandleTypeDef *hhcd = (HCD_HandleTypeDef *)device->user_data;
    hhcd->Instance = USB_OTG_FS;
    hhcd->Init.Host_channels = 8;
    hhcd->Init.speed = HCD_SPEED_FULL;
    hhcd->Init.dma_enable = DISABLE;
    hhcd->Init.phy_itface = HCD_PHY_EMBEDDED;
    hhcd->Init.Sof_enable = DISABLE;
    OS_ASSERT(HAL_HCD_Init(hhcd) == HAL_OK);
    HAL_HCD_Start(hhcd);
#ifdef USBH_ENABLE_CONTROLLABLE_POWER
    rt_pin_mode(USBH_POWER_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(USBH_POWER_PIN, PIN_LOW);
#endif
    return OS_EOK;
}

int stm_usbh_register(void)
{
    rt_err_t res = -OS_ERROR;

    uhcd_t uhcd = (uhcd_t)rt_malloc(sizeof(struct uhcd));
    if (uhcd == OS_NULL)
    {
        rt_kprintf("uhcd malloc failed\r\n");
        return -OS_ERROR;
    }

    rt_memset((void *)uhcd, 0, sizeof(struct uhcd));

    uhcd->parent.type = DEVICE_CLASS_USB_HOST;
    uhcd->parent.init = stm32_hcd_init;
    uhcd->parent.user_data = &stm32_hhcd_fs;

    uhcd->ops = &_uhcd_ops;
    uhcd->num_ports = OTG_FS_PORT;
    stm32_hhcd_fs.pData = uhcd;

    res = rt_device_register(&uhcd->parent, "usbh", DEVICE_FLAG_DEACTIVATE);
    if (res != OS_EOK)
    {
        rt_kprintf("register usb host failed res = %d\r\n", res);
        return -OS_ERROR;
    }

    rt_usb_host_init();

    return OS_EOK;
}
INIT_DEVICE_EXPORT(stm_usbh_register);

#endif
