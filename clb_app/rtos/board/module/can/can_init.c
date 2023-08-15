/**
 *
 * Copyright (c) 2015-2020  xxxx  Development 
 * All rights reserved.
 * *
 * This file is part of the <application> or operating system.
 * *
 *
 * @file                : file for the ad7606_app1.c API or library.
 *
 * @features            :
 *
 * @author              : www
 * 
 * @version             : V 1.0.0
 *
 * @date                : 2020年6月8日
 *
 * @environment         : null
 *
 * @description         : null
 *
 * @module              : ad
 *
 * \n <b>@note</b>      : null
 *
 * \b <b> @warning </b> : null
 * 
 * *
 * Change Logs:
 *  Date                author       notes
 *  2020年6月8日         www         create this file
 *  2020年6月8日
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
 */
#include <drv_config.h>
#if defined(ENABLE_CAN1) || defined(ENABLE_CAN2)

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <drv_can.h>
#include "can_init.h"
#include "can_app.h"

/**
 * xxx note delete
 * sub group
 *
 * @addtogroup group name
 */

/**@{*/
/**@}*/

/** @formatter:off */
/**
 ***************<Private macro>****************
 */

/**
 * @brief macro define  - description
 * @param   param   - description.
 * @param   param   - description.
 */
//#define desc_class(xxx, xxxx)
//        for (xxx = (xxxx)->next; xxx != (xxxx); xxx = xxx->next)


/** @formatter:on */

/**
 ***************<Private define>****************
 */

#if defined(ENABLE_CAN1)
#define CAN1_DEV_NAME    "can1"      /* CAN1 设备名称 */
#endif

#if defined(ENABLE_CAN2)
#define CAN2_DEV_NAME    "can2"      /* CAN2 设备名称 */
#endif

/**
 **************<Private variables>**************
 */
#if defined(ENABLE_CAN1)
static struct rt_semaphore can1_rx_sem; /* 用于接收消息的信号量 */
static rt_device_t can1_dev; /* CAN 设备句柄 */
rt_thread_t can1_thread;
#endif

#if defined(ENABLE_CAN2)
static struct rt_semaphore can2_rx_sem; /* 用于接收消息的信号量 */
static rt_device_t can2_dev; /* CAN 设备句柄 */
rt_thread_t can2_thread;
#endif

all_can_deal_t can_deal; /* CAN 接收数据处理成员变量 */
qzqid_deal_t qzq_deal;

/**
 **************<Private functions>**************
 */
static void protocol_deal(can_id_t can_id, struct rt_can_msg *reframe);
/**
 * @addtogroup group name
 */

/**@{*/

/**@}*/

/**
 * @brief can deal data initialization
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
/*init_can_data*/
static void can_deal_data_init(void)
{
	can_deal.proximitor1.count = 0;
	can_deal.proximitor1.buffer = rt_malloc(CAN_RX_SIZE);
	if (can_deal.proximitor1.buffer == OS_NULL)
	{
		rt_kprintf("proximitor1.buffer malloc failed\n");
	}
	
	can_deal.proximitor2.count = 0;
	can_deal.proximitor2.buffer = rt_malloc(CAN_RX_SIZE);
	if (can_deal.proximitor2.buffer == OS_NULL)
	{
		rt_kprintf("proximitor1.buffer malloc failed\n");
	}
	
	can_deal.proximitor3.count = 0;
	can_deal.proximitor3.buffer = rt_malloc(CAN_RX_SIZE);
	if (can_deal.proximitor3.buffer == OS_NULL)
	{
		rt_kprintf("proximitor1.buffer malloc failed\n");
	}
	
	can_deal.proximitor4.count = 0;
	can_deal.proximitor4.buffer = rt_malloc(CAN_RX_SIZE);
	if (can_deal.proximitor4.buffer == OS_NULL)
	{
		rt_kprintf("proximitor4.buffer malloc failed\n");
	}
	for(int i=0; i<QZQ_TOTAL; i++)
	{
		qzq_deal.qzq_id[i] = 0;
		qzq_deal.proximitor[i].count = 0;
		qzq_deal.proximitor[i].buffer = rt_malloc(CAN_RX_SIZE);//仅用于前置器id非1-4时接收前置器ID用
		if (qzq_deal.proximitor[i].buffer == OS_NULL)
		{
			rt_kprintf("proximitor.buffer malloc failed\n");
		}
	}
}

