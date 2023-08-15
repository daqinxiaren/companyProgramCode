/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_DATA_SAVE_H__
#define __APP_DATA_SAVE_H__

/* includes */
#include "app_board_config.h"
#include "txb_public_protocol.h"
#include "app_channel_switch.h"
#include "app_ad7606_data.h"

/* macro */
#define APP_DATA_SAVE_STACK       DEF_DATA_SAVE_STACK
#define APP_DATA_SAVE_PRO         DEF_DATA_SAVE_PRO

/* type declaration */
enum save_file_type
{
	save_temp               = 0x01,
	save_vib                = 0x02,
};

typedef struct save_file_link
{
		struct save_file_link * prev;
		struct save_file_link * next;
		char file_name[32];

}save_file_link_t, * save_file_link_p;

typedef struct save_info_link
{
		struct save_info_link * prev;
		struct save_info_link * next;
		uint8_t file_type;
		app_select_num_t vib_channel;
		uint8_t* data_buff;
}save_info_link_t, * save_info_link_p;

typedef struct
{
	rt_thread_t   save_thread;
	//rt_mutex_t save_mutex;
	struct rt_semaphore save_sem;
	save_file_link_t save_file_head;
	save_info_link_t save_info_head;
}app_data_save_env_t;

typedef struct {
	struct tm old;
	struct tm new;
}save_time_t, * save_time_p;

/* variable declaration */
extern app_data_save_env_t app_data_save_env;

/* function declaration */
void app_data_save_thread_init(void);
void app_data_save_thread_create(void);
save_file_link_p app_data_save_file_head_get(void);
//save_file_link_t save_list_create();
void app_save_public_info(uint8_t* public_data);
void app_save_get_public_buf(txb_MVB_public_t* public_data);
//rt_mutex_t app_data_save_get_mutex(void);
struct rt_semaphore* app_data_save_get_sem(void);
void app_data_save_vib(app_select_num_p channel, ad7606_data_t data_buff);
void app_data_save_temp(void);
//void add_file_to_unlink(save_file_link_p link_list_node);
void add_file_to_link(char* file_name);
uint16_t save_sum_check_u16(uint8_t *data, uint16_t crclen, uint16_t check);
void get_current_time(struct tm * time_buff);
FILE* app_create_save_file(char* file_name, struct tm file_time, uint8_t file_type);


#endif /* __APP_DATA_SAVE_H__ */

