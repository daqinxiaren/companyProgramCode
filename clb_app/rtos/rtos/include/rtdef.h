/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2007-01-10     Bernard      the first version
 * 2008-07-12     Bernard      remove all rt_int8, rt_uint32_t etc typedef
 * 2010-10-26     yi.qiu       add module support
 * 2010-11-10     Bernard      add cleanup callback function in thread exit.
 * 2011-05-09     Bernard      use builtin va_arg in GCC 4.x
 * 2012-11-16     Bernard      change OS_NULL from ((void*)0) to 0.
 * 2012-12-29     Bernard      change the ENABLE_MEMORY_POOL location and add
 *                             ENABLE_MEMHEAP condition.
 * 2012-12-30     Bernard      add more control command for graphic.
 * 2013-01-09     Bernard      change version number.
 * 2015-02-01     Bernard      change version number to v2.1.0
 * 2017-08-31     Bernard      change version number to v3.0.0
 * 2017-11-30     Bernard      change version number to v3.0.1
 * 2017-12-27     Bernard      change version number to v3.0.2
 * 2018-02-24     Bernard      change version number to v3.0.3
 * 2018-04-25     Bernard      change version number to v3.0.4
 * 2018-05-31     Bernard      change version number to v3.1.0
 * 2018-09-04     Bernard      change version number to v3.1.1
 * 2018-09-14     Bernard      apply Apache License v2.0 to RT-Thread Kernel
 * 2018-10-13     Bernard      change version number to v4.0.0
 * 2018-10-02     Bernard      add 64bit arch support
 * 2018-11-22     Jesven       add smp member to struct rt_thread
 *                             add struct rt_cpu
 *                             add smp relevant macros
 * 2019-01-27     Bernard      change version number to v4.0.1
 * 2019-05-17     Bernard      change version number to v4.0.2
 * 2019-12-20     Bernard      change version number to v4.0.3
 * 2020-08-10     Meco Man     add macro for struct rt_device_ops
 * 2020-10-23     Meco Man     define maximum value of ipc type
 */

#ifndef __RT_DEF_H__
#define __RT_DEF_H__

/* include rtconfig header to import configuration */
#include <rtconfig.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup BasicDef
 */

/**@{*/

/* RT-Thread version information */
#define OS_VERSION                      4L              /**< major version number */
#define OS_SUBVERSION                   0L              /**< minor version number */
#define OS_REVISION                     3L              /**< revise version number */

/* RT-Thread version */
#define OS_THREAD_VERSION                ((OS_VERSION * 10000) + \
                                         (OS_SUBVERSION * 100) + OS_REVISION)

/* RT-Thread basic data type definitions */
#ifndef ENABLE_ARCH_DATA_TYPE
typedef signed   char                   rt_int8_t;      /**<  8bit integer type */
typedef signed   short                  rt_int16_t;     /**< 16bit integer type */
typedef signed   int                    rt_int32_t;     /**< 32bit integer type */
typedef unsigned char                   rt_uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  rt_uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned int                    rt_uint32_t;    /**< 32bit unsigned integer type */

#ifdef ARCH_CPU_64BIT
typedef signed long                     rt_int64_t;     /**< 64bit integer type */
typedef unsigned long                   rt_uint64_t;    /**< 64bit unsigned integer type */
#else
typedef signed long long                rt_int64_t;     /**< 64bit integer type */
typedef unsigned long long              rt_uint64_t;    /**< 64bit unsigned integer type */
#endif
#endif

typedef int                             rt_bool_t;      /**< boolean type */
typedef long                            rt_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   rt_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef rt_base_t                       rt_err_t;       /**< Type for error number */
typedef rt_uint32_t                     rt_time_t;      /**< Type for time stamp */
typedef rt_uint32_t                     rt_tick_t;      /**< Type for tick count */
typedef rt_base_t                       rt_flag_t;      /**< Type for flags */
typedef rt_ubase_t                      rt_size_t;      /**< Type for size number */
typedef rt_ubase_t                      rt_dev_t;       /**< Type for device */
typedef rt_base_t                       rt_off_t;       /**< Type for offset */

/* boolean type definitions */
#define OS_TRUE                         1               /**< boolean true  */
#define OS_FALSE                        0               /**< boolean fails */

/**@}*/

/* maximum value of base type */
#define MAX_UINT8                    0xff            /**< Maxium number of UINT8 */
#define MAX_UINT16                   0xffff          /**< Maxium number of UINT16 */
#define MAX_UINT32                   0xffffffff      /**< Maxium number of UINT32 */
#define TICK_MAX                     MAX_UINT32   /**< Maxium number of tick */

/* maximum value of ipc type */
#define SEMAPHORE_VALUE_MAX                MAX_UINT16   /**< Maxium number of semaphore .value */
#define MUTEX_VALUE_MAX              MAX_UINT16   /**< Maxium number of mutex .value */
#define MUTEX_HOLD_MAX               MAX_UINT8    /**< Maxium number of mutex .hold */
#define MB_ENTRY_MAX                 MAX_UINT16   /**< Maxium number of mailbox .entry */
#define MQ_ENTRY_MAX                 MAX_UINT16   /**< Maxium number of message queue .entry */

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#define __CLANG_ARM
#endif

