/**
 *
 * Copyright (c) 2015-2020  LH  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the can_app.c API or library.
 *
 * @features			:
 *
 * @author       		: www
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020年7月4日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b>@note</b> 		: null
 *
 * \b <b> @warning </b> : null
 * 
 * *
 * Change Logs:
 *	Date				author       notes
 *	2020年7月4日				www         create this file
 *	2020年7月4日
 * 
 */

/**
 * \addtogroup dev module
 * @{
 */

/**
 * \defgroup module API
 *
 * The xxx API module defines a set of functions that a xxx device
 * driver must implement.
 *
 * @{
 */

/**
 ***************** include file ***************
 */
#include <drv_config.h>
#if defined(ENABLE_CAN1) || defined(ENABLE_CAN2)
#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <can_app.h>
#include "util.h"

/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/
/**@}*/

/** @formatter:off */
/**
 ***************<Private macro>****************
 */

/**
 * @brief macro define	- description 
 * @param	param	- description.
 * @param	param	- description.
 */
//#define desc_class(xxx, xxxx)
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/** @formatter:on */

/**
 ***************<Private define>****************
 */
#define PRO_HEAD_LEN     12               /* CAN 协议组包固定长度 */
extern struct switch_ch switchs;

/**
 **************<Private variables>**************
 */
tfdsn_proximitor_t proximitor_data;
/**
 **************<Private functions>**************
 */

/**
 **************<Public IMPL functions>**************
 */

/**
 * @addtogroup group name 
 */
qzq_deal_t qzq_deal_buff = {0};
#define qzq qzq_deal_buff
/**@{*/
static void can_lh_cmd(uint8_t taget, uint8_t id, uint8_t *data, uint32_t len, enum can_dev candev_t);
/**@}*/

/**
 * @brief ad7606 data initialization
 * *
 * @author	www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param [ in or out ]	ad7606_data_p ad7606_data  ad7606 data point to initialization
 * *
 * @return
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			init/destroy ad7606
 *
 * @warning
 *
 * Change Logs:
 * Date				author			notes
 * 2020年6月8日			www			create this class
 *
 */

/**
 * @brief  获取前置器ID，用于发送id时
 * @param
 * @return 存放前置器id的buffer指针
 * @note
**/
qzq_id_buff_t * app_can_qzq_get_id(void)
{
    return &qzq.qzq_id;
}
/**
 * @brief  清零前置器id buffer
 * @param
 * @return
 * @note
**/
void app_can_qzq_clean_id(void)
{
    memset(&qzq.qzq_id, 0, sizeof(qzq_id_buff_t));
}
/**
 * @brief  设置前置器ID
 * @param
 * @return
 * @note
**/
void config_qzq_id(uint8_t oldaddr,uint8_t newaddr)
{
    uint8_t m[1];//
    m[0]=newaddr;

//    can_send_reset_qzq(oldaddr);
    can_lh_send_reset(oldaddr);
    rt_thread_mdelay(5000);
//    send_iap(oldaddr, SET_ID_CMD, m, 1);
    can_lh_cmd(oldaddr, SET_ID_CMD, m, 1, 0);
    printf("set qzqid :%d--->%d\n",oldaddr,newaddr);
}

void proximitor_data_init(tfdsn_proximitor_p proximitor_data)
{
	memset(proximitor_data,0,sizeof(tfdsn_proximitor_t));

	return;
}

/**
 * @brief can initialization
 * *
 * @author	www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * *
 * @return
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			init/destroy ad7606
 *
 * @warning
 *
 * Change Logs:
 * Date				author			notes
 * 2020年6月8日			www			create this class
 *
 */
void can_init()
{
	//tfdsn_proximitor_p proximitor_data;

	//proximitor_data = proximitor_data_get();
	proximitor_data_init(&proximitor_data);

	can_rx_thread_init();
}

