/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the itc.c API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2021年3月23日
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
 * 2021年3月23日		  kernel       create this file
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
 ***************** include file *******************
 */
#include "itc.h"

//#ifdef  XXX    /**< XXX module api major */

/**
 * xxx note delete
 * sub group
 * 
 * @addtogroup group name 
 */

/**@{*/

/**@}*/

/**
 * @formatter:off
 ****************<Private macro>********************
 */

/**
 * xxx  macro define	
 *                    - description 
 * @param  	xxx      :- description.
 */
//#define desc_class(xxx, xxxx) 									"\"
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/**
 * @formatter:on 
 ***************<Private typedef>*******************
 */

/**
 * xxx  typedef define	
 * @description		: null 
 * @note			      : null 
 */


/**
 ***************<Private define>********************
 */

/**
 * xxx  typedef define	
 * @description		: null 
 * @note			      : null 
 */


/**
 **************<Private variables>******************
 */



/**
 **************<Private functions>******************
 */



/**
 **************<Public IMPL functions>**************
 */

/**
 * @addtogroup group name
 */

/**
 * @brief ad7606 event initialization
 * *
 * Parameters
 * *
 * @param [ in or out ]	xxx	struct rt_event * ad7606_event
 * *
 * @return
 *			- xxx   description  or none ,struct rt_event point
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
 * 2021年3月23日		kernel       create this class
 *
 */
//rt_err_t ad7606_event_init(struct rt_event * ad7606_event)
//{
//	rt_err_t result;

//	result = rt_event_init(ad7606_event, "ad7606", IPC_FLAG_FIFO);

//	return result;
//}

/**
 * @brief get ad7606 event
 * *
 * Parameters
 * *
 * * 
 * @return 
 *			- xxx   description  or none ,struct rt_event point
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
 * 2021年3月23日		kernel       create this class
 *						
 */
//struct rt_event * ad7606_event_get()
//{
//		static struct rt_event ad7606_event;

//		return &ad7606_event;
//}

/**
 * @brief up data sem initialization
 * *
 * Parameters
 * *
 * @param [ in or out ]	xxx	rt_sem_t up_sem
 * *
 * @return
 *			- xxx   description  or none ,OS_EOK or other
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
 * 2021年3月23日		kernel       create this class
 *
 */
rt_err_t up_sem_init(rt_sem_t up_sem)
{
	rt_err_t result;

	result = rt_sem_init(up_sem, "up_sem", 0, IPC_FLAG_FIFO);
//	rt_kprintf("up_sem addr:%x\n",up_sem);
//	rt_kprintf("up_sem type:%x\n",up_sem->parent.parent.type);
	if(result != OS_EOK)
	{
		rt_kprintf("up sen init failed!\n");
	}


	return result;
}

/**
 * @brief get ad7606 event
 * *
 * Parameters
 * *
 * *
 * @return
 *			- xxx   description  or none ,rt_sem_t point
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
 * 2021年3月23日		kernel       create this class
 *
 */
rt_sem_t up_sem_get()
{
		static struct rt_semaphore up_sem;

		return &up_sem;
}

/**
 * @brief up data sem initialization
 * *
 * Parameters
 * *
 * @param [ in or out ]	xxx	rt_sem_t up_sem
 * *
 * @return
 *			- xxx   description  or none
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
 * 2021年3月23日		kernel       create this class
 *
 */
void up_list_head_init(link_list_node_p list_head)
{
	memset(list_head,0,sizeof(link_list_node_t));

	return;
}

/**
 * @brief get ad7606 event
 * *
 * Parameters
 * *
 * *
 * @return
 *			- xxx   description  or none ,link_list_node_p point
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
 * 2021年3月23日		kernel       create this class
 *
 */
link_list_node_p up_list_head_get()
{
		static link_list_node_t up_list_head;

		return &up_list_head;
}

/**
 * @brief create a link_list_node
 * *
 * Parameters
 * *
 * *
 * @return
 *			- xxx   description  or none,link_list_node_p point
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
 * 2021年3月23日		kernel       create this class
 *
 */
link_list_node_p up_list_create()
{
	link_list_node_p link_list_node = OS_NULL;

	link_list_node = rt_malloc(sizeof(link_list_node_t));
	memset(link_list_node,0,sizeof(link_list_node_t));

	return link_list_node;
}

/**
 * @brief create a link_list_node
 * *
 * Parameters
 * *
 * *
 * @return
 *			- xxx   description  or none,link_list_node_p point
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
 * 2021年3月23日		kernel       create this class
 *
 */
void up_list_insert_of_end(link_list_node_p link_list_node)
{
	link_list_node_p link_list_tmp;

	link_list_tmp = up_list_head_get();

	while(link_list_tmp->next)
	{
		link_list_tmp = link_list_tmp->next;
	}

	link_list_tmp->next = link_list_node;
	link_list_node->prev = link_list_tmp;

	return;
}

/**
 * @brief itc initialization
 * *
 * Parameters
 * *
 * *
 * @return
 *			- xxx   none
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
 * 2021年3月23日		kernel       create this class
 *
 */
void itc_init()
{
	//struct rt_event * ad7606_event;
	rt_sem_t up_sem;
	link_list_node_p link_list_node;

	//ad7606_event = ad7606_event_get();
	//ad7606_event_init(ad7606_event);

	up_sem = up_sem_get();
	up_sem_init(up_sem);

	link_list_node = up_list_head_get();
	up_list_head_init(link_list_node);
}

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

//#endif /**< end XXX module api major */
