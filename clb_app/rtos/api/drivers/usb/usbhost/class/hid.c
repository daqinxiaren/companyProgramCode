/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-12-12     Yi Qiu      first version
 */

#include <rtthread.h>
#include <drivers/usb_host.h>
#include "hid.h"

#ifdef DEV_USBH_HID

static struct uclass_driver hid_driver;
static rt_list_t _protocal_list;

/**
 * This function will do USB_REQ_SET_IDLE request to set idle period to the usb hid device
 *
 * @param intf the interface instance.
 * @duration the idle period of requesting data.
 * @report_id the report id
 * 
 * @return the error code, OS_EOK on successfully.
*/
rt_err_t rt_usbh_hid_set_idle(struct uintf* intf, int duration, int report_id)
{
    struct urequest setup;
    struct uinstance* device;    
    int timeout = USB_TIMEOUT_BASIC;
        
    /* parameter check */
    OS_ASSERT(intf != OS_NULL);
    OS_ASSERT(intf->device != OS_NULL);

    device = intf->device;

    setup.request_type = USB_REQ_TYPE_DIR_OUT | USB_REQ_TYPE_CLASS | 
        USB_REQ_TYPE_INTERFACE;
    setup.request = USB_REQ_SET_IDLE;
    setup.index = 0;
    setup.length = 0;
    setup.value = (duration << 8 )| report_id;

    if(rt_usb_hcd_control_xfer(device->hcd, device, &setup, OS_NULL, 0, 
        timeout) == 0) return OS_EOK;
    else return -OS_FALSE;    
}

/**
 * This function will do USB_REQ_GET_REPORT request to get report from the usb hid device
 *
 * @param intf the interface instance.
 * @buffer the data buffer to save usb report descriptor.
 * @param nbytes the size of buffer
 * 
 * @return the error code, OS_EOK on successfully.
*/
rt_err_t rt_usbh_hid_get_report(struct uintf* intf, rt_uint8_t type, 
    rt_uint8_t id, rt_uint8_t *buffer, rt_size_t size)
{
    struct urequest setup;
    struct uinstance* device;
    int timeout = USB_TIMEOUT_BASIC;
        
    /* parameter check */
    OS_ASSERT(intf != OS_NULL);
    OS_ASSERT(intf->device != OS_NULL);

    device = intf->device;

    setup.request_type = USB_REQ_TYPE_DIR_IN | USB_REQ_TYPE_CLASS | 
        USB_REQ_TYPE_INTERFACE;
    setup.request = USB_REQ_GET_REPORT;
    setup.index = intf->intf_desc->bInterfaceNumber;
    setup.length = size;
    setup.value = (type << 8 ) + id;

    if(rt_usb_hcd_control_xfer(device->hcd, device, &setup, buffer, size, 
        timeout) == size) return OS_EOK;
    else return -OS_FALSE;    
}

/**
 * This function will do USB_REQ_SET_REPORT request to set report to the usb hid device
 *
 * @param intf the interface instance.
 * @buffer the data buffer to save usb report descriptor.
 * @param nbytes the size of buffer
 * 
 * @return the error code, OS_EOK on successfully.
*/
rt_err_t rt_usbh_hid_set_report(struct uintf* intf, rt_uint8_t *buffer, rt_size_t size)
{
    struct urequest setup;
    struct uinstance* device;        
    int timeout = USB_TIMEOUT_BASIC;
        
    /* parameter check */
    OS_ASSERT(intf != OS_NULL);
    OS_ASSERT(intf->device != OS_NULL);
    
    device = intf->device;

    setup.request_type = USB_REQ_TYPE_DIR_OUT | USB_REQ_TYPE_CLASS | 
        USB_REQ_TYPE_INTERFACE;
    setup.request = USB_REQ_SET_REPORT;
    setup.index = intf->intf_desc->bInterfaceNumber;
    setup.length = size;
    setup.value = 0x02 << 8;

    if(rt_usb_hcd_control_xfer(device->hcd, device, &setup, buffer, size, 
        timeout) == size) return OS_EOK;
    else return -OS_FALSE;    
}

/**
 * This function will do USB_REQ_SET_PROTOCOL request to set protocal to the usb hid device.
 *
 * @param intf the interface instance.
 * @param protocol the protocol id.
 * 
 * @return the error code, OS_EOK on successfully.
 */
