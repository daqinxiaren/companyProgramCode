/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the fs_init.c API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 5, 2021
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
 * Mar 5, 2021		  kernel       create this file
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
#include <rtthread.h>
#ifdef  ENABLE_FS    			/**< ENABLE_FS module api major */
#include <romfs_config.h>

#ifdef  ENABLE_SPI_FLASH
#include "spi_flash.h"
#include "drv_spi.h"

#ifdef ENABLE_SFUD
#include <dfs.h>
#include <dfs_fs.h>
#include "spi_flash_sfud.h"
#endif

#ifdef ENABLE_SPI_FLASH_FS
#include "spi_flash_probe.h"
#endif

#endif //ENABLE_SPI_FLASH

#ifdef ENABLE_SDIO_FS
#include "sdcard_probe.h"
#endif


#ifdef ENABLE_DFS_ROMFS
#include "dfs_romfs.h"
#endif

#ifdef  ENABLE_FLASH_FAL
#include <fal.h>
#endif


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
#ifdef ENABLE_DFS_ROMFS
void mnt_romfs()
{
	rt_kprintf("\tROM file system !\r\n");
	if (dfs_mount(OS_NULL, "/", "rom", 0, &(romfs_root)) == 0)
	{
		rt_kprintf("\tROM file system initializated;\n");    //LOG_I
	}
	else
	{
		rt_kprintf("\tROM file system initializate failed;\n");
	}
}
#endif

#ifdef ENABLE_DFS_RAMFS
void mnt_ramfs()
{
	   if(dfs_mount (RT_NULL,"/ram","ram",0,dfs_ramfs_create(rampool, sizeof(rampool))) == 0)
	    {
	        LOG_I("ram file system initializated;\n");
	    }
	    else
	    {
	        LOG_I("ram file system initializate failed;\n");
	    }
}
#endif

#ifdef ENABLE_SPI_FLASH_FS1
void mnt_flash_fs()
{
	struct statfs state;
	if (dfs_mount(SPI_FLASH_CHIP_NAME, ROMFS_DIR1_MNT, SPI_FLASH_FS_NAME, 0, 0) == 0)
	{
		rt_kprintf("\nDFS mount %s success!\r\n", SPI_FLASH_CHIP_NAME);
		if (dfs_statfs("/", &state) == 0)
		{
			rt_kprintf("\tFS block size %d\r\n", state.f_bsize);
			rt_kprintf("\tFS total data blocks %d\r\n", state.f_blocks);
			rt_kprintf("\tFS free blocks %d\r\n", state.f_bfree);
		}
	}
	else
	{
		rt_kprintf("NOTE:\r\n\tFile system does not exist!\r\n");
		rt_kprintf("\tFile system will be created on %s!\r\n", SPI_FLASH_CHIP_NAME);
		dfs_mkfs(SPI_FLASH_FS_NAME, SPI_FLASH_CHIP_NAME);
		if (dfs_mount(SPI_FLASH_CHIP_NAME, "/", SPI_FLASH_FS_NAME, 0, 0) == 0)
		{
			rt_kprintf("\nDFS mount %s success!\r\n", SPI_FLASH_CHIP_NAME);
			if (dfs_statfs("/", &state) == 0)
			{
				rt_kprintf("\tFS block size %d\r\n", state.f_bsize);
				rt_kprintf("\tFS total data blocks %d\r\n", state.f_blocks);
				rt_kprintf("\tFS free blocks %d\r\n", state.f_bfree);
			}
		}
	}
}
#endif






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
 * Mar 5, 2021		  kernel       create this class
 *						
 */
static int fs_init(void)
{
#ifdef ENABLE_FLASH_FAL
    /* partition initialized */
    fal_init();
#endif

#ifdef ENABLE_DFS_ROMFS
	mnt_romfs();
#endif

#ifdef ENABLE_DFS_RAMFS
    mnt_ramfs();
#endif

#ifdef ENABLE_SPI_FLASH_FS
    mount_spi_flash_fs();
#endif

#ifdef ENABLE_SDIO_FS
    mount_sdcard_fs();
#endif


	return 0;
}
INIT_ENV_EXPORT(fs_init);

/** @} */    //\defgroup module API

/** @} *///\addtogroup dev module

#endif /**< end ENABLE_FS module api major */
