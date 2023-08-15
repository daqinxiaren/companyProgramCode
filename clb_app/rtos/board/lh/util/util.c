/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the util.c API or library.
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
#include "util.h"
#include "error.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
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
 * @brief Find first set bit in word
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param	word	new node to be inserted
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
uint32_t find_first_set_bit(uint32_t word)
{
	uint32_t first_set_bit = 0;

	while (word)
	{
		if (word & 0x1)
		{
			return first_set_bit;
		}
		word >>= 1;
		first_set_bit++;
	}

	return 32;
}

/**
 * @brief Find last set bit in word.
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
uint32_t find_last_set_bit(uint32_t word)
{
	uint32_t bit = 0;
	uint32_t last_set_bit = 32;

	while (word)
	{
		if (word & 0x1)
		{
			last_set_bit = bit;
		}
		word >>= 1;
		bit++;
	}

	return last_set_bit;
}

/**
 * @brief  Locate the closest element in an array.
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
uint32_t find_closest(int32_t val, const int32_t *array, uint32_t size)
{
	int32_t diff = abs(array [0] - val);
	uint32_t ret = 0;
	uint32_t i;

	for (i = 1; i < size; i++)
	{
		if (abs(array[i] - val) < diff)
		{
			diff = abs(array [i] - val);
			ret = i;
		}
	}

	return ret;
}
/**
 * @brief Shift the value and apply the specified mask.
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
uint32_t field_prep(uint32_t mask, uint32_t val)
{
	return (val << find_first_set_bit(mask)) & mask;
}

/**
 * @brief Get a field specified by a mask from a word.
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
uint32_t field_get(uint32_t mask, uint32_t word)
{
	return (word & mask) >> find_first_set_bit(mask);
}

/**
 * @brief Log base 2 of the given number.
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
int32_t log_base_2(uint32_t x)
{
	return find_last_set_bit(x);
}

/**
 * @brief Find greatest common divisor of the given two numbers.
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
uint32_t greatest_common_divisor(uint32_t a, uint32_t b)
{
	uint32_t div;
	uint32_t common_div = 1;

	if ((a == 0) || (b == 0))
	{
		return max(a, b);
	}

	for (div = 1; (div <= a) && (div <= b); div++)
	{
		if (!(a % div) && !(b % div))
		{
			common_div = div;
		}
	}

	return common_div;
}
/**
 * @brief Calculate best rational approximation for a given fraction.
 * @formatter:off
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param	given_numerator	new node to be inserted
 * @param	given_denominator	new node to be inserted
 * @param	max_numerator		new node to be inserted
 * @param	max_denominator	new node to be inserted
 * @param	best_numerator		new node to be inserted
 * @param	best_denominator	new node to be inserted
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
void rational_best_approximation(uint32_t given_numerator,
											uint32_t given_denominator,
											uint32_t max_numerator,
											uint32_t max_denominator,
											uint32_t *best_numerator,
											uint32_t *best_denominator
        									)
{
	uint32_t gcd;

	gcd = greatest_common_divisor(given_numerator, given_denominator);

	*best_numerator = given_numerator / gcd;
	*best_denominator = given_denominator / gcd;

	if ((*best_numerator > max_numerator) || (*best_denominator > max_denominator))
	{
		*best_numerator = 0;
		*best_denominator = 0;
	}
}

/** @formatter:on */
/**
 * @brief Calculate the number of set bits.
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
uint32_t hweight8(uint32_t word)
{
	uint32_t count = 0;

	while (word)
	{
		if (word & 0x1)
		{
			count++;
		}
		word >>= 1;
	}

	return count;
}

/**
 * @brief Calculate the quotient and the remainder of an integer division.
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
uint64_t do_div(uint64_t *n, uint64_t base)
{
	uint64_t mod = 0;

	mod = *n % base;
	*n = *n / base;

	return mod;
}
/**
 * @brief Unsigned 64bit divide with 64bit divisor and remainder
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
uint64_t div64_u64_rem(uint64_t dividend, uint64_t divisor, uint64_t *remainder)
{
	*remainder = dividend % divisor;

	return dividend / divisor;
}
/**
 * @brief Unsigned 64bit divide with 32bit divisor with remainder
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
uint64_t div_u64_rem(uint64_t dividend, uint32_t divisor, uint32_t *remainder)
{
	*remainder = do_div(&dividend, divisor);

	return dividend;
}

/**
 * @brief  Unsigned 64bit divide with 32bit divisor
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
uint64_t div_u64(uint64_t dividend, uint32_t divisor)
{
	uint32_t remainder;

	return div_u64_rem(dividend, divisor, &remainder);
}

/**
 * @brief Converts from string to int32_t
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param	*str	new node to be inserted
 *
 * *
 * @return
 *			- xxx   int32_t
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
int32_t str_to_int32(const char *str)
{
	char *end;
	int32_t value = strtol(str, &end, 0);

	if (end == str)
	{
		return -EINVAL;
	}
	else
	{
		return value;
	}
}

/**
 * @brief Converts from string to uint32_t
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * *
 * @param	*str	new node to be inserted
 *
 * *
 * @return
 *			- xxx   int32_t
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
uint32_t srt_to_uint32(const char *str)
{
	char *end;
	uint32_t value = strtoul(str, &end, 0);

	if (end == str)
	{
		return -EINVAL;
	}
	else
	{
		return value;
	}
}

/**
 * @brief  bubble_sort
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * *
 * @param	uint16_t * buff	buff of buble sort
 *				uint8_t length		buff length
 *
 * *
 * @return
 *			- xxx   int32_t
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
 * 2021年2月22日		kernel       create this class
 *
 */
void bubble_sort(uint16_t * buff, uint8_t length)
{
	int temp;//临时变量

	for(int i = 0; i < length-1; i++)
	{
		for(int j = length-1; j > i; j--)
		{
			if(buff[j] < buff[j-1])
			{
				temp = buff[j];
				buff[j] = buff[j-1];
				buff[j-1] = temp;
			}
		}
	}
 }

uint16_t sum_check_u16(uint8_t *data, uint16_t crclen)
{
	int i;
	uint16_t check_sum=0;
	for(i=0;i<crclen;i++)
	{
		check_sum+=	data[i];
	}
	return check_sum;
}

/** @} */ //\defgroup module API
/** @} *///\addtogroup dev module
//#endif //\end module API
