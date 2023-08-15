/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the drv_clk.c API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 12, 2021
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
 * Mar 12, 2021		  kernel       create this file
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
#include <rtdbg.h>
#include <drv_clk.h>
#include "drv_common.h"

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
#define DBG_TAG "board"
#define DBG_LVL DBG_INFO


//#define RCC_OSC_ALL
#define RCC_OSC_HSE
//#define RCC_OSC_LSE

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
 **************<public impl functions>**************
 */

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @brief development description of class
 * *
 * Parameters
 * *
 * @param  xxx	               new node to be inserted
 * @param [ in or out ]	xxx	new node to be inserted
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
 * Mar 12, 2021		  kernel       create this class
 *						
 */
#if defined(STM32F1XX)

#elif  defined(STM32F4XX)
/**
 * @brief development description of class
 * *
 * Parameters
 * *
 * @param  xxx	               new node to be inserted
 * @param [ in or out ]	xxx	new node to be inserted
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
 * 2021年2月25日		  kernel       create this class
 *
 */
void system_clock_config(int target_freq_mhz)
{
	/**
	 * 外部晶振初始化结构体
	 */
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{
	        0 };
	/**
	 * CPU,AHB,APB等总线时钟初始化结构体
	 */
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{
	        0 };

	/** Configure the main internal regulator output voltage
	 *  AHB时钟使能
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	/**
	 * 这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
	 * 时使性能与功耗实现平衡，此功能只有STM32F42xx和STM32F43xx器件有
	 * PWR_REGULATOR_VOLTAGE_SCALE1 设置调压器输出电压级别1
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks
	 *  使能PWR时钟,时钟源为HSI
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	/**
	 * 打开时钟源
	 */
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	/**
	 * 打开PLL
	 */
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	/**
	 * PLL时钟源选择
	 */
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	/**
	 * 主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63. default:4
	 */
	RCC_OscInitStruct.PLL.PLLM = 8;
	/**
	 * 主PLL倍频系数(PLL倍频),取值范围:64~432. default:168
	 */
	RCC_OscInitStruct.PLL.PLLN = target_freq_mhz;
	/**
	 * 系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
	 */
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	/**
	 * USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
	 */
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 * 选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	/**
	 * 设置系统时钟时钟源为PLL
	 */
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	/**
	 * AHB分频系数为1
	 */
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	/**
	 * AHB分频系数为4
	 */
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	/**
	 * AHB分频系数为2
	 */
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	/**
	 * 同时设置FLASH延时周期为5WS，也就是6个CPU周期。
	 */
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}


#ifdef LH_TFDSN_CLB
 /**
   * @brief System Clock Configuration
   * @retval None
   */
 /**
   * @brief System Clock Configuration
   * @retval None
   */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif


#ifdef LH_TFDSN_ZXB
 /**
   * @brief System Clock Configuration
   * @retval None
   */
 void SystemClock_Config(void)
 {
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
   RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

   /** Configure the main internal regulator output voltage
   */
   __HAL_RCC_PWR_CLK_ENABLE();
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
   /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
   RCC_OscInitStruct.LSIState = RCC_LSI_ON;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 25;
   RCC_OscInitStruct.PLL.PLLN = 336;
   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
   RCC_OscInitStruct.PLL.PLLQ = 4;
   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
   {
     Error_Handler();
   }
   /** Initializes the CPU, AHB and APB buses clocks
   */
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
   {
     Error_Handler();
   }
   PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
   PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
   if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
   {
     Error_Handler();
   }
 }
#endif
/**
 * @brief development description of class
 * *
 * Parameters
 * *
 * @param  xxx	               new node to be inserted
 * @param [ in or out ]	xxx	new node to be inserted
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
 * 2021年2月25日		  kernel       create this class
 *
 */
int clock_information(void)
{
	LOG_D("System Clock information");LOG_D("SYSCLK_Frequency = %d", HAL_RCC_GetSysClockFreq());LOG_D("HCLK_Frequency   = %d", HAL_RCC_GetHCLKFreq());LOG_D("PCLK1_Frequency  = %d", HAL_RCC_GetPCLK1Freq());LOG_D("PCLK2_Frequency  = %d", HAL_RCC_GetPCLK2Freq());

	return OS_EOK;
}

INIT_BOARD_EXPORT(clock_information);

#endif /* STM32F4XX */

/**
 * @brief development description of class
 * *
 * Parameters
 * *
 * @param  xxx	               new node to be inserted
 * @param [ in or out ]	xxx	new node to be inserted
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
 * 2021年2月25日		  kernel       create this class
 *
 */
void clk_init(char *clk_source, int source_freq, int target_freq)
{
	system_clock_config(target_freq);
}



/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

//#endif /**< end XXX module api major */
