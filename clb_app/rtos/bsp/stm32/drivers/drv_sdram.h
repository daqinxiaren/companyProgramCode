/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the drv_sdram.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 12, 2021
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
 * Mar 12, 2021		  kernel       create this file
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
 
#ifndef _DRV_SDRAM_H_
#define _DRV_SDRAM_H_

/**
 ***************** include file ***************
 */
#include <drv_sdram.h>

#ifdef  ENABLE_SDRAM    /**< ENABLE_SDRAM module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */
/* parameters for sdram peripheral */
/* Bank1 or Bank2 */
#define SDRAM_TARGET_BANK               1
/* stm32f4 Bank1:0XC0000000  Bank2:0XD0000000 */
#define SDRAM_BANK_ADDR                 ((uint32_t)0XC0000000)
/* data width: 8, 16, 32 */
#define SDRAM_DATA_WIDTH                32
/* column bit numbers: 8, 9, 10, 11 */
#define SDRAM_COLUMN_BITS               8
/* row bit numbers: 11, 12, 13 */
#define SDRAM_ROW_BITS                  12
/* cas latency clock number: 1, 2, 3 */
#define SDRAM_CAS_LATENCY               3
/* read pipe delay: 0, 1, 2 */
#define SDRAM_RPIPE_DELAY               1
/* clock divid: 2, 3 */
#define SDCLOCK_PERIOD                  2
/* refresh rate counter */
#define SDRAM_REFRESH_COUNT             ((uint32_t)0x0569)
#define SDRAM_SIZE                      ((uint32_t)0x1000000)

/* Timing configuration for W9825G6KH-6 */
/* 90 MHz of SD clock frequency (180MHz/2) */
/* TMRD: 2 Clock cycles */
#define LOADTOACTIVEDELAY               2
/* TXSR: 7x11.90ns */
#define EXITSELFREFRESHDELAY            7
/* TRAS: 4x11.90ns */
#define SELFREFRESHTIME                 4
/* TRC:  7x11.90ns */
#define ROWCYCLEDELAY                   7
/* TWR:  2 Clock cycles */
#define WRITERECOVERYTIME               2
/* TRP:  2x11.90ns */
#define RPDELAY                         2
/* TRCD: 2x11.90ns */
#define RCDDELAY                        2

/* memory mode register */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)


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
 * Mar 12, 2021		  kernel       create this class
 *						
 */




/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /**< end ENABLE_SDRAM module api major */

#endif /* STM32_LIBRARIES_HAL_DRIVERS_DRV_SDRAM_H_ */

