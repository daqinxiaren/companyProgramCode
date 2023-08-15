/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the io_config.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 13, 2021
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
 * Mar 13, 2021		  kernel       create this file
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
 
#ifndef PROJECT_IO_IO_CONFIG_H_
#define PROJECT_IO_IO_CONFIG_H_

/**
 ***************** include file ***************
 */
#include <rtconfig.h>


//#ifdef  XXX    /**< XXX module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */
#if  defined(STM32F4XX)

#if defined(ENABLE_USART)
	#if defined(ENABLE_UART1)
	/**
	 * USART1 GPIO Configuration
	 * PA9     ------> USART1_TX
	 * PA10     ------> USART1_RX
	 **/
		#define USART1_GPIO_BANKA			(GPIOA)
		#define USART1_TX_PIN	      	(GPIO_PIN_9)
		#define USART1_RX_PIN      		(GPIO_PIN_10)
	#endif
#endif


#if defined(ENABLE_SRAM) && defined(LH_TFDSN_CLB)
/**
 * FSMC GPIO Configuration
	 PE3   ------> FSMC_A19
	 PE4   ------> FSMC_A20
	 PF0   ------> FSMC_A0
	 PF1   ------> FSMC_A1
	 PF2   ------> FSMC_A2
	 PF3   ------> FSMC_A3
	 PF4   ------> FSMC_A4
	 PF5   ------> FSMC_A5
	 PF12   ------> FSMC_A6
	 PF13   ------> FSMC_A7
	 PF14   ------> FSMC_A8
	 PF15   ------> FSMC_A9
	 PG0   ------> FSMC_A10
	 PG1   ------> FSMC_A11
	 PE7   ------> FSMC_D4
	 PE8   ------> FSMC_D5
	 PE9   ------> FSMC_D6
	 PE10   ------> FSMC_D7
	 PE11   ------> FSMC_D8
	 PE12   ------> FSMC_D9
	 PE13   ------> FSMC_D10
	 PE14   ------> FSMC_D11
	 PE15   ------> FSMC_D12
	 PD8   ------> FSMC_D13
	 PD9   ------> FSMC_D14
	 PD10   ------> FSMC_D15
	 PD11   ------> FSMC_A16
	 PD12   ------> FSMC_A17
	 PD13   ------> FSMC_A18
	 PD14   ------> FSMC_D0
	 PD15   ------> FSMC_D1
	 PG2   ------> FSMC_A12
	 PG3   ------> FSMC_A13
	 PG4   ------> FSMC_A14
	 PG5   ------> FSMC_A15

	 |	PG6   ------> FSMC_INT2
	 | PG7   ------> FSMC_INT3

	 PD0   ------> FSMC_D2
	 PD1   ------> FSMC_D3
	 PD4   ------> FSMC_NOE
	 PD5   ------> FSMC_NWE

	 |	PD6   ------> FSMC_NWAIT
	 |	PD7   ------> FSMC_NE1
	 | PG9   ------> FSMC_NE2
	 PG10  ------> FSMC_NE3
	 PE0   ------> FSMC_NBL0
	 PE1   ------> FSMC_NBL1
	 */
		#define FSMC_GPIO_BANKF				(GPIOF)
		#define FSMC_A0_PIN	      		(GPIO_PIN_0)
		#define FSMC_A1_PIN      			(GPIO_PIN_1)
		#define FSMC_A2_PIN	      		(GPIO_PIN_2)
		#define FSMC_A3_PIN	      		(GPIO_PIN_3)
		#define FSMC_A4_PIN      			(GPIO_PIN_4)
		#define FSMC_A5_PIN	      		(GPIO_PIN_5)

		#define FSMC_A6_PIN      			(GPIO_PIN_12)
		#define FSMC_A7_PIN	      		(GPIO_PIN_13)
		#define FSMC_A8_PIN      			(GPIO_PIN_14)
		#define FSMC_A9_PIN	      		(GPIO_PIN_15)

		#define FSMC_GPIO_BANKG				(GPIOG)
		#define FSMC_A10_PIN      			(GPIO_PIN_0)
		#define FSMC_A11_PIN	      		(GPIO_PIN_1)

		//#define FSMC_GPIO_BANKG				(GPIOG)
		#define FSMC_A12_PIN      			(GPIO_PIN_2)
		#define FSMC_A13_PIN	      		(GPIO_PIN_3)
		#define FSMC_A14_PIN      			(GPIO_PIN_4)
		#define FSMC_A15_PIN	      		(GPIO_PIN_5)

		#define FSMC_GPIO_BANKD				(GPIOD)
		#define FSMC_A16_PIN      			(GPIO_PIN_11)
		#define FSMC_A17_PIN	      		(GPIO_PIN_12)
		#define FSMC_A18_PIN      			(GPIO_PIN_13)

		#define FSMC_GPIO_BANKE				(GPIOE)
		#define FSMC_A19_PIN	      		(GPIO_PIN_3)
		#define FSMC_A20_PIN      			(GPIO_PIN_4)

		//#define FSMC_GPIO_BANKD				(GPIOD)
		#define FSMC_D0_PIN	      		(GPIO_PIN_14)
		#define FSMC_D1_PIN      			(GPIO_PIN_15)
		#define FSMC_D2_PIN	      		(GPIO_PIN_0)
		#define FSMC_D3_PIN	      		(GPIO_PIN_1)

		//#define FSMC_GPIO_BANKE				(GPIOE)
		#define FSMC_D4_PIN      			(GPIO_PIN_7)
		#define FSMC_D5_PIN	      		(GPIO_PIN_8)
		#define FSMC_D6_PIN      			(GPIO_PIN_9)
		#define FSMC_D7_PIN	      		(GPIO_PIN_10)
		#define FSMC_D8_PIN      			(GPIO_PIN_11)
		#define FSMC_D9_PIN	      		(GPIO_PIN_12)
		#define FSMC_D10_PIN      			(GPIO_PIN_13)
		#define FSMC_D11_PIN	      		(GPIO_PIN_14)
		#define FSMC_D12_PIN      			(GPIO_PIN_15)

		//#define FSMC_GPIO_BANKD				(GPIOD)
		#define FSMC_D13_PIN	      		(GPIO_PIN_8)
		#define FSMC_D14_PIN      			(GPIO_PIN_9)
		#define FSMC_D15_PIN	      		(GPIO_PIN_10)

		//#define FSMC_GPIO_BANKG				(GPIOG)
		#define FSMC_INT2_PIN	      	(GPIO_PIN_6)
		#define FSMC_INT3_PIN	      	(GPIO_PIN_7)

		//#define FSMC_GPIO_BANKD				(GPIOD)
		#define FSMC_NOE_PIN	      		(GPIO_PIN_4)
		#define FSMC_NWE_PIN	      		(GPIO_PIN_5)
		#define FSMC_NWAIT_PIN	      	(GPIO_PIN_6)
		#define FSMC_NE1_PIN	      		(GPIO_PIN_7)

		//#define FSMC_GPIO_BANKG				(GPIOG)
		#define FSMC_NE3_PIN	      		(GPIO_PIN_10)

		//#define FSMC_GPIO_BANKE				(GPIOE)
		#define FSMC_NBL0_PIN	      	(GPIO_PIN_0)
		#define FSMC_NBL1_PIN	      	(GPIO_PIN_1)
