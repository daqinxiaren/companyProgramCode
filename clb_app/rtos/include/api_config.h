/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the api_config.h API or library.
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
 
#ifndef API_CONFIG_H_
#define API_CONFIG_H_

/**
 ***************** include file ***************
 */
#include <system_config.h>
#include <bsp_config.h>


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

/* !< enable bsp onchip adc module 				*/
#if defined(ENABLE_ADC)
	//#define ENABLE_ADC1
	//#define ENABLE_ADC2
	//#define ENABLE_ADC3
#endif

/* !< enable bsp onchip hw cryto module
 * 	hwcrypto.c
 * */
#if defined(ENABLE_HWCRYPTO)
	/*	api cryto api tools	*/
	#define HARDWARE_CRYPTO_DEFAULT_NAME 			"hwcryto"
	#define HARDWARE_CRYPTO_IV_MAX_SIZE 			16
	#define HARDWARE_CRYPTO_KEYBIT_MAX_SIZE 		256
	//#define HARDWARE_CRYPTO_ENABLE_RNG

	/*drv_crypto.c*/
	#define ENABLE_RNG
	#define ENABLE_UDID

#if 0
	/* api in hw_symmetric.c */
	#define HARDWARE_CRYPTO_ENABLE_AES
	#define HARDWARE_CRYPTO_ENABLE_DES
	#define HARDWARE_CRYPTO_ENABLE_3DES
	#define HARDWARE_CRYPTO_ENABLE_RC4
	/* hw_gcm.c */
	#define HARDWARE_CRYPTO_ENABLE_GCM
	/* hw_hash.c*/
	#define HARDWARE_CRYPTO_ENABLE_MD5
	#define HARDWARE_CRYPTO_ENABLE_SHA1
	#define HARDWARE_CRYPTO_ENABLE_SHA2
	/* hw_rng.c*/
	#define HARDWARE_CRYPTO_ENABLE_RNG
	/* hw_crc.c*/
	#define HARDWARE_CRYPTO_ENABLE_CRC
	/* hw_bignum.c*/
	#define HARDWARE_CRYPTO_ENABLE_BIGNUM
#endif

#endif

/* !< enable bsp onchip can module 				*/
#if defined(ENABLE_CAN)
	//#define DEV_CAN_ENABLE_HDR
	//#define DEV_CAN_ENABLE_BUS_HOOK
	#define ENABLE_CAN1
	#if defined(ENABLE_CAN1)
		#define ENABLE_CAN1_IRQ
	#endif

	#define ENABLE_CAN2
	#if defined(ENABLE_CAN2)
		#define ENABLE_CAN2_IRQ
	#endif
#endif

/* !< enable bsp onchip	can legacy module 	*/
#if defined(ENABLED_CAN_LEGACY)

#endif

/* !< enable crc module 		*/
#if defined(ENABLE_CRC)

#endif

/* !< enable bsp onchip	dac module 				*/
#if defined(ENABLE_DAC)
	#define ENABLE_DAC
#endif

/* !< enable dcmi module 		*/
#if defined(ENABLE_DCMI)
	#define ENABLE_DCMI
#endif

/* !< enable dma2d module 		*/
#if defined(ENABLE_DMA2D)
	#define ENABLE_DMA2D
#endif

/* !< enable ETH module 		*/
#if defined(ENABLE_ETH)
	#define ENABLE_PHY
	//#define ENABLE_ETH_MII_MODE
	#define ENABLE_ETH_RMII_MODE
	//#define ENABLE_ETH_IRQ
	#if defined(ENABLE_ETH_MII_MODE)
		#define ETH_MII_HALF_MODE
		#define ETH_MII_FULL_MODE
	#endif //ENABLE_ETH_MII_MODE

	#if defined(ENABLE_ETH_RMII_MODE)
		#define PHY_ENABLE_LAN8720A
		//#define PHY_ENABLE_DM9161CEP
		//#define PHY_ENABLE_DP83848C
		//#define ENABLE_MPU6050
	#endif //ENABLE_ETH_RMII_MODE
#endif

/* !< enable NAND module 		*/
#if defined(ENABLE_NAND)
	#define ENABLE_MTD_NAND
	#define MTD_ENABLE_NAND
	#define MTD_NAND_DEBUG
#endif

