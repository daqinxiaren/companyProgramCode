/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <rtm.h>

/* some export routines for module */

OS_MODE_EXPORT(strstr);
OS_MODE_EXPORT(strlen);
OS_MODE_EXPORT(strchr);
OS_MODE_EXPORT(strcpy);
OS_MODE_EXPORT(strncpy);
OS_MODE_EXPORT(strcmp);
OS_MODE_EXPORT(strncmp);
OS_MODE_EXPORT(strcat);
OS_MODE_EXPORT(strtol);

OS_MODE_EXPORT(memcpy);
OS_MODE_EXPORT(memcmp);
OS_MODE_EXPORT(memmove);
OS_MODE_EXPORT(memset);
OS_MODE_EXPORT(memchr);

OS_MODE_EXPORT(toupper);
OS_MODE_EXPORT(atoi);

#ifdef ENABLE_RTC
OS_MODE_EXPORT(localtime);
OS_MODE_EXPORT(time);
#endif

/* import the full stdio for printf */
#if defined(ENABLE_MODULE) && defined(__MICROLIB)
#error "[ENABLE_LIBC] Please use standard libc but not microlib."
#endif

OS_MODE_EXPORT(puts);
OS_MODE_EXPORT(printf);