/* Compiler Related Definitions */
#if defined(__CC_ARM) || defined(__CLANG_ARM)           /* ARM Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
    #define OS_UNUSED                   __attribute__((unused))
    #define OS_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))

    #define OS_WEAK                     __attribute__((weak))
    #define rt_inline                   static __inline
    /* module compiling */
    #ifdef ENABLE_MODULE
        #define OS_API                 __declspec(dllimport)
    #else
        #define OS_API                 __declspec(dllexport)
    #endif

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  @ x
    #define OS_UNUSED
    #define OS_USED                     __root
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
    #define OS_WEAK                     __weak
    #define rt_inline                   static inline
    #define OS_API

#elif defined (__GNUC__)                /* GNU GCC Compiler */
    #ifdef ENABLE_NEWLIB
        #include <stdarg.h>
    #else
        /* the version of GNU GCC must be greater than 4.x */
        typedef __builtin_va_list       __gnuc_va_list;
        typedef __gnuc_va_list          va_list;
        #define va_start(v,l)           __builtin_va_start(v,l)
        #define va_end(v)               __builtin_va_end(v)
        #define va_arg(v,l)             __builtin_va_arg(v,l)
    #endif

    #define SECTION(x)                  __attribute__((section(x)))
    #define OS_UNUSED                   __attribute__((unused))
    #define OS_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define OS_WEAK                     __attribute__((weak))
    #define rt_inline                   static __inline
    #define OS_API
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
    #define OS_UNUSED                   __attribute__((unused))
    #define OS_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define OS_WEAK                     __attribute__((weak))
    #define rt_inline                   static inline
    #define OS_API
#elif defined (_MSC_VER)
    #include <stdarg.h>
    #define SECTION(x)
    #define OS_UNUSED
    #define OS_USED
    #define ALIGN(n)                    __declspec(align(n))
    #define OS_WEAK
    #define rt_inline                   static __inline
    #define OS_API
#elif defined (__TI_COMPILER_VERSION__)
    #include <stdarg.h>
    /* The way that TI compiler set section is different from other(at least
     * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
     * details. */
    #define SECTION(x)
    #define OS_UNUSED
    #define OS_USED
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)
    #define OS_WEAK
    #define rt_inline                   static inline
    #define OS_API
#else
    #error not supported tool chain
#endif

/* initialization export */
#ifdef ENABLE_COMPONENTS_INIT
typedef int (*init_fn_t)(void);
#ifdef _MSC_VER /* we do not support MS VC++ compiler */
    #define INIT_EXPORT(fn, level)