/* !< enable NOR module 		*/
#if defined(ENABLE_NOR)
	//#define ENABLE_MTD_NOR
	//#define MTD_ENABLE_NOR
#endif

/* !< enable PCCARD module 	*/
#if defined(ENABLE_PCCARD)
	//#define ENABLE_PCCARD
#endif

/* !< enable SRAM module 		*/
#if defined(ENABLE_SRAM)
	#define ENABLE_EXTERNAL_SRAM
   #define ENABLE_EXTERNAL_ADC

	#if defined(ENABLE_EXTERNAL_SRAM)
		//#define IS61WV102416BLL
		#define IS61WV204816BLL
	#else
		#error "the two cannot coexist !"
	#endif

   #if defined(ENABLE_EXTERNAL_ADC)
		#ifndef ENABLE_FSMC_ADC7606
			#define ENABLE_FSMC_ADC7606
		#endif
   #endif
#endif

/* !< enable SDRAM module 		*/
#if defined(ENABLE_SDRAM)
	#define ENABLE_EXTERNAL_SDRAM
   #define ENABLE_EXTERNAL_ADC
   #if defined(ENABLE_EXTERNAL_ADC)
		#ifndef ENABLE_FSMC_ADC7606
			#define ENABLE_FSMC_ADC7606
		#endif
   #endif
#endif

/* !< enable HASH module 		*/
#if defined(ENABLE_HASH)
	//#define ENABLE_HASH
#endif

/* !< enable I2C module 		*/
#if defined(ENABLE_I2C)
	//#define ENABLE_EEPROM
	//#define ENABLE_I2C_IRQ
	#define ENABLE_I2C_BITOPS
	#define DEV_I2C_DEBUG
	#define DEV_I2C_BITOPS_DEBUG

	#define ENABLE_I2C1
	#if defined(ENABLE_I2C1)
		#define ENABLE_I2C1_TX_DMA
		#define ENABLE_I2C1_RX_DMA
		#define ENABLE_I2C1_IRQ
		#define BSP_I2C1_SCL_PIN 24
		#define BSP_I2C1_SDA_PIN 25
	#endif
#endif

/* !< enable I2S module 		*/
#if defined(ENABLE_I2S)
	#define ENABLE_I2S
#endif

/* !< enable IWDG module 		*/
#if defined(ENABLE_IWDG)
	#define ENABLE_WDT
#endif

/* !< enable LTDC module 		*/
#if defined(ENABLE_LTDC)
	//#define ENABLE_XXXX
#endif

/* !< enable RNG module 		*/
#if defined(ENABLE_RNG)
	#define ENABLE_RNG
#endif

/* !< enable RTC module 		*/
#if defined(ENABLE_RTC)
	//#define ENABLE_ALARM

	#define ENABLE_ONCHIP_RTC
	#ifdef ENABLE_ONCHIP_RTC
		//#define BSP_RTC_ENABLE_LSE
//	   #define BSP_RTC_ENABLE_LSI
	#endif
//
//	#ifdef BSP_RTC_ENABLE_LSI
//		#define ENABLE_SOFT_RTC
//	#else
//		#define ENABLE_HW_RTC
//	#endif

	#define ENABLE_CPUTIME			/*<! enable libc time function */
	#ifdef ENABLE_CPUTIME
		#define ENABLE_CPUTIME_CORTEXM
	#endif
#endif

/* !< enable SAI module 		*/
#if defined(ENABLE_SAI)
	#define ENABLE_SAI
#endif

/* !< enable SD module 		*/
#if defined(ENABLE_SD)
	#define ENABLE_SDIO
	#if defined(ENABLE_SDIO)
		/* peripheral libraries and drivers */
		#define SDIO_MAX_FREQ 				1000000
		#define SDIO_STACK_SIZE 			512
		#define SDIO_THREAD_PRIORITY 		15
		#define SDIO_4BIT_MODE
		#define SDIO_DEBUG
	#endif
	#ifndef ENABLE_FS
			#define ENABLE_FS					/*!< enable rtos filesystem    		*/
	#endif
#endif

/* !< enable MMC module 		*/
#if defined(ENABLE_MMC)
	#define MMCSD_STACK_SIZE 				1024
	#define MMCSD_THREAD_PREORITY 		22
	#define MMCSD_MAX_PARTITION 			16
	#ifndef ENABLE_FS
			#define ENABLE_FS					/*!< enable rtos filesystem    		*/
	#endif
