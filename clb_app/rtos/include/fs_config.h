/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the fs_config.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 10, 2021
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
 * Mar 10, 2021		  kernel       create this file
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
 
#ifndef FS_CONFIG_H_
#define FS_CONFIG_H_

/**
 ***************** include file ***************
 */
#include <kernel_config.h>
#include <api_config.h>

#ifdef  ENABLE_RTOS    /**< ENABLE_RTOS module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */
/* Device virtual file system */
#ifdef ENABLE_FS
#define ENABLE_DFS

#ifdef ENABLE_DFS
	#define DFS_ENABLE_WORKDIR
	#define DFS_FILESYSTEMS_MAX 				4
	#define DFS_FILESYSTEM_TYPES_MAX 		4
	#define DFS_FD_MAX 							16
#endif

#define ENABLE_DFS_ELMFAT

/* elm-chan's FatFs, Generic FAT Filesystem Module */
#ifdef ENABLE_DFS_ELMFAT
	#define DFS_ELM_CODE_PAGE 					437
	#define DFS_ELM_WORD_ACCESS
	#define DFS_ELM_USE_LFN_3
	#define DFS_ELM_USE_LFN 					3
	#define DFS_ELM_MAX_LFN 					255
	#define DFS_ELM_DRIVES 						4
	#define DFS_ELM_MAX_SECTOR_SIZE 			4096
	#define DFS_ELM_REENTRANT
	#define DFS_ELM_USE_ERASE
	#define DFS_ELM_REENTRANT
#endif

/**
 * TODO enable devfs, newlib
 */
//#define ENABLE_DFS_MNTTABLE
 /** !< enable devfs module */
#define ENABLE_DFS_DEVFS
#define ENABLE_DFS_ROMFS

//#define ENABLE_FLASH_FAL
#ifdef  ENABLE_FLASH_FAL
	#define FAL_PART_HAS_TABLE_CFG
#endif


#if defined(ENABLE_SDIO) && defined(ENABLE_SPI_FLASH)
	#ifdef ENABLE_SFUD
		#define ENABLE_SPI_FLASH_FS
	#endif
	#ifdef ENABLE_SDIO
		#define ENABLE_SDIO_FS
	#endif

	#if defined(ENABLE_SDIO_FS) && defined(ENABLE_SPI_FLASH_FS) &&  !defined(ENABLE_DFS_ROMFS)
		#error multiple mounting points need to turn on ROMFS!
	#endif

#elif defined(ENABLE_SDIO) || defined(ENABLE_SPI_FLASH)
	#if   defined(ENABLE_SDIO)
		#define ENABLE_SDIO_FS
	#elif defined(ENABLE_SFUD)
		#define ENABLE_SPI_FLASH_FS
	#endif
#endif //defined(ENABLE_SDIO) && defined(ENABLE_SPI_FLASH)

#endif /* END ENABLE_FS */

/**
 * xxx  macro define	
 *                    - description 
 * @param  	xxx      :- description.
 */
//#define desc_class(xxx, xxxx) 									"\"
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/**
 *  @formatter:on 
 ***************<Public typedef>***************
 */


/**
 **************<Public variables>**************
 */



#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/
/**
 **************<inline functions>**************
 */

/**
 * xxx  functions define	
 *                    - description 
 * @param  	xxx      :- description.
 */



/**
 **************<Public functions>**************
 */

/**
 * @brief development description of class
 * *
 * Parameters
 * *
 * @param  xxx               -	new node to be inserted
 * @param [ in or out ]	xxx  -	new node to be inserted
 *
 * * 
 * @return 
 *			- xxx   description or none,the operation status or on successful
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
 * Mar 10, 2021		  kernel       create this class
 *						
 */




/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /**< end ENABLE_RTOS module api major */

#endif /* FS_CONFIG_H_ */

