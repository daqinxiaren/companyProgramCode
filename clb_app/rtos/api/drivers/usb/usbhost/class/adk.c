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
#include "adk.h"

#ifdef DEV_USBH_ADK

static struct uclass_driver adk_driver;
static const char* _adk_manufacturer = OS_NULL;
static const char* _adk_model = OS_NULL;
static const char* _adk_description = OS_NULL;
static const char* _adk_version = OS_NULL;
static const char* _adk_uri = OS_NULL;
static const char* _adk_serial = OS_NULL;

rt_err_t rt_usbh_adk_set_string(const char* manufacturer, const char* model,
    const char* description, const char* _version, const char* uri,
    const char* serial)
{
    _adk_manufacturer = manufacturer;
    _adk_model = model;
    _adk_description = description;
    _adk_version = _version;
    _adk_uri = uri;
    _adk_serial = serial;

    return OS_EOK;
}

#ifdef ENABLE_MODULE
#include <rtm.h>

OS_MODE_EXPORT(rt_usbh_adk_set_string);
#endif

/**
 * This function will do USB_REQ_GET_PROTOCOL request to set idle period to the usb adk device
 *
 * @param intf the interface instance.
 * @duration the idle period of requesting data.
 * @report_id the report id
 *
 * @return the error code, OS_EOK on successfully.
*/
static rt_err_t rt_usbh_adk_get_protocol(struct uintf* intf, rt_uint16_t *protocol)
{
    struct urequest setup;
    uinst_t device;
    int timeout = USB_TIMEOUT_BASIC;

        /* parameter check */
    OS_ASSERT(intf != OS_NULL);
    OS_ASSERT(intf->device != OS_NULL);

    device = intf->device;

    setup.request_type = USB_REQ_TYPE_DIR_IN | USB_REQ_TYPE_VENDOR |
        USB_REQ_TYPE_DEVICE;
    setup.request = USB_REQ_GET_PROTOCOL;
    setup.index = 0;
    setup.length = 2;
    setup.value = 0;

    if(rt_usb_hcd_control_xfer(device->hcd, device, &setup, (void*)protocol, 2,
        timeout) == 0) return OS_EOK;
    else return -OS_FALSE;
}

/**
 * This function will do USB_REQ_SEND_STRING request to set idle period to the usb adk device
 *
 * @param intf the interface instance.
 * @duration the idle period of requesting data.
 * @report_id the report id
 *
 * @return the error code, OS_EOK on successfully.
*/
static rt_err_t rt_usbh_adk_send_string(struct uintf* intf, rt_uint16_t index,
    const char* str)
{
    struct urequest setup;
    uinst_t device;
    int timeout = USB_TIMEOUT_BASIC;

        /* parameter check */
    OS_ASSERT(intf != OS_NULL);
    OS_ASSERT(intf->device != OS_NULL);

    device = intf->device;

    setup.request_type = USB_REQ_TYPE_DIR_OUT | USB_REQ_TYPE_VENDOR |
        USB_REQ_TYPE_DEVICE;
    setup.request = USB_REQ_SEND_STRING;
    setup.index = index;
    setup.length = rt_strlen(str) + 1;
    setup.value = 0;

    if(rt_usb_hcd_control_xfer(device->hcd, device, &setup, (void*)str,
        rt_strlen(str) + 1, timeout) == 0) return OS_EOK;
    else return -OS_FALSE;
}

/**
 * This function will do USB_REQ_START request to set idle period to the usb adk device
 *
 * @param intf the interface instance.
 * @duration the idle period of requesting data.
 * @report_id the report id
 *
 * @return the error code, OS_EOK on successfully.
*/
static rt_err_t rt_usbh_adk_start(struct uintf* intf)
{
    struct urequest setup;
    uinst_t device;
    int timeout = USB_TIMEOUT_BASIC;

        /* parameter check */
    OS_ASSERT(intf != OS_NULL);
    OS_ASSERT(intf->device != OS_NULL);

    device = intf->device;

    setup.request_type = USB_REQ_TYPE_DIR_OUT | USB_REQ_TYPE_VENDOR |
        USB_REQ_TYPE_DEVICE;
    setup.request = USB_REQ_START;
    setup.index = 0;
    setup.length = 0;
    setup.value = 0;

    if(rt_usb_hcd_control_xfer(device->hcd, device, &setup, OS_NULL, 0,
        timeout) == 0) return OS_EOK;
    else return -OS_FALSE;
}