#endif

/* !< enable SPI module 		*/
#if defined(ENABLE_SPI)
	#define ENABLE_SPI_FLASH
	//#define ENABLE_SPI_MSD
	//#define ENABLE_SPI_WIFI
	//#define ENABLE_SPI_PHY
	//#define ENABLE_SPI_AD7606
	//#define ENABLE_TOUCH

	#if defined(ENABLE_SPI) && (defined(STM32F1XX) || defined(STM32F4XX))
		#ifdef LH_TFDSN_CLB
			#define ENABLE_SPI1
		#endif
		 // #define ENABLE_SPI2
		#ifdef LH_TFDSN_ZXB
			#define ENABLE_SPI3
		#endif
			//#define ENABLE_SPI4
			//#define ENABLE_SPI5
		#endif

		#if defined(ENABLE_SPI1)
			//#define SPI1_ENABLE_IRQ
			//#define BSP_SPI1_RX_ENABLE_DMA
			//#define BSP_SPI1_TX_ENABLE_DMA
		#endif

		#if defined(ENABLE_SPI2)
			//#define SPI2_ENABLE_IRQ
			#define BSP_SPI2_RX_ENABLE_DMA
			#define BSP_SPI2_TX_ENABLE_DMA
		#endif

		#if defined(ENABLE_SPI3)
			//#define SPI3_ENABLE_IRQ
			#define BSP_SPI3_RX_ENABLE_DMA
			#define BSP_SPI3_TX_ENABLE_DMA
		#endif

		#if defined(ENABLE_SPI_FLASH)
			#ifndef ENABLE_FS
					#define ENABLE_FS								/*!< enable rtos filesystem    		*/
			#endif
			#define ENABLE_SFUD
			//#define SFUD_ENABLE_QSPI
			#if defined(ENABLE_SFUD)
				#define SFUD_ENABLE_SFDP
			  //#define ENABLE_DEBUG_SFUD
				//#define SFUD_DEBUG_MODE
				#define SFUD_ENABLE_FLASH_INFO_TABLE
				#define SFUD_SPI_MAX_HZ 						50000000
			#endif
		#endif

		#if defined(ENABLE_SPI_WIFI)
			#define ENABLE_WIFI
			#define ENABLE_SPI_WIFI_RW009
		#endif

		#if defined(ENABLE_SPI_PHY)
			#define ENABLE_PHY
			#define ENABLE_SPI_PHY_EN28J60
		#endif

#endif

/* !< enable TIM module
 * 		--HWTIMER 			定时器
 * 		--PULSE_ENCODER  	脉冲编码器,编码器模式
 * 		--PWM					脉冲宽度调制
 * 		--INPUT_CAPTURE   计数器模式 输入捕获模式
 * */
#if defined(ENABLE_TIM)
	//#define ENABLE_PULSE_ENCODER

	#if defined(LH_TFDSN_ZXB) || defined(LH_TFDSN_CLB)
		#define ENABLE_TIM1
	#endif

	#if defined(LH_TFDSN_ZXB) || defined(LH_TFDSN_CLB)
		#define ENABLE_PWM
	#endif

	#if defined(LH_TFDSN_ZXB) || defined(LH_TFDSN_CLB)
		#define ENABLE_HWTIMER
	#endif

	#if defined(LH_TFDSN_ZXB)
		#define ENABLE_INPUT_CAPTURE
	#endif

#endif

#ifdef ENABLE_PWM
	#ifdef LH_TFDSN_CLB
		#define ENABLE_PWM1
	#endif
#endif


#ifdef ENABLE_HWTIMER
	#ifdef LH_TFDSN_CLB
		#define ENABLE_TIM2
	#endif
#endif

#ifdef ENABLE_INPUT_CAPTURE
	#define INPUT_CAPTURE_RB_SIZE 100
	#ifdef LH_TFDSN_ZXB
		#define ENABLE_TIM1_IRQ
	#endif
#endif


/* !< enable PULSE_ENCODER module 		*/
#if defined(ENABLE_PULSE_ENCODER)
	#define ENABLE_PULSE_ENCODER1
	#define ENABLE_PULSE_ENCODER2
	#define ENABLE_PULSE_ENCODER3
	#define ENABLE_PULSE_ENCODER4
	#define ENABLE_PULSE_ENCODER5
	#define ENABLE_PULSE_ENCODER6
