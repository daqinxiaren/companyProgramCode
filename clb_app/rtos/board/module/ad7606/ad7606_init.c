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
 * @date         		: 2020.06.02
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
 * 2020.06.02		kernel       create this file
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
#include <drv_gpio.h>
#include <drv_config.h>
#include <io_config.h>
#include "ad7606_init.h"
#define DRV_DEBUG
#define LOG_TAG         "fsmc.ad7606"
#include <drv_log.h>

#ifdef ENABLE_FSMC_ADC7606

/* BUSY */
#define BUSY_GPIO		   (AD7606_BUSY_IO_PORT)
#define BUSY_GPIO_PIN 	(AD7606_BUSY)
#define BUSY 				GET_PIN(F,AD7606_BUSY)

/* CONVST */
#define CONVST_1()		(AD7606_CONVST_IO_PORT->BSRR = AD7606_CONVST)
#define CONVST_0()		(AD7606_CONVST_IO_PORT->BSRR = (uint32_t)AD7606_CONVST << 16U)
#define CONVST 			GET_PIN(F,AD7606_CONVST)

/* RESET */
#define RESET_1()	  		(AD7606_RESET_IO_PORT->BSRR = AD7606_RESET)
#define RESET_0()	  		(AD7606_RESET_IO_PORT->BSRR = (uint32_t)AD7606_RESET << 16U)
#define RESET				GET_PIN(F,AD7606_RESET)


/*AD7606FSMC-NE3*/
//#define AD_RESULT()	*(__IO uint16_t *)0x64000000

/*AD7606FSMC-NE4*/
#define AD_RESULT()		(*(__IO uint16_t *)0x6C000000)

/*ad7606_fsmc_Init*/
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

//static void ad7606_reset(void)
//{
////	RESET_1();
//	rt_pin_write(RESET,PIN_HIGH);
//	rt_thread_mdelay(3);
////	RESET_0();
//	rt_pin_write(RESET,PIN_LOW);
//	rt_thread_mdelay(1);
//}

/**
 * @brief ad7606 initialization
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2020.06.02
 * @create time  : 2020.06.02
 * *
 * Parameters
 * *
 * @param	param	new node to be inserted
 *
 * @param [ in or out ]	param	new node to be inserted
 *
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
 * Date           author       notes
 * 2020.06.02		kernel       create this class
 *
 */
//void ad7606_init(void)
//{
//	rt_pin_mode(RESET, PIN_MODE_OUTPUT);
//	rt_pin_mode(CONVST, PIN_MODE_OUTPUT);
//	rt_pin_mode(BUSY, PIN_MODE_INPUT);
//
//	rt_thread_mdelay(1);
////	RESET_0();
//	rt_pin_write(RESET,PIN_LOW);
////	CONVST_1();
//	rt_pin_write(CONVST,PIN_HIGH);
//	rt_thread_mdelay(1);
//	ad7606_reset();
//}
//
//void ad7606_convst(void)
//{
////	CONVST_0();
//	rt_pin_write(CONVST,PIN_LOW);
//	rt_thread_mdelay(1);
////	CONVST_1();
//	rt_pin_write(CONVST,PIN_HIGH);
//	rt_thread_mdelay(1);
//}

//void ad7606_read(uint16_t * ad7606_value)
//{
////	while(BUSY_GPIO->IDR & BUSY_GPIO_PIN);/*AD_BUSY 高电平正在转换*/
//	while(rt_pin_read(BUSY));/*AD_BUSY 高电平正在转换*/
//
////	CONVST_0();
//	rt_pin_write(CONVST,PIN_LOW);
//	/**/
//	ad7606_value[0] = AD_RESULT();  //V1 OUT_C
//	ad7606_value[1] = AD_RESULT();  //V2
//	ad7606_value[2] = AD_RESULT();  //V3
//	ad7606_value[3] = AD_RESULT();  //V4
//	ad7606_value[4] = AD_RESULT();  //V5
//	ad7606_value[5] = AD_RESULT();  //V6
////	ad7606_value[6] = AD_RESULT();  //V7
////	ad7606_value[7] = AD_RESULT();  //V8
//	/*AD_CONVST */
////	CONVST_1();
//	rt_pin_write(CONVST,PIN_HIGH);
//}

#endif /* ENABLE_FSMC_ADC7606 */
