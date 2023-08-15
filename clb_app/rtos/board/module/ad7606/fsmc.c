/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the fsmc_ad7606.c API or library.
 *
 * @features			:
 *
 * @author       		: 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020��6��2��
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
 * 2020��6��2��		  kernel       create this file
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
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
#include <drv_config.h>

#if defined(ENABLE_FSMC_ADC7606)
#include <fsmc.h>
#include "ad7606_app.h"
#include "ad7606.h"
#define DRV_DEBUG
#define LOG_TAG             "fsmc.ad7606"
#include <drv_log.h>


/*ad7606_fsmc_Init*/
//int ad7606_fsmc_Init()
//{
//	/* AD7606 device configuration */
//
//	SRAM_HandleTypeDef ad7606_t;
//	FMC_NORSRAM_TimingTypeDef Timing;
//
//	ad7606_t.Instance 						= FSMC_NORSRAM_DEVICE;
//	ad7606_t.Extended 						= FMC_NORSRAM_EXTENDED_DEVICE;
//
//	ad7606_t.Init.NSBank	 					= FSMC_NORSRAM_BANK4;
//	ad7606_t.Init.DataAddressMux 			= FSMC_DATA_ADDRESS_MUX_DISABLE;
//	ad7606_t.Init.MemoryType 				= FSMC_MEMORY_TYPE_SRAM;
//	ad7606_t.Init.MemoryDataWidth 		= FMC_NORSRAM_MEM_BUS_WIDTH_16;
//	ad7606_t.Init.BurstAccessMode 		= FSMC_BURST_ACCESS_MODE_DISABLE;
//	ad7606_t.Init.WaitSignalPolarity 	= FSMC_WAIT_SIGNAL_POLARITY_LOW;
//	ad7606_t.Init.WrapMode 					= FSMC_WRAP_MODE_DISABLE;
//	ad7606_t.Init.WaitSignalActive 		= FSMC_WAIT_TIMING_BEFORE_WS;
//	ad7606_t.Init.WriteOperation 			= FSMC_WRITE_OPERATION_DISABLE;
//	ad7606_t.Init.WaitSignal 				= FSMC_WAIT_SIGNAL_DISABLE;
//	ad7606_t.Init.ExtendedMode 			= FSMC_EXTENDED_MODE_DISABLE;
//	ad7606_t.Init.AsynchronousWait 		= FSMC_ASYNCHRONOUS_WAIT_DISABLE;//FSMC_WRITE_OPERATION_ENABLE;
//	ad7606_t.Init.WriteBurst 				= FSMC_WRITE_BURST_DISABLE;
//	ad7606_t.Init.ContinuousClock 		= FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
//
//	/* SRAM device configuration */
//	Timing.AddressSetupTime 				= AD7606_ADDRESS_SETUP_TIME;
//	Timing.AddressHoldTime	 				= AD7606_ADDRESS_HOLD_TIME; /* Min value, Don't care on SRAM Access mode A */
//	Timing.DataSetupTime 					= AD7606_DATA_SETUP_TIME;
//
//	Timing.BusTurnAroundDuration 			= AD7606_BUS_TURN_AROUND_DURATION;
//	Timing.CLKDivision 						= AD7606_CLK_DIVISION; /* Min value, Don't care on SRAM Access mode A */
//	Timing.DataLatency 						= AD7606_DATA_LATENCY; /* Min value, Don't care on SRAM Access mode A */
//	Timing.AccessMode 						= AD7606_ACCESS_MODE;
//
//	/** @formatter:on  */
//	/* Initialize the SRAM controller */
//	if (HAL_SRAM_Init(&ad7606_t, &Timing, &Timing) != HAL_OK)
//	{
//		LOG_E("ad7606_fsmc init failed!");
//		return -OS_ERROR;
//	}
//	return OS_EOK;
//}
//INIT_BOARD_EXPORT(ad7606_fsmc_Init);
int ad7606_fsmc_init()
{
	/* AD7606 device configuration */

	SRAM_HandleTypeDef ad7606_t;
	FMC_NORSRAM_TimingTypeDef Timing;

	ad7606_t.Instance 						= FSMC_NORSRAM_DEVICE;
	ad7606_t.Extended 						= FMC_NORSRAM_EXTENDED_DEVICE;

	ad7606_t.Init.NSBank	 					= FSMC_NORSRAM_BANK4;
	ad7606_t.Init.DataAddressMux 			= FSMC_DATA_ADDRESS_MUX_DISABLE;
	ad7606_t.Init.MemoryType 				= FSMC_MEMORY_TYPE_SRAM;
	ad7606_t.Init.MemoryDataWidth 		= FMC_NORSRAM_MEM_BUS_WIDTH_16;
	ad7606_t.Init.BurstAccessMode 		= FSMC_BURST_ACCESS_MODE_DISABLE;
	ad7606_t.Init.WaitSignalPolarity 	= FSMC_WAIT_SIGNAL_POLARITY_LOW;
	ad7606_t.Init.WrapMode 					= FSMC_WRAP_MODE_DISABLE;
	ad7606_t.Init.WaitSignalActive 		= FSMC_WAIT_TIMING_BEFORE_WS;
	ad7606_t.Init.WriteOperation 			= FSMC_WRITE_OPERATION_DISABLE;
	ad7606_t.Init.WaitSignal 				= FSMC_WAIT_SIGNAL_DISABLE;
	ad7606_t.Init.ExtendedMode 			= FSMC_EXTENDED_MODE_DISABLE;
	ad7606_t.Init.AsynchronousWait 		= FSMC_ASYNCHRONOUS_WAIT_DISABLE;//FSMC_WRITE_OPERATION_ENABLE;
	ad7606_t.Init.WriteBurst 				= FSMC_WRITE_BURST_DISABLE;
	ad7606_t.Init.ContinuousClock 		= FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC;

	/* SRAM device configuration */
	Timing.AddressSetupTime 				= AD7606_ADDRESS_SETUP_TIME;
	Timing.AddressHoldTime	 				= AD7606_ADDRESS_HOLD_TIME; 	/* Min value, Don't care on SRAM Access mode A */
	Timing.DataSetupTime 					= AD7606_DATA_SETUP_TIME;

	Timing.BusTurnAroundDuration 			= AD7606_BUS_TURN_AROUND_DURATION;
	Timing.CLKDivision 						= AD7606_CLK_DIVISION; 			/* Min value, Don't care on SRAM Access mode A */
	Timing.DataLatency 						= AD7606_DATA_LATENCY; 			/* Min value, Don't care on SRAM Access mode A */
	Timing.AccessMode 						= AD7606_ACCESS_MODE;

	/** @formatter:on  */
	/* Initialize the SRAM controller */
	if (HAL_SRAM_Init(&ad7606_t, &Timing, &Timing) != HAL_OK)
	{
		LOG_E("ad7606_fsmc init failed!");
		return -OS_ERROR;
	}
	return OS_EOK;
}
INIT_BOARD_EXPORT(ad7606_fsmc_init);
/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/
/**@}*/

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#endif //\end module API

