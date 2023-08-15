/**
 *
 * Copyright (c) 2015-2020  LH  Development
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: 故障工程管理 file
 *
 * @features			:
 *
 * @author       		: www
 *
 * @version      		: V 1.0.0
 *
 * @date         		: 2020年6月15日
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
 *	2020年6月15日				www         create this file
 *	2020年6月15日
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
#define ENABLE_FAULT_HAND

/**
 ***************** include file ***************
 */
#include <rtconfig.h>
#if defined(ENABLE_FAULT_HAND)
#include <stdio.h>
#include <rtthread.h>
#include <fault_hand.h>
#include "dev_config.h"
//#include <xfer_protocol.h>
#include <util.h>
#include <rthw.h>
#include "app_cominform_config.h"
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
struct fault_data faults;
//extern void udp_cast_send(uint8_t type,uint8_t data);

/**
 **************<Private variables>**************
 */


/**
 **************<Private functions>**************
 */



/**
 **************<Public IMPL functions>**************
 */

/**
 * @addtogroup group name
 */

/**@{*/


/**@}*/

/**
 * @brief development description of class
 * *
 * @author	www  write time: 2020年6月15日
 * @create time  : 2020年6月15日
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
 * 2020年6月15日			www			create this class
 *
 */

void fault_hand_data_init(void)
{
	memset(&faults,0,sizeof(struct fault_data));
}

//qzq deal
void qzq_fault_deal(uint8_t flag)
{
	switch(flag)
	{
		case 0:

			save_sys_para();
			rt_hw_cpu_reset();//reset clb
			break;

		case 1:
			rt_kprintf("reboot qzq1\n");
			can_lh_send_reset(1);
			break;

		case 2:
			rt_kprintf("reboot qzq2\n");
			can_lh_send_reset(2);
			break;

		case 3:
			rt_kprintf("reboot qzq3\n");
			can_lh_send_reset(3);
			break;

		case 4:
			rt_kprintf("reboot qzq4\n");
			can_lh_send_reset(4);
			break;

		default:
			break;
	}


}


//eth deal
void eth_fault_deal(void)
{
	rt_kprintf("eth---err reset clb \n");
//	udp_cast_send(SYNC_ST_CMD,0);//sync record
	save_sys_para();
	rt_hw_cpu_reset();//reset clb

}


/** @} */ //\defgroup module API fill_polygon_data

/** @} */ //\addtogroup dev module

#endif //\end module API
/* !< end file   */


