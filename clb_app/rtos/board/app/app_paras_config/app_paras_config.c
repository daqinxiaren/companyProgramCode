/**
 * @file
 * @details  System startup parameters. Other parameters are not 
             recommended to be saved in STM32 internal flash
 * @author   ren
 * @date     2021.09
 * @version  V1.0
**/

/* include */
#include "app_paras_config.h"
#include "app_paras_save.h"
#include "app_singlecast_thread.h"

/* macro */
#define APP_PARAS_CONFIG_IP     "255.255.255.255"
#define APP_PARAS_FACTORY_CODE  0x04
#define APP_PARAS_CLB_CODE      0x33
#define APP_PARAS_PTU_CODE      0xFF
#define APP_PARAS_MCU_SN_ADD    ((uint32_t)0x1FFF7A10)

/* type declaration */

/* variable declaration */
app_paras_config_env_t app_paras_config_env;
#define env app_paras_config_env

/* function declaration */
/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_config_get_mcu_sn(void)
{
   for(uint8_t i = 0; i < 3; i++)
    {
      env.mcu_sn[i] = (*(uint32_t *)(APP_PARAS_MCU_SN_ADD + (4 * i)));
		rt_kprintf("mcu_sn[%d] = 0x%02X \n", i, env.mcu_sn[i]);
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_config_msg_send(uint8_t *data, uint16_t size)
{
	struct sockaddr_in send_ipaddr;
	
	if(app_singlecast_thread_env.recv_handle < 0) {
		return ;
	}
	memset(&send_ipaddr, 0, sizeof(send_ipaddr));
   send_ipaddr.sin_family = AF_INET;
   send_ipaddr.sin_addr.s_addr = inet_addr(APP_PARAS_CONFIG_IP);
   send_ipaddr.sin_port = htons(env.config_net_inform.port);
	sendto(app_singlecast_thread_env.recv_handle, data, size, 0, (struct sockaddr *)&send_ipaddr, sizeof(send_ipaddr));
	printf("app_paras_config_msg_send :------\n");
	for(int i = 0; i < size; i++)
	{
		rt_kprintf("0x%02X ",data[i]);
	}
	rt_kprintf("\n");
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static inline void app_paras_config_broadcast_ack(void)
{
	paras_t *paras = OS_NULL;
	broadcast_ack_protocol_t broadcast_ack_data;
	
	memset(&broadcast_ack_data, 0, sizeof(broadcast_ack_protocol_t));

	paras = app_paras_get();

	broadcast_ack_data.data_head[0] = 0xAA;
	broadcast_ack_data.data_head[1] = 0x51;
	broadcast_ack_data.data_len[0] = (uint8_t)(sizeof(broadcast_ack_protocol_t) >> 8);
	broadcast_ack_data.data_len[1] = (uint8_t)(sizeof(broadcast_ack_protocol_t));
	broadcast_ack_data.factory_code = APP_PARAS_FACTORY_CODE;
	broadcast_ack_data.device_code = APP_PARAS_CLB_CODE;
	broadcast_ack_data.cmd = APP_BROADCAST_ACK_CMD;

	pbroadcast_ack_cmd_t broadcast_ack_inform;

	broadcast_ack_inform  = (pbroadcast_ack_cmd_t)broadcast_ack_data.data_inform;
	broadcast_ack_inform->device_code = APP_PARAS_CLB_CODE;
	broadcast_ack_inform->sn[0] = (uint8_t)(env.mcu_sn[0]);
	broadcast_ack_inform->sn[1] = (uint8_t)(env.mcu_sn[1]);

	memmove(broadcast_ack_inform->local_ip, paras->local_ip, 4);
	memmove(broadcast_ack_inform->mask, paras->netmask, 4);
	memmove(broadcast_ack_inform->gateway, paras->gateway, 4);

	broadcast_ack_inform->port[0] = (uint8_t)(paras->local_port >> 8);
	broadcast_ack_inform->port[1] = (uint8_t)(paras->local_port);

	broadcast_ack_inform->train_num = paras->train_num;
	uint16_t sum = sum_check_16((uint8_t *)&broadcast_ack_data, sizeof(broadcast_ack_protocol_t) - 2);
	broadcast_ack_data.sun_crc[0] = (uint8_t)(sum >> 8);
	broadcast_ack_data.sun_crc[1] = (uint8_t)(sum);
	printf("paras_config_broadcast_ack :------\n");

	app_paras_config_msg_send((uint8_t *)&broadcast_ack_data, sizeof(broadcast_ack_protocol_t));

}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static inline void app_paras_config_ack(uint8_t state)
{
	config_ack_protocol_t config_ack_protocol;
	
	memset(&config_ack_protocol, 0, sizeof(config_ack_protocol_t));
	config_ack_protocol.data_head[0] = 0xAA;
	config_ack_protocol.data_head[1] = 0x51;
	config_ack_protocol.data_len[0] = (uint8_t)(sizeof(config_ack_protocol_t) >> 8);
	config_ack_protocol.data_len[1] = (uint8_t)(sizeof(config_ack_protocol_t));
	config_ack_protocol.factory_code = APP_PARAS_FACTORY_CODE;
	config_ack_protocol.device_code = APP_PARAS_CLB_CODE;
	config_ack_protocol.cmd = APP_CONFIG_INFORM_ACK_CMD;
	pconfig_inform_ack_cmd_t config_inform;
	config_inform  = (pconfig_inform_ack_cmd_t)config_ack_protocol.data_inform;
	config_inform->device_code = APP_PARAS_CLB_CODE;
	config_inform->sn[0] = (uint8_t)(env.mcu_sn[0]);
	config_inform->sn[1] = (uint8_t)(env.mcu_sn[1]);
	config_inform->state = state;
	uint16_t sum = sum_check_16((uint8_t *)&config_ack_protocol, sizeof(config_ack_protocol_t) - 2);
	config_ack_protocol.sun_crc[0] = (uint8_t)(sum >> 8);
	config_ack_protocol.sun_crc[1] = (uint8_t)(sum);
	printf("app_paras_config_ack :------\n");
	app_paras_config_msg_send((uint8_t *)&config_ack_protocol, sizeof(config_ack_protocol_t));
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static inline void app_paras_config_inform(pconfig_inform_cmd_t config_inform)
{
	paras_t *paras = OS_NULL;
	
	uint16_t crc_16 = modbus_crc((uint8_t *)config_inform, sizeof(config_inform_cmd_t) - 2);
	if((config_inform->device_code != APP_PARAS_CLB_CODE) ||
	   (config_inform->sn[0] != (uint8_t)(env.mcu_sn[0])) || 
	   (config_inform->sn[1] != (uint8_t)(env.mcu_sn[1])) || 
	   (*(uint16_t *)config_inform->crc16 != crc_16))
	{
		rt_kprintf("app paras config device_cord error or sn error or crc16 error \n");
		return ;
	}
	else //all check ok
	{
		paras = app_paras_get();
		if(app_paras_ip_check(config_inform->local_ip))
		{
			rt_kprintf("app paras config local ip error \n");
			app_paras_config_ack(0);
			return ;
		}
		if(app_paras_mask_check(config_inform->mask))
		{
			rt_kprintf("app paras config mask error \n");
			app_paras_config_ack(0);
			return ;
		}
		if(app_paras_gateway_check(config_inform->gateway))
		{
			rt_kprintf("app paras config gateway error \n");
			app_paras_config_ack(0);
			return ;
		}
		if((config_inform->train_num < APP_BOARD_TRAIN_HEAD) ||
			(config_inform->train_num > app_paras_save_env.group_num))
		{
			rt_kprintf("app paras config train num error \n");
			app_paras_config_ack(0);
			return ;
		}
		memmove(paras->local_ip, config_inform->local_ip, 4);
		memmove(paras->netmask, config_inform->mask, 4);
		memmove(paras->gateway, config_inform->gateway, 4);
		uint16_t port = (config_inform->port[0] << 8) + config_inform->port[1];
		paras->local_port = port;
		paras->train_num = config_inform->train_num;
		rt_kprintf("\t->recv config paras inform: \n");
		app_paras_net_printf();
		uint8_t result = app_paras_update();
		app_paras_config_ack(1);
		rt_kprintf("\t->reset sys %d \n", result);
		rt_hw_cpu_reset();
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_config_get_data(uint8_t *data, uint16_t size, struct sockaddr_in recv_addr)
{
	pnet_config_protocol_t net_config_data;
	net_config_data = (pnet_config_protocol_t)data;
	
	if((*(uint16_t*)&net_config_data->data_head != htons(0xAA51))) {
		return;
	}
	uint16_t host_sum = sum_check_16(data, size - 2);
	uint16_t net_sum = (data[size - 2] << 8) + data[size - 1];
	if(host_sum != net_sum)
	{
		rt_kprintf("recv msg crc failed \n");
		return;
	}
	else
	{
		if((net_config_data->factory_code != APP_PARAS_FACTORY_CODE) ||
		   (net_config_data->device_code != APP_PARAS_PTU_CODE))
		{
			return ;
		}
		else
		{
			strcpy(env.config_net_inform.config_ip, inet_ntoa(recv_addr.sin_addr));
			env.config_net_inform.port = htons(recv_addr.sin_port);
			rt_kprintf("recv config paras inform: %s, port %d\n", env.config_net_inform.config_ip, env.config_net_inform.port);
			pbroadcast_cmd_t broadcast_cmd;
			broadcast_cmd = (pbroadcast_cmd_t)net_config_data->data_inform;
			pconfig_inform_cmd_t config_inform_cmd;
			config_inform_cmd = (pconfig_inform_cmd_t)net_config_data->data_inform;
			switch(net_config_data->cmd)
			{
				case APP_BROADCAST_CMD:
					if(broadcast_cmd->bits.txb == 0x01) {
						app_paras_config_broadcast_ack();
					}
					return ;
				case APP_CONFIG_INFORM_CMD:
					if(config_inform_cmd->target_broad.bits.txb == 0x01) {
						app_paras_config_inform(config_inform_cmd);
					}
					return ;
				default:
					return ;
			}
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_config_init(void)
{
	memset(&env.config_net_inform, 0, sizeof(config_net_inform_t));
	app_paras_config_get_mcu_sn();
}



	
	
	
	
	
	
	
	
	









