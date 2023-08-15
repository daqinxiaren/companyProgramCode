/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * 2014-08-03     bernard      Add file header.
 */

#include "rtthread.h"

#ifdef ENABLE_MEMORY_HEAP

#ifdef __CC_ARM
/* avoid the heap and heap-using library functions supplied by arm */
#pragma import(__use_no_heap)
#endif

void *malloc(size_t n)
{
    return rt_malloc(n);
}
OS_MODE_EXPORT(malloc);

void *realloc(void *rmem, size_t newsize)
{
    return rt_realloc(rmem, newsize);
}
OS_MODE_EXPORT(realloc);

void *calloc(size_t nelem, size_t elsize)
{
    return rt_calloc(nelem, elsize);
}
OS_MODE_EXPORT(calloc);

void free(void *rmem)
{
    rt_free(rmem);
}
OS_MODE_EXPORT(free);
#endif
