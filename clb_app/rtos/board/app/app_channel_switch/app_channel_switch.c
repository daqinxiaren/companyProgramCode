/**
 * @file
 * @details
 * @author
 * @date    振动/冲击采样通道切换
 * @version
**/

/* include */
#include <drv_gpio.h>
#include "app_channel_switch.h"

/* type declaration */

/* variable declaration */
 app_select_pin_t adg1608_select_pin = {0};
 app_select_num_t adg1608_select_num = {0};
#define ch_pin adg1608_select_pin
#define ch_num adg1608_select_num

/* functions declaration */
 /**
  * @brief
  * @param
  * @return
  * @note
 **/
 app_select_num_p app_channel_adg1608_get_select(void)
 {
	 return &ch_num;
 }
 /**
  * @brief
  * @param
  * @return
  * @note
 **/
 void app_channel_pin_init(void)
 {
	 ch_pin.adg1608_axle_box.adg1608_EN = GET_PIN(B,14);
	 ch_pin.adg1608_axle_box.adg1608_A0 = GET_PIN(B,15);
	 ch_pin.adg1608_axle_box.adg1608_A1 = GET_PIN(B,1);
	 ch_pin.adg1608_axle_box.adg1608_A2 = GET_PIN(A,3);

	 ch_pin.adg1608_motor_gear.adg1608_EN = GET_PIN(E,5);
	 ch_pin.adg1608_motor_gear.adg1608_A0 = GET_PIN(E,6);
	 ch_pin.adg1608_motor_gear.adg1608_A1 = GET_PIN(E,2);
	 ch_pin.adg1608_motor_gear.adg1608_A2 = GET_PIN(C,13);

//	 ch_pin.adg1609_wave.adg1609_EN = GET_PIN(B,9);
//	 ch_pin.adg1609_wave.adg1609_A0 = GET_PIN(B,8);
//	 ch_pin.adg1609_wave.adg1609_A1 = GET_PIN(F,6);
}
 /**
  * @brief
  * @param
  * @return
  * @note
 **/
 void app_adg1608_set_output(adg1608_pin_p pin)
 {
   OS_ASSERT(pin != OS_NULL);

 	rt_pin_mode(pin->adg1608_EN, PIN_MODE_OUTPUT);
 	rt_pin_mode(pin->adg1608_A0, PIN_MODE_OUTPUT);
 	rt_pin_mode(pin->adg1608_A1, PIN_MODE_OUTPUT);
 	rt_pin_mode(pin->adg1608_A2, PIN_MODE_OUTPUT);
 }
 /**
  * @brief
  * @param
  * @return
  * @note
 **/
 void app_adg1609_set_output(adg1609_pin_p pin)
 {
   OS_ASSERT(pin != OS_NULL);

 	rt_pin_mode(pin->adg1609_EN, PIN_MODE_OUTPUT);
 	rt_pin_mode(pin->adg1609_A0, PIN_MODE_OUTPUT);
 	rt_pin_mode(pin->adg1609_A1, PIN_MODE_OUTPUT);
 }

 /**
  * @brief
  * @param
  * @return
  * @note
 **/
 void app_adg1608_channel_set(adg1608_pin_p pin, app_adg1608_channel_table_e channel_num)
 {
     OS_ASSERT(pin != OS_NULL);

     if((channel_none==channel_num) || (channel_num>channel_8) || (channel_num<channel_none))
     {
         rt_pin_write(pin->adg1608_EN,PIN_LOW);
      	rt_kprintf("%s line:%d adg1608 chanel disable!\n", __FILE__, __LINE__ );
     }
     else
     {
       rt_pin_write(pin->adg1608_EN,PIN_HIGH);
       //设置片选输出
       switch(channel_num)
         {
          case channel_1:
             rt_pin_write(pin->adg1608_A2,PIN_LOW);
             rt_pin_write(pin->adg1608_A1,PIN_LOW);
             rt_pin_write(pin->adg1608_A0,PIN_LOW);
             break;

          case channel_2:
             rt_pin_write(pin->adg1608_A2,PIN_LOW);
             rt_pin_write(pin->adg1608_A1,PIN_LOW);
             rt_pin_write(pin->adg1608_A0,PIN_HIGH);
             break;

          case channel_3:
             rt_pin_write(pin->adg1608_A2,PIN_LOW);
             rt_pin_write(pin->adg1608_A1,PIN_HIGH);
             rt_pin_write(pin->adg1608_A0,PIN_LOW);
             break;

          case channel_4:
             rt_pin_write(pin->adg1608_A2,PIN_LOW);
             rt_pin_write(pin->adg1608_A1,PIN_HIGH);
             rt_pin_write(pin->adg1608_A0,PIN_HIGH);
             break;

          case channel_5:
             rt_pin_write(pin->adg1608_A2,PIN_HIGH);
             rt_pin_write(pin->adg1608_A1,PIN_LOW);
             rt_pin_write(pin->adg1608_A0,PIN_LOW);
             break;

          case channel_6:
             rt_pin_write(pin->adg1608_A2,PIN_HIGH);
             rt_pin_write(pin->adg1608_A1,PIN_LOW);
             rt_pin_write(pin->adg1608_A0,PIN_HIGH);
             break;

          case channel_7:
             rt_pin_write(pin->adg1608_A2,PIN_HIGH);
             rt_pin_write(pin->adg1608_A1,PIN_HIGH);
             rt_pin_write(pin->adg1608_A0,PIN_LOW);
             break;

          case channel_8:
             rt_pin_write(pin->adg1608_A2,PIN_HIGH);
             rt_pin_write(pin->adg1608_A1,PIN_HIGH);
             rt_pin_write(pin->adg1608_A0,PIN_HIGH);
             break;

          default:
             break;
         }
     }
 }

 /**
  * @brief
  * @param
  * @return
  * @note
 **/
 void app_adg1609_channel_set(adg1609_pin_p pin, app_adg1608_channel_table_e channel_num)
 {
     OS_ASSERT(pin != OS_NULL);

     if((channel_none==channel_num) || (channel_num>channel_4) || (channel_num<channel_none))
     {
         rt_pin_write(pin->adg1609_EN,PIN_LOW);
      	rt_kprintf("%s line:%d adg1609 chanel disable!\n", __FILE__, __LINE__ );
     }
     else
     {
       rt_pin_write(pin->adg1609_EN,PIN_HIGH);
       //设置片选输出
       switch(channel_num)
         {
          case channel_1:
             rt_pin_write(pin->adg1609_A1,PIN_LOW);
             rt_pin_write(pin->adg1609_A0,PIN_LOW);
             break;

          case channel_2:
             rt_pin_write(pin->adg1609_A1,PIN_LOW);
             rt_pin_write(pin->adg1609_A0,PIN_HIGH);
             break;

          case channel_3:
             rt_pin_write(pin->adg1609_A1,PIN_HIGH);
             rt_pin_write(pin->adg1609_A0,PIN_LOW);
             break;

          case channel_4:
             rt_pin_write(pin->adg1609_A1,PIN_HIGH);
             rt_pin_write(pin->adg1609_A0,PIN_HIGH);
             break;

          default:
             break;
         }
     }
 }
 /**
  * @brief  轴箱模拟通道设置
  * @param
  * @return
  * @note
 **/
 void app_channel_axle_box_set(app_select_pin_p pin, app_select_num_p num)
 {
	 OS_ASSERT(pin != OS_NULL);
	 OS_ASSERT(num != OS_NULL);

	 app_adg1608_channel_set(&pin->adg1608_axle_box, num->set_axle_box_num);
    //rt_kprintf("%s line:%d app_channel_axle_box_set ch select is: %d\n", __FILE__, __LINE__, num->set_axle_box_num );
 }
 /**
  * @brief  电机/齿轮模拟通道设置
  * @param
  * @return
  * @note
 **/
 void app_channel_motor_gear_set(app_select_pin_p pin, app_select_num_p num)
 {
	 OS_ASSERT(pin != OS_NULL);
	 OS_ASSERT(num != OS_NULL);

	 app_adg1608_channel_set(&pin->adg1608_motor_gear, num->set_motor_gear_num);
 }
 /**
  * @brief  多边形，波磨专用通道，采集轴箱振动数据
  * @param
  * @return
  * @note
 **/
 void app_channel_wave_set(app_select_pin_p pin, app_select_num_p num)
 {
	 OS_ASSERT(pin != OS_NULL);
	 OS_ASSERT(num != OS_NULL);

	 app_adg1609_channel_set(&pin->adg1609_wave, num->set_wave_num);
 }