#endif




#if defined(ENABLE_FSMC_ADC7606) && defined(LH_TFDSN_CLB)
    /**
     * AD7606 GPIO Configuration
     * PB0     ------> AD7606_BUSY1
     * PA3     ------> AD7606_RESET
     * PB1     ------> AD7606_CONVST
     * ...     ------> AD_RANGE       	//default hight
     * ...     ------> AD_OS0				//
     * ...     ------> AD_OS1				//
     * ...     ------> AD_OS2				//
     * ...     ------> AD_OS2				//
     * PG12    ------> AD7606_CS			//
    */
	#define AD7606_BUSY						GPIO_PIN_11
	#define AD7606_RESET						GPIO_PIN_8
	#define AD7606_CONVST					GPIO_PIN_7
	#define AD7606_CS							GPIO_PIN_12

	#define AD7606_BUSY_IO_PORT			GPIOF
	#define AD7606_RESET_IO_PORT			GPIOF
	#define AD7606_CONVST_IO_PORT			GPIOF
	#define AD7606_CS_IO_PORT				GPIOG

	#define AD7606_CS_IO_ALT				GPIO_AF12_FSMC
#endif


#if defined(ENABLE_ETH)
#if defined(LH_TFDSN_CLB)
	/**
	* ETH GPIO Configuration
	* PC1     ------> ETH_MDC
	* PA1     ------> ETH_REF_CLK
	* PA2     ------> ETH_MDIO
	* PA7     ------> ETH_CRS_DV
	* PC4     ------> ETH_RXD0
	* PC5     ------> ETH_RXD1
	* PG11    ------> ETH_TX_EN
	* PG13    ------> ETH_TXD0
	* PG14    ------> ETH_TXD1
	* PA0      ------> ETH_RESET
	* */
		#define ETH_GPIO_BANKA       		(GPIOA)
		#define ETH_REF_CLK	      		(GPIO_PIN_1)
		#define ETH_MDIO	      			(GPIO_PIN_2)
		#define ETH_CRS_DV     				(GPIO_PIN_7)

		#define ETH_GPIO_BANKC       		(GPIOC)
		#define ETH_MDC	      			(GPIO_PIN_1)
		#define ETH_RXD0	      			(GPIO_PIN_4)
		#define ETH_RXD1     				(GPIO_PIN_5)

		#define ETH_GPIO_BANKG       		(GPIOG)
		#define ETH_TX_EN	      			(GPIO_PIN_11)
		#define ETH_TXD0	      			(GPIO_PIN_13)
		#define ETH_TXD1     				(GPIO_PIN_14)
