/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/
 
#ifndef _APP_TFTP_UP_RAW_H_
#define _APP_TFTP_UP_RAW_H_

/* includes */
#include "app_board_config.h"
#include "app_data_save.h"

/* macro */
//#define REMOTE_UP_TO_PATH            "hdd/clb/raw"
#define PATH_HDD                     "hdd/train_"
#define PATH_WTS                     "hdd/wts"
#define PATH_RAW                     "clb/raw"
#define TFTP_UP_HEAD_IP              DEF_TFTP_HEAD_CONFIG_FILE_IP
#define TFTP_UP_TAIL_IP              DEF_TFTP_TAIL_CONFIG_FILE_IP

/* type declaration */
typedef struct
{
	rt_thread_t   tftp_thread;
	struct rt_semaphore* up_sem;
	save_file_link_p save_list_head;
}app_tftp_env_t;

/* variable declaration */

/* function declaration */
 void app_tftp_thread_create(void);
 void app_tftp_thread_init(void);


/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

//#endif  /**< end XXX module api major */

#endif /* _APP_TFTP_UP_RAW_H_ */

