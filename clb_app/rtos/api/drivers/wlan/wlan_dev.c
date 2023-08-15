/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-03     tyx          the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <wlan_dev.h>
#include <wlan_prot.h>

#define DBG_TAG "WLAN.dev"
#ifdef WLAN_DEV_DEBUG
#define DBG_LVL DBG_LOG
#else
#define DBG_LVL DBG_INFO
#endif /* WLAN_DEV_DEBUG */
#include <rtdbg.h>

#if defined(ENABLE_WIFI) || defined(ENABLE_WLAN)

#ifndef RT_DEVICE
#define RT_DEVICE(__device) ((rt_device_t)__device)
#endif

#define WLAN_DEV_LOCK(_wlan)      (rt_mutex_take(&(_wlan)->lock, WAITING_FOREVER))
#define WLAN_DEV_UNLOCK(_wlan)    (rt_mutex_release(&(_wlan)->lock))

#if WLAN_SSID_MAX_LENGTH < 1
#error "SSID length is too short"
#endif

#if WLAN_BSSID_MAX_LENGTH < 1
#error "BSSID length is too short"
#endif

#if WLAN_PASSWORD_MAX_LENGTH < 1
#error "password length is too short"
#endif

#if WLAN_DEV_EVENT_NUM < 2
#error "dev num Too little"
#endif

rt_err_t rt_wlan_dev_init(struct rt_wlan_device *device, rt_wlan_mode_t mode)
{
    rt_err_t result = OS_EOK;

    /* init wlan device */
    LOG_D("F:%s L:%d is run device:0x%08x mode:%d", __FUNCTION__, __LINE__, device, mode);
    if ((device == OS_NULL) || (mode >= WLAN_MODE_MAX))
    {
        LOG_E("F:%s L:%d Parameter Wrongful device:0x%08x mode:%d", __FUNCTION__, __LINE__, device, mode);
        return -OS_ERROR;
    }

    if (mode == WLAN_AP && device->flags & WLAN_FLAG_STA_ONLY)
    {
        LOG_E("F:%s L:%d This wlan device can only be set to sta mode!", __FUNCTION__, __LINE__);
        return -OS_ERROR;
    }
    else if (mode == WLAN_STATION && device->flags & WLAN_FLAG_AP_ONLY)
    {
        LOG_E("F:%s L:%d This wlan device can only be set to ap mode!", __FUNCTION__, __LINE__);
        return -OS_ERROR;
    }

    result = rt_device_init(RT_DEVICE(device));
    if (result != OS_EOK)
    {
        LOG_E("L:%d wlan init failed", __LINE__);
        return -OS_ERROR;
    }
    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_MODE, (void *)&mode);
    if (result != OS_EOK)
    {
        LOG_E("L:%d wlan config mode failed", __LINE__);
        return -OS_ERROR;
    }
    device->mode = mode;
    return result;
}

rt_err_t rt_wlan_dev_connect(struct rt_wlan_device *device, struct rt_wlan_info *info, const char *password, int password_len)
{
    rt_err_t result = OS_EOK;
    struct rt_sta_info sta_info;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }
    if (info == OS_NULL)
    {
        return -OS_ERROR;
    }

    if ((password_len > WLAN_PASSWORD_MAX_LENGTH) ||
            (info->ssid.len > WLAN_SSID_MAX_LENGTH))
    {
        LOG_E("L:%d password or ssid is too long", __LINE__);
        return -OS_ERROR;
    }
    rt_memset(&sta_info, 0, sizeof(struct rt_sta_info));
    rt_memcpy(&sta_info.ssid, &info->ssid, sizeof(rt_wlan_ssid_t));
    rt_memcpy(sta_info.bssid, info->bssid, WLAN_BSSID_MAX_LENGTH);
    if (password != OS_NULL)
    {
        rt_memcpy(sta_info.key.val, password, password_len);
        sta_info.key.len = password_len;
    }
    sta_info.channel = info->channel;
    sta_info.security = info->security;

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_JOIN, &sta_info);
    return result;
}

rt_err_t rt_wlan_dev_disconnect(struct rt_wlan_device *device)
{
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_DISCONNECT, OS_NULL);
    return result;
}

