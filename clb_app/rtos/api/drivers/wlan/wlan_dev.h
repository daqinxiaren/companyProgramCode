/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-03     tyx          the first version
 */

#ifndef __WLAN_DEVICE_H__
#define __WLAN_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    WLAN_NONE,
    WLAN_STATION,
    WLAN_AP,
    WLAN_MODE_MAX
} rt_wlan_mode_t;

typedef enum
{
    WLAN_CMD_MODE = 0x10,
    WLAN_CMD_SCAN,              /* trigger scanning (list cells) */
    WLAN_CMD_JOIN,
    WLAN_CMD_SOFTAP,            /* start soft-AP */
    WLAN_CMD_DISCONNECT,
    WLAN_CMD_AP_STOP,           /* stop soft-AP */
    WLAN_CMD_AP_DEAUTH,
    WLAN_CMD_SCAN_STOP,
    WLAN_CMD_GET_RSSI,          /* get sensitivity (dBm) */
    WLAN_CMD_SET_POWERSAVE,
    WLAN_CMD_GET_POWERSAVE,
    WLAN_CMD_CFG_PROMISC,       /* start/stop minitor */
    WLAN_CMD_CFG_FILTER,        /* start/stop frame filter */
    WLAN_CMD_CFG_MGNT_FILTER,   /* start/stop management frame filter */
    WLAN_CMD_SET_CHANNEL,
    WLAN_CMD_GET_CHANNEL,
    WLAN_CMD_SET_COUNTRY,
    WLAN_CMD_GET_COUNTRY,
    WLAN_CMD_SET_MAC,
    WLAN_CMD_GET_MAC
} rt_wlan_cmd_t;

typedef enum
{
    WLAN_DEV_EVT_INIT_DONE = 0,
    WLAN_DEV_EVT_CONNECT,
    WLAN_DEV_EVT_CONNECT_FAIL,
    WLAN_DEV_EVT_DISCONNECT,
    WLAN_DEV_EVT_AP_START,
    WLAN_DEV_EVT_AP_STOP,
    WLAN_DEV_EVT_AP_ASSOCIATED,
    WLAN_DEV_EVT_AP_DISASSOCIATED,
    WLAN_DEV_EVT_AP_ASSOCIATE_FAILED,
    WLAN_DEV_EVT_SCAN_REPORT,
    WLAN_DEV_EVT_SCAN_DONE,
    WLAN_DEV_EVT_MAX,
} rt_wlan_dev_event_t;

#define SHARED_ENABLED  0x00008000
#define WPA_SECURITY    0x00200000
#define WPA2_SECURITY   0x00400000
#define WPS_ENABLED     0x10000000
#define WEP_ENABLED     0x0001
#define TKIP_ENABLED    0x0002
#define AES_ENABLED     0x0004
#define WSEC_SWFLAG     0x0008

#define WLAN_FLAG_STA_ONLY    (0x1 << 0)
#define WLAN_FLAG_AP_ONLY     (0x1 << 1)

#ifndef WLAN_SSID_MAX_LENGTH
#define WLAN_SSID_MAX_LENGTH  (32)   /* SSID MAX LEN */
#endif

#ifndef WLAN_BSSID_MAX_LENGTH
#define WLAN_BSSID_MAX_LENGTH (6)    /* BSSID MAX LEN (default is 6) */
#endif

#ifndef WLAN_PASSWORD_MAX_LENGTH
#define WLAN_PASSWORD_MAX_LENGTH   (32)   /* PASSWORD MAX LEN*/
#endif

#ifndef WLAN_DEV_EVENT_NUM
#define WLAN_DEV_EVENT_NUM  (2)   /* EVENT GROUP MAX NUM */
#endif

/**
 * Enumeration of Wi-Fi security modes
 */
