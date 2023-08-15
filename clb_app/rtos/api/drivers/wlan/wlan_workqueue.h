/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-19     tyx          the first version
 */

#ifndef __WLAN_WORKQUEUE_H__
#define __WLAN_WORKQUEUE_H__

#include <ipc/workqueue.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WLAN_WORKQUEUE_THREAD_NAME
#define WLAN_WORKQUEUE_THREAD_NAME  ("wlan_job")
#endif

#ifndef WLAN_WORKQUEUE_THREAD_SIZE
#define WLAN_WORKQUEUE_THREAD_SIZE  (2048)
#endif

#ifndef WLAN_WORKQUEUE_THREAD_PRIO
#define WLAN_WORKQUEUE_THREAD_PRIO  (20)
#endif

int rt_wlan_workqueue_init(void);

rt_err_t rt_wlan_workqueue_dowork(void (*func)(void *parameter), void *parameter);

struct rt_workqueue *rt_wlan_get_workqueue(void);

#ifdef __cplusplus
}
#endif

#endif