/**
 * @brief
 * @param
 * @return
 * @note
**/
 void app_channel_adg1608_init(void)
 {
	app_channel_pin_init();
	app_adg1608_set_output(&ch_pin.adg1608_axle_box);
 	app_adg1608_set_output(&ch_pin.adg1608_motor_gear);
 	app_adg1609_set_output(&ch_pin.adg1609_wave);

 	//初始化设置采集通道为 通道1
 	ch_num.set_axle_box_num = channel_1;
 	ch_num.set_motor_gear_num = channel_1;
 	ch_num.set_wave_num = channel_1;
    app_channel_axle_box_set(&ch_pin, &ch_num);
	app_channel_motor_gear_set(&ch_pin, &ch_num);
//	app_channel_wave_set(&ch_pin, &ch_num);//因算力不足，波磨通道不适用
 }
/**
 * @brief
 * @param
 * @return
 * @note
**/
 void app_channel_adg1608_switch_all(void)
 {
	 int set_channel_num = 0;
	 ch_num.save_axle_box_num = ch_num.set_axle_box_num;
	 ch_num.save_motor_gear_num = ch_num.set_motor_gear_num;

	 set_channel_num = ch_num.save_axle_box_num;
	 set_channel_num ++;
	 if(set_channel_num >= channel_max_out)
	 {
		 set_channel_num = channel_1;
	 }
	 ch_num.set_axle_box_num = set_channel_num;
	 ch_num.set_motor_gear_num = set_channel_num;
//	 if(set_channel_num>channel_4)//采集波磨数据的通道，因还算力不够，不适用
//	     ch_num.set_wave_num = set_channel_num - 4;
//	 else
//		 ch_num.set_wave_num = set_channel_num;

	 app_channel_axle_box_set(&ch_pin, &ch_num);
	 app_channel_motor_gear_set(&ch_pin, &ch_num);
//	 app_channel_wave_set(&ch_pin, &ch_num); //算力有限，不采集波磨
 }