#elif defined(LH_TFDSN_TCLB)

#elif defined(LH_TFDSN_TXB)

#elif defined(LH_TFDSN_ZXB)
	/**
	* ETH GPIO Configuration
    PC1     ------> ETH_MDC
    PA1     ------> ETH_REF_CLK
    PA2     ------> ETH_MDIO
    PA7     ------> ETH_CRS_DV
    PC4     ------> ETH_RXD0
    PC5     ------> ETH_RXD1
    PB11     ------> ETH_TX_EN
    PB12     ------> ETH_TXD0
    PB13     ------> ETH_TXD1
	* PA0      ------> ETH_RESET
	* */
		#define ETH_GPIO_BANKA       		(GPIOA)
		#define ETH_REF_CLK	      		(GPIO_PIN_1)
		#define ETH_MDIO	      			(GPIO_PIN_2)
		#define ETH_CRS_DV     				(GPIO_PIN_7)

		#define ETH_GPIO_BANKC       		(GPIOC)
		#define ETH_MDC	      			(GPIO_PIN_1)
		#define ETH_RXD0	      			(GPIO_PIN_4)
		#define ETH_RXD1     				(GPIO_PIN_5)

		#define ETH_GPIO_BANKB       		(GPIOB)
		#define ETH_TX_EN	      			(GPIO_PIN_11)
		#define ETH_TXD0	      			(GPIO_PIN_12)
		#define ETH_TXD1     				(GPIO_PIN_13)

#endif

#endif//defined(ENABLE_ETH)&

#endif //#if  defined(STM32F4XX)
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
 * Mar 13, 2021		  kernel       create this class
 *						
 */




/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

//#endif  /**< end XXX module api major */

#endif /* PROJECT_IO_IO_CONFIG_H_ */