/**
 * This function will read data from usb adk device
 *
 * @param intf the interface instance.
 *
 * @return the error code, OS_EOK on successfully.
*/
static rt_size_t rt_usbh_adk_read(rt_device_t device, rt_off_t pos, void* buffer,
    rt_size_t size)
{
    uadk_t adk;
    rt_size_t length;
    struct uintf* intf;

    /* check parameter */
    OS_ASSERT(device != OS_NULL);
    OS_ASSERT(buffer != OS_NULL);

    intf = (struct uintf*)device->user_data;
    adk = (uadk_t)intf->user_data;

    length = rt_usb_hcd_bulk_xfer(intf->device->hcd, adk->pipe_in,
        buffer, size, 300);

    return length;

}

/**
 * This function will write data to usb adk device
 *
 * @param intf the interface instance.
 *
 * @return the error code, OS_EOK on successfully.
*/
static rt_size_t rt_usbh_adk_write (rt_device_t device, rt_off_t pos, const void* buffer,
    rt_size_t size)
{
    uadk_t adk;
    rt_size_t length;
    struct uintf* intf;

    OS_ASSERT(buffer != OS_NULL);

    intf = (struct uintf*)device->user_data;
    adk = (uadk_t)intf->user_data;

    length = rt_usb_hcd_bulk_xfer(intf->device->hcd, adk->pipe_out,
        (void*)buffer, size, 300);

    return length;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops adk_device_ops =
{
    OS_NULL;
    OS_NULL;
    OS_NULL;
    rt_usbh_adk_read;
    rt_usbh_adk_write;
    OS_NULL;
};
#endif

/**
 * This function will run adk class driver when usb device is detected and identified
 *  as a adk class device, it will continue the enumulate process.
 *
 * @param arg the argument.
 *
 * @return the error code, OS_EOK on successfully.
 */
static rt_err_t rt_usbh_adk_enable(void* arg)
{
    int i = 0;
    uadk_t adk;
    struct uintf* intf = (struct uintf*)arg;
    udev_desc_t dev_desc;
    rt_uint16_t protocol;
    rt_err_t ret;

    /* parameter check */
    if(intf == OS_NULL)
    {
        rt_kprintf("the interface is not available\n");
        return -OS_EIO;
    }

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("rt_usbh_adk_run\n"));

    dev_desc = &intf->device->dev_desc;
    if(dev_desc->idVendor == USB_ACCESSORY_VENDOR_ID &&
        (dev_desc->idProduct == USB_ACCESSORY_PRODUCT_ID ||
        dev_desc->idProduct == USB_ACCESSORY_ADB_PRODUCT_ID))
    {
        if(intf->intf_desc->bInterfaceSubClass != 0xFF) return -OS_ERROR;

        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("found android accessory device\n"));
    }
    else
    {
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("switch device\n"));

        if((ret = rt_usbh_adk_get_protocol(intf, &protocol)) != OS_EOK)
        {
            rt_kprintf("rt_usbh_adk_get_protocol failed\n");
            return ret;
        }

        if(protocol != 1)
        {
            rt_kprintf("read protocol failed\n");
            return -OS_ERROR;
        }

        rt_usbh_adk_send_string(intf,
            ACCESSORY_STRING_MANUFACTURER, _adk_manufacturer);
        rt_usbh_adk_send_string(intf,
            ACCESSORY_STRING_MODEL, _adk_model);
        rt_usbh_adk_send_string(intf,
            ACCESSORY_STRING_DESCRIPTION, _adk_description);
        rt_usbh_adk_send_string(intf,
            ACCESSORY_STRING_VERSION, _adk_version);
        rt_usbh_adk_send_string(intf,
            ACCESSORY_STRING_URI, _adk_uri);
        rt_usbh_adk_send_string(intf,
            ACCESSORY_STRING_SERIAL, _adk_serial);

        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("manufacturer %s\n", _adk_manufacturer));
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("model %s\n", _adk_model));
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("description %s\n", _adk_description));
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("version %s\n", _adk_version));
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("uri %s\n", _adk_uri));
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("serial %s\n", _adk_serial));

        if((ret = rt_usbh_adk_start(intf)) != OS_EOK)
        {
            rt_kprintf("rt_usbh_adk_start failed\n");
            return ret;
        }

        return OS_EOK;
    }

    adk = rt_malloc(sizeof(struct uadkinst));
    OS_ASSERT(adk != OS_NULL);

    /* initilize the data structure */
    rt_memset(adk, 0, sizeof(struct uadkinst));
    intf->user_data = (void*)adk;

    for(i=0; i<intf->intf_desc->bNumEndpoints; i++)
    {
        uep_desc_t ep_desc;

        /* get endpoint descriptor from interface descriptor */
        rt_usbh_get_endpoint_descriptor(intf->intf_desc, i, &ep_desc);
        if(ep_desc == OS_NULL)
        {
            rt_kprintf("rt_usb_get_endpoint_descriptor error\n");
            return -OS_ERROR;
        }

        /* the endpoint type of adk class should be BULK */
        if((ep_desc->bmAttributes & USB_EP_ATTR_TYPE_MASK) != USB_EP_ATTR_BULK)
            continue;

        /* allocate pipes according to the endpoint type */
        if(ep_desc->bEndpointAddress & USB_DIR_IN)
        {
            /* allocate an in pipe for the adk instance */
            ret = rt_usb_hcd_alloc_pipe(intf->device->hcd, &adk->pipe_in,
                intf, ep_desc, OS_NULL);
            if(ret != OS_EOK) return ret;
        }
        else
        {
            /* allocate an output pipe for the adk instance */
            ret = rt_usb_hcd_alloc_pipe(intf->device->hcd, &adk->pipe_out,
                intf, ep_desc, OS_NULL);
            if(ret != OS_EOK) return ret;
        }
    }

    /* check pipes infomation */
    if(adk->pipe_in == OS_NULL || adk->pipe_out == OS_NULL)
    {
        rt_kprintf("pipe error, unsupported device\n");
        return -OS_ERROR;
    }

    /* set configuration */
    ret = rt_usbh_set_configure(intf->device, 1);
    if(ret != OS_EOK) return ret;

    /* register adk device */
    adk->device.type    = DEVICE_CLASS_CHAR;