/* 接收数据回调函数 */
#if defined(ENABLE_CAN1)
static rt_err_t can1_rx_call(rt_device_t dev, rt_size_t size)
{
	/* CAN 接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
	rt_sem_release(&can1_rx_sem);
	
	return OS_EOK;
}
#endif

#if defined(ENABLE_CAN2)
static rt_err_t can2_rx_call(rt_device_t dev, rt_size_t size)
{
	/* CAN 接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
	rt_sem_release(&can2_rx_sem);
	
	return OS_EOK;
}
#endif

/* can1 recv 处理线程 */
#if defined(ENABLE_CAN1)
static void can1_rx_thread(void *parameter)
{
	
	struct rt_can_msg rxmsg =
	{
	        0 };
	can_id_t can_rx_id;
	
	/* 设置接收回调函数 */
	rt_device_set_rx_indicate(can1_dev, can1_rx_call);
	rt_kprintf("can1_dev init !\n");
	
#ifdef RT_CAN_USING_HDR

    struct rt_can_filter_item items ;
    items.id=0x10000000;
    items.ide=1;
    items.rtr=0;
    items.mode=1;
    items.mask=0x10000000;
    items.hdr=-1;
    struct rt_can_filter_config cfg = {1, 1, &items}; /* 一共有 5 个过滤表 */
    /* 设置硬件过滤表 */
    res = rt_device_control(can_dev, DEV_CAN_CMD_SET_FILTER, &cfg);
    RT_ASSERT(res == RT_EOK);

#endif
	
	while (1)
	{
		/* hdr 值为 - 1，表示直接从 uselist 链表读取数据 */
		rxmsg.hdr = -1;
		/* 阻塞等待接收信号量 */
		rt_sem_take(&can1_rx_sem, WAITING_FOREVER);
		/* 从 CAN 读取一帧数据 */
		uint32_t res = rt_device_read(can1_dev, 0, &rxmsg, sizeof(rxmsg));
		
		can_rx_id.word = rxmsg.id;
		
//		rt_kprintf("can1_dev recv id: %x  len: %x\n", rxmsg.id,rxmsg.len);
//		for (int j = 0; j < rxmsg.len; j++)
//		{
//			rt_kprintf("%x ", rxmsg.data [j]);
//		}
//		rt_kprintf("\n");
		
		if (res > 0 && (can_rx_id.bits.tag_id == BOARD_NUMB))
		{
			protocol_deal(can_rx_id, &rxmsg);
		}
	}
}
#endif

#if defined(ENABLE_CAN2)
static void can2_rx_thread(void *parameter)
{
	rt_err_t res;
	struct rt_can_msg rxmsg =
	{
	        0 };
	can_id_t can2_rx_id;
	
	/* 设置接收回调函数 */
	rt_device_set_rx_indicate(can2_dev, can2_rx_call);
	rt_kprintf("can2_dev init!\n");
	
#ifdef RT_CAN_USING_HDR
    struct rt_can_filter_item items ;
    items.id=0x10000000;
    items.ide=1;
    items.rtr=0;
    items.mode=1;
    items.mask=0x10000000;
    items.hdr=-1;
    struct rt_can_filter_config cfg = {1, 1, &items}; /* 一共有 5 个过滤表 */
    /* 设置硬件过滤表 */
    res = rt_device_control(can2_dev, DEV_CAN_CMD_SET_FILTER, &cfg);
    RT_ASSERT(res == RT_EOK);
#endif
	
	while (1)
	{
		/* hdr 值为 - 1，表示直接从 uselist 链表读取数据 */
		rxmsg.hdr = -1;
		/* 阻塞等待接收信号量 */
		rt_sem_take(&can2_rx_sem, WAITING_FOREVER);
		/* 从 CAN 读取一帧数据 */
		res = rt_device_read(can2_dev, 0, &rxmsg, sizeof(rxmsg));
		
		can2_rx_id.word = rxmsg.id;
		
//		rt_kprintf("can2_dev recv id:%x \n", rxmsg.id);
//		for (int j = 0; j < rxmsg.len; j++)
//		{
//			rt_kprintf("%x ", rxmsg.data [j]);
//		}
//		rt_kprintf("\n");
		
		if (res > 0 && (can2_rx_id.bits.tag_id == BOARD_NUMB))
		{
			protocol_deal(can2_rx_id, &rxmsg);
		}
	}
}
#endif

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
int can_rx_thread_init()
{
	rt_err_t res;
	
	/*初始化接收参数 */
	can_deal_data_init();
	
	/* 查找 CAN1 设备 */
#if defined(ENABLE_CAN1)
	can1_dev = rt_device_find(CAN1_DEV_NAME);
	if (!can1_dev)
	{
		rt_kprintf("find %s failed!\n", CAN1_DEV_NAME);
		return OS_ERROR;
	}
	/* 初始化 CAN 接收信号量 */
	rt_sem_init(&can1_rx_sem, "can1_rx_sem", 0, IPC_FLAG_FIFO);
	
	/* 以中断接收及发送方式打开 CAN 设备 */
	res = rt_device_open(can1_dev, DEVICE_OFLAG_RDWR | DEVICE_FLAG_INT_TX | DEVICE_FLAG_INT_RX);
	OS_ASSERT(res == OS_EOK);
	
	/* 设置 CAN 的工作模式 */
	res = rt_device_control(can1_dev, DEV_CAN_CMD_SET_BAUD, (void*) CAN500kBaud);
	res = rt_device_control(can1_dev, DEV_CAN_CMD_SET_MODE, (void*) DEV_CAN_MODE_NORMAL);
	
	/* 创建数据接收线程 */
	can1_thread = rt_thread_create("can1_rx", can1_rx_thread, OS_NULL, 3096, 22, 30);
	if (can1_thread != OS_NULL)
	{
		rt_thread_startup(can1_thread);
	}
	else
	{
		rt_kprintf("create can_rx thread failed!\n");
	}
	
#endif
	/* 查找 CAN2 设备 */
#if defined(ENABLE_CAN2)
	
	can2_dev = rt_device_find(CAN2_DEV_NAME);
	if (!can2_dev)
	{
		rt_kprintf("find %s failed!\n", CAN2_DEV_NAME);
		return OS_ERROR;
	}
	/* 初始化 CAN 接收信号量 */
	rt_sem_init(&can2_rx_sem, "can2_rx_sem", 0, IPC_FLAG_FIFO);
	
	/* 以中断接收及发送方式打开 CAN 设备 */
	res = rt_device_open(can2_dev, DEVICE_OFLAG_RDWR | DEVICE_FLAG_INT_TX | DEVICE_FLAG_INT_RX);
	OS_ASSERT(res == OS_EOK);
	
	/* 设置 CAN 的工作模式 */
	res = rt_device_control(can2_dev, DEV_CAN_CMD_SET_BAUD, (void*) CAN500kBaud);
	res = rt_device_control(can2_dev, DEV_CAN_CMD_SET_MODE, (void*) DEV_CAN_MODE_NORMAL);
	
	/* 创建数据接收线程 */
	can2_thread = rt_thread_create("can2_rx", can2_rx_thread, OS_NULL, 3096, 22, 30);
	if (can2_thread != OS_NULL)
	{
		rt_thread_startup(can2_thread);
	}
	else
	{
		rt_kprintf("create can2_rx thread failed!\n");
	}
#endif
	
	return 0;
}

int can_dev_reset(void)
{
	rt_err_t res;
	/* 查找 CAN1 设备 */
#if defined(ENABLE_CAN1)
	//step1 close can dev
	if(can1_dev)
	{
		rt_device_close(can1_dev);
		can1_dev=NULL;
		rt_kprintf("close %s dev!\n", CAN1_DEV_NAME);
	}
	can1_dev = rt_device_find(CAN1_DEV_NAME);
	if (!can1_dev)
	{
		rt_kprintf("find %s failed!\n", CAN1_DEV_NAME);
		return OS_ERROR;
	}
#endif

	/* 查找 CAN2 设备 */
#if defined(ENABLE_CAN2)
	//step1 close can dev
	if(can2_dev)
	{
		rt_device_close(can2_dev);
		can2_dev=NULL;
		rt_kprintf("close %s dev!\n", CAN2_DEV_NAME);
	}

	can2_dev = rt_device_find(CAN2_DEV_NAME);
	if (!can2_dev)
	{
		rt_kprintf("find %s failed!\n", CAN2_DEV_NAME);
		return OS_ERROR;
	}
#endif

	rt_thread_mdelay(1000);
#if defined(ENABLE_CAN1)
	/* 以中断接收及发送方式打开 CAN 设备 */
	res = rt_device_open(can1_dev, DEVICE_OFLAG_RDWR | DEVICE_FLAG_INT_TX | DEVICE_FLAG_INT_RX);
	OS_ASSERT(res == OS_EOK);

	/* 设置 CAN 的工作模式 */
	res = rt_device_control(can1_dev, DEV_CAN_CMD_SET_BAUD, (void*) CAN500kBaud);
	res = rt_device_control(can1_dev, DEV_CAN_CMD_SET_MODE, (void*) DEV_CAN_MODE_NORMAL);
	rt_kprintf("%s dev reset!\n", CAN1_DEV_NAME);
#endif

#if defined(ENABLE_CAN2)
	/* 以中断接收及发送方式打开 CAN 设备 */
	res = rt_device_open(can2_dev, DEVICE_OFLAG_RDWR | DEVICE_FLAG_INT_TX | DEVICE_FLAG_INT_RX);
	OS_ASSERT(res == OS_EOK);

	/* 设置 CAN 的工作模式 */
	res = rt_device_control(can2_dev, DEV_CAN_CMD_SET_BAUD, (void*) CAN500kBaud);
	res = rt_device_control(can2_dev, DEV_CAN_CMD_SET_MODE, (void*) DEV_CAN_MODE_NORMAL);
	rt_kprintf("%s dev reset!\n", CAN2_DEV_NAME);

#endif

	return 0;
}

MSH_CMD_EXPORT(can_dev_reset, can dev reset );


/**
 * @brief development description of class
 * *
 * @author  www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @param               param   new node to be inserted
 * @param [ in or out ] param   new node to be inserted
 * *
 * @return
 *          - xxx   description or none
 *          - xxx   description ,the operation status or on successful
 * @retval
 *          - xxx   description
 * *
 * @see  reference @file
 *
 * @note            CAN/CAN2 数据接收处理
 *
 * @warning
 *
 * Change Logs:
 * Date             author          notes
 * 2020年6月8日            www         create this class
 *
 */
/*protocol_deal 根据CAN_ID_t 数据组包*/
static void protocol_deal(can_id_t can_id, struct rt_can_msg *reframe)
{
	
	extern void rx_deal(uint8_t *data, uint32_t len);
	switch (can_id.bits.souc_id)
	{
		
		case proximitor1_board:

			if (can_deal.proximitor1.buffer == OS_NULL)
				return;
			
			if (can_id.bits.type == SINGLE)
			{
				rt_memcpy(can_deal.proximitor1.buffer, reframe->data, reframe->len);
				rx_deal(can_deal.proximitor1.buffer, reframe->len);
			}
			else
			{
				
				if (can_id.bits.type == MULTI_BEGIN)
				{
					can_deal.proximitor1.count = 0;
					
					rt_memcpy(&can_deal.proximitor1.buffer [can_deal.proximitor1.count], reframe->data, reframe->len);
					can_deal.proximitor1.count += reframe->len;
					
				}
				else
				{
					rt_memcpy(&can_deal.proximitor1.buffer [can_deal.proximitor1.count], reframe->data, reframe->len);
					can_deal.proximitor1.count += reframe->len;
					
				}
				
				if (can_id.bits.type == MULTI_END)
				{
					rx_deal(can_deal.proximitor1.buffer, can_deal.proximitor1.count);
					can_deal.proximitor1.count = 0;
				}
			}
			break;
			
		case proximitor2_board:

			if (can_deal.proximitor2.buffer == OS_NULL)
				return;
			
			if (can_id.bits.type == SINGLE)
			{
				rt_memcpy(can_deal.proximitor2.buffer, reframe->data, reframe->len);
				rx_deal(can_deal.proximitor2.buffer, reframe->len);
			}
			else
			{
				
				if (can_id.bits.type == MULTI_BEGIN)
				{
					can_deal.proximitor2.count = 0;
					
					rt_memcpy(&can_deal.proximitor2.buffer [can_deal.proximitor2.count], reframe->data, reframe->len);
					can_deal.proximitor2.count += reframe->len;
					
				}
				else
				{
					rt_memcpy(&can_deal.proximitor2.buffer [can_deal.proximitor2.count], reframe->data, reframe->len);
					can_deal.proximitor2.count += reframe->len;
					
				}
				
				if (can_id.bits.type == MULTI_END)
				{
					rx_deal(can_deal.proximitor2.buffer, can_deal.proximitor2.count);
					can_deal.proximitor2.count = 0;
				}
			}
			
			break;
			
		case proximitor3_board:

			if (can_deal.proximitor3.buffer == OS_NULL)
				return;
			
			if (can_id.bits.type == SINGLE)
			{
				rt_memcpy(can_deal.proximitor3.buffer, reframe->data, reframe->len);
				rx_deal(can_deal.proximitor3.buffer, reframe->len);
			}
			else
			{
				
				if (can_id.bits.type == MULTI_BEGIN)
				{
					can_deal.proximitor3.count = 0;
					rt_memcpy(&can_deal.proximitor3.buffer [can_deal.proximitor3.count], reframe->data, reframe->len);
					can_deal.proximitor3.count += reframe->len;
				}
				else
				{
					rt_memcpy(&can_deal.proximitor3.buffer [can_deal.proximitor3.count], reframe->data, reframe->len);
					can_deal.proximitor3.count += reframe->len;
				}
				
				if (can_id.bits.type == MULTI_END)
				{
					rx_deal(can_deal.proximitor3.buffer, can_deal.proximitor3.count);
					can_deal.proximitor3.count = 0;
				}
			}
			
			break;
			
		case proximitor4_board:

			if (can_deal.proximitor4.buffer == OS_NULL)
				return;
			
			if (can_id.bits.type == SINGLE)
			{
				rt_memcpy(can_deal.proximitor4.buffer, reframe->data, reframe->len);
				rx_deal(can_deal.proximitor4.buffer, reframe->len);
			}
			else
			{
				
				if (can_id.bits.type == MULTI_BEGIN)
				{
					can_deal.proximitor4.count = 0;
					rt_memcpy(&can_deal.proximitor4.buffer [can_deal.proximitor4.count], reframe->data, reframe->len);
					can_deal.proximitor4.count += reframe->len;
				}
				else
				{
					rt_memcpy(&can_deal.proximitor4.buffer [can_deal.proximitor4.count], reframe->data, reframe->len);
					can_deal.proximitor4.count += reframe->len;
				}
				
				if (can_id.bits.type == MULTI_END)
				{
					rx_deal(can_deal.proximitor4.buffer, can_deal.proximitor4.count);
					can_deal.proximitor4.count = 0;
				}
			}
			
			break;
			
		default:
		{
			can_deal_t* pro_p;
			int i = 0;
//			if(GET_ID_CMD!=pro_p->buffer[4] && SET_ID_CMD!=pro_p->buffer[4])  break;

			for (i=0; i<QZQ_TOTAL; i++)
			{
//				rt_kprintf("qzq_deal.qzq_id[%d] = %d\n",i,qzq_deal.qzq_id[i]);
				if (qzq_deal.qzq_id[i]==can_id.bits.souc_id)
				{
					pro_p = &qzq_deal.proximitor[i];
					break;
				}
			}
			if (QZQ_TOTAL == i)//没有查找到对应的前置器id
			{
				for (i=0; i<QZQ_TOTAL; i++)
				{
					if (0 == qzq_deal.qzq_id[i])
					{
						qzq_deal.qzq_id[i] = can_id.bits.souc_id;
						pro_p = &qzq_deal.proximitor[i];
						break;
					}
				}
			}
			if (can_id.bits.type == MULTI_BEGIN)
			{
				pro_p->count = 0;
				rt_memcpy(&pro_p->buffer[pro_p->count], reframe->data, reframe->len);
				pro_p->count += reframe->len;
			}
			else
			{
				rt_memcpy(&pro_p->buffer[pro_p->count], reframe->data, reframe->len);
				pro_p->count += reframe->len;
			}
			if (can_id.bits.type == MULTI_END)
			{
				qzq_deal.qzq_id[i] = 0;
				if(GET_ID_CMD==pro_p->buffer[4] || SET_ID_CMD==pro_p->buffer[4])
				{
					rx_deal(pro_p->buffer, pro_p->count);
					pro_p->count = 0;
				}
			}
			break;
		}
	}
	
}

/**
 * @brief can send data
 * *
 * @author  www  write time: 2020年6月8日
 * @create time  : 2020年6月8日
 * *
 * Parameters
 * *
 * @uint8_t *data   			send data address
 * @uint32_t len    			send data length
 * @enum can_dev candev_t	can device flag
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
/*can数据发送 按照CAN_ID_t 发送数据*/
void can_send_data(uint8_t *data, uint32_t len, enum can_dev candev_t)
{
	rt_base_t level;
	struct rt_can_msg msg =
	{
	        0 };
	can_id_t can_tx_id;
	rt_size_t size;
	rt_uint32_t send_group, send_remain, send_cnt, len_tmp;
	rt_uint32_t i, j, r_index = 0;
	rt_uint32_t multi_flag;
	
	static rt_uint32_t cnt_save = 0;
	
	if (data == OS_NULL || len == 0)
		return;
	
	can_tx_id.word = 0;
	can_tx_id.bits.souc_id = data [6];
	can_tx_id.bits.tag_id = data [5];
	can_tx_id.bits.ack = 0x0;
	can_tx_id.bits.reserve = 0x0;
	can_tx_id.bits.priority = 0x7;
	
	send_group = len >> CAN_R_BITS;
	send_remain = len & CAN_AND;
	
	if (send_group > 1 || (send_group == 1 && send_remain > 0))
	{
		multi_flag = OS_TRUE;
		send_cnt = send_group;
		if (send_remain > 0)
			send_cnt++;
	}
	else
	{
		send_cnt = 1;
		multi_flag = OS_FALSE;
	}
	
	for (i = 0; i < send_cnt; i++)
	{
		/* disable interrupt */
		level = rt_hw_interrupt_disable();
		if (multi_flag)
		{
			if (i == 0x0)
			{
				can_tx_id.bits.type = MULTI_BEGIN;
				len_tmp = CAN_LEN;
			}
			else if (i == send_cnt - 1)
			{
				can_tx_id.bits.type = MULTI_END;
				if (send_remain > 0)
					len_tmp = send_remain;
				else
					len_tmp = CAN_LEN;
			}
			else
			{
				can_tx_id.bits.type = MULTI_MID;
				len_tmp = CAN_LEN;
			}
		}
		else
		{
			can_tx_id.bits.type = SINGLE;
			if (send_group == 1)
				len_tmp = CAN_LEN;
			else
				len_tmp = send_remain;
		}
		
		can_tx_id.bits.cnt = cnt_save++;
		msg.id = can_tx_id.word; /* ID */
		msg.ide = DEV_CAN_EXTID;    // RT_CAN_EXTID;  /* 扩展帧格式 */
		msg.rtr = DEV_CAN_DTR; /* 数据帧 */
		msg.len = len_tmp; /* 数据长度为 8 */
		msg.hdr = -1;
		for (j = 0; j < len_tmp; j++)
		{
			msg.data [j] = data [r_index++];
			//rt_kprintf(" %d ",msg.data[j]);
		}
		//rt_kprintf(" \n");
		
		/* enable interrupt */
		rt_hw_interrupt_enable(level);
		
		/* 发送一帧 CAN 数据 */
#if defined(ENABLE_CAN1)
		if ((candev_t != CAN_2)&&can1_dev)
		{
			size = rt_device_write(can1_dev, 0, &msg, sizeof(msg));
			if (size == 0)
			{
				//rt_kprintf("can dev write data failed! id:%x\n",msg.id);
			}
			else
			{
				//rt_kprintf("can send size :%d \n",size);
			}
		}
#endif
		
#if defined(ENABLE_CAN2)
		if ((candev_t != CAN_1)&&can2_dev)
		{
			size = rt_device_write(can2_dev, 0, &msg, sizeof(msg));
			if (size == 0)
			{
				//rt_kprintf("can2 dev write data failed! id:%x\n",msg.id);
			}
			else
			{
				//rt_kprintf("can2 send size :%d \n",size);
			}
		}
#endif
		
	}
}

#endif //\end module API