typedef enum
{
    SECURITY_OPEN           = 0,                                                /* Open security                           */
    SECURITY_WEP_PSK        = WEP_ENABLED,                                      /* WEP Security with open authentication   */
    SECURITY_WEP_SHARED     = (WEP_ENABLED | SHARED_ENABLED),                   /* WEP Security with shared authentication */
    SECURITY_WPA_TKIP_PSK   = (WPA_SECURITY  | TKIP_ENABLED),                   /* WPA Security with TKIP                  */
    SECURITY_WPA_AES_PSK    = (WPA_SECURITY  | AES_ENABLED),                    /* WPA Security with AES                   */
    SECURITY_WPA2_AES_PSK   = (WPA2_SECURITY | AES_ENABLED),                    /* WPA2 Security with AES                  */
    SECURITY_WPA2_TKIP_PSK  = (WPA2_SECURITY | TKIP_ENABLED),                   /* WPA2 Security with TKIP                 */
    SECURITY_WPA2_MIXED_PSK = (WPA2_SECURITY | AES_ENABLED | TKIP_ENABLED),     /* WPA2 Security with AES & TKIP           */
    SECURITY_WPS_OPEN       = WPS_ENABLED,                                      /* WPS with open security                  */
    SECURITY_WPS_SECURE     = (WPS_ENABLED | AES_ENABLED),                      /* WPS with AES security                   */
    SECURITY_UNKNOWN        = -1,                                               /* May be returned by scan function if security is unknown.
                                                                                    Do not pass this to the join function! */
} rt_wlan_security_t;

typedef enum
{
    DEV_802_11_BAND_5GHZ  =  0,             /* Denotes 5GHz radio band   */
    DEV_802_11_BAND_2_4GHZ =  1,            /* Denotes 2.4GHz radio band */
    DEV_802_11_BAND_UNKNOWN = 0x7fffffff,   /* unknown */
} rt_802_11_band_t;

