/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_PARAS_SAVE_H__
#define __APP_PARAS_SAVE_H__

/* include */
#include <rthw.h>
#include "stdint.h"
#include "app_utility.h"
#include "bsp_internal_flash.h"
#include "app_cominform_config.h"
#include "app_board_config.h"

/* macro */

/* type declaration */
typedef struct
{
	uint8_t  local_ip[4];
	uint8_t  netmask[4];
	uint8_t  gateway[4];
	uint16_t local_port;
    uint8_t  train_num; //当前车厢号
    uint8_t  group_num; //编组数
	sys_para_t sys_para;
}paras_t;
typedef struct
{
    uint8_t head[2];
    paras_t paras;
    uint8_t check_sum;
}app_paras_t;

typedef struct
{
	app_paras_t paras_inform;
	uint8_t     train_num;
	uint8_t     temp_flag;
	uint8_t 	clb_ckdy;
	uint8_t 	clb_reboot;
	uint8_t 	group_num;
	uint8_t     host_flag;
	uint8_t     host_qzq_num;
}app_paras_save_env_t;

/* variable declaration */
extern app_paras_save_env_t app_paras_save_env;

/* function declaration */
uint8_t app_paras_update(void);
void app_paras_net_printf(void);
uint8_t app_paras_ip_check(uint8_t *ip);
uint8_t app_paras_mask_check(uint8_t *mask);
uint8_t app_paras_gateway_check(uint8_t *gateway);
void app_paras_save_init(comm_para_t *comm_para_config);

/**
 * @brief
 * @param
 * @return
 * @note
**/
static inline paras_t *app_paras_get(void)
{
    return &app_paras_save_env.paras_inform.paras;
}

#endif /*__APP_PARAS_SAVE_H__*/


