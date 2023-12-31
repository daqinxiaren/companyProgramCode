/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-03-22     Bernard      first version
 */
 
#include <rtthread.h>

#if defined(ENABLE_FINSH) && !defined(FINSH_ENABLE_SYMTAB)

#include "finsh.h"

long hello(void);
long version(void);
long list(void);
long list_thread(void);
long list_sem(void);
long list_mutex(void);
long list_fevent(void);
long list_event(void);
long list_mailbox(void);
long list_msgqueue(void);
long list_mempool(void);
long list_timer(void);

struct finsh_syscall _syscall_table[] =
{
    {"hello", hello},
    {"version", version},
    {"list", list},
    {"list_thread", list_thread},
#ifdef ENABLE_SEMAPHORE
    {"list_sem", list_sem},
#endif
#ifdef ENABLE_MUTEX
    {"list_mutex", list_mutex},
#endif
#ifdef ENABLE_FEVENT
    {"list_fevent", list_fevent},
#endif
#ifdef ENABLE_EVENT
    {"list_event", list_event},
#endif
#ifdef ENABLE_MAIL_BOX
    {"list_mb", list_mailbox},
#endif
#ifdef ENABLE_MESSAGE_QUEUE
    {"list_mq", list_msgqueue},
#endif
#ifdef ENABLE_MEMORY_POOL
    {"list_memp", list_mempool},
#endif
    {"list_timer", list_timer},
};
struct finsh_syscall *_syscall_table_begin = &_syscall_table[0];
struct finsh_syscall *_syscall_table_end   = &_syscall_table[sizeof(_syscall_table) / sizeof(struct finsh_syscall)];

struct finsh_sysvar *_sysvar_table_begin  = NULL;
struct finsh_sysvar *_sysvar_table_end    = NULL;

#endif /* ENABLE_FINSH && !FINSH_ENABLE_SYMTAB */

