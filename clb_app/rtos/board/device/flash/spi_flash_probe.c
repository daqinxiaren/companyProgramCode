/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the spi_flash_probe.c API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 2, 2021
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
 * Mar 2, 2021		  kernel       create this file
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
#include "spi_flash_probe.h"
#ifdef  ENABLE_SPI_FLASH    /**< ENABLE_SPI_FLASH module api major */
#include "spi_flash.h"
#include "spi_flash_sfud.h"
#include "drv_spi.h"
#ifdef ENABLE_SFUD
#include <dfs.h>
#include <dfs_fs.h>
#include "spi_flash_sfud.h"
#endif

#ifdef  ENABLE_SPI_FLASH_FS
#include <romfs_config.h>
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

#if defined(ENABLE_SPI1)
	#define SPI_FLASH_BUS_NAME	 		("spi1") 			/**< major version number */
	#define SPI_FLASH_DEV_NAME			("spi10")
	#define SPI_FLASH_IO_BANK			(GPIOA)
	#define SPI_FLASH_IO_PIN			(GPIO_PIN_4)
	#define SPI_FLASH_CHIP_NAME		("w25q256")
	#define SPI_FLASH_FS_NAME			("elm")
#elif defined(ENABLE_SPI3)
	#define SPI_FLASH_BUS_NAME	 		("spi3") 			/**< major version number */
	#define SPI_FLASH_DEV_NAME			("spi30")
	#define SPI_FLASH_IO_BANK			(GPIOA)
	#define SPI_FLASH_IO_PIN			(GPIO_PIN_15)
	#define SPI_FLASH_CHIP_NAME		("w25q256")
	#define SPI_FLASH_FS_NAME			("elm")
#else
#error "enable SPI flash error!"
#endif


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
 * Mar 2, 2021		  kernel       create this class
 *						
 */
static int hw_spi_flash_init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	rt_hw_spi_device_attach(SPI_FLASH_BUS_NAME, SPI_FLASH_DEV_NAME, SPI_FLASH_IO_BANK, SPI_FLASH_IO_PIN);

	if (OS_NULL == rt_sfud_flash_probe(SPI_FLASH_CHIP_NAME, SPI_FLASH_DEV_NAME))
	{
		rt_kprintf("\r\nNOTE:\r\n");
		rt_kprintf("\tSFUD flash probe \t\t[fail]!\r\n");
		return -OS_ERROR;
	};
	return OS_EOK;
}
INIT_PREV_EXPORT(hw_spi_flash_init);

#ifdef ENABLE_SPI_FLASH_FS
void mount_spi_flash_fs(void)
{
	struct statfs state;
	if (dfs_mount(SPI_FLASH_CHIP_NAME, ROMFS_DIR1_MNT, SPI_FLASH_FS_NAME, 0, 0) == 0)
	{
		rt_kprintf("\r\nNOTE:\r\n");
		rt_kprintf("\tDFS mount %s \t\t\t[success]!\r\n", SPI_FLASH_CHIP_NAME);
		if (dfs_statfs(ROMFS_DIR1_MNT, &state) == 0)
		{
			rt_kprintf("\tSPI FLASH FS block size %d\r\n", state.f_bsize);
			rt_kprintf("\tSPI FLASH FS total data blocks %d\r\n", state.f_blocks);
			rt_kprintf("\tSPI FLASH FS free blocks %d\r\n", state.f_bfree);
		}
	}
	else
	{
		rt_kprintf("\r\nNOTE:\r\n\tFile system does not exist!\r\n");
		rt_kprintf("\tFile system will be created on %s!\r\n", SPI_FLASH_CHIP_NAME);
		dfs_mkfs(SPI_FLASH_FS_NAME, SPI_FLASH_CHIP_NAME);
		if (dfs_mount(SPI_FLASH_CHIP_NAME, "/", SPI_FLASH_FS_NAME, 0, 0) == 0)
		{
			rt_kprintf("\tDFS mount %s success!\r\n", SPI_FLASH_CHIP_NAME);
			if (dfs_statfs(ROMFS_DIR1_MNT, &state) == 0)
			{
				rt_kprintf("\tSPI FLASH FS block size %d\r\n", state.f_bsize);
				rt_kprintf("\tSPI FLASH FS total data blocks %d\r\n", state.f_blocks);
				rt_kprintf("\tSPI FLASH FS free blocks %d\r\n", state.f_bfree);
			}
		}
	}
}
//INIT_ENV_EXPORT(dfs_mount_spi_flash);

#endif

/** @} */    //\defgroup module API
/** @} *///\addtogroup dev module
#endif /**< end ENABLE_SPI_FLASH module api major */