typedef enum
{
    DEV_COUNTRY_AFGHANISTAN,
    DEV_COUNTRY_ALBANIA,
    DEV_COUNTRY_ALGERIA,
    DEV_COUNTRY_AMERICAN_SAMOA,
    DEV_COUNTRY_ANGOLA,
    DEV_COUNTRY_ANGUILLA,
    DEV_COUNTRY_ANTIGUA_AND_BARBUDA,
    DEV_COUNTRY_ARGENTINA,
    DEV_COUNTRY_ARMENIA,
    DEV_COUNTRY_ARUBA,
    DEV_COUNTRY_AUSTRALIA,
    DEV_COUNTRY_AUSTRIA,
    DEV_COUNTRY_AZERBAIJAN,
    DEV_COUNTRY_BAHAMAS,
    DEV_COUNTRY_BAHRAIN,
    DEV_COUNTRY_BAKER_ISLAND,
    DEV_COUNTRY_BANGLADESH,
    DEV_COUNTRY_BARBADOS,
    DEV_COUNTRY_BELARUS,
    DEV_COUNTRY_BELGIUM,
    DEV_COUNTRY_BELIZE,
    DEV_COUNTRY_BENIN,
    DEV_COUNTRY_BERMUDA,
    DEV_COUNTRY_BHUTAN,
    DEV_COUNTRY_BOLIVIA,
    DEV_COUNTRY_BOSNIA_AND_HERZEGOVINA,
    DEV_COUNTRY_BOTSWANA,
    DEV_COUNTRY_BRAZIL,
    DEV_COUNTRY_BRITISH_INDIAN_OCEAN_TERRITORY,
    DEV_COUNTRY_BRUNEI_DARUSSALAM,
    DEV_COUNTRY_BULGARIA,
    DEV_COUNTRY_BURKINA_FASO,
    DEV_COUNTRY_BURUNDI,
    DEV_COUNTRY_CAMBODIA,
    DEV_COUNTRY_CAMEROON,
    DEV_COUNTRY_CANADA,
    DEV_COUNTRY_CAPE_VERDE,
    DEV_COUNTRY_CAYMAN_ISLANDS,
    DEV_COUNTRY_CENTRAL_AFRICAN_REPUBLIC,
    DEV_COUNTRY_CHAD,
    DEV_COUNTRY_CHILE,
    DEV_COUNTRY_CHINA,
    DEV_COUNTRY_CHRISTMAS_ISLAND,
    DEV_COUNTRY_COLOMBIA,
    DEV_COUNTRY_COMOROS,
    DEV_COUNTRY_CONGO,
    DEV_COUNTRY_CONGO_THE_DEMOCRATIC_REPUBLIC_OF_THE,
    DEV_COUNTRY_COSTA_RICA,
    DEV_COUNTRY_COTE_DIVOIRE,
    DEV_COUNTRY_CROATIA,
    DEV_COUNTRY_CUBA,
    DEV_COUNTRY_CYPRUS,
    DEV_COUNTRY_CZECH_REPUBLIC,
    DEV_COUNTRY_DENMARK,
    DEV_COUNTRY_DJIBOUTI,
    DEV_COUNTRY_DOMINICA,
    DEV_COUNTRY_DOMINICAN_REPUBLIC,
    DEV_COUNTRY_DOWN_UNDER,
    DEV_COUNTRY_ECUADOR,
    DEV_COUNTRY_EGYPT,
    DEV_COUNTRY_EL_SALVADOR,
    DEV_COUNTRY_EQUATORIAL_GUINEA,
    DEV_COUNTRY_ERITREA,
    DEV_COUNTRY_ESTONIA,
    DEV_COUNTRY_ETHIOPIA,
    DEV_COUNTRY_FALKLAND_ISLANDS_MALVINAS,
    DEV_COUNTRY_FAROE_ISLANDS,
    DEV_COUNTRY_FIJI,
    DEV_COUNTRY_FINLAND,
    DEV_COUNTRY_FRANCE,
    DEV_COUNTRY_FRENCH_GUINA,
    DEV_COUNTRY_FRENCH_POLYNESIA,
    DEV_COUNTRY_FRENCH_SOUTHERN_TERRITORIES,
    DEV_COUNTRY_GABON,
    DEV_COUNTRY_GAMBIA,
    DEV_COUNTRY_GEORGIA,
    DEV_COUNTRY_GERMANY,
    DEV_COUNTRY_GHANA,
    DEV_COUNTRY_GIBRALTAR,
    DEV_COUNTRY_GREECE,
    DEV_COUNTRY_GRENADA,
    DEV_COUNTRY_GUADELOUPE,
    DEV_COUNTRY_GUAM,
    DEV_COUNTRY_GUATEMALA,
    DEV_COUNTRY_GUERNSEY,
    DEV_COUNTRY_GUINEA,
    DEV_COUNTRY_GUINEA_BISSAU,
    DEV_COUNTRY_GUYANA,
    DEV_COUNTRY_HAITI,
    DEV_COUNTRY_HOLY_SEE_VATICAN_CITY_STATE,
    DEV_COUNTRY_HONDURAS,
    DEV_COUNTRY_HONG_KONG,
    DEV_COUNTRY_HUNGARY,
    DEV_COUNTRY_ICELAND,
    DEV_COUNTRY_INDIA,
    DEV_COUNTRY_INDONESIA,
    DEV_COUNTRY_IRAN_ISLAMIC_REPUBLIC_OF,
    DEV_COUNTRY_IRAQ,
    DEV_COUNTRY_IRELAND,
    DEV_COUNTRY_ISRAEL,
    DEV_COUNTRY_ITALY,
    DEV_COUNTRY_JAMAICA,
    DEV_COUNTRY_JAPAN,
    DEV_COUNTRY_JERSEY,
    DEV_COUNTRY_JORDAN,
    DEV_COUNTRY_KAZAKHSTAN,
    DEV_COUNTRY_KENYA,
    DEV_COUNTRY_KIRIBATI,
    DEV_COUNTRY_KOREA_REPUBLIC_OF,
    DEV_COUNTRY_KOSOVO,
    DEV_COUNTRY_KUWAIT,
    DEV_COUNTRY_KYRGYZSTAN,
    DEV_COUNTRY_LAO_PEOPLES_DEMOCRATIC_REPUBIC,
    DEV_COUNTRY_LATVIA,
    DEV_COUNTRY_LEBANON,
    DEV_COUNTRY_LESOTHO,
    DEV_COUNTRY_LIBERIA,
    DEV_COUNTRY_LIBYAN_ARAB_JAMAHIRIYA,
    DEV_COUNTRY_LIECHTENSTEIN,
    DEV_COUNTRY_LITHUANIA,
    DEV_COUNTRY_LUXEMBOURG,
    DEV_COUNTRY_MACAO,
    DEV_COUNTRY_MACEDONIA_FORMER_YUGOSLAV_REPUBLIC_OF,
    DEV_COUNTRY_MADAGASCAR,
    DEV_COUNTRY_MALAWI,
    DEV_COUNTRY_MALAYSIA,
    DEV_COUNTRY_MALDIVES,
    DEV_COUNTRY_MALI,
    DEV_COUNTRY_MALTA,
    DEV_COUNTRY_MAN_ISLE_OF,
    DEV_COUNTRY_MARTINIQUE,
    DEV_COUNTRY_MAURITANIA,
    DEV_COUNTRY_MAURITIUS,
    DEV_COUNTRY_MAYOTTE,
    DEV_COUNTRY_MEXICO,
    DEV_COUNTRY_MICRONESIA_FEDERATED_STATES_OF,
    DEV_COUNTRY_MOLDOVA_REPUBLIC_OF,
    DEV_COUNTRY_MONACO,
    DEV_COUNTRY_MONGOLIA,
    DEV_COUNTRY_MONTENEGRO,
    DEV_COUNTRY_MONTSERRAT,
    DEV_COUNTRY_MOROCCO,
    DEV_COUNTRY_MOZAMBIQUE,
    DEV_COUNTRY_MYANMAR,
    DEV_COUNTRY_NAMIBIA,
    DEV_COUNTRY_NAURU,
    DEV_COUNTRY_NEPAL,
    DEV_COUNTRY_NETHERLANDS,
    DEV_COUNTRY_NETHERLANDS_ANTILLES,
    DEV_COUNTRY_NEW_CALEDONIA,
    DEV_COUNTRY_NEW_ZEALAND,
    DEV_COUNTRY_NICARAGUA,
    DEV_COUNTRY_NIGER,
    DEV_COUNTRY_NIGERIA,
    DEV_COUNTRY_NORFOLK_ISLAND,
    DEV_COUNTRY_NORTHERN_MARIANA_ISLANDS,
    DEV_COUNTRY_NORWAY,
    DEV_COUNTRY_OMAN,
    DEV_COUNTRY_PAKISTAN,
    DEV_COUNTRY_PALAU,
    DEV_COUNTRY_PANAMA,
    DEV_COUNTRY_PAPUA_NEW_GUINEA,
    DEV_COUNTRY_PARAGUAY,
    DEV_COUNTRY_PERU,
    DEV_COUNTRY_PHILIPPINES,
    DEV_COUNTRY_POLAND,
    DEV_COUNTRY_PORTUGAL,
    DEV_COUNTRY_PUETO_RICO,
    DEV_COUNTRY_QATAR,
    DEV_COUNTRY_REUNION,
    DEV_COUNTRY_ROMANIA,
    DEV_COUNTRY_RUSSIAN_FEDERATION,
    DEV_COUNTRY_RWANDA,
    DEV_COUNTRY_SAINT_KITTS_AND_NEVIS,
    DEV_COUNTRY_SAINT_LUCIA,
    DEV_COUNTRY_SAINT_PIERRE_AND_MIQUELON,
    DEV_COUNTRY_SAINT_VINCENT_AND_THE_GRENADINES,
    DEV_COUNTRY_SAMOA,
    DEV_COUNTRY_SANIT_MARTIN_SINT_MARTEEN,
    DEV_COUNTRY_SAO_TOME_AND_PRINCIPE,
    DEV_COUNTRY_SAUDI_ARABIA,
    DEV_COUNTRY_SENEGAL,
    DEV_COUNTRY_SERBIA,
    DEV_COUNTRY_SEYCHELLES,
    DEV_COUNTRY_SIERRA_LEONE,
    DEV_COUNTRY_SINGAPORE,
    DEV_COUNTRY_SLOVAKIA,
    DEV_COUNTRY_SLOVENIA,
    DEV_COUNTRY_SOLOMON_ISLANDS,
    DEV_COUNTRY_SOMALIA,
    DEV_COUNTRY_SOUTH_AFRICA,
    DEV_COUNTRY_SPAIN,
    DEV_COUNTRY_SRI_LANKA,
    DEV_COUNTRY_SURINAME,
    DEV_COUNTRY_SWAZILAND,
    DEV_COUNTRY_SWEDEN,
    DEV_COUNTRY_SWITZERLAND,
    DEV_COUNTRY_SYRIAN_ARAB_REPUBLIC,
    DEV_COUNTRY_TAIWAN_PROVINCE_OF_CHINA,
    DEV_COUNTRY_TAJIKISTAN,
    DEV_COUNTRY_TANZANIA_UNITED_REPUBLIC_OF,
    DEV_COUNTRY_THAILAND,
    DEV_COUNTRY_TOGO,
    DEV_COUNTRY_TONGA,
    DEV_COUNTRY_TRINIDAD_AND_TOBAGO,
    DEV_COUNTRY_TUNISIA,
    DEV_COUNTRY_TURKEY,
    DEV_COUNTRY_TURKMENISTAN,
    DEV_COUNTRY_TURKS_AND_CAICOS_ISLANDS,
    DEV_COUNTRY_TUVALU,
    DEV_COUNTRY_UGANDA,
    DEV_COUNTRY_UKRAINE,
    DEV_COUNTRY_UNITED_ARAB_EMIRATES,
    DEV_COUNTRY_UNITED_KINGDOM,
    DEV_COUNTRY_UNITED_STATES,
    DEV_COUNTRY_UNITED_STATES_REV4,
    DEV_COUNTRY_UNITED_STATES_NO_DFS,
    DEV_COUNTRY_UNITED_STATES_MINOR_OUTLYING_ISLANDS,
    DEV_COUNTRY_URUGUAY,
    DEV_COUNTRY_UZBEKISTAN,
    DEV_COUNTRY_VANUATU,
    DEV_COUNTRY_VENEZUELA,
    DEV_COUNTRY_VIET_NAM,
    DEV_COUNTRY_VIRGIN_ISLANDS_BRITISH,
    DEV_COUNTRY_VIRGIN_ISLANDS_US,
    DEV_COUNTRY_WALLIS_AND_FUTUNA,
    DEV_COUNTRY_WEST_BANK,
    DEV_COUNTRY_WESTERN_SAHARA,
    DEV_COUNTRY_WORLD_WIDE_XX,
    DEV_COUNTRY_YEMEN,
    DEV_COUNTRY_ZAMBIA,
    DEV_COUNTRY_ZIMBABWE,
    DEV_COUNTRY_UNKNOWN
} rt_country_code_t;

