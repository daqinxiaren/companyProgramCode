/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the bearing_algorithm.c API or library.
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
 

/**
 ***************** include file *******************
 */
#include <math.h>
#include <stdio.h>
#include <rtthread.h>
#include <stdint.h>
#include <bearing.h>
#include <algorithm.h>


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
static bearing_charac_value_datum_t bearing_charac_value_datum_all[] =
{
	{0.3587f,0.2586f,0.3964f,0.6348f},
	{0.8198f,0.5973f,1.4560f,0.6874f},
	{0.6074f,0.4765f,0.7476f,0.5390f},
};

static fault_frequency_to_speed_t fault_frequency_to_speed_all[] =
{
 	{1.1464f,0.8535f,0.6927f,0.0449f},
 	{1.0943f,0.8147f,0.6612f,0.0429f},
 	{1.0467f,0.7793f,0.6325f,0.041f},
};

static speed_class_threshold_t speed_class_threshold =
{
	180u,220u,250u
};

static troubleshooting_threshold_t troubleshooting_threshold =
{
	30u,36u,42u
};

static the_wheel_diameter_t the_wheel_diameter =
{
	840u,880u,920u
};

/**
 **************<Private functions>******************
 */



/**
 **************<Public IMPL functions>**************
 */

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/
/**
 * @brief fft parameter create
 * *
 * Parameters
 * *
 * *
 * @return
 *							- the point of bearing parameters
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
 * 2021年4月13日		  kernel       create this class
 *
 */
bearing_algorithm_para_p bearing_create()
{
	bearing_algorithm_para_p bearing_para;

	bearing_para = rt_malloc(sizeof(bearing_algorithm_para_t));
	if(NULL == bearing_para)
	{
		ALGO_PRINTF("bearing_para create failed!");

		return NULL;
	}

	bearing_para->fft_para = fft_create();
	if(NULL == bearing_para->fft_para)
	{
		ALGO_PRINTF("bearing_para create failed!");

		return NULL;
	}

	return bearing_para;
}
/**
 * @brief bearing_algorithm_para initialization
 * *
 * Parameters
 * *
 * @param bearing_para	-the point of bearing parameters
 * * 
 * @return 
 *			 *					- return code
 *					Example: -ALGO_OK -bearing parameter initialization success
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
int bearing_init(bearing_algorithm_para_p bearing_para)
{
	int result = 0;

	OS_ASSERT(bearing_para != NULL);

	bearing_para->speed_class_threshold = speed_class_threshold;
	bearing_para->troubleshooting_threshold = troubleshooting_threshold;
	bearing_para->the_wheel_diameter = the_wheel_diameter;

	result = fft_init(bearing_para->fft_para);
	if(result < 0)
	{
		ALGO_PRINTF("bearing parameters initialization failed!\n");

		return result;
	}

	return ALGO_OK;
}

/**
 * @brief set bearing parameters
 * *
 * Parameters
 * *
 * @param bearing_para	-the point of bearing parameters
 * @param arg				-the point of bearing parameters
 * @param type				-the type of bearing parameters
 * *
 * @return
 *			- xxx   description  or none ,the operation status or on successful
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
int bearing_set(bearing_algorithm_para_p bearing_para, void * arg, int type)
{
	OS_ASSERT(bearing_para != NULL);
	OS_ASSERT(arg != NULL);

	switch(type)
	{
		case speed:

			bearing_para->speed = *(uint16_t *)arg;

			if(bearing_para->speed < bearing_para->speed_class_threshold.level_1)
			{
				bearing_para->speed_level = zero_onehundredeighty;
			}
			else if((bearing_para->speed > bearing_para->speed_class_threshold.level_1) && (bearing_para->speed < bearing_para->speed_class_threshold.level_2))
			{
				bearing_para->speed_level = onehundredeighty_twohundredtwenty;
			}
			else if((bearing_para->speed > bearing_para->speed_class_threshold.level_2) && (bearing_para->speed < bearing_para->speed_class_threshold.level_3))
			{
				bearing_para->speed_level = twohundredtwenty_twohundredfifty;
			}

			bearing_para->bearing_charac_value_datum = bearing_charac_value_datum_all[bearing_para->speed_level];

			break;

		case wheel_diameter:

			bearing_para->wheel_diameter = *(uint16_t *)arg;

			if(bearing_para->wheel_diameter == bearing_para->the_wheel_diameter.wheel_diameter_1)
			{
				bearing_para->wheel_diameter_level = wheel_diameter_1;
			}
			else if(bearing_para->wheel_diameter == bearing_para->the_wheel_diameter.wheel_diameter_2)
			{
				bearing_para->wheel_diameter_level = wheel_diameter_2;
			}
			else if(bearing_para->wheel_diameter == bearing_para->the_wheel_diameter.wheel_diameter_3)
			{
				bearing_para->wheel_diameter_level = wheel_diameter_3;
			}
			else
			{
				ALGO_PRINTF("wheel_diameter parameter error!exit!\n");
				return -ALGO_EINVAL;
			}

			bearing_para->fault_frequency_to_speed = fault_frequency_to_speed_all[bearing_para->wheel_diameter_level];

			break;

		default:
			break;
	}

	return ALGO_OK;
}

/**
 * @brief bearing_algorithm_para initialization
 * *
 * Parameters
 * *
 * @param bearing_para	-the point of bearing parameters
 * @param buff				-the point of data to bearing algorithm
 * *
 * @return
 *			- xxx   description  or none ,the operation status or on successful
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
void bearing(bearing_algorithm_para_p bearing_para,uint16_t * buff,int len)
{
	int i,j;
	int charac_freq_pos_max, charac_freq_pos_min, max_freq_pos, max_freq_value;

	fft(bearing_para->fft_para, buff);

	for(i = 0; i < 4; i++)
	{
		charac_freq_pos_min = 0;
		charac_freq_pos_max = 0;
		max_freq_value = 0;
		max_freq_pos = 0;

		charac_freq_pos_min = (((float *)&bearing_para->fault_frequency_to_speed)[i] * bearing_para->speed - 5) / (len / bearing_para->fft_para->fft_size) + 1 + 0.5;
		charac_freq_pos_max = (((float *)&bearing_para->fault_frequency_to_speed)[i] * bearing_para->speed + 5) / (len / bearing_para->fft_para->fft_size) + 1 + 0.5;

		if(charac_freq_pos_min <= 1)
		{
			charac_freq_pos_min = 0;
			((uint8_t *)&bearing_para->bearing_eigenvalue)[i] = 0;
		}
		else
		{
			for(j = charac_freq_pos_min; j <= charac_freq_pos_max; j++)
			{
				if(bearing_para->fft_para->fft_cache[j] > max_freq_value)
				{
					max_freq_value = bearing_para->fft_para->fft_cache[j];
					max_freq_pos = j;
				}
			}

			((uint8_t *)&bearing_para->bearing_eigenvalue)[i] = 20.0 * log10(bearing_para->fft_para->fft_cache[max_freq_pos] / ((float *)&bearing_para->bearing_charac_value_datum)[i]) + 10;
		}
	}

}

/**@}*/
/**
 * @brief fft parameter create
 * *
 * Parameters
 * *
 * *
 * @return
 *							- the point of bearing parameters
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
 * 2021年4月13日		  kernel       create this class
 *
 */
void bearing_destroy(bearing_algorithm_para_p bearing_para)
{
	OS_ASSERT(bearing_para != NULL);

	fft_destroy(bearing_para->fft_para);

	rt_free(bearing_para);
}

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

//#endif /**< end XXX module api major */
