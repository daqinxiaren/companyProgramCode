/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the crc8.c API or library.
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
#include "crc8.h"

//#ifdef ENABLE_CRC8

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

/**
 * macro define	- description 
 * @xxx:	description.
 * @xxx:	description.
 * @xxx:	description.
 */
//#define desc_class(xxx, xxxx)
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


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
 **************<Private functions>**************
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
 * @brief Creates the CRC-8 lookup table for a given polynomial.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param table      - Pointer to a CRC-8 lookup table to write to.
 * @param polynomial - msb-first representation of desired polynomial.
 *
 * Polynomials in CRC algorithms are typically represented as shown below.
 *
 *	poly = x^8 + x^2 + x^1 + 1
 *
 * Using msb-first direction, x^7 maps to the msb.
 *
 * 	msb first: poly = (1)00000111 = 0x07
 * *
 * @return
 *			- None.
 * @retval
 *			- None.
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
 */
void crc8_populate_msb(uint8_t *table, const uint8_t polynomial)
{
	if (!table)
	{
		return;
	}

	for (int16_t n = 0; n < CRC8_TABLE_SIZE; n++)
	{
		uint8_t current_byte = (uint8_t) n;
		for (uint8_t bit = 0; bit < 8; bit++)
		{
			if ((current_byte & 0x80) != 0)
			{
				current_byte <<= 1;
				current_byte ^= polynomial;
			}
			else
			{
				current_byte <<= 1;
			}
		}
		table [n] = current_byte;
	}
}

/**
 * @brief Computes the CRC-8 over a buffer of data.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param table     - Pointer to a CRC-8 lookup table for the desired polynomial.
 * @param pdata     - Pointer to 8-bit data buffer.
 * @param nbytes    - Number of bytes to compute the CRC-8 over.
 * @param crc       - Initial value for the CRC-8 computation. Can be used to
 *                    cascade calls to this function by providing a previous
 *                    output of this function as the crc parameter.
 *
 * *
 * @return
 *			- crc      - Computed CRC-8 value.
 * @retval
 *			- NULL
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
uint8_t crc8(const uint8_t *table, const uint8_t *pdata, size_t nbytes, uint8_t crc)
{
	unsigned int idx;

	while (nbytes--)
	{
		idx = (crc ^ *pdata);
		crc = (table [idx]) & 0xff;
		pdata++;
	}
	return crc;
}


/** @} */ //\defgroup module API
/** @} *///\addtogroup dev module
//#endif //\end module API
