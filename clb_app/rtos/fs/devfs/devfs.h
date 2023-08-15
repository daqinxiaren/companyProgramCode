/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#ifndef __DEVICE_FS_H__
#define __DEVICE_FS_H__

#include <rtthread.h>

#ifdef ENABLE_DFS_DEVFS

#ifdef __cplusplus
extern "C" {
#endif

int devfs_init(void);

#ifdef __cplusplus
}
#endif

#endif

#endif
