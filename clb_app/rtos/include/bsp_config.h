/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the bsp_config.h API or library.
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
 
#ifndef BSP_CONFIG_H_
#define BSP_CONFIG_H_

/**
 ***************** include file ***************
 */
#include <system_config.h>
#include <arm_config.h>
#if defined(STM32F1XX)
#include <stm32f1xx.h>
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_conf.h>
#elif  defined(STM32F4XX)
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_conf.h>
#endif


#ifdef  ENABLE_DEVICE_MANAGE    /**< ENABLE_DEVICE_MANAGE module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */

#if defined(HAL_MODULE_ENABLED)

#endif

#if defined(HAL_ADC_MODULE_ENABLED)
	#define ENABLE_ADC
#endif

#if defined(HAL_CRYP_MODULE_ENABLED)
	#define ENABLE_HWCRYPTO
#endif

#if defined(HAL_CAN_MODULE_ENABLED)
	#define ENABLE_CAN
#endif

#if defined(HAL_CRC_MODULE_ENABLED)
	#define ENABLE_CRC
#endif

#if defined(HAL_CAN_LEGACY_MODULE_ENABLED)
	#define ENABLED_CAN_LEGACY
#endif

#if defined(HAL_DAC_MODULE_ENABLED)
	#define ENABLE_DAC
#endif

#if defined(HAL_DCMI_MODULE_ENABLED)
	#define ENABLE_DCMI
#endif

#if defined(HAL_DMA2D_MODULE_ENABLED)
	#define ENABLE_DMA2D
#endif

#if defined(HAL_ETH_MODULE_ENABLED)
	#define ENABLE_ETH
#endif

#if defined(HAL_NAND_MODULE_ENABLED)
	#define ENABLE_NAND
#endif

#if defined(HAL_NOR_MODULE_ENABLED)
	#define ENABLE_NOR
#endif

#if defined(HAL_PCCARD_MODULE_ENABLED)
	#define ENABLE_PCCARD
#endif

#if defined(HAL_SRAM_MODULE_ENABLED)
	#define ENABLE_SRAM
#endif

#if defined(HAL_SDRAM_MODULE_ENABLED)
	#define ENABLE_SDRAM
#endif

#if defined(HAL_HASH_MODULE_ENABLED)
	#define ENABLE_HASH
#endif

#if defined(HAL_I2C_MODULE_ENABLED)
	//#define ENABLE_I2C
#endif

#if defined(HAL_I2S_MODULE_ENABLED)
	#define ENABLE_I2S
#endif

#if defined(HAL_IWDG_MODULE_ENABLED)
	#define ENABLE_IWDG
#endif

#if defined(HAL_LTDC_MODULE_ENABLED)
	#define ENABLE_LTDC
#endif

#if defined(HAL_RNG_MODULE_ENABLED)
	#define ENABLE_RNG
#endif

#if defined(HAL_RTC_MODULE_ENABLED)
	#define ENABLE_RTC
#endif

#if defined(HAL_SAI_MODULE_ENABLED)
	#define ENABLE_SAI
#endif

#if defined(HAL_SD_MODULE_ENABLED)
	#define ENABLE_SD
#endif

#if defined(HAL_MMC_MODULE_ENABLED)
	#define ENABLE_MMC
#endif

#if defined(HAL_SPI_MODULE_ENABLED)
	#define ENABLE_SPI
#endif

#if defined(HAL_TIM_MODULE_ENABLED)
	#define ENABLE_TIM
#endif

#if defined(HAL_UART_MODULE_ENABLED)
	#define ENABLE_UART
#endif

#if defined(HAL_USART_MODULE_ENABLED)
	#define ENABLE_USART
#endif

#if defined(HAL_IRDA_MODULE_ENABLED)
	#define ENABLE_IRDA
#endif

#if defined(HAL_SMARTCARD_MODULE_ENABLED)
	#define ENABLE_SMARTCARD
#endif

#if defined(HAL_SMBUS_MODULE_ENABLED)
	#define ENABLE_SMBUS
#endif

#if defined(HAL_WWDG_MODULE_ENABLED)
	#define ENABLE_WWDG
#endif

#if defined(HAL_PCD_MODULE_ENABLED)
	#define ENABLE_USBD
#endif

#if defined(HAL_HCD_MODULE_ENABLED)
	#define ENABLE_USBH
#endif

#if defined(HAL_DSI_MODULE_ENABLED)
	#define ENABLE_DSI
#endif

#if defined(HAL_QSPI_MODULE_ENABLED)
	#define ENABLE_QSPI
#endif

#if defined(HAL_CEC_MODULE_ENABLED)
	#define ENABLE_CEC
#endif

#if defined(HAL_FMPI2C_MODULE_ENABLED)
	#define ENABLE_FMPI2C
#endif

#if defined(HAL_SPDIFRX_MODULE_ENABLED)
	#define ENABLE_SPDIFRX
#endif

#if defined(HAL_DFSDM_MODULE_ENABLED)
	#define ENABLE_DFSDM
#endif

#if defined(HAL_LPTIM_MODULE_ENABLED)
	#define ENABLE_LPTIM
#endif

#if defined(HAL_GPIO_MODULE_ENABLED)
	#define ENABLE_PIN
#endif

#if defined(HAL_EXTI_MODULE_ENABLED)

#endif

#if defined(HAL_DMA_MODULE_ENABLED)
	#define ENABLE_DMA
#endif

#if defined(HAL_RCC_MODULE_ENABLED)
	#define ENABLE_RCC
#endif

#if defined(HAL_FLASH_MODULE_ENABLED)
	#define ENABLE_FLASH
#endif

#if defined(HAL_PWR_MODULE_ENABLED)
	#define ENABLE_PWR
#endif

#if defined(HAL_CORTEX_MODULE_ENABLED)

#endif


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

#endif  /**< end XXX module api major */

#endif /* BSP_CONFIG_H_ */

