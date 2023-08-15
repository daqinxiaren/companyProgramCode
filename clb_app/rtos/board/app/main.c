/**
 * @file
 * @details
 * @author
 * @date    tfdsz
 * @version
**/

/* include */
#include <rtthread.h>
#include <drv_config.h>
#include <rtdevice.h>
#include <drv_gpio.h>
#include "can_app.h"
#include "switch_app.h"
#include "ad7606_app.h"
#include "save.h"
#include "tftp_up_raw.h"
#include "../lh/itc/itc.h"
#include "sec.h"
#include "../app/include/fault_hand.h"
#include "app_multicast_thread.h"
#include "app_singlecast_thread.h"
#include "app_paras_save.h"
#include "app_paras_config.h"
#include "app_ad7606_data.h"
#include "app_data_save.h"
#include "app_cominform_config.h"
#include "app_led_control.h"
#include "app_iwdg.h"
#include "app_timer.h"
#include "app_self_test.h"
#include "app_pwm.h"
#include "app_channel_switch.h"
#include "app_temp_save.h"
#include "app_tftp_up_raw.h"
#include "app_diagnosis.h"

/**
 * @brief
 * @param
 * @return
 * @note   Main function
**/
int main(void)
{
	extern void netdev_set_if(char* netdev_name, char* ip_addr, char* gw_addr, char* nm_addr);
	extern void eth_reset_ip(int id,char *ip_addr);
	
	char ipaddr[16];

	//uint32_t count = 1;
	comm_para_t comm_para;
	//sys_dev_state_t* system_para = app_get_self_test_para();

	app_pwm_init(25000,12500);
	//itc_init();

	//led_init();
    //config_ini_file_init();//udp ip_addr

    app_ad7606_data_init();

	app_paras_save_init(&comm_para);

	app_paras_config_init();

	app_channel_adg1608_init();

	app_data_save_thread_init();

	app_temp_save_thread_init();

	app_singlecast_thread_init();

	app_multicast_thread_init();

	app_led_control_thread_init();

	app_self_test_init();

	app_diag_init();

	app_tftp_thread_init();

	rt_kprintf("\t-- THDS-Z7-CLB  update data: 2023-07-14 18:14  Version:1.0.1\n");
	rt_kprintf("ip:%s gateway:%s mask:%s\n", comm_para.local_ip, comm_para.local_gateway, comm_para.local_mask);
	
	eth_reset_ip(comm_para.train_id, ipaddr);

    netdev_set_if("e0", ipaddr, OS_LWIP_GWADDR, OS_LWIP_MSKADDR);



	fault_hand_data_init();

	rt_thread_mdelay(1);

	adg1608_init_all();

	app_timer_init(0,244);

	can_rx_thread_init();
	//system_para->reset_para = update_sys_para();
	app_iwdg_init(15);
//	sec_init();
	app_data_save_thread_create();
	app_singlecast_thread_create();
	app_multicast_thread_create();
	app_led_control_thread_create();
	app_temp_save_thread_create();
	app_diag_thread();
	app_tftp_thread_create();

   while (1)
    {
	  app_feed_dog();
//		rt_kprintf("\t-- test  Version:1.0.1\n");
      rt_thread_mdelay(5000);
    }

   return OS_EOK;
}
