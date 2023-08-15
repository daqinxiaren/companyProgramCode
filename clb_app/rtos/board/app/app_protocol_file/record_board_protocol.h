/**
 * @file
 * @details
 * @author
 * @date    record send clb txb tclb protocol
 * @version
**/

#ifndef __RECORD_BOARD_PROTOCOL_H__
#define __RECORD_BOARD_PROTOCOL_H__

/* include */
#include "dev_config.h"
#include "rtdevice.h"

/* type declaration */
typedef struct
{
	uint8_t target_addr;
	uint8_t cmd_date;
}record_cmd_date_t, *precord_cmd_date_t;
typedef struct
{
	uint8_t data_head[2];
	uint8_t data_len[2];
	uint8_t factory_code;
	uint8_t device_code;
	uint8_t life_signal[2];
	uint8_t target_addr[2];
	uint8_t resend_flag;
	uint8_t answer_flag;
	uint8_t udp_packet;
	uint8_t temp_flag;
    uint8_t clb_ckdy;              //处理板串口打印开关
    uint8_t clb_reboot;            //处理板重起开关
    uint8_t gear_number;           //总线板计算转速的车轮上的测点数
    uint8_t group_num;             //编组数
	uint8_t reserve[6];
	uint8_t cmd;
	uint8_t target_broad;
	uint8_t add_data[];
//	uint8_t sum_crc[2];
}record_unicast_protocol_t, *precord_unicast_protocol_t;

typedef struct
{
    uint8_t head[2];               //"0xAA50：过程数据包0xAA51-0xAA54：原始数据包（根据实际增减）
    uint8_t len[2];                //振动过程数据为360，失稳数据为256 平稳为256 原始为1024 轴温为256
    uint8_t company_id;            //板卡供应商编号 参考供应商定义
    uint8_t board_id;              //本身板卡编号 参考宏定义
    uint8_t life_signal[2];        //生命信号，每秒加1
    uint8_t target_board_group[2]; //目标板卡的位集合
    uint8_t resend_flag;           //"0x55：表示首次发送该包数据，0xAA：表示重发该包数据；重发时的数据与首次发送的数据需全部一样，且仅对未应答的单板重发数据（通过Byte8-9来选型），超时时间为300ms，最多重发3次。"
    uint8_t ack_flag;              //"0x5A:目标板需要返回给请求板收到一包数据的应答帧   0x00:无需应答其它无效"
    uint8_t packet_num;            //当前数据类型发送的总包数
    uint8_t res[11];               //预留
    uint8_t cmd;                   //命令
    uint8_t board_id_2;            //本身板卡编号  前置器发送的时候使用
}send_data_head_t;

void app_paras_config_get_data(uint8_t *data, uint16_t size, struct sockaddr_in recv_addr);
void app_protocol_paras_ptu_data(uint8_t *data, uint16_t size, struct sockaddr_in recv_addr);

#endif /*__RECORD_BOARD_PROTOCOL_H__*/





