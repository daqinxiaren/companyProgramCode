/**
 *
 * Copyright (c) 2015-2020  Driver  Development
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the analog_device.c API or library.
 *
 * @features			:
 *
 * @author       		: plum <tiansyzxm@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020年5月28日
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
 * 2020年5月28日		  plum         create this file
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
#include <rtthread.h>
#include "switch_init.h"
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
 * settings depend check 
 * 
 */

/**
 * macro define	- description 
 * @xxx:	description.
 * @xxx:	description.
 * @xxx:	description.
 */
 
//#define desc_class(xxx, xxxx) 									
//for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)

/**
 ***************<Private typedef>***************
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
 * @brief adg1608 device initialization
 * *
 * @author	plum <tiansyzxm@163.com>  write time: 2020年5月28日
 * @create time  : 2020年5月28日
 * *
 * Parameters
 * *
 * @param	adg1608_pin_config_t adg1608 pin configuration
 * 
 * * 
 * @return 
 *			- none
 * @retval
 *			- none
 * *
 * @see  reference @file
 *
 * @note 	- none
 *							
 * @warning	- none
 *	
 * Change Logs:
 * Date           author       notes
 * 2020年5月28日	  plum         create this class
 *						
 */
 
void adg1608_init(adg1608_pin_config_p config)
{
     OS_ASSERT(config != OS_NULL);

	rt_pin_mode(config->adg1608_EN, PIN_MODE_OUTPUT);
	rt_pin_mode(config->adg1608_A0, PIN_MODE_OUTPUT);
	rt_pin_mode(config->adg1608_A1, PIN_MODE_OUTPUT);
	rt_pin_mode(config->adg1608_A2, PIN_MODE_OUTPUT);
}

 /**
 * @brief adg1608 device channel switch
 * *
 * @author	plum <tiansyzxm@163.com>  write time: 2020年5月28日
 * @create time  : 2020年5月28日
 * *
 * Parameters
 * *
 * @param	config  adg1608 pin configuration
 * @param   chanel  none or 1-8
 * *
 * @return
 *			- none
 * @retval
 *			- none
 * *
 * @see  reference @file
 *
 * @note 	- none
 *
 * @warning	- none
 *
 * Change Logs:
 * Date           author       notes
 * 2020年5月28日	  plum         create this class
 *
 */
 
void adg1608_switch(adg1608_pin_config_t * config, enum adg1608_truth_table chanel)
{
    OS_ASSERT(config != OS_NULL);

    if(terminal_none == chanel)
    {
        rt_pin_write(config->adg1608_EN,PIN_LOW);
    }
    else
    {
    	rt_kprintf("----chanel----%d\n", chanel);
        rt_pin_write(config->adg1608_EN,PIN_HIGH);
        switch(chanel)
        {
            case terminal_1:

                rt_pin_write(config->adg1608_A2,PIN_LOW);
                rt_pin_write(config->adg1608_A1,PIN_LOW);
                rt_pin_write(config->adg1608_A0,PIN_LOW);
                break;

            case terminal_2:

                rt_pin_write(config->adg1608_A2,PIN_LOW);
                rt_pin_write(config->adg1608_A1,PIN_LOW);
                rt_pin_write(config->adg1608_A0,PIN_HIGH);
                break;

            case terminal_3:

                rt_pin_write(config->adg1608_A2,PIN_LOW);
                rt_pin_write(config->adg1608_A1,PIN_HIGH);
                rt_pin_write(config->adg1608_A0,PIN_LOW);
                break;

            case terminal_4:

                rt_pin_write(config->adg1608_A2,PIN_LOW);
                rt_pin_write(config->adg1608_A1,PIN_HIGH);
                rt_pin_write(config->adg1608_A0,PIN_HIGH);
                break;

            case terminal_5:

                rt_pin_write(config->adg1608_A2,PIN_HIGH);
                rt_pin_write(config->adg1608_A1,PIN_LOW);
                rt_pin_write(config->adg1608_A0,PIN_LOW);
                break;

            case terminal_6:

                rt_pin_write(config->adg1608_A2,PIN_HIGH);
                rt_pin_write(config->adg1608_A1,PIN_LOW);
                rt_pin_write(config->adg1608_A0,PIN_HIGH);
                break;

            case terminal_7:

                rt_pin_write(config->adg1608_A2,PIN_HIGH);
                rt_pin_write(config->adg1608_A1,PIN_HIGH);
                rt_pin_write(config->adg1608_A0,PIN_LOW);
                break;

            case terminal_8:

                rt_pin_write(config->adg1608_A2,PIN_HIGH);
                rt_pin_write(config->adg1608_A1,PIN_HIGH);
                rt_pin_write(config->adg1608_A0,PIN_HIGH);
                break;

            default:
                break;
        }
    }

}

/**
 * @brief adg1609 device initialization
 * *
 * @author  plum <tiansyzxm@163.com>  write time: 2020年5月28日
 * @create time  : 2020年5月28日
 * *
 * Parameters
 * *
 * @param   config adg1609 pin configuration
 *
 * *
 * @return
 *          - none
 * @retval
 *          - none
 * *
 * @see  reference @file
 *
 * @note    - none
 *
 * @warning - none
 *
 * Change Logs:
 * Date           author       notes
 * 2020年5月28日     plum         create this class
 *
 */
void adg1609_init(adg1609_pin_config_t * config)
{
    OS_ASSERT(config != OS_NULL);

    rt_pin_mode(config->adg1609_EN, PIN_MODE_OUTPUT);
    rt_pin_mode(config->adg1609_A0, PIN_MODE_OUTPUT);
    rt_pin_mode(config->adg1609_A1, PIN_MODE_OUTPUT);
}

 /**
 * @brief adg1609 device channel switch
 * *
 * @author  plum <tiansyzxm@163.com>  write time: 2020年5月28日
 * @create time  : 2020年5月28日
 * *
 * Parameters
 * *
 * @param   config  adg1609 pin configuration
 * @param   chanel  none or 1-8
 * *
 * @return
 *          - none
 * @retval
 *          - none
 * *
 * @see  reference @file
 *
 * @note    - none
 *
 * @warning - none
 *
 * Change Logs:
 * Date           author       notes
 * 2020年5月28日     plum         create this class
 *
 */
void adg1609_switch(adg1609_pin_config_t * config, enum adg1609_truth_table chanel)
{
    OS_ASSERT(config != OS_NULL);

    if(adg_terminal_none == chanel)
    {
        rt_pin_write(config->adg1609_EN,PIN_LOW);
    }
    else
    {
        rt_pin_write(config->adg1609_EN,PIN_HIGH);
		
        switch(chanel)
        {
            case adg_terminal_1:

                rt_pin_write(config->adg1609_A1,PIN_LOW);
                rt_pin_write(config->adg1609_A0,PIN_LOW);

                break;

            case adg_terminal_2:

                rt_pin_write(config->adg1609_A1,PIN_LOW);
                rt_pin_write(config->adg1609_A0,PIN_HIGH);

                break;

            case adg_terminal_3:

                rt_pin_write(config->adg1609_A1,PIN_HIGH);
                rt_pin_write(config->adg1609_A0,PIN_LOW);

                break;

            case adg_terminal_4:

                rt_pin_write(config->adg1609_A1,PIN_HIGH);
                rt_pin_write(config->adg1609_A0,PIN_HIGH);

                break;

            default:
                break;
        }
    }

}

/** @} */ //\defgroup driver API
/** @} */ //\addtogroup drivers module