rt_err_t rt_wlan_dev_ap_start(struct rt_wlan_device *device, struct rt_wlan_info *info, const char *password, int password_len)
{
    rt_err_t result = OS_EOK;
    struct rt_ap_info ap_info;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }
    if (info == OS_NULL)
    {
        return -OS_ERROR;
    }

    if ((password_len > WLAN_PASSWORD_MAX_LENGTH) ||
            (info->ssid.len > WLAN_SSID_MAX_LENGTH))
    {
        LOG_E("L:%d password or ssid is too long", __LINE__);
        return -OS_ERROR;
    }

    rt_memset(&ap_info, 0, sizeof(struct rt_ap_info));
    rt_memcpy(&ap_info.ssid, &info->ssid, sizeof(rt_wlan_ssid_t));
    if (password != OS_NULL)
    {
        rt_memcpy(ap_info.key.val, password, password_len);
    }
    ap_info.key.len = password_len;
    ap_info.hidden = info->hidden;
    ap_info.channel = info->channel;
    ap_info.security = info->security;

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_SOFTAP, &ap_info);
    return result;
}

rt_err_t rt_wlan_dev_ap_stop(struct rt_wlan_device *device)
{
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_AP_STOP, OS_NULL);
    return result;
}

rt_err_t rt_wlan_dev_ap_deauth(struct rt_wlan_device *device, rt_uint8_t mac[6])
{
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_AP_DEAUTH, mac);
    return result;
}

int rt_wlan_dev_get_rssi(struct rt_wlan_device *device)
{
    int rssi = 0;
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        rt_set_errno(-OS_EIO);
        return 0;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_GET_RSSI, &rssi);
    if (result != OS_EOK)
    {
        rt_set_errno(result);
        return 0;
    }

    return rssi;
}

rt_err_t rt_wlan_dev_get_mac(struct rt_wlan_device *device, rt_uint8_t mac[6])
{
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_GET_MAC, &mac[0]);
    return result;
}

rt_err_t rt_wlan_dev_set_mac(struct rt_wlan_device *device, rt_uint8_t mac[6])
{
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_SET_MAC, &mac[0]);
    return result;
}

rt_err_t rt_wlan_dev_set_powersave(struct rt_wlan_device *device, int level)
{
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_SET_POWERSAVE, &level);
    return result;
}

int rt_wlan_dev_get_powersave(struct rt_wlan_device *device)
{
    int level = -1;
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        rt_set_errno(-OS_EIO);
        return -1;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_GET_POWERSAVE, &level);
    if (result != OS_EOK)
    {
        rt_set_errno(result);
    }

    return level;
}

rt_err_t rt_wlan_dev_register_event_handler(struct rt_wlan_device *device, rt_wlan_dev_event_t event, rt_wlan_dev_event_handler handler, void *parameter)
{
    int i = 0;
    rt_base_t level;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }
    if (event >= WLAN_DEV_EVT_MAX)
    {
        return -OS_EINVAL;
    }

    level = rt_hw_interrupt_disable();
    for (i = 0; i < WLAN_DEV_EVENT_NUM; i++)
    {
        if (device->handler_table[event][i].handler == OS_NULL)
        {
            device->handler_table[event][i].handler = handler;
            device->handler_table[event][i].parameter = parameter;
            rt_hw_interrupt_enable(level);
            return OS_EOK;
        }
    }
    rt_hw_interrupt_enable(level);

    /* No space found */
    return -OS_ERROR;
}

rt_err_t rt_wlan_dev_unregister_event_handler(struct rt_wlan_device *device, rt_wlan_dev_event_t event, rt_wlan_dev_event_handler handler)
{
    int i = 0;
    rt_base_t level;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }
    if (event >= WLAN_DEV_EVT_MAX)
    {
        return -OS_EINVAL;
    }

    level = rt_hw_interrupt_disable();
    for (i = 0; i < WLAN_DEV_EVENT_NUM; i++)
    {
        if (device->handler_table[event][i].handler == handler)
        {
            rt_memset(&device->handler_table[event][i], 0, sizeof(struct rt_wlan_dev_event_desc));
            rt_hw_interrupt_enable(level);
            return OS_EOK;
        }
    }
    rt_hw_interrupt_enable(level);
    /* not find iteam */
    return -OS_ERROR;
}