struct rt_wlan_device;
struct rt_wlan_buff;

typedef void (*rt_wlan_dev_event_handler)(struct rt_wlan_device *device, rt_wlan_dev_event_t event, struct rt_wlan_buff *buff, void *parameter);

typedef void (*rt_wlan_pormisc_callback_t)(struct rt_wlan_device *device, void *data, int len);

typedef void (*rt_wlan_mgnt_filter_callback_t)(struct rt_wlan_device *device, void *data, int len);

struct rt_wlan_ssid
{
    rt_uint8_t len;
    rt_uint8_t val[WLAN_SSID_MAX_LENGTH + 1];
};
typedef struct rt_wlan_ssid rt_wlan_ssid_t;

struct rt_wlan_key
{
    rt_uint8_t len;
    rt_uint8_t val[WLAN_PASSWORD_MAX_LENGTH + 1];
};
typedef struct rt_wlan_key rt_wlan_key_t;

#define INVALID_INFO(_info)       do {    \
                                        rt_memset((_info), 0, sizeof(struct rt_wlan_info)); \
                                        (_info)->band = DEV_802_11_BAND_UNKNOWN; \
                                        (_info)->security = SECURITY_UNKNOWN; \
                                        (_info)->channel = -1; \
                                    } while(0)

#define SSID_SET(_info, _ssid)    do {    \
                                        rt_strncpy((char *)(_info)->ssid.val, (_ssid), WLAN_SSID_MAX_LENGTH); \
                                        (_info)->ssid.len = rt_strlen((char *)(_info)->ssid.val); \
                                    } while(0)

