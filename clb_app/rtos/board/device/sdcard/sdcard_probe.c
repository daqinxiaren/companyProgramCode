/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the sdcard_probe.c API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 6, 2021
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b> @NOTE </b> 	: null
 *
 * \b @Warning      		: null
 * 
 * *
 * Change Logs:
 * Date           author       notes
 * Mar 6, 2021		  kernel       create this file
 * 
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
 ***************** include file *******************
 */
#include "sdcard_probe.h"

#ifdef  ENABLE_SDIO_FS    /**< ENABLE_SDIO_FS module api major */
#include <dfs_elm.h>
#include <dfs_fs.h>
#include <dfs_posix.h>

#include <romfs_config.h>
  /**SDIO GPIO Configuration
    PC8     ------> SDIO_D0
    PC9     ------> SDIO_D1
    PC10     ------> SDIO_D2
    PC11     ------> SDIO_D3
    PC12     ------> SDIO_CK
    PD2     ------> SDIO_CMD
    */
/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/

/**@}*/

/**
 * @formatter:off
 ****************<Private macro>********************
 */

/**
 * xxx  macro define	
 *                    - description 
 * @param  	xxx      :- description.
 */
//#define desc_class(xxx, xxxx) 									"\"
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/**
 * @formatter:on 
 ***************<Private typedef>*******************
 */

/**
 * xxx  typedef define	
 * @description		: null 
 * @note			      : null 
 */

/**
 ***************<Private define>********************
 */

/**
 * xxx  typedef define	
 * @description		: null 
 * @note			      : null 
 */

/**
 **************<Private variables>******************
 */

/**
 **************<Private functions>******************
 */
void sd_mount(void *parameter)
{
	int retry_count = 0;
	do
	{
		rt_thread_mdelay(500);
		// 没找到OS_NULL
		if (rt_device_find("sd0") != OS_NULL)
		{
			if (dfs_mount("sd0", ROMFS_DIR3_MNT, "elm", 0, 0) == OS_EOK)
			{
				rt_kprintf("\r\nNOTE:\r\n");
				rt_kprintf("\tSD Card mount to '%s'\r\n", ROMFS_DIR3_MNT);
				break;
			}
			else
			{
				//dfs_mkfs("sd0", SPI_FLASH_CHIP_NAME);
				retry_count++;
			}
		}
		else
		{
			rt_kprintf("\r\nNOTE:\r\n");
			rt_kprintf("\t\tSD Card device 'sd0'\t\t[null]!\r\n");
			break;
		}
		if(retry_count > 10)
		{
			rt_kprintf("\r\nNOTE:\r\n");
			rt_kprintf("\t\tSD Card mount to '%s'\t\t[failed]!\r\n", ROMFS_DIR3_MNT);
		}
	}
	while (retry_count > 10);
}

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
 * Parameters
 * *
 * @param  xxx	               new node to be inserted
 * @param [ in or out ]	xxx	new node to be inserted
 * * 
 * @return 
 *			- xxx   description  or none ,the operation status or on successful
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
 * Date           author       notes
 * Mar 6, 2021		  kernel       create this class
 *						
 */
void mount_sdcard_fs(void)
{
	rt_thread_t tid;

	tid = rt_thread_create("sd_mount", sd_mount, OS_NULL, 1024, THREAD_PRIORITY_MAX - 2, 20);
	if (tid != OS_NULL)
	{
		rt_thread_startup(tid);
	}
	else
	{
		rt_kprintf("\r\nNOTE:\r\n");
		rt_kprintf("\tCreate SD mount thread\t\t[error]!\r\n");
	}
}
//INIT_APP_EXPORT(mount_sdcard_fs);

/** @} */    //\defgroup module API
/** @} *///\addtogroup dev module
#endif /**< end ENABLE_SDIO_FS module api major */