void rt_wlan_dev_indicate_event_handle(struct rt_wlan_device *device, rt_wlan_dev_event_t event, struct rt_wlan_buff *buff)
{
    void *parameter[WLAN_DEV_EVENT_NUM];
    rt_wlan_dev_event_handler handler[WLAN_DEV_EVENT_NUM];
    int i;
    rt_base_t level;

    if (device == OS_NULL)
    {
        return;
    }
    if (event >= WLAN_DEV_EVT_MAX)
    {
        return;
    }

    /* get callback handle */
    level = rt_hw_interrupt_disable();
    for (i = 0; i < WLAN_DEV_EVENT_NUM; i++)
    {
        handler[i] = device->handler_table[event][i].handler;
        parameter[i] = device->handler_table[event][i].parameter;
    }
    rt_hw_interrupt_enable(level);

    /* run callback */
    for (i = 0; i < WLAN_DEV_EVENT_NUM; i++)
    {
        if (handler[i] != OS_NULL)
        {
            handler[i](device, event, buff, parameter[i]);
        }
    }
}

rt_err_t rt_wlan_dev_enter_promisc(struct rt_wlan_device *device)
{
    rt_err_t result = OS_EOK;
    int enable = 1;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_CFG_PROMISC, &enable);
    return result;
}

rt_err_t rt_wlan_dev_exit_promisc(struct rt_wlan_device *device)
{
    rt_err_t result = OS_EOK;
    int enable = 0;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_CFG_PROMISC, &enable);
    return result;
}

rt_err_t rt_wlan_dev_set_promisc_callback(struct rt_wlan_device *device, rt_wlan_pormisc_callback_t callback)
{
    if (device == OS_NULL)
    {
        return -OS_EIO;
    }
    device->pormisc_callback = callback;

    return OS_EOK;
}

void rt_wlan_dev_promisc_handler(struct rt_wlan_device *device, void *data, int len)
{
    rt_wlan_pormisc_callback_t callback;

    if (device == OS_NULL)
    {
        return;
    }

    callback = device->pormisc_callback;

    if (callback != OS_NULL)
    {
        callback(device, data, len);
    }
}

rt_err_t rt_wlan_dev_cfg_filter(struct rt_wlan_device *device, struct rt_wlan_filter *filter)
{
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }
    if (filter == OS_NULL)
    {
        return -OS_ERROR;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_CFG_FILTER, filter);
    return result;
}

rt_err_t rt_wlan_dev_set_channel(struct rt_wlan_device *device, int channel)
{
    rt_err_t result = OS_EOK;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }
    if (channel < 0)
    {
        return -OS_ERROR;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_SET_CHANNEL, &channel);
    return result;
}

int rt_wlan_dev_get_channel(struct rt_wlan_device *device)
{
    rt_err_t result = OS_EOK;
    int channel = -1;

    if (device == OS_NULL)
    {
        rt_set_errno(-OS_EIO);
        return -1;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_GET_CHANNEL, &channel);
    if (result != OS_EOK)
    {
        rt_set_errno(result);
        return -1;
    }

    return channel;
}

rt_err_t rt_wlan_dev_set_country(struct rt_wlan_device *device, rt_country_code_t country_code)
{
    int result = OS_EOK;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_SET_COUNTRY, &country_code);
    return result;
}

rt_country_code_t rt_wlan_dev_get_country(struct rt_wlan_device *device)
{
    int result = OS_EOK;
    rt_country_code_t country_code = DEV_COUNTRY_UNKNOWN;

    if (device == OS_NULL)
    {
        rt_set_errno(-OS_EIO);
        return DEV_COUNTRY_UNKNOWN;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_GET_COUNTRY, &country_code);
    if (result != OS_EOK)
    {
        rt_set_errno(result);
        return DEV_COUNTRY_UNKNOWN;
    }

    return country_code;
}