#else
    #if ENABLE_DEBUG_INIT
        struct rt_init_desc
        {
            const char* fn_name;
            const init_fn_t fn;
        };
        #define INIT_EXPORT(fn, level)                                                       \
            const char __rti_##fn##_name[] = #fn;                                            \
            OS_USED const struct rt_init_desc __rt_init_desc_##fn SECTION(".rti_fn." level) = \
            { __rti_##fn##_name, fn};
    #else
        #define INIT_EXPORT(fn, level)                                                       \
            OS_USED const init_fn_t __rt_init_##fn SECTION(".rti_fn." level) = fn
    #endif
#endif
#else
#define INIT_EXPORT(fn, level)
#endif

/* board init routines will be called in board_init() function */
#define INIT_BOARD_EXPORT(fn)           INIT_EXPORT(fn, "1")

/* pre/device/component/env/app init routines will be called in init_thread */
/* components pre-initialization (pure software initilization) */
#define INIT_PREV_EXPORT(fn)            INIT_EXPORT(fn, "2")
/* device initialization */
#define INIT_DEVICE_EXPORT(fn)          INIT_EXPORT(fn, "3")
/* components initialization (dfs, lwip, ...) */
#define INIT_COMPONENT_EXPORT(fn)       INIT_EXPORT(fn, "4")
/* environment initialization (mount disk, ...) */
#define INIT_ENV_EXPORT(fn)             INIT_EXPORT(fn, "5")
/* appliation initialization (rtgui application etc ...) */
#define INIT_APP_EXPORT(fn)             INIT_EXPORT(fn, "6")

#if !defined(ENABLE_FINSH)
/* define these to empty, even if not include finsh.h file */
#define FINSH_FUNCTION_EXPORT(name, desc)
#define FINSH_FUNCTION_EXPORT_ALIAS(name, alias, desc)
#define FINSH_VAR_EXPORT(name, type, desc)

#define MSH_CMD_EXPORT(command, desc)
#define MSH_CMD_EXPORT_ALIAS(command, alias, desc)
#elif !defined(FINSH_ENABLE_SYMTAB)
#define FINSH_FUNCTION_EXPORT_CMD(name, cmd, desc)
#endif

/* event length */
#define EVENT_LENGTH                 32

/* memory management option */
#define MEMORY_PAGE_SIZE                 4096
#define MEMORY_PAGE_MASK                 (MEMORY_PAGE_SIZE - 1)
#define MEMORY_PAGE_BITS                 12

/* kernel malloc definitions */
#ifndef KERNEL_MALLOC
#define KERNEL_MALLOC(sz)            rt_malloc(sz)
#endif

#ifndef KERNEL_FREE
#define KERNEL_FREE(ptr)             rt_free(ptr)
#endif

#ifndef KERNEL_REALLOC
#define KERNEL_REALLOC(ptr, size)    rt_realloc(ptr, size)
#endif

/**
 * @addtogroup Error
 */

/**@{*/

/* RT-Thread error code definitions */
#define OS_EOK                          0               /**< There is no error */
#define OS_ERROR                        1               /**< A generic error happens */
#define OS_ETIMEOUT                     2               /**< Timed out */
#define OS_EFULL                        3               /**< The resource is full */
#define OS_EEMPTY                       4               /**< The resource is empty */
#define OS_ENOMEM                       5               /**< No memory */
#define OS_ENOSYS                       6               /**< No system */
#define OS_EBUSY                        7               /**< Busy */
#define OS_EIO                          8               /**< IO error */
#define OS_EINTR                        9               /**< Interrupted system call */
#define OS_EINVAL                       10              /**< Invalid argument */

/**@}*/

/**
 * @ingroup BasicDef
 *
 * @def OS_ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. OS_ALIGN(13, 4)
 * would return 16.
 */
#define OS_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def OS_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. OS_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define OS_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def OS_NULL
 * Similar as the \c NULL in C library.
 */
#define OS_NULL                         (0)

/**
 * Double List structure
 */
struct rt_list_node
{
    struct rt_list_node *next;                          /**< point to next node. */
    struct rt_list_node *prev;                          /**< point to prev node. */
};
typedef struct rt_list_node rt_list_t;                  /**< Type for lists. */

/**
 * Single List structure
 */
struct rt_slist_node
{
    struct rt_slist_node *next;                         /**< point to next node. */
};
typedef struct rt_slist_node rt_slist_t;                /**< Type for single list. */

/**
 * @addtogroup KernelObject
 */

/**@{*/

/*
 * kernel object macros
 */
#define OS_OBJECT_FLAG_MODULE           0x80            /**< is module object. */

/**
 * Base structure of Kernel object
 */
struct rt_object
{
    char       name[OS_NAME_MAX];                       /**< name of kernel object */
    rt_uint8_t type;                                    /**< type of kernel object */
    rt_uint8_t flag;                                    /**< flag of kernel object */

#ifdef ENABLE_MODULE
    void      *module_id;                               /**< id of application module */
#endif
    rt_list_t  list;                                    /**< list node of kernel object */
};
typedef struct rt_object *rt_object_t;                  /**< Type for kernel objects. */

/**
 *  The object type can be one of the follows with specific
 *  macros enabled:
 *  - Thread
 *  - Semaphore
 *  - Mutex
 *  - Event
 *  - MailBox
 *  - MessageQueue
 *  - MemHeap
 *  - MemPool
 *  - Device
 *  - Timer
 *  - Module
 *  - Unknown
 *  - Static
 */
enum rt_object_class_type
{
    OBJECT_CLASS_NULL          = 0x00,      /**< The object is not used. */
    OBJECT_CLASS_THREAD        = 0x01,      /**< The object is a thread. */
    OBJECT_CLASS_SEMAPHORE     = 0x02,      /**< The object is a semaphore. */
    OBJECT_CLASS_MUTEX         = 0x03,      /**< The object is a mutex. */
    OBJECT_CLASS_EVENT         = 0x04,      /**< The object is a event. */
    OBJECT_CLASS_MAILBOX       = 0x05,      /**< The object is a mail box. */
    OBJECT_CLASS_MESSAGE_QUEUE  = 0x06,      /**< The object is a message queue. */
    OBJECT_CLASS_MEMORY_HEAP       = 0x07,      /**< The object is a memory heap. */
    OBJECT_CLASS_MEMORY_POOL       = 0x08,      /**< The object is a memory pool. */
    OBJECT_CLASS_DEVICE        = 0x09,      /**< The object is a device. */
    OBJECT_CLASS_TIMER         = 0x0a,      /**< The object is a timer. */
    OBJECT_CLASS_MODULE        = 0x0b,      /**< The object is a module. */
    OBJECT_CLASS_UNKNOWN       = 0x0c,      /**< The object is unknown. */
    OBJECT_CLASS_STATIC        = 0x80       /**< The object is a static object. */
};

/**
 * The information of the kernel object
 */
struct rt_object_information
{
    enum rt_object_class_type type;                     /**< object class type */
    rt_list_t                 object_list;              /**< object list */
    rt_size_t                 object_size;              /**< object size */
};

/**
 * The hook function call macro
 */
#ifdef ENABLE_HOOK
#define OBJECT_HOOK_CALL(func, argv) \
    do { if ((func) != OS_NULL) func argv; } while (0)
#else
#define OBJECT_HOOK_CALL(func, argv)
#endif

/**@}*/

/**
 * @addtogroup Clock
 */

/**@{*/

/**
 * clock & timer macros
 */
#define TIMER_FLAG_DEACTIVATED       0x0             /**< timer is deactive */
#define TIMER_FLAG_ACTIVATED         0x1             /**< timer is active */
#define TIMER_FLAG_ONE_SHOT          0x0             /**< one shot timer */
#define TIMER_FLAG_PERIODIC          0x2             /**< periodic timer */

#define TIMER_FLAG_HARD_TIMER        0x0             /**< hard timer,the timer's callback function will be called in tick isr. */
#define TIMER_FLAG_SOFT_TIMER        0x4             /**< soft timer,the timer's callback function will be called in timer thread. */

#define TIMER_CTRL_SET_TIME          0x0             /**< set timer control command */
#define TIMER_CTRL_GET_TIME          0x1             /**< get timer control command */
#define TIMER_CTRL_SET_ONESHOT       0x2             /**< change timer to one shot */
#define TIMER_CTRL_SET_PERIODIC      0x3             /**< change timer to periodic */
#define TIMER_CTRL_GET_STATE         0x4             /**< get timer run state active or deactive*/

#ifndef TIMER_SKIP_LIST_LEVEL
#define TIMER_SKIP_LIST_LEVEL          1
#endif

/* 1 or 3 */
#ifndef TIMER_SKIP_LIST_MASK
#define TIMER_SKIP_LIST_MASK         0x3
#endif

/**
 * timer structure
 */
struct rt_timer
{
    struct rt_object parent;                            /**< inherit from rt_object */

    rt_list_t        row[TIMER_SKIP_LIST_LEVEL];

    void (*timeout_func)(void *parameter);              /**< timeout function */
    void            *parameter;                         /**< timeout function's parameter */

    rt_tick_t        init_tick;                         /**< timer timeout tick */
    rt_tick_t        timeout_tick;                      /**< timeout tick */
};
typedef struct rt_timer *rt_timer_t;

/**@}*/

/**
 * @addtogroup Signal
 */
#ifdef ENABLE_SIGNALS
#include <libc/libc_signal.h>
typedef unsigned long rt_sigset_t;
typedef void (*rt_sighandler_t)(int signo);
typedef siginfo_t rt_siginfo_t;

#define SIGNAL_MAX          32
#endif
/**@}*/

/**
 * @addtogroup Thread
 */

/**@{*/

/*
 * Thread
 */

/*
 * thread state definitions
 */
#define THREAD_INIT                  0x00                /**< Initialized status */
#define THREAD_READY                 0x01                /**< Ready status */
#define THREAD_SUSPEND               0x02                /**< Suspend status */
#define THREAD_RUNNING               0x03                /**< Running status */
#define THREAD_BLOCK                 THREAD_SUSPEND   /**< Blocked status */
#define THREAD_CLOSE                 0x04                /**< Closed status */
#define THREAD_STAT_MASK             0x07

#define THREAD_STAT_YIELD            0x08                /**< indicate whether remaining_tick has been reloaded since last schedule */
#define THREAD_STAT_YIELD_MASK       THREAD_STAT_YIELD

#define THREAD_STAT_SIGNAL           0x10                /**< task hold signals */
#define THREAD_STAT_SIGNAL_READY     (THREAD_STAT_SIGNAL | THREAD_READY)
#define THREAD_STAT_SIGNAL_WAIT      0x20                /**< task is waiting for signals */
#define THREAD_STAT_SIGNAL_PENDING   0x40                /**< signals is held and it has not been procressed */
#define THREAD_STAT_SIGNAL_MASK      0xf0

/**
 * thread control command definitions
 */
#define THREAD_CTRL_STARTUP          0x00                /**< Startup thread. */
#define THREAD_CTRL_CLOSE            0x01                /**< Close thread. */
#define THREAD_CTRL_CHANGE_PRIORITY  0x02                /**< Change thread priority. */
#define THREAD_CTRL_INFO             0x03                /**< Get thread information. */
#define THREAD_CTRL_BIND_CPU         0x04                /**< Set thread bind cpu. */

#ifdef ENABLE_SMP

#define OS_CPU_DETACHED                 OS_CPUS_NUMBER          /**< The thread not running on cpu. */
#define OS_CPU_MASK                     ((1 << OS_CPUS_NUMBER) - 1) /**< All CPUs mask bit. */

#ifndef OS_SCHEDULE_IPI
#define OS_SCHEDULE_IPI                 0
#endif

/**
 * CPUs definitions
 *
 */
struct rt_cpu
{
    struct rt_thread *current_thread;

    rt_uint16_t irq_nest;
    rt_uint8_t  irq_switch_flag;

    rt_uint8_t current_priority;
    rt_list_t priority_table[THREAD_PRIORITY_MAX];
#if THREAD_PRIORITY_MAX > 32
    rt_uint32_t priority_group;
    rt_uint8_t ready_table[32];
#else
    rt_uint32_t priority_group;
#endif

    rt_tick_t tick;
};

#endif

/**
 * Thread structure
 */
struct rt_thread
{
    /* rt object */
    char        name[OS_NAME_MAX];                      /**< the name of thread */
    rt_uint8_t  type;                                   /**< type of object */
    rt_uint8_t  flags;                                  /**< thread's flags */

#ifdef ENABLE_MODULE
    void       *module_id;                              /**< id of application module */
#endif

    rt_list_t   list;                                   /**< the object list */
    rt_list_t   tlist;                                  /**< the thread list */

    /* stack point and entry */
    void       *sp;                                     /**< stack point */
    void       *entry;                                  /**< entry */
    void       *parameter;                              /**< parameter */
    void       *stack_addr;                             /**< stack address */
    rt_uint32_t stack_size;                             /**< stack size */

    /* error code */
    rt_err_t    error;                                  /**< error code */

    rt_uint8_t  stat;                                   /**< thread status */

#ifdef ENABLE_SMP
    rt_uint8_t  bind_cpu;                               /**< thread is bind to cpu */
    rt_uint8_t  oncpu;                                  /**< process on cpu` */

    rt_uint16_t scheduler_lock_nest;                    /**< scheduler lock count */
    rt_uint16_t cpus_lock_nest;                         /**< cpus lock count */
    rt_uint16_t critical_lock_nest;                     /**< critical lock count */
#endif /*ENABLE_SMP*/

    /* priority */
    rt_uint8_t  current_priority;                       /**< current priority */
    rt_uint8_t  init_priority;                          /**< initialized priority */
#if THREAD_PRIORITY_MAX > 32
    rt_uint8_t  number;
    rt_uint8_t  high_mask;
#endif
    rt_uint32_t number_mask;

#if defined(ENABLE_EVENT)
    /* thread event */
    rt_uint32_t event_set;
    rt_uint8_t  event_info;
#endif

#if defined(ENABLE_SIGNALS)
    rt_sigset_t     sig_pending;                        /**< the pending signals */
    rt_sigset_t     sig_mask;                           /**< the mask bits of signal */

#ifndef ENABLE_SMP
    void            *sig_ret;                           /**< the return stack pointer from signal */
#endif
    rt_sighandler_t *sig_vectors;                       /**< vectors of signal handler */
    void            *si_list;                           /**< the signal infor list */
#endif

    rt_ubase_t  init_tick;                              /**< thread's initialized tick */
    rt_ubase_t  remaining_tick;                         /**< remaining tick */

    struct rt_timer thread_timer;                       /**< built-in thread timer */

    void (*cleanup)(struct rt_thread *tid);             /**< cleanup function when thread exit */

    /* light weight process if present */
#ifdef ENABLE_LWP
    void        *lwp;
#endif

    rt_ubase_t user_data;                             /**< private user data beyond this thread */
};
typedef struct rt_thread *rt_thread_t;

/**@}*/

/**
 * @addtogroup IPC
 */

/**@{*/

/**
 * IPC flags and control command definitions
 */
#define IPC_FLAG_FIFO                0x00            /**< FIFOed IPC. @ref IPC. */
#define IPC_FLAG_PRIO                0x01            /**< PRIOed IPC. @ref IPC. */

#define IPC_CMD_UNKNOWN              0x00            /**< unknown IPC command */
#define IPC_CMD_RESET                0x01            /**< reset IPC object */

#define WAITING_FOREVER              -1              /**< Block forever until get resource. */
#define WAITING_NO                   0               /**< Non-block. */

/**
 * Base structure of IPC object
 */
struct rt_ipc_object
{
    struct rt_object parent;                            /**< inherit from rt_object */

    rt_list_t        suspend_thread;                    /**< threads pended on this resource */
};

#ifdef ENABLE_SEMAPHORE
/**
 * Semaphore structure
 */
struct rt_semaphore
{
    struct rt_ipc_object parent;                        /**< inherit from ipc_object */

    rt_uint16_t          value;                         /**< value of semaphore. */
    rt_uint16_t          reserved;                      /**< reserved field */
};
typedef struct rt_semaphore *rt_sem_t;
#endif

#ifdef ENABLE_MUTEX
/**
 * Mutual exclusion (mutex) structure
 */
struct rt_mutex
{
    struct rt_ipc_object parent;                        /**< inherit from ipc_object */

    rt_uint16_t          value;                         /**< value of mutex */

    rt_uint8_t           original_priority;             /**< priority of last thread hold the mutex */
    rt_uint8_t           hold;                          /**< numbers of thread hold the mutex */

    struct rt_thread    *owner;                         /**< current owner of mutex */
};
typedef struct rt_mutex *rt_mutex_t;
#endif

#ifdef ENABLE_EVENT
/**
 * flag defintions in event
 */
#define EVENT_FLAG_AND               0x01            /**< logic and */
#define EVENT_FLAG_OR                0x02            /**< logic or */
#define EVENT_FLAG_CLEAR             0x04            /**< clear flag */

/*
 * event structure
 */
struct rt_event
{
    struct rt_ipc_object parent;                        /**< inherit from ipc_object */

    rt_uint32_t          set;                           /**< event set */
};
typedef struct rt_event *rt_event_t;
#endif

#ifdef ENABLE_MAIL_BOX
/**
 * mailbox structure
 */
struct rt_mailbox
{
    struct rt_ipc_object parent;                        /**< inherit from ipc_object */

    rt_ubase_t          *msg_pool;                      /**< start address of message buffer */

    rt_uint16_t          size;                          /**< size of message pool */

    rt_uint16_t          entry;                         /**< index of messages in msg_pool */
    rt_uint16_t          in_offset;                     /**< input offset of the message buffer */
    rt_uint16_t          out_offset;                    /**< output offset of the message buffer */

    rt_list_t            suspend_sender_thread;         /**< sender thread suspended on this mailbox */
};
typedef struct rt_mailbox *rt_mailbox_t;
#endif

#ifdef ENABLE_MESSAGE_QUEUE
/**
 * message queue structure
 */
struct rt_messagequeue
{
    struct rt_ipc_object parent;                        /**< inherit from ipc_object */

    void                *msg_pool;                      /**< start address of message queue */

    rt_uint16_t          msg_size;                      /**< message size of each message */
    rt_uint16_t          max_msgs;                      /**< max number of messages */

    rt_uint16_t          entry;                         /**< index of messages in the queue */

    void                *msg_queue_head;                /**< list head */
    void                *msg_queue_tail;                /**< list tail */
    void                *msg_queue_free;                /**< pointer indicated the free node of queue */

    rt_list_t            suspend_sender_thread;         /**< sender thread suspended on this message queue */
};
typedef struct rt_messagequeue *rt_mq_t;
#endif

/**@}*/

/**
 * @addtogroup MM
 */

/**@{*/

/*
 * memory management
 * heap & partition
 */

#ifdef ENABLE_MEMHEAP
/**
 * memory item on the heap
 */
struct rt_memheap_item
{
    rt_uint32_t             magic;                      /**< magic number for memheap */
    struct rt_memheap      *pool_ptr;                   /**< point of pool */

    struct rt_memheap_item *next;                       /**< next memheap item */
    struct rt_memheap_item *prev;                       /**< prev memheap item */

    struct rt_memheap_item *next_free;                  /**< next free memheap item */
    struct rt_memheap_item *prev_free;                  /**< prev free memheap item */
};

/**
 * Base structure of memory heap object
 */
struct rt_memheap
{
    struct rt_object        parent;                     /**< inherit from rt_object */

    void                   *start_addr;                 /**< pool start address and size */

    rt_uint32_t             pool_size;                  /**< pool size */
    rt_uint32_t             available_size;             /**< available size */
    rt_uint32_t             max_used_size;              /**< maximum allocated size */

    struct rt_memheap_item *block_list;                 /**< used block list */

    struct rt_memheap_item *free_list;                  /**< free block list */
    struct rt_memheap_item  free_header;                /**< free block list header */

    struct rt_semaphore     lock;                       /**< semaphore lock */
};
#endif

#ifdef ENABLE_MEMORY_POOL
/**
 * Base structure of Memory pool object
 */
struct rt_mempool
{
    struct rt_object parent;                            /**< inherit from rt_object */

    void            *start_address;                     /**< memory pool start */
    rt_size_t        size;                              /**< size of memory pool */

    rt_size_t        block_size;                        /**< size of memory blocks */
    rt_uint8_t      *block_list;                        /**< memory blocks list */

    rt_size_t        block_total_count;                 /**< numbers of memory block */
    rt_size_t        block_free_count;                  /**< numbers of free memory block */

    rt_list_t        suspend_thread;                    /**< threads pended on this resource */
};
typedef struct rt_mempool *rt_mp_t;
#endif

/**@}*/

#ifdef ENABLE_DEVICE
/**
 * @addtogroup Device
 */

/**@{*/

/**
 * device (I/O) class type
 */
enum rt_device_class_type
{
    DEVICE_CLASS_CHAR = 0,                           /**< character device */
    DEVICE_CLASS_BLOCK,                              /**< block device */
    DEVICE_CLASS_NETIF,                              /**< net interface */
    DEVICE_CLASS_MTD,                                /**< memory device */
    DEVICE_CLASS_CAN_BUS,                                /**< CAN device */
    DEVICE_CLASS_RTC,                                /**< RTC device */
    DEVICE_CLASS_SOUND,                              /**< Sound device */
    DEVICE_CLASS_GRAPHIC,                            /**< Graphic device */
    DEVICE_CLASS_I2C_BUS,                             /**< I2C bus device */
    DEVICE_CLASS_USB_DEVICE,                          /**< USB slave device */
    DEVICE_CLASS_USB_HOST,                            /**< USB host bus */
    DEVICE_CLASS_SPI_BUS,                             /**< SPI bus device */
    DEVICE_CLASS_SPI_DEVICE,                          /**< SPI device */
    DEVICE_CLASS_SDIO,                               /**< SDIO bus device */
    DEVICE_CLASS_PM,                                 /**< PM pseudo device */
    DEVICE_CLASS_PIPE,                               /**< Pipe device */
    DEVICE_CLASS_PORTAL,                             /**< Portal device */
    DEVICE_CLASS_TIMER,                              /**< Timer device */
    DEVICE_CLASS_MISCELLANEOUS,                      /**< Miscellaneous device */
    DEVICE_CLASS_SENSOR,                             /**< Sensor device */
    DEVICE_CLASS_TOUCH,                              /**< Touch device */
    DEVICE_CLASS_PHY,                                /**< PHY device */
    DEVICE_CLASS_UNKNOWN                             /**< unknown device */
};

/**
 * device flags defitions
 */
#define DEVICE_FLAG_DEACTIVATE       0x000           /**< device is not not initialized */

#define DEVICE_FLAG_RDONLY           0x001           /**< read only */
#define DEVICE_FLAG_WRONLY           0x002           /**< write only */
#define DEVICE_FLAG_RDWR             0x003           /**< read and write */

#define DEVICE_FLAG_REMOVABLE        0x004           /**< removable device */
#define DEVICE_FLAG_STANDALONE       0x008           /**< standalone device */
#define DEVICE_FLAG_ACTIVATED        0x010           /**< device is activated */
#define DEVICE_FLAG_SUSPENDED        0x020           /**< device is suspended */
#define DEVICE_FLAG_STREAM           0x040           /**< stream mode */

#define DEVICE_FLAG_INT_RX           0x100           /**< INT mode on Rx */
#define DEVICE_FLAG_DMA_RX           0x200           /**< DMA mode on Rx */
#define DEVICE_FLAG_INT_TX           0x400           /**< INT mode on Tx */
#define DEVICE_FLAG_DMA_TX           0x800           /**< DMA mode on Tx */

#define DEVICE_OFLAG_CLOSE           0x000           /**< device is closed */
#define DEVICE_OFLAG_RDONLY          0x001           /**< read only access */
#define DEVICE_OFLAG_WRONLY          0x002           /**< write only access */
#define DEVICE_OFLAG_RDWR            0x003           /**< read and write */
#define DEVICE_OFLAG_OPEN            0x008           /**< device is opened */
#define DEVICE_OFLAG_MASK            0xf0f           /**< mask of open flag */

/**
 * general device commands
 */
#define DEVICE_CTRL_RESUME           0x01            /**< resume device */
#define DEVICE_CTRL_SUSPEND          0x02            /**< suspend device */
#define DEVICE_CTRL_CONFIG           0x03            /**< configure device */
#define DEVICE_CTRL_CLOSE            0x04            /**< close device */

#define DEVICE_CTRL_SET_INT          0x10            /**< set interrupt */
#define DEVICE_CTRL_CLR_INT          0x11            /**< clear interrupt */
#define DEVICE_CTRL_GET_INT          0x12            /**< get interrupt status */

/**
 * special device commands
 */
#define DEVICE_CTRL_CHAR_STREAM      0x10            /**< stream mode on char device */
#define DEVICE_CTRL_BLK_GETGEOME     0x10            /**< get geometry information   */
#define DEVICE_CTRL_BLK_SYNC         0x11            /**< flush data to block device */
#define DEVICE_CTRL_BLK_ERASE        0x12            /**< erase block on block device */
#define DEVICE_CTRL_BLK_AUTOREFRESH  0x13            /**< block device : enter/exit auto refresh mode */
#define DEVICE_CTRL_NETIF_GETMAC     0x10            /**< get mac address */
#define DEVICE_CTRL_MTD_FORMAT       0x10            /**< format a MTD device */
#define DEVICE_CTRL_RTC_GET_TIME     0x10            /**< get time */
#define DEVICE_CTRL_RTC_SET_TIME     0x11            /**< set time */
#define DEVICE_CTRL_RTC_GET_ALARM    0x12            /**< get alarm */
#define DEVICE_CTRL_RTC_SET_ALARM    0x13            /**< set alarm */

typedef struct rt_device *rt_device_t;

#ifdef ENABLE_DEVICE_OPS
/**
 * operations set for device object
 */
struct rt_device_ops
{
    /* common device interface */
    rt_err_t  (*init)   (rt_device_t dev);
    rt_err_t  (*open)   (rt_device_t dev, rt_uint16_t oflag);
    rt_err_t  (*close)  (rt_device_t dev);
    rt_size_t (*read)   (rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
    rt_size_t (*write)  (rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size);
    rt_err_t  (*control)(rt_device_t dev, int cmd, void *args);
};
#endif

/**
 * WaitQueue structure
 */
struct rt_wqueue
{
    rt_uint32_t flag;
    rt_list_t waiting_list;
};
typedef struct rt_wqueue rt_wqueue_t;

/**
 * Device structure
 */
struct rt_device
{
    struct rt_object          parent;                   /**< inherit from rt_object */

    enum rt_device_class_type type;                     /**< device type */
    rt_uint16_t               flag;                     /**< device flag */
    rt_uint16_t               open_flag;                /**< device open flag */

    rt_uint8_t                ref_count;                /**< reference count */
    rt_uint8_t                device_id;                /**< 0 - 255 */

    /* device call back */
    rt_err_t (*rx_indicate)(rt_device_t dev, rt_size_t size);
    rt_err_t (*tx_complete)(rt_device_t dev, void *buffer);

#ifdef ENABLE_DEVICE_OPS
    const struct rt_device_ops *ops;
#else
    /* common device interface */
    rt_err_t  (*init)   (rt_device_t dev);
    rt_err_t  (*open)   (rt_device_t dev, rt_uint16_t oflag);
    rt_err_t  (*close)  (rt_device_t dev);
    rt_size_t (*read)   (rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
    rt_size_t (*write)  (rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size);
    rt_err_t  (*control)(rt_device_t dev, int cmd, void *args);
#endif

#if defined(ENABLE_POSIX)
    const struct dfs_file_ops *fops;
    struct rt_wqueue wait_queue;
#endif

    void                     *user_data;                /**< device private data */
};

/**
 * block device geometry structure
 */
struct rt_device_blk_geometry
{
    rt_uint32_t sector_count;                           /**< count of sectors */
    rt_uint32_t bytes_per_sector;                       /**< number of bytes per sector */
    rt_uint32_t block_size;                             /**< number of bytes to erase one block */
};

/**
 * sector arrange struct on block device
 */
struct rt_device_blk_sectors
{
    rt_uint32_t sector_begin;                           /**< begin sector */
    rt_uint32_t sector_end;                             /**< end sector   */
};

/**
 * cursor control command
 */
#define DEVICE_CTRL_CURSOR_SET_POSITION  0x10
#define DEVICE_CTRL_CURSOR_SET_TYPE      0x11

/**
 * graphic device control command
 */
#define GRAPHIC_CTRL_RECT_UPDATE      0
#define GRAPHIC_CTRL_POWERON          1
#define GRAPHIC_CTRL_POWEROFF         2
#define GRAPHIC_CTRL_GET_INFO         3
#define GRAPHIC_CTRL_SET_MODE         4
#define GRAPHIC_CTRL_GET_EXT          5

/* graphic deice */
enum
{
    GRAPHIC_PIXEL_FORMAT_MONO = 0,
    GRAPHIC_PIXEL_FORMAT_GRAY4,
    GRAPHIC_PIXEL_FORMAT_GRAY16,
    GRAPHIC_PIXEL_FORMAT_RGB332,
    GRAPHIC_PIXEL_FORMAT_RGB444,
    GRAPHIC_PIXEL_FORMAT_RGB565,
    GRAPHIC_PIXEL_FORMAT_RGB565P,
    GRAPHIC_PIXEL_FORMAT_BGR565 = GRAPHIC_PIXEL_FORMAT_RGB565P,
    GRAPHIC_PIXEL_FORMAT_RGB666,
    GRAPHIC_PIXEL_FORMAT_RGB888,
    GRAPHIC_PIXEL_FORMAT_ARGB888,
    GRAPHIC_PIXEL_FORMAT_ABGR888,
    GRAPHIC_PIXEL_FORMAT_ARGB565,
    GRAPHIC_PIXEL_FORMAT_ALPHA,
    GRAPHIC_PIXEL_FORMAT_COLOR,
};

/**
 * build a pixel position according to (x, y) coordinates.
 */
#define GRAPHIC_PIXEL_POSITION(x, y)  ((x << 16) | y)

/**
 * graphic device information structure
 */
struct rt_device_graphic_info
{
    rt_uint8_t  pixel_format;                           /**< graphic format */
    rt_uint8_t  bits_per_pixel;                         /**< bits per pixel */
    rt_uint16_t reserved;                               /**< reserved field */

    rt_uint16_t width;                                  /**< width of graphic device */
    rt_uint16_t height;                                 /**< height of graphic device */

    rt_uint8_t *framebuffer;                            /**< frame buffer */
};

/**
 * rectangle information structure
 */
struct rt_device_rect_info
{
    rt_uint16_t x;                                      /**< x coordinate */
    rt_uint16_t y;                                      /**< y coordinate */
    rt_uint16_t width;                                  /**< width */
    rt_uint16_t height;                                 /**< height */
};

/**
 * graphic operations
 */
struct rt_device_graphic_ops
{
    void (*set_pixel) (const char *pixel, int x, int y);
    void (*get_pixel) (char *pixel, int x, int y);

    void (*draw_hline)(const char *pixel, int x1, int x2, int y);
    void (*draw_vline)(const char *pixel, int x, int y1, int y2);

    void (*blit_line) (const char *pixel, int x, int y, rt_size_t size);
};
#define rt_graphix_ops(device)          ((struct rt_device_graphic_ops *)(device->user_data))

/**@}*/
#endif

/* definitions for libc */
#include "rtlibc.h"

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/* RT-Thread definitions for C++ */
namespace rtthread {

enum TICK_WAIT {
    WAIT_NONE = 0,
    WAIT_FOREVER = -1,
};

}

#endif /* end of __cplusplus */

#endif
