/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the itc.h API or library.
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
 
#ifndef LH_ITC_ITC_H_
#define LH_ITC_ITC_H_

/**
 ***************** include file ***************
 */
#include <rtthread.h>
//#include "xfer_protocol.h"

//#ifdef  XXX    /**< XXX module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */

//#define XXX_VERSION	 		1 			/**< major version number */
#define AD7606_EVENT_SEND_SAVE_FLAG         1


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
typedef struct link_list_node
{
		struct link_list_node * prev;
		struct link_list_node * next;
		char file_name[32];

}link_list_node_t, * link_list_node_p;

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
 struct rt_event * ad7606_event_get();

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
 rt_sem_t up_sem_get();

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
 link_list_node_p up_list_head_get();

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
 link_list_node_p up_list_create();

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
 void up_list_insert_of_end(link_list_node_p link_list_node);

 void itc_init();

// void comm_pub_data_init(comm_pub_data_p comm_pub_data);

// comm_pub_data_p comm_pub_data_get();

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

//#endif  /**< end XXX module api major */

#endif /* LH_ITC_ITC_H_ */

