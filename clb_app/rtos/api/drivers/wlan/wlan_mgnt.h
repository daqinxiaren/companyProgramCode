/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-06     tyx          the first version
 */

#ifndef __WLAN_MGNT_H__
#define __WLAN_MGNT_H__

#include <wlan_dev.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WLAN_SCAN_WAIT_MS
#define WLAN_SCAN_WAIT_MS       (10 * 1000)
#endif

#ifndef WLAN_SCAN_CACHE_NUM
#define WLAN_SCAN_CACHE_NUM     (50)
#endif

#ifndef WLAN_CONNECT_WAIT_MS
#define WLAN_CONNECT_WAIT_MS    (10 * 1000)
#endif

#ifndef WLAN_START_AP_WAIT_MS
#define WLAN_START_AP_WAIT_MS    (10 * 1000)
#endif

#ifndef WLAN_EBOX_NUM
#define WLAN_EBOX_NUM           (10)
#endif

#ifndef WLAN_SCAN_RETRY_CNT
#define WLAN_SCAN_RETRY_CNT      (3)
#endif

#ifndef AUTO_CONNECTION_PERIOD_MS
#define AUTO_CONNECTION_PERIOD_MS (2000)
#endif

/*state fot station*/
#define WLAN_STATE_CONNECT     (1UL << 0)
#define WLAN_STATE_CONNECTING  (1UL << 1)
#define WLAN_STATE_READY       (1UL << 2)
#define WLAN_STATE_POWERSAVE   (1UL << 3)

/*flags fot station*/
#define WLAN_STATE_AUTOEN      (1UL << 0)

/*state fot ap*/
#define WLAN_STATE_ACTIVE      (1UL << 0)

typedef enum
{
    WLAN_EVT_READY = 0,              /* connect and prot is ok, You can send data*/
    WLAN_EVT_SCAN_DONE,              /* Scan a info */
    WLAN_EVT_SCAN_REPORT,            /* Scan end */
    WLAN_EVT_STA_CONNECTED,          /* connect success */
    WLAN_EVT_STA_CONNECTED_FAIL,     /* connection failed */
    WLAN_EVT_STA_DISCONNECTED,       /* disconnect */
    WLAN_EVT_AP_START,               /* AP start */
    WLAN_EVT_AP_STOP,                /* AP stop */
    WLAN_EVT_AP_ASSOCIATED,          /* sta associated */
    WLAN_EVT_AP_DISASSOCIATED,       /* sta disassociated */
    WLAN_EVT_MAX
} rt_wlan_event_t;

typedef void (*rt_wlan_event_handler)(int event, struct rt_wlan_buff *buff, void *parameter);

struct rt_wlan_scan_result
{
    rt_int32_t num;
    struct rt_wlan_info *info;
};

/*
 * wifi init interface
 */
int rt_wlan_init(void);
rt_err_t rt_wlan_set_mode(const char *dev_name, rt_wlan_mode_t mode);
rt_wlan_mode_t rt_wlan_get_mode(const char *dev_name);

/*
 * wifi station mode interface
 */
rt_err_t rt_wlan_connect(const char *ssid, const char *password);
rt_err_t rt_wlan_connect_adv(struct rt_wlan_info *info, const char *password);
rt_err_t rt_wlan_disconnect(void);
rt_bool_t rt_wlan_is_connected(void);
rt_bool_t rt_wlan_is_ready(void);
rt_err_t rt_wlan_set_mac(rt_uint8_t *mac);
rt_err_t rt_wlan_get_mac(rt_uint8_t *mac);
rt_err_t rt_wlan_get_info(struct rt_wlan_info *info);
int rt_wlan_get_rssi(void);

/*
 * wifi ap mode interface
 */
rt_err_t rt_wlan_start_ap(const char *ssid, const char *password);
rt_err_t rt_wlan_start_ap_adv(struct rt_wlan_info *info, const char *password);
rt_bool_t rt_wlan_ap_is_active(void);
rt_err_t rt_wlan_ap_stop(void);
rt_err_t rt_wlan_ap_get_info(struct rt_wlan_info *info);
int rt_wlan_ap_get_sta_num(void);
int rt_wlan_ap_get_sta_info(struct rt_wlan_info *info, int num);
rt_err_t rt_wlan_ap_deauth_sta(rt_uint8_t *mac);
rt_err_t rt_wlan_ap_set_country(rt_country_code_t country_code);
rt_country_code_t rt_wlan_ap_get_country(void);

/*
 * wifi scan interface
 */
rt_err_t rt_wlan_scan(void);
struct rt_wlan_scan_result *rt_wlan_scan_sync(void);
struct rt_wlan_scan_result *rt_wlan_scan_with_info(struct rt_wlan_info *info);
int rt_wlan_scan_get_info_num(void);
int rt_wlan_scan_get_info(struct rt_wlan_info *info, int num);
struct rt_wlan_scan_result *rt_wlan_scan_get_result(void);
void rt_wlan_scan_result_clean(void);
int rt_wlan_scan_find_cache(struct rt_wlan_info *info, struct rt_wlan_info *out_info, int num);
rt_bool_t rt_wlan_find_best_by_cache(const char *ssid, struct rt_wlan_info *info);

/*
 * wifi auto connect interface
 */
void rt_wlan_config_autoreconnect(rt_bool_t enable);
rt_bool_t rt_wlan_get_autoreconnect_mode(void);

/*
 * wifi power management interface
 */
rt_err_t rt_wlan_set_powersave(int level);
int rt_wlan_get_powersave(void);

/*
 * wifi event management interface
 */
rt_err_t rt_wlan_register_event_handler(rt_wlan_event_t event, rt_wlan_event_handler handler, void *parameter);
rt_err_t rt_wlan_unregister_event_handler(rt_wlan_event_t event);

/*
 * wifi management lock interface
 */
void rt_wlan_mgnt_lock(void);
void rt_wlan_mgnt_unlock(void);

#ifdef __cplusplus
}
#endif

#endif