rt_err_t rt_usbh_hid_set_protocal(struct uintf* intf, int protocol)
{
    struct urequest setup;
    struct uinstance* device;
    int timeout = USB_TIMEOUT_BASIC;
        
    /* parameter check */
    OS_ASSERT(intf != OS_NULL);
    OS_ASSERT(intf->device != OS_NULL);
    
    device = intf->device;

    setup.request_type = USB_REQ_TYPE_DIR_OUT | USB_REQ_TYPE_CLASS | 
        USB_REQ_TYPE_INTERFACE;
    setup.request = USB_REQ_SET_PROTOCOL;
    setup.index = 0;
    setup.length = 0;
    setup.value = protocol;

    if(rt_usb_hcd_control_xfer(device->hcd, device, &setup, OS_NULL, 0, 
        timeout) == 0) return OS_EOK;
    else return -OS_FALSE;    
}

/**
 * This function will do USB_REQ_GET_DESCRIPTOR request for the device instance 
 * to set feature of the hub port.
 *
 * @param intf the interface instance.
 * @buffer the data buffer to save usb report descriptor.
 * @param nbytes the size of buffer
 * 
 * @return the error code, OS_EOK on successfully.
 */
rt_err_t rt_usbh_hid_get_report_descriptor(struct uintf* intf, 
    rt_uint8_t *buffer, rt_size_t size)
{
    struct urequest setup;
    struct uinstance* device;    
    int timeout = USB_TIMEOUT_BASIC;
        
    /* parameter check */
    OS_ASSERT(intf != OS_NULL);
    OS_ASSERT(intf->device != OS_NULL);
    
    device = intf->device;

    setup.request_type = USB_REQ_TYPE_DIR_IN | USB_REQ_TYPE_STANDARD| 
        USB_REQ_TYPE_INTERFACE;
    setup.request = USB_REQ_GET_DESCRIPTOR;
    setup.index = 0;
    setup.length = size;
    setup.value = USB_DESC_TYPE_REPORT << 8;

    if(rt_usb_hcd_control_xfer(device->hcd, device, &setup, buffer, size, 
        timeout) == size) return OS_EOK;
    else return -OS_FALSE;
}

/**
 * This function will register specified hid protocal to protocal list
 *
 * @param protocal the specified protocal.
 * 
 * @return the error code, OS_EOK on successfully.
 */
rt_err_t rt_usbh_hid_protocal_register(uprotocal_t protocal)
{
    OS_ASSERT(protocal != OS_NULL);

    if (protocal == OS_NULL) return -OS_ERROR;

    /* insert class driver into driver list */
    rt_list_insert_after(&_protocal_list, &(protocal->list));
    
    return OS_EOK;    
}

/**
 * This function is the callback function of hid's int endpoint, it is invoked when data comes.
 *
 * @param context the context of the callback function.
 * 
 * @return none.
 */
static void rt_usbh_hid_callback(void* context)
{
    upipe_t pipe; 
    struct uhid* hid;
    int timeout = USB_TIMEOUT_LONG;

    /* parameter check */
    OS_ASSERT(context != OS_NULL);
    
    pipe = (upipe_t)context;
    hid = (struct uhid*)pipe->intf->user_data;

    /* invoke protocal callback function */
    hid->protocal->callback((void*)hid);

    /* parameter check */
     OS_ASSERT(pipe->intf->device->hcd != OS_NULL);

    rt_usb_hcd_int_xfer(pipe->intf->device->hcd, pipe, hid->buffer, 
        pipe->ep.wMaxPacketSize, timeout);
}

/**
 * This function will find specified hid protocal from protocal list
 *
 * @param pro_id the protocal id.
 * 
 * @return the found protocal or OS_NULL if there is no this protocal.
 */
static uprotocal_t rt_usbh_hid_protocal_find(int pro_id)
{
    struct rt_list_node *node;

    /* try to find protocal object */
    for (node = _protocal_list.next; node != &_protocal_list; node = node->next)
    {
        uprotocal_t protocal = 
            (uprotocal_t)rt_list_entry(node, struct uprotocal, list);
        if (protocal->pro_id == pro_id) return protocal;
    }

    /* not found */
    return OS_NULL;
}

