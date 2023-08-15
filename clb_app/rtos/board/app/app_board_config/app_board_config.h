/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_BOARD_CONFIG_H__
#define __APP_BOARD_CONFIG_H__

/* include */
#include "app_utility.h"

/* macro */
/*************************** DEV CONFIG ***************************/
#define DEVICE_FACTORY_CODE                 0x04
#define CLB_RECORD_TARGET_ADDR              0x01

#define APP_BOARD_TRAIN_HEAD                1
//#define APP_BOARD_TRAIN_TAIL                6 //尾车编号根据取记录板下发的编组信息
#define DEF_QZQ_HOST_NUM_2                  2
#define DEF_QZQ_HOST_NUM_3                  3
#define DEF_QZQ_TOTAL                       4

/*************************** NET CONFIG ***************************/
#define DEF_MULTICAST_SEND_IP               "239.255.10.2"
#define DEF_MULTICAST_SEND_PORT             9002
#define DEF_MULTICAST_RECV_IP               "239.255.10.1"
#define DEF_MULTICAST_RECV_PORT             9001
#define DEF_MULTICAST_RECV_BUFF_MAX         1024

#define DEF_SINGLECAST_SERVER_IP            "192.168.1.11"
#define DEF_SINGLECAST_SERVER_PORT          8000
#define DEF_SINGLECAST_LOCAL_PORT           8000
#define DEF_SINGLECAST_RECV_BUFF_MAX        1024

#define DEF_RESERVE_CMD                     0x00
#define DEF_APP_UPDATE_CMD                  0x01
#define DEF_SYSTEM_REBOOT_CMD               0x02
#define DEF_CONFIG_PARA_SET_CMD             0x03
#define DEF_CONFIG_PARA_READ_CMD            0x04
#define DEF_GET_QZQ_DEFAULT_ID_CMD          0x06
#define DEF_SET_QZQ_NEW_ID_CMD              0x07

/*********************** TASK STACK AND PRO CONFIG **************/
#define DEF_MULTICAST_SEND_STACK            2048
#define DEF_MULTICAST_RECV_STACK            2048
#define DEF_SINGLECAST_RECV_STACK           2048
#define DEF_DATA_SAVE_STACK                 4096
#define DEF_TEMP_SAVE_STACK                 2048
#define DEF_UDP_MCAST_STACK                 3072
#define DEF_UDP_CAST_STACK                  4096
#define DEF_LED_CONTROL_STACK               512
#define DEF_COLLECT_SPEED_STACK             4096
#define DEF_CAN1_STACK                      2048
#define DEF_CAN2_STACK                      2048
#define DEF_TFTP_STACK                      4096
#define DEF_DIAG_STACK                      9216
#define DEF_MULTICAST_SEND_PRO              22
#define DEF_MULTICAST_RECV_PRO              23
#define DEF_SINGLECAST_RECV_PRO             24
#define DEF_DATA_SAVE_PRO                   20
#define DEF_TEMP_SAVE_PRO                   21
#define DEF_LED_CONTROL_PRO                 23
#define DEF_UDP_MCAST_PRO                   20
#define DEF_UDP_CAST_PRO                    20
#define DEF_COLLECT_SPEED_PRO               23
#define DEF_CAN1_PRO                        22
#define DEF_CAN2_PRO                        22
#define DEF_TFTP_PRO                        19
#define DEF_DIAG_THREAD_PRO                 23

/*********************** LED PIN CONFIG D1，D7无控制需求，D6预留 **************/
#define DEF_LED_PIN_D2                      OS_NULL
#define DEF_LED_PIN_D3                      GET_PIN(G,8)
#define DEF_LED_PIN_D4                      GET_PIN(B,6)
#define DEF_LED_PIN_D5                      OS_NULL
#define DEF_LED_PIN_D6                      OS_NULL
#define DEF_LED_PIN_D8                      OS_NULL
#define DEF_LED_PIN_D9                      OS_NULL
#define DEF_POWER_24V_CH1_PIN               GET_PIN(F,9)
#define DEF_POWER_24V_CH2_PIN               GET_PIN(F,10)

/*************************** DEVICE CODE CONFIG ***************************/
#define DEF_UDP_DATA_HEAD_BYTES             28
#define DEF_JLB_DEVICE_CODE                 0x11  // 记录版
#define DEF_TXB_DEVICE_CODE                 0x22  // 通信版
#define DEF_CLB_DEVICE_CODE                 0x33  // 轴承处理板
#define DEF_TGB1_DEVICE_CODE                0x44  // 脱轨处理板1
#define DEF_TGB2_DEVICE_CODE                0x55  // 脱轨处理板1
#define DEF_ZXB_DEVICE_CODE                 0x66  // 转速总线板
#define DEF_PTU_DEVICE_CODE                 0XFF  // PTU平台

/*************************** TFTP CONFIG **********************************/
#define DEF_TFTP_HEAD_CONFIG_FILE_IP        "192.168.1.11"
#define DEF_TFTP_TAIL_CONFIG_FILE_IP        "192.168.6.11" //根据车厢数配置主机ip
#define DEF_CONFIG_REMOTE_PATH     			"update/update_config/"
#define DEF_LOCAL_CONFIG_PATH      			"/config/"
#define DEF_REMOTE_FILE_NAME				"system.ini"
#define DEF_LOCAL_FILE_NAME					"system.ini"

/*************************** CLB COLLECT CONFIG ***************************/
#define DEF_CLB_DATA_CH_MAX                 4    //无脱轨板的项目，需要用到v5v6，总数为6；有脱轨板的项目，只用到4通道，不采集波磨信息
#define DEF_AD7606_DATA_CH_MAX              4    /*AD7606_6*/
#define DEF_AD7606_CH_BUFF_SIZE_MAX         (4096u)
#define DEF_AD7606_FILTER_SIZE              (24576u) // 切换通道后采集间隔次数
#define DEF_AD7606_DEVICE_ID                2    // /*ID_AD7606_6*/
#define DEF_ADG1608_SELECT_CH_MAX           8
#define DEF_ADG1609_SELECT_CH_MAX           4

#define DEF_CAN_RX_SIZE                     512  //can recv_buf 接收缓存大小
#define DEF_TEMP_SHIFT                      0  //根据客户mvb协议定义温度取值偏移量
#define DEF_TEMP_INVALID                    -127 //温度传感器故障时,没有采集到温度的无效值
#define DEF_TEMP_DEFAULT                    -127 //温度传感器故障时温度默认值，此为配置协议规定的默认值
#define DEF_TEMP_WORNING                    (80+DEF_TEMP_SHIFT)
#define DEF_TEMP_ALARM                      (90+DEF_TEMP_SHIFT)
#define DEF_TEMP_MAX_SIZE                   400 //所有前置器温度值buffer大小，4个前置器，每个前置器5个温度，每个温度10个值，每个值2byte
#define DEF_TEMP_ONE_SIZE                   100 //一个前置器温度buffer大小，一个前置器4个传感器，包括5个温度，每个温度10个值，每个值2byte
#define DEV_SAVE_VIB_ONE_SIZE               512
#define DEF_BOARD_UDP_DATA_HEAD_BYTES       28
/* type declaration */

/* variable declaration */

/* function declaration */

#endif /*__APP_BOARD_CONFIG_H__*/


