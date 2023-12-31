/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-01-03     Yi Qiu      first version
 */

#include <rtthread.h>
#include <drivers/usb_host.h>
#include "hid.h"

#ifdef ENABLE_GUI
#include <rtgui/event.h>
#include <rtgui/rtgui_server.h>
#include "drv_lcd.h"
#endif

#if defined(DEV_USBH_HID) && defined(DEV_USBH_HID_MOUSE)
static struct uprotocal mouse_protocal;

#ifdef ENABLE_GUI
#define LKEY_PRESS 0x01
#define RKEY_PRESS 0x02
#define MKEY_PRESS 0x04
#define MOUSE_SCALING 0x02

static rt_bool_t lkey_down=OS_FALSE;
//static rt_bool_t rkey_down=OS_FALSE;
//static rt_bool_t mkey_down=OS_FALSE;
static struct rtgui_event_mouse emouse;
#endif

static rt_err_t rt_usbh_hid_mouse_callback(void* arg)
{
    struct uhid* hid;
#ifdef ENABLE_GUI
    rt_uint16_t xoffset=0;
    rt_uint16_t yoffset=0;
#endif
    hid = (struct uhid*)arg;

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("hid 0x%x 0x%x\n",
                                *(rt_uint32_t*)hid->buffer,
                                *(rt_uint32_t*)(&hid->buffer[4])));
#ifdef ENABLE_GUI
    if(hid->buffer[1]!=0)
    {
        if(hid->buffer[1]>127)
        {
            xoffset=(256-hid->buffer[1])*MOUSE_SCALING;
            if(emouse.x>xoffset)
            {
                emouse.x-=xoffset;
            }
            else
            {
                emouse.x=0;
            }
        }
        else
        {
            xoffset=(hid->buffer[1])*MOUSE_SCALING;
            if((emouse.x+xoffset)<480)
            {
                emouse.x+=xoffset;
            }
            else
            {
                emouse.x=480;
            }
        }
    }
    if(hid->buffer[2]!=0)
    {

        if(hid->buffer[2]>127)
        {
            yoffset=(256-hid->buffer[2])*MOUSE_SCALING;
            if(emouse.y>yoffset)
            {
                emouse.y-=yoffset;
            }
            else
            {
                emouse.y=0;
            }
        }
        else
        {
            yoffset=hid->buffer[2]*MOUSE_SCALING;
            if(emouse.y+yoffset<272)
            {
                emouse.y+=yoffset;
            }
            else
            {
                emouse.y=272;
            }
        }
    }
    if(xoffset!=0||yoffset!=0)
    {
        cursor_set_position(emouse.x,emouse.y);
    }
    if(hid->buffer[0]&LKEY_PRESS)
    {
        if(lkey_down==OS_FALSE)
        {
            // rt_kprintf("mouse left key press down\n");
            emouse.button = (GUI_MOUSE_BUTTON_LEFT | GUI_MOUSE_BUTTON_DOWN);
            rtgui_server_post_event(&emouse.parent, sizeof(struct rtgui_event_mouse));
            lkey_down=OS_TRUE;
        }
    }
    else if(lkey_down==OS_TRUE)
    {
        // rt_kprintf("mouse left key press up\n");
        emouse.button = (GUI_MOUSE_BUTTON_LEFT | GUI_MOUSE_BUTTON_UP);
        rtgui_server_post_event(&emouse.parent, sizeof(struct rtgui_event_mouse));
        lkey_down=OS_FALSE;
    }
#endif
    return OS_EOK;
}

static rt_err_t rt_usbh_hid_mouse_init(void* arg)
{
    struct uintf* intf = (struct uintf*)arg;

    OS_ASSERT(intf != OS_NULL);

    rt_usbh_hid_set_protocal(intf, 0);

    rt_usbh_hid_set_idle(intf, 10, 0);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("start usb mouse\n"));
#ifdef ENABLE_GUI
    GUI_EVENT_MOUSE_BUTTON_INIT(&emouse);
    emouse.wid = OS_NULL;
    cursor_display(OS_TRUE);
#endif
    return OS_EOK;
}

/**
 * This function will define the hid mouse protocal, it will be register to the protocal list.
 *
 * @return the keyboard protocal structure.
 */
uprotocal_t rt_usbh_hid_protocal_mouse(void)
{
    mouse_protocal.pro_id = USB_HID_MOUSE;

    mouse_protocal.init = rt_usbh_hid_mouse_init;
    mouse_protocal.callback = rt_usbh_hid_mouse_callback;

    return &mouse_protocal;
}

#endif

