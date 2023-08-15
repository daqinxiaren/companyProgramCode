/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <drivers/pin.h>
#include "error.h"
#include "util.h"
#include "crc.h"
#include "bsp_internal_ad7606.h"
#include "drv_gpio.h"

/* macro */

/* variable declaration */
static const bsp_ad7606_chip_info_t bsp_ad7606_chip_info_tbl[] =
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

/* function delcaration */

/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/
static int32_t bsp_internal_ad7606_convst(bsp_ad7606_dev_t *dev, uint32_t value)
{
	rt_pin_write(dev->gpio_convst, value);

	return 0;
}
/**
 * @brief ad7606读取数据
 * @param
 * @return if ok return 0 else fail
 * @note
**/
int32_t bsp_internal_ad7606_read(bsp_ad7606_dev_t *dev, uint16_t *data)
{
	int i;

	bsp_internal_ad7606_convst(dev, PIN_HIGH);
	/* Wait for BUSY falling edge */
	while(rt_pin_read(dev->gpio_busy));

	bsp_internal_ad7606_convst(dev, PIN_LOW);

	for (i = 0; i < dev->num_channels; i++)
	{
		data [i] = AD_RESULT();
	}
	return 0;
}
/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/

int32_t bsp_internal_ad7606_reset(bsp_ad7606_dev_t *dev)
{
	rt_pin_write(dev->gpio_reset, PIN_HIGH);
	rt_thread_mdelay(3);
	rt_pin_write(dev->gpio_reset, PIN_LOW);
	rt_thread_mdelay(1);

	return 0;
}
/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/
int32_t bsp_internal_ad7606_remove(bsp_ad7606_dev_t *dev)
{

	free(dev);

	return 0;
}
/**
 * @brief
 * @param
 * @return if ok return 0 else fail
 * @note
**/
int32_t bsp_internal_ad7606_init(bsp_ad7606_dev_t **device, bsp_ad7606_init_param_t *init_param)
{
	bsp_ad7606_dev_t *dev;

	dev = (bsp_ad7606_dev_t*) calloc(1, sizeof(*dev));
	if (!dev)
	{
		return -ENOMEM;
	}

	dev->device_id = init_param->device_id;

	dev->gpio_reset = init_param->gpio_reset;
	dev->gpio_convst = init_param->gpio_convst;
	dev->gpio_busy = init_param->gpio_busy;

	dev->num_channels = bsp_ad7606_chip_info_tbl [dev->device_id].num_channels;

	rt_pin_mode(dev->gpio_reset, PIN_MODE_OUTPUT);
	rt_pin_mode(dev->gpio_convst, PIN_MODE_OUTPUT);
	rt_pin_mode(dev->gpio_busy, PIN_MODE_INPUT);

	*device = dev;

	printf("ad7606 successfully initialized\n");

	return 0;
}


