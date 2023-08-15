/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the bearing_algorithm.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2021年4月12日
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
 * 2021年4月12日		  kernel       create this file
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
 
#ifndef LH_ALGORITHM_INCLUDE_BEARING_ALGORITHM_H_
#define LH_ALGORITHM_INCLUDE_BEARING_ALGORITHM_H_

/**
 ***************** include file ***************
 */
#include <fft.h>

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
typedef struct the_wheel_diameter
{
		int wheel_diameter_1;
		int wheel_diameter_2;
		int wheel_diameter_3;

}the_wheel_diameter_t,the_wheel_diameter_p;

typedef struct troubleshooting_thresholds
{
		int anticipation;
		int warning;
		int alarm;

}troubleshooting_threshold_t, * troubleshooting_threshold_p;

typedef struct speed_classification_threshold
{
		int level_1;
		int level_2;
		int level_3;

}speed_class_threshold_t, * speed_class_threshold_p;

typedef struct fault_frequency_to_speed
{
		float inner_ring;
		float outer_ring;
		float rolling_element;
		float cage;

}fault_frequency_to_speed_t, * fault_frequency_to_speed_p;

typedef struct bearing_characteristic_value_datum
{
		float inner_ring;
		float outer_ring;
		float rolling_element;
		float cage;

}bearing_charac_value_datum_t, * bearing_charac_value_datum_p;

typedef struct bearing_eigenvalue
{
		uint8_t inner_ring;
		uint8_t outer_ring;
		uint8_t rolling_element;
		uint8_t cage;

}bearing_eigenvalue_t,bearing_eigenvalue_p;

typedef struct bearing_algorithm_parameter
{
		uint16_t speed_level;
		uint16_t speed;
		uint16_t wheel_diameter_level;
		uint16_t wheel_diameter;
		fft_parameter_p fft_para;
		the_wheel_diameter_t the_wheel_diameter;
		troubleshooting_threshold_t troubleshooting_threshold;
		speed_class_threshold_t speed_class_threshold;
		fault_frequency_to_speed_t fault_frequency_to_speed;
		bearing_charac_value_datum_t bearing_charac_value_datum;
		bearing_eigenvalue_t bearing_eigenvalue;

}bearing_algorithm_para_t, * bearing_algorithm_para_p;

/**
 **************<Public variables>**************
 */
enum fault_diagnosis_result
{
	normal = 0,
	anticipation = 1,
	warning = 2,
	alarm = 3
};

enum speed_level
{
	zero_onehundredeighty = 0,
	onehundredeighty_twohundredtwenty = 1,
	twohundredtwenty_twohundredfifty = 2,
};

enum wheel_diameter_level
{
	wheel_diameter_1 = 0,
	wheel_diameter_2 = 1,
	wheel_diameter_3 = 2,
};

enum bearing_algorithm_para_type
{
	speed = 0,
	wheel_diameter = 1,
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
 * @brief development description of class
 * *
 * Parameters
 * *
 * @param  xxx               -	new node to be inserted
 * @param [ in or out ]	xxx  -	new node to be inserted
 *
 * * 
 * @return 
 *			- xxx   description or none,the operation status or on successful
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
 * 2021年4月12日		  kernel       create this class
 *						
 */




/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

//#endif  /**< end XXX module api major */

#endif /* LH_ALGORITHM_INCLUDE_BEARING_ALGORITHM_H_ */