rt_err_t rt_wlan_dev_scan(struct rt_wlan_device *device, struct rt_wlan_info *info)
{
    struct rt_scan_info scan_info = { 0 };
    struct rt_scan_info *p_scan_info = OS_NULL;
    rt_err_t result = 0;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    if (info != OS_NULL)
    {
        if (info->ssid.len > WLAN_SSID_MAX_LENGTH)
        {
            LOG_E("L:%d ssid is too long", __LINE__);
            return -OS_EINVAL;
        }
        rt_memcpy(&scan_info.ssid, &info->ssid, sizeof(rt_wlan_ssid_t));
        rt_memcpy(scan_info.bssid, info->bssid, WLAN_BSSID_MAX_LENGTH);
        if (info->channel > 0)
        {
            scan_info.channel_min = info->channel;
            scan_info.channel_max = info->channel;
        }
        else
        {
            scan_info.channel_min = -1;
            scan_info.channel_max = -1;
        }
        scan_info.passive = info->hidden ? OS_TRUE : OS_FALSE;
        p_scan_info = &scan_info;
    }
    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_SCAN, p_scan_info);
    return result;
}

rt_err_t rt_wlan_dev_scan_stop(struct rt_wlan_device *device)
{
    rt_err_t result = 0;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_SCAN_STOP, OS_NULL);
    return result;
}

rt_err_t rt_wlan_dev_report_data(struct rt_wlan_device *device, void *buff, int len)
{
#ifdef WLAN_PROT_ENABLE
    return rt_wlan_dev_transfer_prot(device, buff, len);
#else
    return -OS_ERROR;
#endif
}

rt_err_t rt_wlan_dev_enter_mgnt_filter(struct rt_wlan_device *device)
{
    rt_err_t result = OS_EOK;
    int enable = 1;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_CFG_MGNT_FILTER, &enable);
    return result;
}

rt_err_t rt_wlan_dev_exit_mgnt_filter(struct rt_wlan_device *device)
{
    rt_err_t result = OS_EOK;
    int enable = 0;

    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    result = rt_device_control(RT_DEVICE(device), WLAN_CMD_CFG_MGNT_FILTER, &enable);
    return result;
}

rt_err_t rt_wlan_dev_set_mgnt_filter_callback(struct rt_wlan_device *device, rt_wlan_mgnt_filter_callback_t callback)
{
    if (device == OS_NULL)
    {
        return -OS_EIO;
    }
    device->mgnt_filter_callback = callback;

    return OS_EOK;
}

void rt_wlan_dev_mgnt_filter_handler(struct rt_wlan_device *device, void *data, int len)
{
    rt_wlan_mgnt_filter_callback_t callback;

    if (device == OS_NULL)
    {
        return;
    }

    callback = device->mgnt_filter_callback;

    if (callback != OS_NULL)
    {
        callback(device, data, len);
    }
}

int rt_wlan_dev_send_raw_frame(struct rt_wlan_device *device, void *buff, int len)
{
    if (device == OS_NULL)
    {
        return -OS_EIO;
    }

    if (device->ops->wlan_send_raw_frame)
    {
        return device->ops->wlan_send_raw_frame(device, buff, len);
    }

    return -OS_ERROR;
}

static rt_err_t _rt_wlan_dev_init(rt_device_t dev)
{
    struct rt_wlan_device *wlan = (struct rt_wlan_device *)dev;
    rt_err_t result = OS_EOK;

    rt_mutex_init(&wlan->lock, "wlan_dev", IPC_FLAG_FIFO);

    if (wlan->ops->wlan_init)
        result = wlan->ops->wlan_init(wlan);

    if (result == OS_EOK)
    {
        LOG_I("wlan init success");
    }
    else
    {
        LOG_I("wlan init failed");
    }

    return result;
}