/**
 * @brief development description of class
 * *
 * @author	www  write time: 2020年7月4日
 * @create time  : 2020年7月4日
 * *
 * Parameters
 * *
 * @param				param	new node to be inserted
 * @param [ in or out ]	param	new node to be inserted
 * * 
 * @return 
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			description
 *							
 * @warning			description 
 *	
 * Change Logs:
 * Date				author			notes
 * 2020年7月4日			www			create this class
 *						
 */
void printf_proximitor_data(proximitor_data_p proximitor_data,uint8_t qzqid)
{
	rt_kprintf("QZQ%d_data:",qzqid);
	for(int i = 0; i < 5; i++)//put_senosr
	{
		rt_kprintf(" %d",(proximitor_data->temp_orignal[i][0] - 1270) / 10);
	}
	rt_kprintf(" sensor:0x%x-0x%x\n\n",proximitor_data->vib_sensor_status,proximitor_data->temp_sensor_status);

}

/**
 * @brief development description of class
 * *
 * @author	www  write time: 2020年7月4日
 * @create time  : 2020年7月4日
 * *
 * Parameters
 * *
 * @param				param	new node to be inserted
 * @param [ in or out ]	param	new node to be inserted
 * *
 * @return
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date				author			notes
 * 2020年7月4日			www			create this class
 *
 */
/*RX_deal 应用层数据解析*/
void rx_deal(uint8_t *data, uint32_t len)
{
	if(*(uint16_t*) (&data [0]) == 0xAA55 && *(uint16_t*) (&data [2]) == len)
	{
		uint8_t crc = 0;
		for (int j = 0; j < len - 1; j++)
		{
			crc += data [j];
			rt_kprintf("%x ",data[j]);
		}
		rt_kprintf("\n");
		if (crc == data [len - 1])
		{
			switch (data [4])
			{
				
				case SYS_DATA_CMD:

					switch(data[6])
					{
						case proximitor1_board:

							proximitor_data.proximitor_status.proximitor1_comm_status = 0;
							proximitor_data.proximitor_status.proximitor1_power_on = 1;
							memmove(&proximitor_data.proximitor_data1,&data[11],sizeof(proximitor_data_t));

							printf_proximitor_data(&proximitor_data.proximitor_data1,1);
							break;

						case proximitor2_board:

							proximitor_data.proximitor_status.proximitor2_comm_status = 0;
							proximitor_data.proximitor_status.proximitor2_power_on = 1;
							memmove(&proximitor_data.proximitor_data2,&data[11],sizeof(proximitor_data_t));

							printf_proximitor_data(&proximitor_data.proximitor_data2,2);
							break;

						case proximitor3_board:

							proximitor_data.proximitor_status.proximitor3_comm_status = 0;
							proximitor_data.proximitor_status.proximitor3_power_on = 1;
							memmove(&proximitor_data.proximitor_data3,&data[11],sizeof(proximitor_data_t));

							printf_proximitor_data(&proximitor_data.proximitor_data3,3);
							break;

						case proximitor4_board:

							proximitor_data.proximitor_status.proximitor4_comm_status = 0;
							proximitor_data.proximitor_status.proximitor4_power_on = 1;
							memmove(&proximitor_data.proximitor_data4,&data[11],sizeof(proximitor_data_t));

							printf_proximitor_data(&proximitor_data.proximitor_data4,4);
							break;
					}

					break;

				case SYSTEM_RESET_CMD:
				    if(data[6] == proximitor1_board)
				    {
				        rt_kprintf("proximitor1 board reset!\n");
				    }
				    else if(data[6] == proximitor2_board)
				    {
				        rt_kprintf("proximitor2 board reset!\n");
				    }
				    else if(data[6] == proximitor3_board)
                    {
				        rt_kprintf("proximitor3 board reset!\n");
                    }
				    else if(data[6] == proximitor4_board)
                    {
				        rt_kprintf("proximitor4 board reset!\n");
                    }
					break;
					
				case SET_ID_CMD:
					rt_kprintf("old_board_id:%d new_board_id:%d \n", data [11], data [12]);
                    udp_para_ack(SET_ID_CMD,&data [11],2);
					break;

                case GET_ID_CMD:
                    rt_kprintf(" ------- QZQ ID = %d   id_index = %d\n", data[6],  qzq.qzq_id.id_index);
                    qzq.qzq_id.getid[qzq.qzq_id.id_index] = data[6];//data [6]是获取的ID
                    qzq.qzq_id.id_index++;
                    break;
					
				default:

					break;
			}
		}
	}
}

