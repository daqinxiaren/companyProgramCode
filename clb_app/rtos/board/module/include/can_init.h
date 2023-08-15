/**
 *
 * Copyright (c) 2021-2026  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file         		: Header file for the can_init.h API or library.
 *
 * @features			:
 *
 * @author       		: kernel <kernel_sa@163.com> 
 * 
 * @version      		: V 1.0.0
 *
 * @date         		: Mar 17, 2021
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
 * Mar 17, 2021		  kernel       create this file
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
 
#ifndef MODULE_INCLUDE_CAN_INIT_H_
#define MODULE_INCLUDE_CAN_INIT_H_

/**
 ***************** include file ***************
 */

//#ifdef  XXX    /**< XXX module api major */


/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/

/**
 * @formatter:off 
 ****************<Public macro>****************
 */

#define CAN_R_BITS 			0x03
#define CAN_LEN 				0x08
#define CAN_AND 				0x07
#define BOARD_NUMB 			0x20

#define CAN_RX_SIZE			512	//recv_buf 接收缓存
#define QZQ_TOTAL           4
/**
 * xxx  macro define	
 *                    - description 
 * @param  	xxx      :- description.
 */
//#define desc_class(xxx, xxxx) 									"\"
//		  for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/**
 *  @formatter:on 
 ***************<Public typedef>***************
 */
typedef union can_id
{
  struct{

    rt_uint32_t souc_id  :6;
    rt_uint32_t type     :2;
    rt_uint32_t tag_id   :6;
    rt_uint32_t ack      :2;
    rt_uint32_t cnt      :6;
    rt_uint32_t reserve  :4;
    rt_uint32_t priority :3;
    rt_uint32_t nc       :3;

  }bits;

	rt_uint32_t word;

}can_id_t,* can_id_p;

typedef struct can_deal
{
	 uint16_t count;
	 uint8_t *buffer;

}can_deal_t, * can_deal_p;

typedef struct all_can_deal
{
	can_deal_t proximitor1;
	can_deal_t proximitor2;
	can_deal_t proximitor3;
	can_deal_t proximitor4;
}all_can_deal_t,* all_can_deal_p;

typedef struct
{
	uint8_t qzq_id[QZQ_TOTAL];
	can_deal_t proximitor[QZQ_TOTAL];
}qzqid_deal_t,* qzqid_deal_p;//用于大于4的前置器ID修改ID

enum frame_type
{
	SINGLE,
	MULTI_BEGIN,
	MULTI_MID,
	MULTI_END
};

enum can_dev
{
  CAN_12,
  CAN_1,
  CAN_2
};

enum board_id
{
	radio_board = 0x00,
	proximitor1_board = 0x01,
	proximitor2_board = 0x02,
	proximitor3_board = 0x03,
	proximitor4_board = 0x04,
	process_board = 0x20
};

/**
 **************<Public variables>**************
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup group name 
 */

/**@{*/


/**@}*/
/**
 **************<inline functions>**************
 */

/**
 * xxx  functions define	
 *                    - description 
 * @param  	xxx      :- description.
 */


/**
 **************<Public functions>**************
 */

 /**
  * @brief can receive thread initialization
  * *
  * @author  www  write time: 2020年6月8日
  * @create time  : 2020年6月8日
  * *
  * Parameters
  * *
  * *
  * @return
  *          - xxx   description or none
  *          - xxx   description ,the operation status or on successful
  * @retval
  *          - xxx   description
  * *
  * @see  reference @file
  *
  * @note            init CAN/CAN2
  *
  * @warning
  *
  * Change Logs:
  * Date             author          notes
  * 2020年6月8日            www         create this class
  *
  */
int can_rx_thread_init();

 /**
  * @brief can send data
  * *
  * @author  www  write time: 2020年6月8日
  * @create time  : 2020年6月8日
  * *
  * Parameters
  * *
 * @uint8_t *data   send data address
 * @uint32_t len    send data length
 * @uint32_t len    can device flag
  * *
  * @return
  *          - xxx   description or none
  *          - xxx   description ,the operation status or on successful
  * @retval
  *          - xxx   description
  * *
  * @see  reference @file
  *
  * @note             CAN/CAN2 发送处理
  *
  * @warning
  *
  * Change Logs:
  * Date             author          notes
  * 2020年6月8日            www         create this class
  *
  */

void can_send_data(uint8_t *data, uint32_t len,enum can_dev candev_t);

 /**
  * @brief can_dev_reset
  * *
  * @author  www  write time: 2020年6月8日
  * @create time  : 2020年6月8日
  * *
  * Parameters
  * *
 * @uint8_t *data   send data address
 * @uint32_t len    send data length
 * @uint32_t len    can device flag
  * *
  * @return
  *          - xxx   description or none
  *          - xxx   description ,the operation status or on successful
  * @retval
  *          - xxx   description
  * *
  * @see  reference @file
  *
  * @note             CAN/CAN2 发送处理
  *
  * @warning
  *
  * Change Logs:
  * Date             author          notes
  * 2020年6月8日            www         create this class
  *
  */

int can_dev_reset(void);

/** @} */ //\defgroup module API
/** @} */ //\addtogroup dev module

#ifdef __cplusplus
}
#endif  /* end of __cplusplus */

//#endif  /**< end XXX module api major */

#endif /* MODULE_INCLUDE_CAN_INIT_H_ */

