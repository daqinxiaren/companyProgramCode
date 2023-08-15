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
 
#ifndef AD7606_H_
#define AD7606_H_
#include <rtthread.h>

#if defined(ENABLE_FSMC_ADC7606)

/* Public typedef ------------------------------------------------------------*/
/* AD�ɼ��ź�����` */
typedef enum
{
	AD_WENDU, /*�¶�*/
	AD_COMMON,/*Ӧ�䡢ѹ��*/
	AD_IMS,   /*IMS*/
	AD_WRDS,  /*�ֶԲ�Բ˳*/
	AD_PWX    /*ƽ����*/
}AD_TYPE_E;


/* ��ѹ��Χ */
typedef enum
{
	AD_RANGE_0_2_5 = 10,
	AD_RANGE_0_5   = 11,
	AD_RANGE_0_10  = 12,
	AD_RANGE_2_5   = 13,
	AD_RANGE_5     = 14,
	AD_RANGE_10    = 15
}AD_RANGE_E;/*10=0-2.5V,11=0-5V,12=0-10V,13=2.5V,14=5V,15=10V*/

/* ���������� */
typedef enum
{
	AD_OS_NO  = 0,
	AD_OS_X2  = 1,
	AD_OS_X4  = 2,
	AD_OS_X8  = 3,
	AD_OS_X16 = 4,
	AD_OS_X32 = 5,
	AD_OS_X64 = 6
}AD7606_OS_E;

typedef struct
{
	uint8_t    ucOS;			/* ���������ʣ�0-6��0��ʾ�޹����� */
	AD_RANGE_E ucRange;		    /* ��������*/
	uint16_t   sNowAdc[32];		/* ��ǰADCֵ���޷�����*/
}AD7606_VAR_T;

/* Exported functions ------------------------------------------------------- */
void InitAD7606(void);
void AD7606_ReadNowAdc(void);
void AD7606_StartConvst(void);
void Stop_AD7606(void);
extern AD7606_VAR_T  g_tAD7606;
#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

#endif

#endif /*_AD7606_H*/