static rt_err_t _rt_wlan_dev_control(rt_device_t dev, int cmd, void *args)
{
    struct rt_wlan_device *wlan = (struct rt_wlan_device *)dev;
    rt_err_t err = OS_EOK;

    OS_ASSERT(dev != OS_NULL);

    WLAN_DEV_LOCK(wlan);

    switch (cmd)
    {
    case WLAN_CMD_MODE:
    {
        rt_wlan_mode_t mode = *((rt_wlan_mode_t *)args);

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_MODE, "WLAN_CMD_MODE");
        if (wlan->ops->wlan_mode)
            err = wlan->ops->wlan_mode(wlan, mode);
        break;
    }
    case WLAN_CMD_SCAN:
    {
        struct rt_scan_info *scan_info = args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_SCAN, "WLAN_CMD_SCAN");
        if (wlan->ops->wlan_scan)
            err = wlan->ops->wlan_scan(wlan, scan_info);
        break;
    }
    case WLAN_CMD_JOIN:
    {
        struct rt_sta_info *sta_info = args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_JOIN, "WLAN_CMD_JOIN");
        if (wlan->ops->wlan_join)
            err = wlan->ops->wlan_join(wlan, sta_info);
        break;
    }
    case WLAN_CMD_SOFTAP:
    {
        struct rt_ap_info *ap_info = args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_SOFTAP, "WLAN_CMD_SOFTAP");
        if (wlan->ops->wlan_softap)
            err = wlan->ops->wlan_softap(wlan, ap_info);
        break;
    }
    case WLAN_CMD_DISCONNECT:
    {
        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_DISCONNECT, "WLAN_CMD_DISCONNECT");
        if (wlan->ops->wlan_disconnect)
            err = wlan->ops->wlan_disconnect(wlan);
        break;
    }
    case WLAN_CMD_AP_STOP:
    {
        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_AP_STOP, "WLAN_CMD_AP_STOP");
        if (wlan->ops->wlan_ap_stop)
            err = wlan->ops->wlan_ap_stop(wlan);
        break;
    }
    case WLAN_CMD_AP_DEAUTH:
    {
        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_AP_DEAUTH, "WLAN_CMD_AP_DEAUTH");
        if (wlan->ops->wlan_ap_deauth)
            err = wlan->ops->wlan_ap_deauth(wlan, args);
        break;
    }
    case WLAN_CMD_SCAN_STOP:
    {
        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_SCAN_STOP, "WLAN_CMD_SCAN_STOP");
        if (wlan->ops->wlan_scan_stop)
            err = wlan->ops->wlan_scan_stop(wlan);
        break;
    }
    case WLAN_CMD_GET_RSSI:
    {
        int *rssi = args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_GET_RSSI, "WLAN_CMD_GET_RSSI");
        if (wlan->ops->wlan_get_rssi)
            *rssi = wlan->ops->wlan_get_rssi(wlan);
        break;
    }
    case WLAN_CMD_SET_POWERSAVE:
    {
        int level = *((int *)args);

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_SET_POWERSAVE, "WLAN_CMD_SET_POWERSAVE");
        if (wlan->ops->wlan_set_powersave)
            err = wlan->ops->wlan_set_powersave(wlan, level);
        break;
    }
    case WLAN_CMD_GET_POWERSAVE:
    {
        int *level = args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_GET_POWERSAVE, "WLAN_CMD_GET_POWERSAVE");
        if (wlan->ops->wlan_get_powersave)
            *level = wlan->ops->wlan_get_powersave(wlan);
        break;
    }
    case WLAN_CMD_CFG_PROMISC:
    {
        rt_bool_t start = *((rt_bool_t *)args);

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_CFG_PROMISC, "WLAN_CMD_CFG_PROMISC");
        if (wlan->ops->wlan_cfg_promisc)
            err = wlan->ops->wlan_cfg_promisc(wlan, start);
        break;
    }
    case WLAN_CMD_CFG_FILTER:
    {
        struct rt_wlan_filter *filter = args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_CFG_FILTER, "WLAN_CMD_CFG_FILTER");
        if (wlan->ops->wlan_cfg_filter)
            err = wlan->ops->wlan_cfg_filter(wlan, filter);
        break;
    }
    case WLAN_CMD_CFG_MGNT_FILTER:
    {
        rt_bool_t start = *((rt_bool_t *)args);

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_CFG_MGNT_FILTER, "WLAN_CMD_CFG_MGNT_FILTER");
        if (wlan->ops->wlan_cfg_mgnt_filter)
            err = wlan->ops->wlan_cfg_mgnt_filter(wlan, start);
        break;
    }
    case WLAN_CMD_SET_CHANNEL:
    {
        int channel = *(int *)args;
        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_SET_CHANNEL, "WLAN_CMD_SET_CHANNEL");
        if (wlan->ops->wlan_set_channel)
            err = wlan->ops->wlan_set_channel(wlan, channel);
        break;
    }
    case WLAN_CMD_GET_CHANNEL:
    {
        int *channel = args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_GET_CHANNEL, "WLAN_CMD_GET_CHANNEL");
        if (wlan->ops->wlan_get_channel)
            *channel = wlan->ops->wlan_get_channel(wlan);
        break;
    }
    case WLAN_CMD_SET_COUNTRY:
    {
        rt_country_code_t country = *(rt_country_code_t *)args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_SET_COUNTRY, "WLAN_CMD_SET_COUNTRY");
        if (wlan->ops->wlan_set_country)
            err = wlan->ops->wlan_set_country(wlan, country);
        break;
    }
    case WLAN_CMD_GET_COUNTRY:
    {
        rt_country_code_t *country = args;
        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_GET_COUNTRY, "WLAN_CMD_GET_COUNTRY");
        if (wlan->ops->wlan_get_country)
            *country = wlan->ops->wlan_get_country(wlan);
        break;
    }
    case WLAN_CMD_SET_MAC:
    {
        rt_uint8_t *mac = args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_SET_MAC, "WLAN_CMD_SET_MAC");
        if (wlan->ops->wlan_set_mac)
            err = wlan->ops->wlan_set_mac(wlan, mac);
        break;
    }
    case WLAN_CMD_GET_MAC:
    {
        rt_uint8_t *mac = args;

        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, WLAN_CMD_GET_MAC, "WLAN_CMD_GET_MAC");
        if (wlan->ops->wlan_get_mac)
            err = wlan->ops->wlan_get_mac(wlan, mac);
        break;
    }
    default:
        LOG_D("%s %d cmd[%d]:%s  run......", __FUNCTION__, __LINE__, -1, "UNKUOWN");
        break;
    }

    WLAN_DEV_UNLOCK(wlan);

    return err;
}

