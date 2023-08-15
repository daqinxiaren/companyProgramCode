/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_IWDG_H__
#define __APP_IWDG_H__

/* include */
#include <stm32f4xx.h>
#include <rtthread.h>

/* macro */
/* STM32F407XGT6*/

/* type declaration */

/* function declaration */
/*初始化看门狗*/
int app_iwdg_init(rt_uint32_t timeout_s);
/*喂狗*/
void app_feed_dog(void);


#endif /*__APP_IWDG_H__*/


