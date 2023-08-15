/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the ad7606_app1.h API or library.
 *
 * @features			:
 *
 * @author       		: www
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020年6月8日
 *
 * @environment  		: null
 *
 * @description  		: null 
 *
 * @module       		: null
 *
 * \n <b>@note</b> 		: null
 *
 * \b @warning     		: null
 * 
 * *
 * Change Logs:
 * Date				author		notes
 * 2020年6月8日			www		create this file
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
 
#ifndef _AD7606_APP_H_
#define _AD7606_APP_H_

/**
 ***************** include file ***************
 */
#include <rtthread.h>
#include <stm32f4xx.h>
#include "ad7606_init.h"


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/** @formatter:off  */
/**
 ****************<Public macro>****************
 */


/**
 * macro define	- description 
 * @description		: null 
 * @note			: null 
 * @param	param	-	description.
 * @param	param	-	description.
 * @note			: null 
 */
//#define desc_class(xxx, xxxx)
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)
#define FS_AD7606                  	(4096u)
#define FS_FILTER                  	(20480u) //固定采集周期 6s 采集一个通道

/** @formatter:on */
/*error mailbox*/


/**
 **************<Public variables>**************
 */

/**
 ***************<Public typedef>***************
 */
#if 0
typedef struct ad7606_data_head
{
		uint16_t ad7606_pack_head;
		uint8_t reserve[62];

}ad7606_data_head_t, * ad7606_data_head_p;

typedef struct ad7606_channel_data
{
		ad7606_data_head_t data_head;
		uint16_t * ad7606_data_buffer;

}ad7606_ch_data_t, * ad7606_ch_data_p;

typedef struct ad7606_data
{
	ad7606_ch_data_t ad7606_data_ch0;
	ad7606_ch_data_t ad7606_data_ch1;
	ad7606_ch_data_t ad7606_data_ch2;
	ad7606_ch_data_t ad7606_data_ch3;
	ad7606_ch_data_t ad7606_data_ch4;
	ad7606_ch_data_t ad7606_data_ch5;
	ad7606_ch_data_t ad7606_data_ch6;
	ad7606_ch_data_t ad7606_data_ch7;

}ad7606_data_t, * ad7606_data_p;

#endif
struct filter_data{
	uint8_t filter_flag;
	uint16_t filter_length;
};


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
 */

/**@}*/
/**
 * @brief ad7606 data initialization
 * *
 * @author	www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * *
 * @return
 *			- xxx   ad7606_data_p ad7606 data point
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			init/destroy ad7606
 *
 * @warning
 *
 * Change Logs:
 * Date				author			notes
 * 2020年6月8日			www			create this class
 *
 */
//ad7606_data_p ad7606_databuff_get();

/**
 * @brief ad7606 data initialization
 * *
 * @author	www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param				param	new node to be inserted
 * @param [ in or out ]	param	new node to be inserted
 * *
 * @return
 *			- xxx   description or none
 *			- xxx   description ,the operation status or on successful
 * @retval
 *			- xxx   description
 * *
 * @see  reference @file
 *
 * @note 			init/destroy ad7606
 *
 * @warning
 *
 * Change Logs:
 * Date				author			notes
 * 2020年6月8日			www			create this class
 *
 */
void ad7606_data_init(void);

/**
 * @brief development description of class
 * *
 * @author	www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param				param	new node to be inserted
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
 * 2020年6月8日		  www       create this class
 *						
 */
void timer2_interrupt_deal(void);

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif /* PROJECT_LH_BOARD_MODULE_ADC_AD7606_APP_H_ */
/* !< end file   */