#ifdef ENABLE_DEVICE_OPS
    adk->device.ops     = &adk_device_ops;
#else
    adk->device.init    = OS_NULL;
    adk->device.open    = OS_NULL;
    adk->device.close   = OS_NULL;
    adk->device.read    = rt_usbh_adk_read;
    adk->device.write   = rt_usbh_adk_write;
    adk->device.control = OS_NULL;
#endif
    adk->device.user_data = (void*)intf;

    rt_device_register(&adk->device, "adkdev", DEVICE_FLAG_RDWR);

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
static rt_err_t rt_usbh_adk_disable(void* arg)
{
    uadk_t adk;
    struct uintf* intf = (struct uintf*)arg;

    OS_ASSERT(intf != OS_NULL);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_USB, ("rt_usbh_adk_stop\n"));

    adk = (uadk_t)intf->user_data;
    if(adk == OS_NULL)
    {
        rt_free(intf);
        return OS_EOK;
    }

    if(adk->pipe_in != OS_NULL)
        rt_usb_hcd_free_pipe(intf->device->hcd, adk->pipe_in);

    if(adk->pipe_out != OS_NULL)
        rt_usb_hcd_free_pipe(intf->device->hcd, adk->pipe_out);

    /* unregister adk device */
    rt_device_unregister(&adk->device);

    /* free adk instance */
    if(adk != OS_NULL)
    {
        rt_free(adk);
    }

    /* free interface instance */
    rt_free(intf);

    return OS_EOK;
}

/**
 * This function will register adk class driver to the usb class driver manager.
 * and it should be invoked in the usb system initialization.
 *
 * @return the error code, OS_EOK on successfully.
 */
ucd_t rt_usbh_class_driver_adk(void)
{
    adk_driver.class_code = USB_CLASS_ADK;

    adk_driver.enable = rt_usbh_adk_enable;
    adk_driver.disable = rt_usbh_adk_disable;

    return &adk_driver;
}

#endif

