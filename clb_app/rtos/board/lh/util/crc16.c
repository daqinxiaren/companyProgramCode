/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the crc16.c API or library.
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
#include "crc16.h"

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
 * @brief Creates the CRC-16 lookup table for a given polynomial.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param table      - Pointer to a CRC-16 lookup table to write to.
 * @param polynomial - msb-first representation of desired polynomial.
 *
 * Polynomials in CRC algorithms are typically represented as shown below.
 *
 *    poly = x^16 + x^14 + x^13 + x^12 + x^10 + x^8 + x^6 + x^4 + x^3 +
 *           x^1 + 1
 *
 * Using msb-first direction, x^15 maps to the msb.
 *
 *    msb first: poly = (1)0111010101011011 = 0x755B
 *
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
 *
 */
void crc16_populate_msb(uint16_t *table, const uint16_t polynomial)
{
	if (!table)
	{
		return;
	}

	for (int16_t n = 0; n < CRC16_TABLE_SIZE; n++)
	{
		uint16_t current_byte = (uint16_t) (n << 8);
		for (uint8_t bit = 0; bit < 8; bit++)
		{
			if ((current_byte & 0x8000) != 0)
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
 * @brief Computes the CRC-16 over a buffer of data.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param table     - Pointer to a CRC-16 lookup table for the desired polynomial.
 * @param pdata     - Pointer to data buffer.
 * @param nbytes    - Number of bytes to compute the CRC-16 over.
 * @param crc       - Initial value for the CRC-16 computation. Can be used to
 *                    cascade calls to this function by providing a previous
 *                    output of this function as the crc parameter.
 *
 *
 * * 
 * @return 
 *			- crc      - Computed CRC-16 value.
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
uint16_t crc16(const uint16_t *table, const uint8_t *pdata, size_t nbytes, uint16_t crc)
{
	unsigned int idx;

	while (nbytes--)
	{
		idx = ((crc >> 8) ^ *pdata) & 0xff;
		crc = (table [idx] ^ (crc << 8)) & 0xffff;
		pdata++;
	}

	return crc;
}

/** @} */ //\defgroup module API
/** @} *///\addtogroup dev module
//#endif //\end module API
