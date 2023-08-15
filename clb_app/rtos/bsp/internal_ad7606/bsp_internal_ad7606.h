/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __BSP_INTERNAL_AD7606_H__
#define __BSP_INTERNAL_AD7606_H__

/* include */
#include <stm32f4xx.h>
#include <rtthread.h>
#include "app_board_config.h"

/* macro */

#define AD_RESULT()									(*(__IO uint16_t *)0x6C000000)
#define AD7606_MAX_CHANNELS						8

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

/* type declaration */
typedef struct
{
	uint8_t num_channels;
	uint8_t bits;
	uint8_t device_id;
}bsp_ad7606_chip_info_t;

typedef struct
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
}bsp_ad7606_dev_t;

typedef struct ad7606_init_param
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
}bsp_ad7606_init_param_t;

/* function declaration */
int32_t bsp_internal_ad7606_init(bsp_ad7606_dev_t **device, bsp_ad7606_init_param_t *init_param);
int32_t bsp_internal_ad7606_reset(bsp_ad7606_dev_t *dev);
int32_t bsp_internal_ad7606_read(bsp_ad7606_dev_t *dev, uint16_t *data);
int32_t bsp_internal_ad7606_remove(bsp_ad7606_dev_t *dev);



#endif /*__BSP_INTERNAL_AD7606_H__*/