#endif


/* !< enable PWM module 		*/
#if defined(ENABLE_PWM)
	#if defined(STM32L4XX)
		#define ENABLE_PWM1
		#if defined(ENABLE_PWM1)
			#define ENABLE_PWM1_CH1
			#define ENABLE_PWM1_CH2
			#define ENABLE_PWM1_CH3
			#define ENABLE_PWM1_CH4
		#endif
	#endif

	#if defined(STM32F0XX) || defined(STM32F1XX) || defined(STM32F2XX) || defined(STM32F4XX) || defined(STM32F7XX)
		#if defined(ENABLE_PWM1)
			#ifdef LH_TFDSN_CLB
				#define ENABLE_PWM1_CH1
			#endif

//			#define ENABLE_PWM1_CH2
//			#define ENABLE_PWM1_CH3
//			#define ENABLE_PWM1_CH4
		#endif

		//#define ENABLE_PWM2
		#if defined(ENABLE_PWM2)
			#define ENABLE_PWM2_CH1
			#define ENABLE_PWM2_CH2
			#define ENABLE_PWM2_CH3
			#define ENABLE_PWM2_CH4
		#endif

		//#define ENABLE_PWM3
		#if defined(ENABLE_PWM3)
			#define ENABLE_PWM3_CH1
			#define ENABLE_PWM3_CH2
			#define ENABLE_PWM3_CH3
			#define ENABLE_PWM3_CH4
		#endif

		//#define ENABLE_PWM4
		#if defined(ENABLE_PWM4)
			#define ENABLE_PWM4_CH1
			#define ENABLE_PWM4_CH2
			#define ENABLE_PWM4_CH3
			#define ENABLE_PWM4_CH4
		#endif

		//#define ENABLE_PWM5
		#if defined(ENABLE_PWM5)
			#define ENABLE_PWM5_CH1
			#define ENABLE_PWM5_CH2
			#define ENABLE_PWM5_CH3
			#define ENABLE_PWM5_CH4
		#endif
	#endif

	#if 0
		#define ENABLE_PWM6
		#define ENABLE_PWM7
		#define ENABLE_PWM8
		#define ENABLE_PWM9
		#define ENABLE_PWM10
		#define ENABLE_PWM11
	#endif

	#if defined(STM32F4XX)
		//#define ENABLE_PWM12
		#if defined(ENABLE_PWM12)
			#define ENABLE_PWM12_CH1
			#define ENABLE_PWM12_CH2
		#endif
	#endif

	#if 0
		#define ENABLE_PWM13
		#define ENABLE_PWM14
		#define ENABLE_PWM15
		#define ENABLE_PWM16
		#define ENABLE_PWM17
	#endif
#endif

/* !< enable UART module 		*/
#if defined(ENABLE_UART) || defined(ENABLE_USART)
	#define ENABLE_SERIAL
	#define SERIAL_ENABLE_DMA
	#define SERIAL_RB_BUFSZ 							64

	//#define ENABLE_COM2
	//#define ENABLE_COM3

	#define ENABLE_USART
	#if defined(ENABLE_USART)
		#define ENABLE_UART1
		//#define ENABLE_UART2
		//#define ENABLE_UART3
		//#define ENABLE_UART4
		//#define ENABLE_UART5
		//#define ENABLE_UART6
		//#define ENABLE_UART7
		//#define ENABLE_UART8

		#if defined(ENABLE_UART1) && (defined(STM32F1XX) || defined(STM32F407xx))
			//#define ENABLE_UART1_IRQ
			//#define BSP_UART1_RX_ENABLE_DMA
			//#define BSP_UART1_TX_ENABLE_DMA
		#endif

		#if defined(ENABLE_UART2) && (defined(STM32F1XX) || defined(STM32F407xx))
			#define ENABLE_UART2_IRQ
			#define BSP_UART2_RX_ENABLE_DMA
			#define BSP_UART2_TX_ENABLE_DMA
		#endif

		#if defined(ENABLE_UART3) && (defined(STM32F1XX) || defined(STM32F407xx))
			#define ENABLE_UART3_IRQ
			#define BSP_UART3_RX_ENABLE_DMA
			#define BSP_UART3_TX_ENABLE_DMA
		#endif

		#if defined(ENABLE_UART4) && (defined(STM32F1XX) || defined(STM32F407xx))
			#define ENABLE_UART4_IRQ
			#define BSP_UART4_RX_ENABLE_DMA
			#define BSP_UART4_TX_ENABLE_DMA
		#endif

		#if defined(ENABLE_UART5) && (defined(STM32F1XX) || defined(STM32F407xx))
			#define ENABLE_UART5_IRQ
			#define BSP_UART5_RX_ENABLE_DMA
			#define BSP_UART5_TX_ENABLE_DMA
		#endif

		#if defined(ENABLE_UART6) && (defined(STM32F1XX) || defined(STM32F407xx))
			#define ENABLE_UART6_IRQ
			#define BSP_UART6_RX_ENABLE_DMA
			#define BSP_UART6_TX_ENABLE_DMA
		#endif
	#endif
