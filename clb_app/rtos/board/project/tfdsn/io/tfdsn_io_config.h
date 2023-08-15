/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the tfdsn_io_config.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2021年3月15日
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
 * 2021年3月15日		  kernel       create this file
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

#ifndef _TFDSN_IO_CONFIG_H_
#define _TFDSN_IO_CONFIG_H_

/**
 ***************** include file ***************
 */
#include <io_config.h>

#ifdef LH_TFDSN    /**< LH_TFDSN module api major */

/**
 * @addtogroup group name 
 */

/**@{*/

/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */

#if defined(ENABLE_CAN)
	#if defined(ENABLE_CAN1)
		/**
		 * CAN1 GPIO Configuration
		 * PA11     ------> CAN1_RX
		 * PA12     ------> CAN1_TX
		 */

		#define CAN1_GPIO_BANK       		(GPIOA)
		#define CAN1_RX_PIN	      		(GPIO_PIN_11)
		#define CAN1_TX_PIN	      		(GPIO_PIN_12)

	#endif
	#if defined(ENABLE_CAN2)
		/**
		 * CAN2 GPIO Configuration
		 * PB12     ------> CAN2_RX
		 * PB13     ------> CAN2_TX
		 */
		#define CAN2_GPIO_BANK       		(GPIOB)
		#define CAN2_RX_PIN	      		(GPIO_PIN_12)
		#define CAN2_TX_PIN	      		(GPIO_PIN_13)
	#endif
#endif

#if defined(ENABLE_FSMC_ADC7606)
		#define CONVST_GPIO       		F
		#define CONVST_PIN	      	7

		#define RESET_GPIO       		F
		#define RESET_PIN	      		8

		#define CS_GPIO       		G
		#define CS_PIN	      		12

		#define BUSY_GPIO       	F
		#define BUSY_PIN	      	11
#endif

#if defined(ENABLE_I2C)
	#if defined(ENABLE_I2C1)
    /**
     * CAN1 GPIO Configuration
     * PB6     ------> I2C1_SCL
     * PB7     ------> I2C1_SDA
     */
		#define I2C1_GPIO_BANK       		(GPIOA)
		#define I2C1_SCL_PIN	      		(GPIO_PIN_6)
		#define I2C1_SDA_PIN	      		(GPIO_PIN_7)
	#endif
#endif

#if defined(ENABLE_SDIO)
	/*
	 * SDIO GPIO Configuration
		 PC8     ------> SDIO_D0
		 PC9     ------> SDIO_D1
		 PC10    ------> SDIO_D2
		 PC11    ------> SDIO_D3
		 PC12    ------> SDIO_CK
		 PD2     ------> SDIO_CMD
		 */
		#define SDIO_GPIO_BANKC       	(GPIOC)
		#define SDIO_D0	      			(GPIO_PIN_8)
		#define SDIO_D1	      			(GPIO_PIN_9)
		#define SDIO_D2     					(GPIO_PIN_10)
		#define SDIO_D3     					(GPIO_PIN_11)
		#define SDIO_CK     					(GPIO_PIN_12)

		#define SDIO_GPIO_BANKD       	(GPIOD)
		#define SDIO_CMD	      			(GPIO_PIN_2)
#endif

#if defined(ENABLE_SPI)
	#if defined(ENABLE_SPI1)
	/**
	 * SPI1 GPIO Configuration
	 * PA5     ------> SPI1_SCK
	 * PA6     ------> SPI1_MISO
	 * PB5     ------> SPI1_MOSI
	 **/
		#define SPI1_GPIO_BANKA       	(GPIOA)
		#define SPI1_SCK	      			(GPIO_PIN_5)
		#define SPI1_MISO	      			(GPIO_PIN_6)

		#define SPI1_GPIO_BANKB       	(GPIOB)
		#define SPI1_MOSI     				(GPIO_PIN_5)
	#endif

	#if defined(ENABLE_SPI2)
	#endif
#endif

#if defined(ENABLE_TIM)
	/**
	 * TIM1 GPIO Configuration
	 * PA8     ------> TIM1_CH1
	 **/
		#define TIM1_CH1_BANK       		(GPIOA)
		#define TIM1_CH1_PIN	      		(GPIO_PIN_8)
		#define TIM1_CH1_PIN_PULL    		(GPIO_NOPULL)
#endif
#if defined(ENABLE_TIM)
	/**
	 * TIM1 GPIO Configuration
	 * PA8     ------> TIM1_CH1
	 **/
		#define PWM_CH1_BANK       		(GPIOA)
		#define PWM_CH1_PIN	      		(GPIO_PIN_8)
		#define PWM_CH1_PIN_PULL    		(GPIO_NOPULL)
		#define PWM_CH1_BANK_CLK			__HAL_RCC_GPIOA_CLK_ENABLE();
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
 * 2021年3月15日		  kernel       create this class
 *						
 */

/** @} */    //\defgroup module API
/** @} *///\addtogroup dev module
#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif  /**< end LH_TFDSN module api major */

#endif /* PROJECT_TFDSN_IO_TFDSN_IO_CONFIG_H_ */