/*can数据发送 组包*/
void can_lh_send(uint8_t target, uint8_t id, uint8_t *data, uint32_t len)
{
	
	static uint8_t can_data [256];
	uint8_t crc, i, candev_t;
	can_data [0] = 0x55;
	can_data [1] = 0xAA;
	can_data [2] = len + PRO_HEAD_LEN;
	can_data [3] = (uint8_t) ((len + PRO_HEAD_LEN) >> 8);
	can_data [4] = id;
	can_data [5] = target;
	can_data [6] = BOARD_NUMB;
	
	can_data [7] = 0x00;
	can_data [8] = 0x00;
	can_data [9] = 0x00;
	can_data [10] = 0x00;
	
	for (i = 0; i < len; i++)
	{
		can_data [11 + i] = data [i];
	}
	
	crc = 0;
	for (i = 0; i < PRO_HEAD_LEN + len - 1; i++)
	{
		crc += can_data [i];
	}
	can_data [PRO_HEAD_LEN + len - 1] = crc;
	
	if (target == proximitor1_board || target == proximitor4_board)
	{
		candev_t = CAN_1;
	}
	else if (target == proximitor2_board || target == proximitor3_board)
	{
		candev_t = CAN_2;
	}
	else
	{
		candev_t = CAN_12;
	}
	
	can_send_data(can_data, PRO_HEAD_LEN + len, candev_t);
	
}

/*can config 组包*/
static void can_lh_cmd(uint8_t taget, uint8_t id, uint8_t *data, uint32_t len, enum can_dev candev_t)
{
	
	static uint8_t can_data [256];
	uint8_t crc, i;
	can_data [0] = 0x55;
	can_data [1] = 0xAA;
	can_data [2] = len + PRO_HEAD_LEN;
	can_data [3] = (uint8_t) ((len + PRO_HEAD_LEN) >> 8);
	can_data [4] = id;
	can_data [5] = taget;
	can_data [6] = BOARD_NUMB;
	
	can_data [7] = 0x00;
	can_data [8] = 0x00;
	can_data [9] = 0x00;
	can_data [10] = 0x00;
	
	for (i = 0; i < len; i++)
	{
		can_data [11 + i] = data [i];
	}
	
	crc = 0;
	for (i = 0; i < PRO_HEAD_LEN + len - 1; i++)
	{
		crc += can_data [i];
	}
	can_data [PRO_HEAD_LEN + len - 1] = crc;
	
	can_send_data(can_data, PRO_HEAD_LEN + len, candev_t);
}

/**
 *重启命令
 **/
void can_lh_send_reset(uint8_t taget)
{
	uint8_t data [1];
	data [0] = 0;
	can_lh_cmd(taget, SYSTEM_RESET_CMD, data,0, 0);
}

/**
 * @brief modify proximitor board ID
 * *
 * @author	www  write time: 2020年7月4日
 * @create time  : 2020年7月4日
 * *
 * Parameters
 * *
 * @uint8_t proximitor_curr_id
 * @uint8_t proximitor_modify_id
 * *
 * @return
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			description
 *
 * @warning			description
 *
 * Change Logs:
 * Date				author			notes
 * 2020年7月4日			www			create this class
 *
 */
void can_lh_send_set_dev_id(uint8_t proximitor_curr_id, uint8_t proximitor_modify_id, uint8_t can_bus)
{
	uint8_t data [1];
	
	data [0] = proximitor_modify_id;
	can_lh_cmd(proximitor_modify_id, SET_ID_CMD, data, 1, can_bus);
}