#endif

/* !< enable IRDA module 		*/
#if defined(ENABLE_IRDA)
	#define ENABLE_IRDA
#endif

/* !< enable SMARTCARD module 		*/
#if defined(ENABLE_SMARTCARD)
	#define ENABLE_SMARTCARD
#endif

/* !< enable SMBUS module 		*/
#if defined(ENABLE_SMBUS)
	#define ENABLE_SMBUS
#endif

/* !< enable WWDG module 		*/
#if defined(ENABLE_WWDG)
	#define ENABLE_WWDG
#endif

/* !< enable USBD module 		*/
#if defined(ENABLE_USBD)
	#define BSP_USBD_SPEED_HS
	#define ENABLE_USB_DEVICE
	#if defined(ENABLE_USB_DEVICE)
		#define DEV_USBH_MSTORAGE
		#define UDISK_MOUNTPOINT "/"
		#define ENABLE_USB_DEVICE
		#define DEV_USBD_THREAD_STACK_SZ 4096
		#define USB_VENDOR_ID 0x0FFE
		#define USB_PRODUCT_ID 0x0001
		#define DEV_USB_DEVICE_COMPOSITE
		#define DEV_USB_DEVICE_CDC
		#define DEV_USB_DEVICE_NONE
		#define DEV_USB_DEVICE_MSTORAGE
		#define DEV_USB_DEVICE_HID
		#define DEV_USB_DEVICE_RNDIS
		#define DEV_USB_DEVICE_ECM
		#define DEV_USB_DEVICE_WINUSB
		#define DEV_USB_DEVICE_AUDIO
		#define DEV_VCOM_TASK_STK_SIZE 512
		#define DEV_VCOM_TX_USE_DMA
		#define DEV_VCOM_SERNO "32021919830108"
		#define DEV_VCOM_SER_LEN 14
		#define DEV_VCOM_TX_TIMEOUT 1000
		#define DEV_WINUSB_GUID "{6860DC3C-C05F-4807-8807-1CA861CC1D66}"
		#define DEV_USB_MSTORAGE_DISK_NAME "flash0"
		#define RNDIS_DELAY_LINK_UP
		#define DEV_USB_DEVICE_HID_KEYBOARD
		#define DEV_USB_DEVICE_HID_KEYBOARD_NUMBER 1
		#define DEV_USB_DEVICE_HID_MOUSE
		#define DEV_USB_DEVICE_HID_GENERAL
		#define DEV_USB_DEVICE_HID_GENERAL_OUT_REPORT_LENGTH 63
		#define DEV_USB_DEVICE_HID_GENERAL_IN_REPORT_LENGTH 63
		#define DEV_USB_DEVICE_HID_MEDIA
		#define DEV_USB_DEVICE_AUDIO_MIC
		#define DEV_USBD_MIC_DEVICE_NAME "mic0"
		#define DEV_USB_DEVICE_AUDIO_SPEAKER
		#define DEV_USBD_SPEAKER_DEVICE_NAME "sound0"
	#endif
#endif

