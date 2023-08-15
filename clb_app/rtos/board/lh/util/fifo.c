/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the fifo.c API or library.
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
#include "fifo.h"
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
 * @brief Create new fifo element
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param buff - Data to be saved in fifo.
 * @param len - Length of the data.
 * *
 * @return
 *			- fifo element in case of success, NULL otherwise
 *
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
static struct fifo_element* fifo_new_element(char *buff, uint32_t len)
{
	struct fifo_element *q = calloc(1, sizeof(struct fifo_element));
	if (!q)
	{
		return NULL;
	}

	q->len = len;
	q->data = calloc(1, len);
	if (!(q->data))
	{
		free(q);
		return NULL;
	}
	memcpy(q->data, buff, len);

	return q;
}

/**
 * @brief Get last element in fifo
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param p_fifo - pointer to fifo
 * *
 * @return
 *			- fifo last element if exists, NULL otherwise
 *
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
static struct fifo_element* fifo_get_last(struct fifo_element *p_fifo)
{
	if (p_fifo == NULL)
	{
		return NULL;
	}
	while (p_fifo->next)
	{
		p_fifo = p_fifo->next;
	}
	return p_fifo;
}

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
 * @brief Insert element to fifo, in the last position.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param p_fifo 	- Pointer to fifo.
 * @param buff 	- Data to be saved in fifo.
 * @param len 		- Length of the data.
 *
 * *
 * @return
 *			- SUCCESS in case of success, FAILURE otherwise
 *
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
int32_t fifo_insert(struct fifo_element **p_fifo, char *buff, uint32_t len)
{
	struct fifo_element *p, *q;

	if (len <= 0)
	{
		return FAILURE;
	}

	q = fifo_new_element(buff, len);
	if (!q)
	{
		return FAILURE;
	}

	if (!(*p_fifo))
	{
		*p_fifo = q;
	}
	else
	{
		p = fifo_get_last(*p_fifo);
		p->next = q;
	}

	return SUCCESS;
}

/**
 * @brief Remove fifo head.
 * *
 * @author	kernel	<kernel_sa@163.com>  write time: 2021年2月22日
 * @create time  : 2021年2月22日
 * *
 * Parameters
 * *
 * @param	p_fifo	new node to be inserted
 *
 * * 
 * @return 
 *			- next element in fifo if exists, NULL otherwise.
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
struct fifo_element* fifo_remove(struct fifo_element *p_fifo)
{
	struct fifo_element *p = p_fifo;

	if (p_fifo != NULL)
	{
		p_fifo = p_fifo->next;
		free(p->data);
		free(p);
	}

	return p_fifo;
}

/** @} */ //\defgroup module API
/** @} *///\addtogroup dev module
//#endif //\end module API