struct rt_wlan_info
{
    /* security type */
    rt_wlan_security_t security;
    /* 2.4G/5G */
    rt_802_11_band_t band;
    /* maximal data rate */
    rt_uint32_t datarate;
    /* radio channel */
    rt_int16_t channel;
    /* signal strength */
    rt_int16_t  rssi;
    /* ssid */
    rt_wlan_ssid_t ssid;
    /* hwaddr */
    rt_uint8_t bssid[WLAN_BSSID_MAX_LENGTH];
    rt_uint8_t hidden;
};

struct rt_wlan_buff
{
    void *data;
    rt_int32_t len;
};

struct rt_filter_pattern
{
    rt_uint16_t offset;     /* Offset in bytes to start filtering (referenced to the start of the ethernet packet) */
    rt_uint16_t mask_size;  /* Size of the mask in bytes */
    rt_uint8_t *mask;       /* Pattern mask bytes to be ANDed with the pattern eg. "\xff00" (must be in network byte order) */
    rt_uint8_t *pattern;    /* Pattern bytes used to filter eg. "\x0800"  (must be in network byte order) */
};

typedef enum
{
    DEV_POSITIVE_MATCHING  = 0, /* Receive the data matching with this pattern and discard the other data  */
    DEV_NEGATIVE_MATCHING  = 1  /* Discard the data matching with this pattern and receive the other data */
} rt_filter_rule_t;