#ifdef ENABLE_DEVICE_OPS
const static struct rt_device_ops wlan_ops =
{
    _rt_wlan_dev_init,
    OS_NULL,
    OS_NULL,
    OS_NULL,
    OS_NULL,
    _rt_wlan_dev_control
};
#endif

rt_err_t rt_wlan_dev_register(struct rt_wlan_device *wlan, const char *name, const struct rt_wlan_dev_ops *ops, rt_uint32_t flag, void *user_data)
{
    rt_err_t err = OS_EOK;

    if ((wlan == OS_NULL) || (name == OS_NULL) || (ops == OS_NULL) ||
        (flag & WLAN_FLAG_STA_ONLY && flag & WLAN_FLAG_AP_ONLY))
    {
        LOG_E("F:%s L:%d parameter Wrongful", __FUNCTION__, __LINE__);
        return OS_NULL;
    }

    rt_memset(wlan, 0, sizeof(struct rt_wlan_device));

#ifdef ENABLE_DEVICE_OPS
    wlan->device.ops = &wlan_ops;
#else
    wlan->device.init       = _rt_wlan_dev_init;
    wlan->device.open       = OS_NULL;
    wlan->device.close      = OS_NULL;
    wlan->device.read       = OS_NULL;
    wlan->device.write      = OS_NULL;
    wlan->device.control    = _rt_wlan_dev_control;
#endif

    wlan->device.user_data  = OS_NULL;

    wlan->device.type = DEVICE_CLASS_NETIF;

    wlan->ops = ops;
    wlan->user_data  = user_data;

    wlan->flags = flag;
    err = rt_device_register(&wlan->device, name, DEVICE_FLAG_RDWR);

    LOG_D("F:%s L:%d run", __FUNCTION__, __LINE__);

    return err;
}

#endif
