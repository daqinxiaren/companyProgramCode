/**
 * @file
 * @details  System startup parameters. Other parameters are not 
             recommended to be saved in STM32 internal flash
 * @author   ren
 * @date     2021.09
 * @version  V1.0
**/

/* include */
#include "app_paras_save.h"
#include "app_cominform_config.h"

/* macro */
#define APP_PARAS_CHUNK_HEAD               0x6ACA
#define APP_PARAS_CHUNK_CHECKSUM_CODE      0xC5

/* variable declaration */
paras_t def_paras = 
{
	.local_ip            = {192, 168, 1, 13},
	.netmask             = {255, 255, 0, 0},
	.gateway             = {192, 168, 1, 1},
	.local_port          = 8000,
	.train_num           = 1,
};
app_paras_save_env_t app_paras_save_env;

#define env app_paras_save_env

/* function declaration */
/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_net_printf(void)
{
	#define paras_inform env.paras_inform.paras
	
	rt_kprintf("\t->local_ip : %d.%d.%d.%d \n", paras_inform.local_ip[0], paras_inform.local_ip[1],
											paras_inform.local_ip[2], paras_inform.local_ip[3]);
	rt_kprintf("\t->netmask : %d.%d.%d.%d \n", paras_inform.netmask[0], paras_inform.netmask[1],
										   paras_inform.netmask[2], paras_inform.netmask[3]);
	rt_kprintf("\t->gateway : %d.%d.%d.%d \n", paras_inform.gateway[0], paras_inform.gateway[1],
										   paras_inform.gateway[2], paras_inform.gateway[3]);
	rt_kprintf("\t->local_port : %d \n", paras_inform.local_port);
	rt_kprintf("\t->train_num : %d \n", paras_inform.train_num);
	rt_kprintf("\t->group_num : %d \n", paras_inform.group_num);
	
	#undef paras_inform
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
uint8_t app_paras_ip_check(uint8_t *ip)
{
	uint8_t result = 0;
	
	if((ip[0] != 192) || (ip[1] != 168) || (ip[3] != 13)) //192.168.xx.13
	{
		result = 1;
	}
   return result;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
uint8_t app_paras_gateway_check(uint8_t *gateway)
{
	uint8_t result = 0;
	
	if((gateway[0] != 192) || (gateway[1] != 168) || (gateway[3] != 1)) //192.168.xx.1
	{
		result = 1;
	}
    return result;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
uint8_t app_paras_mask_check(uint8_t *mask)
{
	uint8_t result = 0;
	
	if((mask[0] != 255) || (mask[1] != 255)) //255.255.xx.xx
	{
		result = 1;
	}
    return result;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
uint8_t app_paras_write(void)
{
	rt_base_t level;
	uint8_t result = 0;
	
   env.paras_inform.head[0] = DATA16_H(APP_PARAS_CHUNK_HEAD);
   env.paras_inform.head[1] = DATA16_L(APP_PARAS_CHUNK_HEAD);
   env.paras_inform.check_sum = check_xor_sum(&env.paras_inform, STRUCT_OFFSET(app_paras_t, check_sum),
                                         APP_PARAS_CHUNK_CHECKSUM_CODE);
   rt_kprintf("write parameters app_paras_write   train_id %d !\n", env.paras_inform.paras.train_num);
	level = rt_hw_interrupt_disable();
    result = bsp_internal_flash_write((uint8_t *)&env.paras_inform, sizeof(app_paras_t));
	rt_hw_interrupt_enable(level);
	rt_kprintf("write parameters result %d !\n", result);
	return result;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_check(paras_t *paras)
{
	uint8_t paras_state = 0;
	
	if(app_paras_ip_check(paras->local_ip))
	{
		paras_state = 1;
		rt_memmove(paras->local_ip, def_paras.local_ip, 4);
	}
	if(app_paras_gateway_check(paras->gateway))
	{
		paras_state = 1;
		rt_memmove(paras->gateway, def_paras.gateway, 4);
	}
	if(app_paras_mask_check(paras->netmask))
	{
		paras_state = 1;
		rt_memmove(paras->netmask, def_paras.netmask, 4);
	}
	if(paras->local_port != 8000)
	{
		paras_state = 1;
		paras->local_port = def_paras.local_port;
	}
	if((paras->train_num < APP_BOARD_TRAIN_HEAD) ||
		(paras->train_num > paras->group_num))
	{
		paras_state = 1;
		paras->train_num = def_paras.train_num;
	}
	if(paras_state == 1)
	{
		rt_kprintf("app paras check error !\n");
		if(!app_paras_write()) {
			rt_kprintf("rewrite default parameters success !\n");
		}
		else {
			rt_kprintf("rewrite default parameters fail !\n");
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_upload(void)
{
   bsp_internal_flash_read((uint8_t *)&env.paras_inform, sizeof(app_paras_t));
   if( !((env.paras_inform.head[0] == DATA16_H(APP_PARAS_CHUNK_HEAD)) &&
	   (env.paras_inform.head[1] == DATA16_L(APP_PARAS_CHUNK_HEAD)) &&
	   (env.paras_inform.check_sum == check_xor_sum(&env.paras_inform,
											                  STRUCT_OFFSET(app_paras_t, check_sum),
												               APP_PARAS_CHUNK_CHECKSUM_CODE))) )
    {
		rt_kprintf("\t->app paras check fail !\n");
		rt_kprintf("\t->read old paras :\n");
		app_paras_net_printf();

		// 将默认配置新保存在相应位置
		memmove(&env.paras_inform.paras, &def_paras, sizeof(paras_t));

		memcpy(env.paras_inform.paras.sys_para.prod_fixed.prod_number,"20210306",9);
		memcpy(env.paras_inform.paras.sys_para.prod_fixed.prod_timer,"2021.03.06",11);
		memcpy(env.paras_inform.paras.sys_para.prod_fixed.hard_version,"LH-TFDSN-CLB",13);
		memcpy(env.paras_inform.paras.sys_para.dev_para.train_number,"2021",5);
		memcpy(env.paras_inform.paras.sys_para.dev_para.car_num,"1",2);
		env.paras_inform.paras.sys_para.dev_para.board_num = CU_LI_BOADR;
		memcpy(env.paras_inform.paras.sys_para.dev_para.install_time,"2021.03.06",11);
		memcpy(env.paras_inform.paras.sys_para.dev_para.old_soft_version,"1.1",4);
		memcpy(env.paras_inform.paras.sys_para.dev_para.curr_soft_version,"1.2",4);
		memcpy(env.paras_inform.paras.sys_para.dev_para.upgr_time,"2021.03.06",11);
		memcpy(env.paras_inform.paras.sys_para.dev_para.conf_time,"2021.03.06",11);
		env.paras_inform.paras.sys_para.dev_para.reset_flag=0;
		env.paras_inform.paras.train_num=1;
		env.paras_inform.paras.group_num=6;

      app_paras_write();
    }
	app_paras_check(&env.paras_inform.paras);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
uint8_t app_paras_update(void)
{
	uint8_t result = 0;
	result = app_paras_write();
   return result;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void app_paras_save_init(comm_para_t *comm_para_config)
{
//	comm_para_t comm_para_config;
	char temp[16];

	rt_memset(&env.paras_inform, 0, sizeof(app_paras_t));

    app_paras_upload();

	rt_kprintf("\t->read flash clb start-up paras:\n");

	app_paras_net_printf();

	//车厢号，若是非法车厢号，默认为1车
	env.train_num = env.paras_inform.paras.train_num;
	if (env.train_num<1 ||env.train_num>6) env.train_num = 1;
	env.group_num = env.paras_inform.paras.group_num;
	//编组号，若时非法编组，默认为6编
	if (env.group_num!=4 && env.group_num!=6) env.group_num = 6;
	//主从标识初始化
	if ((APP_BOARD_TRAIN_HEAD==env.train_num) || (env.train_num==env.group_num))
	{
		env.host_flag = 1;
	}
	else env.host_flag = 0;
	//前置器数量初始化
	if (6 == env.group_num) 	 env.host_qzq_num = DEF_QZQ_HOST_NUM_2;
	else if (4 == env.group_num)  env.host_qzq_num = DEF_QZQ_HOST_NUM_3;

	comm_para_config->train_id = env.train_num;

	sprintf(temp,"%d.%d.%d.%d", 192, 168, env.train_num, 13);
	strcpy(comm_para_config->local_ip, temp);
	strcpy(comm_para_config->local_gateway, "192.168.0.1");
	strcpy(comm_para_config->local_mask, "255.255.0.0");
//	write_communi_ini(&comm_para_config);
}



	
	
	
	
	
	
	
	
	









