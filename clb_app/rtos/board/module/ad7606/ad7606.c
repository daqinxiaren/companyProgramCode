/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: file for the ad7606.c API or library.
 *
 * @features			:
 *
 * @author       		: 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: 2020��6��1��
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
 * 2020��6��1��		  kernel       create this file
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
#include <drv_config.h>

#if defined(ENABLE_FSMC_ADC7606)
#include <ad7606.h>
#include <io_config.h>

/*BUSY */
#define BUSY_GPIO		   AD7606_BUSY_IO_PORT//
static uint16_t  BUSY_GPIO_PIN=AD7606_BUSY;

/*CONVST*/
#define CONVST_1()	AD7606_CONVST_IO_PORT->BSRR = AD7606_CONVST
#define CONVST_0()	AD7606_CONVST_IO_PORT->BSRR = (uint32_t)AD7606_CONVST << 16U

/* RESET */
#define RESET_1()	  AD7606_RESET_IO_PORT->BSRR = AD7606_RESET
#define RESET_0()	  AD7606_RESET_IO_PORT->BSRR = (uint32_t)AD7606_RESET << 16U

/*AD7606FSMC-NE3*/
//#define AD_RESULT()	*(__IO uint16_t *)0x64000000

/*AD7606FSMC-NE4*/
#define AD_RESULT()	*(__IO uint16_t *)0x6C000000

/* Public variables ---------------------------------------------------------*/
AD7606_VAR_T  g_tAD7606;		/**/

/* Private function prototypes -----------------------------------------------*/
//static void AD7606_CtrlLinesConfig(void);
//static void AD7606_FSMCConfig(void);
//static void AD7606_SetOS(AD7606_OS_E _ucOS);
//static void AD7606_SetInputRange(AD_RANGE_E range);
static void AD7606_Reset(void);

/*
*********************************************************************************************************
*   函 数 名: Delay_ARMJISHU
*   功能说明: 汇编语句是5条指令
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void Delay_ARMJISHU(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--)
  {
  }
}

/*******************************************************************************
* Function Name  : InitAD7606
* Description    :
* Input          :
* Output         : None
* Return         : None
*******************************************************************************/
void InitAD7606(void)
{
	Delay_ARMJISHU(1);
	RESET_0();
	CONVST_1();
	Delay_ARMJISHU(1);
	AD7606_Reset();
}


/*
*/
static void AD7606_Reset(void)
{
	RESET_1();
	Delay_ARMJISHU(3);
	RESET_0();
	Delay_ARMJISHU(1);
}

/*
*/
void AD7606_StartConvst(void)
{
	InitAD7606();
	CONVST_0();
	Delay_ARMJISHU(1);
	CONVST_1();
	Delay_ARMJISHU(1);
}

void AD7606_ReadNowAdc(void)
{

	while(BUSY_GPIO->IDR & BUSY_GPIO_PIN);/*AD_BUSY 高电平正在转换�*/

    CONVST_0();
    /**/
    g_tAD7606.sNowAdc[0] = AD_RESULT();  //V1 OUT_C
    g_tAD7606.sNowAdc[1] = AD_RESULT();  //V2 
    g_tAD7606.sNowAdc[2] = AD_RESULT();  //V3 
    g_tAD7606.sNowAdc[3] = AD_RESULT();  //V4
	g_tAD7606.sNowAdc[4] = AD_RESULT();  //V5
	g_tAD7606.sNowAdc[5] = AD_RESULT();  //V6
    /*AD_CONVST */
	CONVST_1();
}
/*******************************************************************************
 * Function Name  : Stop_AD7606
 * Description    : ֹͣAD����
 * Input          :
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Stop_AD7606(void)
{
	//TIM_Cmd(TIM2, DISABLE);
	rt_kprintf("Stop ADC_ACC!\n");
}

#if XXXX
/**
 * xxx note delete
 * sub group
 *
 * @addtogroup group name 
 */
#endif //\end module API


#endif