/**
 * @brief  发送获取前置器ID命令
 * @param
 * @return
 * @note
**/
void send_radio_getid(void)
{
    uint8_t m=0;//
//    send_iap(0x00, GET_ID_CMD, &m, 0);
    can_lh_cmd(0x00, GET_ID_CMD, &m,0, 0);
}

void proximitor_cmd(int argc, char **argv)
{
	
#define CMD_HELP_INDEX        			0
#define CMD_OPS_INDEX						1
#define CMD_PROXIMITOR_ID_INDEX			2
#define CMD_CAN_BUS_INDEX					3
	
#define OPS_RERET                     "r"
#define OPS_CONFIG_DEV_ID             "c"
	
#define CAN_BUS_1                     "can_1"
#define CAN_BUS_2                     "can_2"
#define CAN_BUS_12                    "can_12"
	
	const char *proximitor_cmd_help_info [] =
	        {
	                [CMD_HELP_INDEX] = "proximitor_cmd [ops] [proximitor_id] [value] [can_bus]   - config format,ops have r(re" ,
	                [CMD_OPS_INDEX
	                        ] = "proximitor_cmd -ops                      	               - ops have r(reset) c(config device id)" ,
	                [CMD_PROXIMITOR_ID_INDEX
	                        ] = "proximitor_cmd -proximitor_id                            - proximitor_id value range is 1-4" ,
	                [CMD_CAN_BUS_INDEX
	                        ] = "proximitor_cmd -can_bus                            		- can_bus maybe can_1,can_2,can_12" };
	
	char *ops = NULL;
	uint8_t proximitor_id = 0;
	uint8_t value = 0;
	char *can_bus = NULL;
	uint8_t can_bus_id = 0;
	
	int i = 0;
	
	if (argc < 2)
	{
		rt_kprintf("Usage:\n");
		for (i = 0; i < sizeof(proximitor_cmd_help_info) / sizeof(char*); i++)
		{
			rt_kprintf("%s\n", proximitor_cmd_help_info [i]);
		}
		rt_kprintf("\n");
	}
	else if (argc < 3)
	{
		rt_kprintf("ERROR:format is error!\n");
	}
	else if (argc < 4)
	{
		ops = argv [1];
		proximitor_id = srt_to_uint32(argv [2]);
		
		if (!strcmp(OPS_RERET, ops))
		{
			can_lh_send_reset(proximitor_id);
		}
		else
		{
			rt_kprintf("ERROR:ops parameter is error!\n");
		}
	}
	else if (argc < 5)
	{
		rt_kprintf("ERROR:format is error!\n");
	}
	else if (argc < 6)
	{
		ops = argv [1];
		proximitor_id = srt_to_uint32(argv [2]);
		value = srt_to_uint32(argv [3]);
		can_bus = argv [4];
		
		if (!strcmp(OPS_CONFIG_DEV_ID, ops))
		{
			if (!strcmp(CAN_BUS_1, can_bus))
			{
				can_bus_id = CAN_1;
			}
			else if (!strcmp(CAN_BUS_2, can_bus))
			{
				can_bus_id = CAN_2;
			}
			else if (!strcmp(CAN_BUS_12, can_bus))
			{
				can_bus_id = CAN_12;
			}
			else
			{
				rt_kprintf("ERROR:bus parameter is error!\n");
			}
			
			can_lh_send_set_dev_id(proximitor_id, value, can_bus_id);
		}
		else
		{
			rt_kprintf("ERROR:ops parameter is error!\n");
		}
	}
	else
	{
		rt_kprintf("ERROR:format is error!\n");
	}
}
MSH_CMD_EXPORT(proximitor_cmd, proximitor option);

/** @} */    //\defgroup module API
/** @} *///\addtogroup dev module
#endif //\end module API
/* !< end file   */
