/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017/10/15     bernard      the first version
 */
#include <rtthread.h>
#include <rtm.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

OS_MODE_EXPORT(strcpy);
OS_MODE_EXPORT(strncpy);
OS_MODE_EXPORT(strlen);
OS_MODE_EXPORT(strcat);
OS_MODE_EXPORT(strstr);
OS_MODE_EXPORT(strchr);
OS_MODE_EXPORT(strcmp);
OS_MODE_EXPORT(strtol);
OS_MODE_EXPORT(strtoul);
OS_MODE_EXPORT(strncmp);

OS_MODE_EXPORT(memcpy);
OS_MODE_EXPORT(memcmp);
OS_MODE_EXPORT(memmove);
OS_MODE_EXPORT(memset);
OS_MODE_EXPORT(memchr);

OS_MODE_EXPORT(putchar);
OS_MODE_EXPORT(puts);
OS_MODE_EXPORT(printf);
OS_MODE_EXPORT(sprintf);
OS_MODE_EXPORT(snprintf);

OS_MODE_EXPORT(fwrite);

#include <time.h>
OS_MODE_EXPORT(localtime);
OS_MODE_EXPORT(time);

#include <setjmp.h>
OS_MODE_EXPORT(longjmp);
OS_MODE_EXPORT(setjmp);

OS_MODE_EXPORT(exit);
OS_MODE_EXPORT(abort);

OS_MODE_EXPORT(rand);

#include <assert.h>
OS_MODE_EXPORT(__assert_func);
