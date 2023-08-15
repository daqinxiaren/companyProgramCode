/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_TEMP_SAVE_H__
#define __APP_TEMP_SAVE_H__

/* includes */
#include "app_board_config.h"
#include "app_data_save.h"

/* macro */
#define APP_TEMP_SAVE_STACK       DEF_TEMP_SAVE_STACK
#define APP_TEMP_SAVE_PRO         DEF_TEMP_SAVE_PRO

/* type declaration */
typedef struct
{
	rt_thread_t   temp_thread;
}app_temp_save_env_t;

/* variable declaration */

/* function declaration */
void app_temp_save_thread_create(void);
void app_temp_save_thread_init(void);

#endif /* __APP_DATA_SAVE_H__ */


