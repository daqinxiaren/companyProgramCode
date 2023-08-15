/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the ad7606_spi.c API or library.
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

/**
 ***************** include file ***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
#include <ad7606_fsmc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <drivers/pin.h>
#include "error.h"
#include "util.h"
#include "crc.h"
//#if XXXX

/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/
/**@}*/

/** @formatter:off */
/**
 ****************<Private macro>****************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */
struct ad7606_chip_info
{
	uint8_t num_channels;
	uint8_t bits;
	uint8_t device_id;
};

static const struct ad7606_chip_info ad7606_chip_info_tbl[] =
{
	[ID_AD7605_4] =
	{
		.num_channels = 4,
		.bits = 16,
	},
	[ID_AD7606_4] =
	{
		.num_channels = 4,
		.bits = 16,
	},
	[ID_AD7606_6] =
	{
		.num_channels = 6,
		.bits = 16,
	},
	[ID_AD7606_8] =
	{
		.num_channels = 8,
		.bits = 16,
	},
	[ID_AD7606B] =
	{
		.num_channels = 8,
		.bits = 16,
	},
	[ID_AD7606C_16] =
	{
		.num_channels = 8,
		.bits = 16,
	},
	[ID_AD7606C_18] =
	{
		.num_channels = 8,
		.bits = 18,
	},
	[ID_AD7608] =
	{
		.num_channels = 8,
		.bits = 18,
	},
	[ID_AD7609] =
	{
		.num_channels = 8,
		.bits = 18,
	},
};

//static const uint16_t tconv_max[] =
//{
//	1, 	/* AD7606_OSR_1 */
//	3,		/* AD7606_OSR_2 */
//	5,		/* AD7606_OSR_4 */
//	10,	/* AD7606_OSR_8 */
//	20,	/* AD7606_OSR_16 */
//	41,	/* AD7606_OSR_32 */
//	81,	/* AD7606_OSR_64 */
//	162,	/* AD7606_OSR_128 */
//	324	/* AD7606_OSR_256 */
//};

/** @formatter:on */

/**
 ***************<Private typedef>***************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 ***************<Private define>****************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 **************<Private variables>**************
 * @description		: null 
 * @note			: null 
 * @note			: null  
 */

/**
 **************<Public IMPL functions>**************
 * @description		: null 
 * @note			: null 
 * @note			: null 
 */

/**
 * @addtogroup group name 
 */

/**@{*/

/**@}*/

/**
 * @brief  Blocking conversion start and data read.
 *
 * This function performs a conversion start and then proceeds to reading
 * the conversion data.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 *
 * @param dev        - The device structure.
 * @param data       - Pointer to location of buffer where to store the data.
 *
 * *
 * @return
 *			- ret - return code.
 *         Example: -EIO - SPI communication error.
 *                  -ENOTSUP - Device not in software mode.
 *                  -EBADMSG - CRC computation mismatch.
 *                  -SUCCESS - No errors encountered.
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
int32_t ad7606_read(struct ad7606_dev *dev, uint16_t *data)
{
	int i;
//	uint32_t timeout = tconv_max [AD7606_OSR_256];
	
	ad7606_convst(dev, PIN_HIGH);
	/* Wait for BUSY falling edge */
	while(rt_pin_read(dev->gpio_busy));

//	rt_kprintf("1111111111111111111111111111111111\n");
	ad7606_convst(dev, PIN_LOW);

	for (i = 0; i < dev->num_channels; i++)
	{
		data[i] = AD_RESULT();
	}
//	rt_kprintf("data[0]=0x%4x data[1]=0x%4x data[2]=0x%4x data[3]=0x%4x data[4]=0x%4x data[5]=0x%4x\n",\
			data[0], data[1],data[2],data[3],data[4],data[5]);
	
//	ad7606_convst(dev, PIN_HIGH);
	
	return 0;
}
/**
 * @brief  Toggle the CONVST pin to start a conversion.
 *
 * If needed, this function also puts the device in ADC reading mode by a write
 * at address zero.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param dev        - The device structure.
 * *
 * @return
 *			- ret - return code.
 *         Example: -EIO - SPI communication error.
 *                  -ENOTSUP - Device not in software mode.
 *                  -EBADMSG - CRC computation mismatch.
 *                  -SUCCESS - No errors encountered.
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
int32_t ad7606_convst(struct ad7606_dev *dev, uint32_t value)
{
	rt_pin_write(dev->gpio_convst, value);

	return 0;
}
/**
 * @brief  Reset the device by toggling the reset GPIO.
 *
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param dev        - The device structure.
 * *
 * @return
 *			- ret - return code.
 *         Example: -EIO - SPI communication error.
 *                  -ENOTSUP - Device not in software mode.
 *                  -EBADMSG - CRC computation mismatch.
 *                  -SUCCESS - No errors encountered.
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
int32_t ad7606_reset(struct ad7606_dev *dev)
{
	rt_pin_write(dev->gpio_reset, PIN_HIGH);
	rt_thread_mdelay(3);
	rt_pin_write(dev->gpio_reset, PIN_LOW);
	rt_thread_mdelay(1);

	return 0;
}

/**
 * @brief Initialize the ad7606 device structure.
 *
 * Performs memory allocation of the device structure.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param device     - Pointer to location of device structure to write.
 * @param init_param - Pointer to configuration of the driver.
 *
 * *
 * @return ret - return code.
 *         Example: -ENOMEM - Memory allocation error.
 *                  -EIO - SPI communication error.
 *                  -EIO - GPIO initialization error.
 *                  -ENODEV - Unexpected device id.
 *                  -EBADMSG - CRC computation mismatch.
 *                  -SUCCESS - No errors encountered.
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
int32_t ad7606_init(struct ad7606_dev **device, struct ad7606_init_param *init_param)
{
	struct ad7606_dev *dev;

	dev = (struct ad7606_dev*) calloc(1, sizeof(*dev));
	if (!dev)
	{
		return -ENOMEM;
	}

	dev->device_id = init_param->device_id;

	dev->gpio_reset = init_param->gpio_reset;
	dev->gpio_convst = init_param->gpio_convst;
	dev->gpio_busy = init_param->gpio_busy;

	dev->num_channels = ad7606_chip_info_tbl [dev->device_id].num_channels;

	rt_pin_mode(dev->gpio_reset, PIN_MODE_OUTPUT);
	rt_pin_mode(dev->gpio_convst, PIN_MODE_OUTPUT);
	rt_pin_mode(dev->gpio_busy, PIN_MODE_INPUT);

	*device = dev;

	printf("ad7606 successfully initialized\n");

	return 0;
}
/**
 * @brief Free any resource used by the driver.
 *
 * This function performs CRC8 computation and checking if enabled in the device.
 *
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param dev        - The device structure.
 * *
 * @return
 *			- ret - return code.
 *         Example: -EIO - SPI communication error.
 *                  -ENOTSUP - Device not in software mode.
 *                  -EBADMSG - CRC computation mismatch.
 *                  -SUCCESS - No errors encountered.
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
int32_t ad7606_remove(struct ad7606_dev *dev)
{
	
	free(dev);
	
	return 0;
}

/** @} */    //\defgroup module API
/** @} *///\addtogroup dev module
//#endif //\end module API