struct rt_wlan_filter
{
    struct rt_filter_pattern patt;
    rt_filter_rule_t rule;
    rt_uint8_t enable;
};

struct rt_wlan_dev_event_desc
{
    rt_wlan_dev_event_handler handler;
    void *parameter;
};

struct rt_wlan_device
{
    struct rt_device device;
    rt_wlan_mode_t mode;
    struct rt_mutex lock;
    struct rt_wlan_dev_event_desc handler_table[WLAN_DEV_EVT_MAX][WLAN_DEV_EVENT_NUM];
    rt_wlan_pormisc_callback_t pormisc_callback;
    rt_wlan_mgnt_filter_callback_t mgnt_filter_callback;
    const struct rt_wlan_dev_ops *ops;
    rt_uint32_t flags;
    struct netdev *netdev;
    void *prot;
    void *user_data;
};

struct rt_sta_info
{
    rt_wlan_ssid_t ssid;
    rt_wlan_key_t key;
    rt_uint8_t bssid[6];
    rt_uint16_t channel;
    rt_wlan_security_t security;
};

struct rt_ap_info
{
    rt_wlan_ssid_t ssid;
    rt_wlan_key_t key;
    rt_bool_t hidden;
    rt_uint16_t channel;
    rt_wlan_security_t security;
};

struct rt_scan_info
{
    rt_wlan_ssid_t ssid;
    rt_uint8_t bssid[6];
    rt_int16_t channel_min;
    rt_int16_t channel_max;
    rt_bool_t passive;
};

struct rt_wlan_dev_ops
{
    rt_err_t (*wlan_init)(struct rt_wlan_device *wlan);
    rt_err_t (*wlan_mode)(struct rt_wlan_device *wlan, rt_wlan_mode_t mode);
    rt_err_t (*wlan_scan)(struct rt_wlan_device *wlan, struct rt_scan_info *scan_info);
    rt_err_t (*wlan_join)(struct rt_wlan_device *wlan, struct rt_sta_info *sta_info);
    rt_err_t (*wlan_softap)(struct rt_wlan_device *wlan, struct rt_ap_info *ap_info);
    rt_err_t (*wlan_disconnect)(struct rt_wlan_device *wlan);
    rt_err_t (*wlan_ap_stop)(struct rt_wlan_device *wlan);
    rt_err_t (*wlan_ap_deauth)(struct rt_wlan_device *wlan, rt_uint8_t mac[]);
    rt_err_t (*wlan_scan_stop)(struct rt_wlan_device *wlan);
    int (*wlan_get_rssi)(struct rt_wlan_device *wlan);
    rt_err_t (*wlan_set_powersave)(struct rt_wlan_device *wlan, int level);
    int (*wlan_get_powersave)(struct rt_wlan_device *wlan);
    rt_err_t (*wlan_cfg_promisc)(struct rt_wlan_device *wlan, rt_bool_t start);
    rt_err_t (*wlan_cfg_filter)(struct rt_wlan_device *wlan, struct rt_wlan_filter *filter);
    rt_err_t (*wlan_cfg_mgnt_filter)(struct rt_wlan_device *wlan, rt_bool_t start);
    rt_err_t (*wlan_set_channel)(struct rt_wlan_device *wlan, int channel);
    int (*wlan_get_channel)(struct rt_wlan_device *wlan);
    rt_err_t (*wlan_set_country)(struct rt_wlan_device *wlan, rt_country_code_t country_code);
    rt_country_code_t (*wlan_get_country)(struct rt_wlan_device *wlan);
    rt_err_t (*wlan_set_mac)(struct rt_wlan_device *wlan, rt_uint8_t mac[]);
    rt_err_t (*wlan_get_mac)(struct rt_wlan_device *wlan, rt_uint8_t mac[]);
    int (*wlan_recv)(struct rt_wlan_device *wlan, void *buff, int len);
    int (*wlan_send)(struct rt_wlan_device *wlan, void *buff, int len);
    int (*wlan_send_raw_frame)(struct rt_wlan_device *wlan, void *buff, int len);
};