/* !< enable USBH module 		*/
#if defined(ENABLE_USBH)
	#define ENABLE_USBH_MSTORAGE
	#define ENABLE_DEBUG_USB
	#if defined(ENABLE_USBH_MSTORAGE)
		//#define USBH_MSTORAGE
		//#define USB_MSTORAGE_DISK_NAME 								"flash0"
		//#define UDISK_MOUNTPOINT 										"/"
	#endif

	//#define ENABLE_USBH_VCOM
	#if defined(ENABLE_USBH_VCOM)
		//#define VCOM_TASK_STK_SIZE 										512
		//#define VCOM_TX_USE_DMA
		//#define VCOM_SERNO 												"32021919830108"
		//#define VCOM_SER_LEN 												14
		//#define VCOM_TX_TIMEOUT 											1000
		//#define WINUSB_GUID 												"{6860DC3C-C05F-4807-8807-1CA861CC1D66}"
	#endif

	//#define USB_DEVICE_HID_KEYBOARD
	#if defined(USB_DEVICE_HID_KEYBOARD)
		//#define USB_DEVICE_HID_KEYBOARD_NUMBER 						1
	#endif

	//#define USB_DEVICE_HID_MOUSE
	//#define USB_DEVICE_HID_GENERAL
	#if defined(USB_DEVICE_HID_GENERAL)
		//#define USB_DEVICE_HID_GENERAL_OUT_REPORT_LENGTH 		63
		//#define USB_DEVICE_HID_GENERAL_IN_REPORT_LENGTH 			63
	#endif
	//#define USB_DEVICE_HID_MEDIA
	#if defined(USB_DEVICE_HID_MEDIA)
		//#define USB_DEVICE_AUDIO_MIC
		//#define USBD_MIC_DEVICE_NAME 									"mic0"
		//#define USB_DEVICE_AUDIO_SPEAKER
		//#define USBD_SPEAKER_DEVICE_NAME 								"sound0"
	#endif

		//#define ENABLE_VBUS
		//#define RNDIS_DELAY_LINK_UP
		//#define ENABLE_USB_TO_USART
#endif

#ifdef ENABLE_VBUS
	#define ENABLE_VBUS_RFS
	#define ENABLE_VBUS_RSHELL
	#define DEV_VBUS_ENABLE_TESTS
	#define _DEV_VBUS_RING_BASE
	#define _DEV_VBUS_RING_SIZE
	#define DEV_VBUS_GUEST_VIRQ
	#define DEV_VBUS_HOST_VIRQ
	#define DEV_VBUS_SHELL_DEV_NAME "vbser0"
	#define DEV_VBUS_RFS_DEV_NAME "rfs"
#endif

/* !< enable DSI module 		*/
#if defined(ENABLE_DSI) || defined(ENABLE_LTDC)
	#define ENABLE_MIPI
	#define ENABLE_LCD
#endif

/* !< enable QSPI module 		*/
#if defined(ENABLE_QSPI)
	#define BSP_QSPI_ENABLE_DMA
#endif

/* !< enable CEC module 		*/
#if defined(ENABLE_CEC)
	#define ENABLE_CEC
#endif

/* !< enable FMPI2C module 		*/
#if defined(ENABLE_FMPI2C)
	//#define ENABLE_FMPI2C
#endif

/* !< enable SPDIFRX module 		*/
#if defined(ENABLE_SPDIFRX)
	#define ENABLE_AUDIO
	#define AUDIO_REPLAY_MP_BLOCK_SIZE 								4096
	#define AUDIO_REPLAY_MP_BLOCK_COUNT 							2
	#define AUDIO_RECORD_PIPE_SIZE 									2048
#endif

/* !< enable DFSDM module 		*/
#if defined(ENABLE_DFSDM)
	//#define ENABLE_DFSDM
#endif

/* !< enable LPTIM module 		*/
#if defined(ENABLE_LPTIM)
	#ifdef STM32L4XX
		//#define ENABLE_LPTIM
	#endif
#endif

/* !< enable GPIO module 		*/
#if defined(ENABLE_PIN)
	#define ENABLE_GPIO
	//#define ENABLE_SENSOR
	//#define ENABLE_SENSOR_CMD
#endif

#if defined(HAL_EXTI_MODULE_ENABLED)

#endif

/* !< enable DMA module 		*/
#if defined(ENABLE_DMA)
	//#define ENABLE_XXXX
#endif

/* !< enable RCC module 		*/
#if defined(ENABLE_RCC)
	//#define ENABLE_XXXX
#endif

/* !< enable FLASH module 		*/
#if defined(ENABLE_FLASH)
	#define ENABLE_ON_CHIP_FLASH
#endif

/* !< enable PWR module 		*/
#if defined(ENABLE_PWR)
	#ifdef STM32L4XX
		//#define ENABLE_PM
	#endif
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

#endif /* API_CONFIG_H_ */