/**
 * This function will run hid class driver when usb device is detected and identified
 *  as a hid class device, it will continue the enumulate process.
 *
 * @param arg the argument.
 * 
 * @return the error code, OS_EOK on successfully.
 */
static rt_err_t rt_usbh_hid_enable(void* arg)
{
    int i = 0, pro_id;
    uprotocal_t protocal;    
    struct uhid* hid;
    struct uintf* intf = (struct uintf*)arg;
    int timeout = USB_TIMEOUT_BASIC;
    upipe_t pipe;
    
    /* parameter check */
    if(intf == OS_NULL)
    {
        rt_kprintf("the interface is not available\n");
        return -OS_EIO;
    }

    pro_id = intf->intf_desc->bInterfaceProtocol;

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB,
                 ("HID device enable, protocal id %d\n", pro_id));   

    protocal = rt_usbh_hid_protocal_find(pro_id);    
    if(protocal == OS_NULL)
    {
        rt_kprintf("can't find hid protocal %d\n", pro_id);
        intf->user_data = OS_NULL;        
        return -OS_ERROR;
    }
    
    hid = rt_malloc(sizeof(struct uhid));
    OS_ASSERT(hid != OS_NULL);

    /* initilize the data structure */    
    rt_memset(hid, 0, sizeof(struct uhid));
    intf->user_data = (void*)hid;
    hid->protocal = protocal;
    
    for(i=0; i<intf->intf_desc->bNumEndpoints; i++)
    {    
        rt_err_t ret;
        uep_desc_t ep_desc;

        /* get endpoint descriptor */        
        rt_usbh_get_endpoint_descriptor(intf->intf_desc, i, &ep_desc);
        if(ep_desc == OS_NULL)
        {
            rt_kprintf("rt_usbh_get_endpoint_descriptor error\n");
            return -OS_ERROR;
        }
                
        if(USB_EP_ATTR(ep_desc->bmAttributes) != USB_EP_ATTR_INT) 
            continue;
    
        if(!(ep_desc->bEndpointAddress & USB_DIR_IN)) continue;

        ret = rt_usb_hcd_alloc_pipe(intf->device->hcd, &hid->pipe_in, 
            intf, ep_desc, rt_usbh_hid_callback);        
        if(ret != OS_EOK) return ret;
    }

    /* initialize hid protocal */
    hid->protocal->init((void*)intf);    
    pipe = hid->pipe_in;

    /* parameter check */
     OS_ASSERT(pipe->intf->device->hcd != OS_NULL);
    
    rt_usb_hcd_int_xfer(pipe->intf->device->hcd, hid->pipe_in, 
        hid->buffer, hid->pipe_in->ep.wMaxPacketSize, timeout);
    return OS_EOK;
}

/**
 * This function will be invoked when usb device plug out is detected and it would clean 
 * and release all hub class related resources.
 *
 * @param arg the argument.
 * 
 * @return the error code, OS_EOK on successfully.
 */
static rt_err_t rt_usbh_hid_disable(void* arg)
{
    struct uhid* hid;
    struct uintf* intf = (struct uintf*)arg;

    OS_ASSERT(intf != OS_NULL);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("rt_usbh_hid_disable\n"));

    hid = (struct uhid*)intf->user_data;
    if(hid != OS_NULL)
    {
        if(hid->pipe_in != OS_NULL)
        {
            /* free the HID in pipe */
            rt_usb_hcd_free_pipe(intf->device->hcd, hid->pipe_in);
        }

        /* free the hid instance */    
        rt_free(hid);
    }
    
    /* free the instance */
    rt_free(intf);

    return OS_EOK;
}

/**
 * This function will register hid class driver to the usb class driver manager.
 * and it should be invoked in the usb system initialization.
 * 
 * @return the error code, OS_EOK on successfully.
 */
ucd_t rt_usbh_class_driver_hid(void)
{
    rt_list_init(&_protocal_list);

    hid_driver.class_code = USB_CLASS_HID;
    
    hid_driver.enable = rt_usbh_hid_enable;
    hid_driver.disable = rt_usbh_hid_disable;

    return &hid_driver;
}

#endif