/*
 * wlan device init
 */
rt_err_t rt_wlan_dev_init(struct rt_wlan_device *device, rt_wlan_mode_t mode);

/*
 * wlan device station interface
 */
rt_err_t rt_wlan_dev_connect(struct rt_wlan_device *device, struct rt_wlan_info *info, const char *password, int password_len);
rt_err_t rt_wlan_dev_disconnect(struct rt_wlan_device *device);
int rt_wlan_dev_get_rssi(struct rt_wlan_device *device);

/*
 * wlan device ap interface
 */
rt_err_t rt_wlan_dev_ap_start(struct rt_wlan_device *device, struct rt_wlan_info *info, const char *password, int password_len);
rt_err_t rt_wlan_dev_ap_stop(struct rt_wlan_device *device);
rt_err_t rt_wlan_dev_ap_deauth(struct rt_wlan_device *device, rt_uint8_t mac[6]);

/*
 * wlan device scan interface
 */
rt_err_t rt_wlan_dev_scan(struct rt_wlan_device *device, struct rt_wlan_info *info);
rt_err_t rt_wlan_dev_scan_stop(struct rt_wlan_device *device);

/*
 * wlan device mac interface
 */
rt_err_t rt_wlan_dev_get_mac(struct rt_wlan_device *device, rt_uint8_t mac[6]);
rt_err_t rt_wlan_dev_set_mac(struct rt_wlan_device *device, rt_uint8_t mac[6]);

/*
 * wlan device powersave interface
 */
rt_err_t rt_wlan_dev_set_powersave(struct rt_wlan_device *device, int level);
int rt_wlan_dev_get_powersave(struct rt_wlan_device *device);

/*
 * wlan device event interface
 */
rt_err_t rt_wlan_dev_register_event_handler(struct rt_wlan_device *device, rt_wlan_dev_event_t event, rt_wlan_dev_event_handler handler, void *parameter);
rt_err_t rt_wlan_dev_unregister_event_handler(struct rt_wlan_device *device, rt_wlan_dev_event_t event, rt_wlan_dev_event_handler handler);
void rt_wlan_dev_indicate_event_handle(struct rt_wlan_device *device, rt_wlan_dev_event_t event, struct rt_wlan_buff *buff);

/*
 * wlan device promisc interface
 */
rt_err_t rt_wlan_dev_enter_promisc(struct rt_wlan_device *device);
rt_err_t rt_wlan_dev_exit_promisc(struct rt_wlan_device *device);
rt_err_t rt_wlan_dev_set_promisc_callback(struct rt_wlan_device *device, rt_wlan_pormisc_callback_t callback);
void rt_wlan_dev_promisc_handler(struct rt_wlan_device *device, void *data, int len);

/*
 * wlan device filter interface
 */
rt_err_t rt_wlan_dev_cfg_filter(struct rt_wlan_device *device, struct rt_wlan_filter *filter);

/*
 * wlan device channel interface
 */
rt_err_t rt_wlan_dev_set_channel(struct rt_wlan_device *device, int channel);
int rt_wlan_dev_get_channel(struct rt_wlan_device *device);

/*
 * wlan device country interface
 */
rt_err_t rt_wlan_dev_set_country(struct rt_wlan_device *device, rt_country_code_t country_code);
rt_country_code_t rt_wlan_dev_get_country(struct rt_wlan_device *device);

/*
 * wlan device datat transfer interface
 */
rt_err_t rt_wlan_dev_report_data(struct rt_wlan_device *device, void *buff, int len);
// void rt_wlan_dev_data_ready(struct rt_wlan_device *device, int len);

/*
 * wlan device register interface
 */
rt_err_t rt_wlan_dev_register(struct rt_wlan_device *wlan, const char *name, 
    const struct rt_wlan_dev_ops *ops, rt_uint32_t flag, void *user_data);

#ifdef __cplusplus
}
#endif

#endif
