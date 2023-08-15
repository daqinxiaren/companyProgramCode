/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the ad7606_spi.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2021年2月22日
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
 * 2021年2月22日		  kernel       create this file
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

#ifndef MODULE_AD7606_AD7606_SPI_H_
#define MODULE_AD7606_AD7606_SPI_H_

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
/**
 * @addtogroup group name 
 */

/**@{*/

/**@}*/

/** @formatter:off  */
/**
 ****************<Public macro>****************
 * @description		: Macros and Constants Definitions
 * @note					: null
 * @note					: null
 */

/*AD7606FSMC-NE4*/
#define AD_RESULT()									(*(__IO uint16_t *)0x6C000000)
#define AD7606_MAX_CHANNELS						8

/**
 ***************<Public typedef>***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 **************<Public variables>**************
 * @description		: null 
 * @note			: null 
 * @note			: null  
 */

/**
 * @enum ad7606_device_id
 * @brief Device ID definitions
 */
enum ad7606_device_id
{
	/** 4-Channel DAS with 16-Bit, Bipolar Input, Simultaneous Sampling ADC */
	ID_AD7605_4,
	/** 4-Channel DAS with 16-Bit, Bipolar Input, Simultaneous Sampling ADC */
	ID_AD7606_4,
	/** 6-Channel DAS with 16-Bit, Bipolar Input, Simultaneous Sampling ADC */
	ID_AD7606_6,
	/** 8-Channel DAS with 16-Bit, Bipolar Input, Simultaneous Sampling ADC */
	ID_AD7606_8,
	/** 8-Channel DAS with 16-Bit, 800 kSPS, Bipolar Input, Simultaneous Sampling ADC */
	ID_AD7606B,
	/** 8-Channel DAS with 16-Bit, 1 MSPS, Bipolar Input, Simultaneous Sampling ADC */
	ID_AD7606C_16,
	/** 8-Channel DAS with 18-Bit, 1 MSPS, Bipolar Input, Simultaneous Sampling ADC */
	ID_AD7606C_18,
	/** 8-Channel DAS with 18-Bit, Bipolar, Simultaneous Sampling ADC */
	ID_AD7608,
	/** 8-Channel Differential DAS with 18-Bit, Bipolar, Simultaneous Sampling ADC */
	ID_AD7609,
};

/**
 * @enum ad7606_osr
 * @brief Oversampling ratio
 */
enum ad7606_osr
{
	/** Oversample by 1 */
	AD7606_OSR_1,
	/** Oversample by 2 */
	AD7606_OSR_2,
	/** Oversample by 4 */
	AD7606_OSR_4,
	/** Oversample by 8 */
	AD7606_OSR_8,
	/** Oversample by 16 */
	AD7606_OSR_16,
	/** Oversample by 32 */
	AD7606_OSR_32,
	/** Oversample by 64 */
	AD7606_OSR_64,
	/** Oversample by 128, available for chips that have software mode only */
	AD7606_OSR_128,
	/** Oversample by 256, available for chips that have software mode only */
	AD7606_OSR_256
};

/**
 * @struct ad7606_dev
 * @brief Device driver structure
 */
struct ad7606_dev
{
	/** RESET GPIO descriptor */
	uint32_t gpio_reset;
	/** CONVST GPIO descriptor */
	uint32_t gpio_convst;
	/** BUSY GPIO descriptor */
	uint32_t gpio_busy;
	/** STBYn GPIO descriptor */
	uint32_t gpio_stby_n;
	/** RANGE GPIO descriptor */
	uint32_t gpio_range;
	/** OS0 GPIO descriptor */
	uint32_t gpio_os0;
	/** OS1 GPIO descriptor */
	uint32_t gpio_os1;
	/** OS2 GPIO descriptor */
	uint32_t gpio_os2;
	/** PARn/SER GPIO descriptor */
	uint32_t gpio_par_ser;
	/** Device ID */
	enum ad7606_device_id device_id;
	/** Number of input channels of the device */
	uint8_t num_channels;
};

/**
 * @struct ad7606_dev
 * @brief Device driver initialization parameters
 */
struct ad7606_init_param
{
	/** RESET GPIO descriptor */
	uint32_t gpio_reset;
	/** CONVST GPIO descriptor */
	uint32_t gpio_convst;
	/** BUSY GPIO descriptor */
	uint32_t gpio_busy;
	/** STBYn GPIO descriptor */
	uint32_t gpio_stby_n;
	/** RANGE GPIO descriptor */
	uint32_t gpio_range;
	/** OS0 GPIO descriptor */
	uint32_t gpio_os0;
	/** OS1 GPIO descriptor */
	uint32_t gpio_os1;
	/** OS2 GPIO descriptor */
	uint32_t gpio_os2;
	/** PARn/SER GPIO descriptor */
	uint32_t gpio_par_ser;
	/** Device ID */
	enum ad7606_device_id device_id;
};

/**
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/** @formatter:on */

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
 **************<Public functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 * @brief development description of class
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
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
 * 2021年2月22日		  kernel       create this class
 *						
 */
int32_t ad7606_spi_reg_read(struct ad7606_dev *dev, uint8_t reg_addr, uint8_t *reg_data);

int32_t ad7606_spi_reg_write(struct ad7606_dev *dev, uint8_t reg_addr, uint8_t reg_data);

int32_t ad7606_spi_write_mask(struct ad7606_dev *dev, uint32_t addr, uint32_t mask, uint32_t val);

int32_t ad7606_spi_data_read(struct ad7606_dev *dev, uint32_t *data);

int32_t ad7606_read(struct ad7606_dev *dev, uint16_t *data);

int32_t ad7606_convst(struct ad7606_dev *dev,uint32_t value);

int32_t ad7606_reset(struct ad7606_dev *dev);

int32_t ad7606_set_ch_offset(struct ad7606_dev *dev, uint8_t ch, int8_t offset);

int32_t ad7606_set_ch_phase(struct ad7606_dev *dev, uint8_t ch, uint8_t phase);

int32_t ad7606_set_ch_gain(struct ad7606_dev *dev, uint8_t ch, uint8_t gain);

int32_t ad7606_init(struct ad7606_dev **device, struct ad7606_init_param *init_param);

int32_t ad7606_remove(struct ad7606_dev *dev);

/** @} */ //\defgroup module API
/** @} *///\addtogroup dev module
#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* MODULE_AD7606_AD7606_SPI_H_ */

