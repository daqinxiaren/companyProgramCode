/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-14     Bernard      the first version
 * 2006-04-21     Bernard      change the scheduler lock to interrupt lock
 * 2006-05-18     Bernard      fix the object init bug
 * 2006-08-03     Bernard      add hook support
 * 2007-01-28     Bernard      rename OBJECT_Class_Static to OBJECT_CLASS_STATIC
 * 2010-10-26     yi.qiu       add module support in rt_object_allocate and rt_object_free
 * 2017-12-10     Bernard      Add object_info enum.
 * 2018-01-25     Bernard      Fix the object find issue when enable MODULE.
 */

#include <rtthread.h>
#include <rthw.h>

#ifdef ENABLE_MODULE
#include <dlmodule.h>
#endif

/*
 * define object_info for the number of rt_object_container items.
 */
enum rt_object_info_type
{
    OBJECT_INFO_THREAD = 0,                         /**< The object is a thread. */
#ifdef ENABLE_SEMAPHORE
    OBJECT_INFO_SEMAPHORE,                          /**< The object is a semaphore. */
#endif
#ifdef ENABLE_MUTEX
    OBJECT_INFO_MUTEX,                              /**< The object is a mutex. */
#endif
#ifdef ENABLE_EVENT
    OBJECT_INFO_EVENT,                              /**< The object is a event. */
#endif
#ifdef ENABLE_MAIL_BOX
    OBJECT_INFO_MAILBOX,                            /**< The object is a mail box. */
#endif
#ifdef ENABLE_MESSAGE_QUEUE
    OBJECT_INFO_MESSAGE_QUEUE,                       /**< The object is a message queue. */
#endif
#ifdef ENABLE_MEMHEAP
    OBJECT_INFO_MEMORY_HEAP,                            /**< The object is a memory heap */
#endif
#ifdef ENABLE_MEMORY_POOL
    OBJECT_INFO_MEMORY_POOL,                            /**< The object is a memory pool. */
#endif
#ifdef ENABLE_DEVICE
    OBJECT_INFO_DEVICE,                             /**< The object is a device */
#endif
    OBJECT_INFO_TIMER,                              /**< The object is a timer. */
#ifdef ENABLE_MODULE
    OBJECT_INFO_MODULE,                             /**< The object is a module. */
#endif
    OBJECT_INFO_UNKNOWN,                            /**< The object is unknown. */
};

#define _OBJ_CONTAINER_LIST_INIT(c)     \
    {&(rt_object_container[c].object_list), &(rt_object_container[c].object_list)}
static struct rt_object_information rt_object_container[OBJECT_INFO_UNKNOWN] =
{
    /* initialize object container - thread */
    {OBJECT_CLASS_THREAD, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_THREAD), sizeof(struct rt_thread)},
#ifdef ENABLE_SEMAPHORE
    /* initialize object container - semaphore */
    {OBJECT_CLASS_SEMAPHORE, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_SEMAPHORE), sizeof(struct rt_semaphore)},
#endif
#ifdef ENABLE_MUTEX
    /* initialize object container - mutex */
    {OBJECT_CLASS_MUTEX, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_MUTEX), sizeof(struct rt_mutex)},
#endif
#ifdef ENABLE_EVENT
    /* initialize object container - event */
    {OBJECT_CLASS_EVENT, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_EVENT), sizeof(struct rt_event)},
#endif
#ifdef ENABLE_MAIL_BOX
    /* initialize object container - mailbox */
    {OBJECT_CLASS_MAILBOX, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_MAILBOX), sizeof(struct rt_mailbox)},
#endif
#ifdef ENABLE_MESSAGE_QUEUE
    /* initialize object container - message queue */
    {OBJECT_CLASS_MESSAGE_QUEUE, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_MESSAGE_QUEUE), sizeof(struct rt_messagequeue)},
#endif
#ifdef ENABLE_MEMHEAP
    /* initialize object container - memory heap */
    {OBJECT_CLASS_MEMORY_HEAP, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_MEMORY_HEAP), sizeof(struct rt_memheap)},
#endif
#ifdef ENABLE_MEMORY_POOL
    /* initialize object container - memory pool */
    {OBJECT_CLASS_MEMORY_POOL, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_MEMORY_POOL), sizeof(struct rt_mempool)},
#endif
#ifdef ENABLE_DEVICE
    /* initialize object container - device */
    {OBJECT_CLASS_DEVICE, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_DEVICE), sizeof(struct rt_device)},
#endif
    /* initialize object container - timer */
    {OBJECT_CLASS_TIMER, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_TIMER), sizeof(struct rt_timer)},
#ifdef ENABLE_MODULE
    /* initialize object container - module */
    {OBJECT_CLASS_MODULE, _OBJ_CONTAINER_LIST_INIT(OBJECT_INFO_MODULE), sizeof(struct rt_dlmodule)},
#endif
};

#ifdef ENABLE_HOOK
static void (*rt_object_attach_hook)(struct rt_object *object);
static void (*rt_object_detach_hook)(struct rt_object *object);
void (*rt_object_trytake_hook)(struct rt_object *object);
void (*rt_object_take_hook)(struct rt_object *object);
void (*rt_object_put_hook)(struct rt_object *object);

/**
 * @addtogroup Hook
 */

/**@{*/

/**
 * This function will set a hook function, which will be invoked when object
 * attaches to kernel object system.
 *
 * @param hook the hook function
 */
void rt_object_attach_sethook(void (*hook)(struct rt_object *object))
{
    rt_object_attach_hook = hook;
}

/**
 * This function will set a hook function, which will be invoked when object
 * detaches from kernel object system.
 *
 * @param hook the hook function
 */
void rt_object_detach_sethook(void (*hook)(struct rt_object *object))
{
    rt_object_detach_hook = hook;
}

/**
 * This function will set a hook function, which will be invoked when object
 * is taken from kernel object system.
 *
 * The object is taken means:
 * semaphore - semaphore is taken by thread
 * mutex - mutex is taken by thread
 * event - event is received by thread
 * mailbox - mail is received by thread
 * message queue - message is received by thread
 *
 * @param hook the hook function
 */
void rt_object_trytake_sethook(void (*hook)(struct rt_object *object))
{
    rt_object_trytake_hook = hook;
}

/**
 * This function will set a hook function, which will be invoked when object
 * have been taken from kernel object system.
 *
 * The object have been taken means:
 * semaphore - semaphore have been taken by thread
 * mutex - mutex have been taken by thread
 * event - event have been received by thread
 * mailbox - mail have been received by thread
 * message queue - message have been received by thread
 * timer - timer is started
 *
 * @param hook the hook function
 */
void rt_object_take_sethook(void (*hook)(struct rt_object *object))
{
    rt_object_take_hook = hook;
}

/**
 * This function will set a hook function, which will be invoked when object
 * is put to kernel object system.
 *
 * @param hook the hook function
 */
void rt_object_put_sethook(void (*hook)(struct rt_object *object))
{
    rt_object_put_hook = hook;
}

/**@}*/
#endif

/**
 * @addtogroup KernelObject
 */

/**@{*/

/**
 * This function will return the specified type of object information.
 *
 * @param type the type of object, which can be 
 *             OBJECT_CLASS_THREAD/Semaphore/Mutex... etc
 *
 * @return the object type information or OS_NULL
 */
struct rt_object_information *
rt_object_get_information(enum rt_object_class_type type)
{
    int index;

    for (index = 0; index < OBJECT_INFO_UNKNOWN; index ++)
        if (rt_object_container[index].type == type) return &rt_object_container[index];

    return OS_NULL;
}
OS_MODE_EXPORT(rt_object_get_information);

/**
 * This function will return the length of object list in object container.
 *
 * @param type the type of object, which can be 
 *             OBJECT_CLASS_THREAD/Semaphore/Mutex... etc
 * @return the length of object list
 */
int rt_object_get_length(enum rt_object_class_type type)
{
    int count = 0;
    rt_ubase_t level;
    struct rt_list_node *node = OS_NULL;
    struct rt_object_information *information = OS_NULL;

    information = rt_object_get_information((enum rt_object_class_type)type);
    if (information == OS_NULL) return 0;

    level = rt_hw_interrupt_disable();
    /* get the count of objects */
    rt_list_for_each(node, &(information->object_list))
    {
        count ++;
    }
    rt_hw_interrupt_enable(level);

    return count;
}
OS_MODE_EXPORT(rt_object_get_length);

/**
 * This function will copy the object pointer of the specified type, 
 * with the maximum size specified by maxlen.
 *
 * @param type the type of object, which can be 
 *             OBJECT_CLASS_THREAD/Semaphore/Mutex... etc
 * @param pointers the pointers will be saved to
 * @param maxlen the maximum number of pointers can be saved
 *
 * @return the copied number of object pointers
 */
int rt_object_get_pointers(enum rt_object_class_type type, rt_object_t *pointers, int maxlen)
{
    int index = 0;
    rt_ubase_t level;

    struct rt_object *object;
    struct rt_list_node *node = OS_NULL;
    struct rt_object_information *information = OS_NULL;

    if (maxlen <= 0) return 0;

    information = rt_object_get_information((enum rt_object_class_type)type);
    if (information == OS_NULL) return 0;

    level = rt_hw_interrupt_disable();
    /* retrieve pointer of object */
    rt_list_for_each(node, &(information->object_list))
    {
        object = rt_list_entry(node, struct rt_object, list);

        pointers[index] = object;
        index ++;

        if (index >= maxlen) break;
    }
    rt_hw_interrupt_enable(level);

    return index;
}
OS_MODE_EXPORT(rt_object_get_pointers);

/**
 * This function will initialize an object and add it to object system
 * management.
 *
 * @param object the specified object to be initialized.
 * @param type the object type.
 * @param name the object name. In system, the object's name must be unique.
 */
void rt_object_init(struct rt_object         *object,
                    enum rt_object_class_type type,
                    const char               *name)
{
    register rt_base_t temp;
    struct rt_list_node *node = OS_NULL;
    struct rt_object_information *information;
#ifdef ENABLE_MODULE
    struct rt_dlmodule *module = dlmodule_self();
#endif

    /* get object information */
    information = rt_object_get_information(type);
    OS_ASSERT(information != OS_NULL);

    /* check object type to avoid re-initialization */

    /* enter critical */
    rt_enter_critical();
    /* try to find object */
    for (node  = information->object_list.next;
            node != &(information->object_list);
            node  = node->next)
    {
        struct rt_object *obj;

        obj = rt_list_entry(node, struct rt_object, list);
        if (obj) /* skip warning when disable debug */
        {
            OS_ASSERT(obj != object);
        }
    }
    /* leave critical */
    rt_exit_critical();

    /* initialize object's parameters */
    /* set object type to static */
    object->type = type | OBJECT_CLASS_STATIC;
    /* copy name */
    rt_strncpy(object->name, name, OS_NAME_MAX);

    OBJECT_HOOK_CALL(rt_object_attach_hook, (object));

    /* lock interrupt */
    temp = rt_hw_interrupt_disable();

#ifdef ENABLE_MODULE
    if (module)
    {
        rt_list_insert_after(&(module->object_list), &(object->list));
        object->module_id = (void *)module;
    }
    else
#endif
    {
        /* insert object into information object list */
        rt_list_insert_after(&(information->object_list), &(object->list));
    }

    /* unlock interrupt */
    rt_hw_interrupt_enable(temp);
}

/**
 * This function will detach a static object from object system,
 * and the memory of static object is not freed.
 *
 * @param object the specified object to be detached.
 */
void rt_object_detach(rt_object_t object)
{
    register rt_base_t temp;

    /* object check */
    OS_ASSERT(object != OS_NULL);

    OBJECT_HOOK_CALL(rt_object_detach_hook, (object));

    /* reset object type */
    object->type = 0;

    /* lock interrupt */
    temp = rt_hw_interrupt_disable();

    /* remove from old list */
    rt_list_remove(&(object->list));

    /* unlock interrupt */
    rt_hw_interrupt_enable(temp);
}

#ifdef ENABLE_MEMORY_HEAP
/**
 * This function will allocate an object from object system
 *
 * @param type the type of object
 * @param name the object name. In system, the object's name must be unique.
 *
 * @return object
 */
rt_object_t rt_object_allocate(enum rt_object_class_type type, const char *name)
{
    struct rt_object *object;
    register rt_base_t temp;
    struct rt_object_information *information;
#ifdef ENABLE_MODULE
    struct rt_dlmodule *module = dlmodule_self();
#endif

    ENABLE_DEBUG_NOT_IN_INTERRUPT;

    /* get object information */
    information = rt_object_get_information(type);
    OS_ASSERT(information != OS_NULL);

    object = (struct rt_object *)KERNEL_MALLOC(information->object_size);
    if (object == OS_NULL)
    {
        /* no memory can be allocated */
        return OS_NULL;
    }

    /* clean memory data of object */
    rt_memset(object, 0x0, information->object_size);

    /* initialize object's parameters */

    /* set object type */
    object->type = type;

    /* set object flag */
    object->flag = 0;

    /* copy name */
    rt_strncpy(object->name, name, OS_NAME_MAX);

    OBJECT_HOOK_CALL(rt_object_attach_hook, (object));

    /* lock interrupt */
    temp = rt_hw_interrupt_disable();

#ifdef ENABLE_MODULE
    if (module)
    {
        rt_list_insert_after(&(module->object_list), &(object->list));
        object->module_id = (void *)module;
    }
    else
#endif
    {
        /* insert object into information object list */
        rt_list_insert_after(&(information->object_list), &(object->list));
    }

    /* unlock interrupt */
    rt_hw_interrupt_enable(temp);

    /* return object */
    return object;
}

/**
 * This function will delete an object and release object memory.
 *
 * @param object the specified object to be deleted.
 */
void rt_object_delete(rt_object_t object)
{
    register rt_base_t temp;

    /* object check */
    OS_ASSERT(object != OS_NULL);
    OS_ASSERT(!(object->type & OBJECT_CLASS_STATIC));

    OBJECT_HOOK_CALL(rt_object_detach_hook, (object));

    /* reset object type */
    object->type = OBJECT_CLASS_NULL;

    /* lock interrupt */
    temp = rt_hw_interrupt_disable();

    /* remove from old list */
    rt_list_remove(&(object->list));

    /* unlock interrupt */
    rt_hw_interrupt_enable(temp);

    /* free the memory of object */
    KERNEL_FREE(object);
}
#endif

/**
 * This function will judge the object is system object or not.
 * Normally, the system object is a static object and the type
 * of object set to OBJECT_CLASS_STATIC.
 *
 * @param object the specified object to be judged.
 *
 * @return OS_TRUE if a system object, OS_FALSE for others.
 */
rt_bool_t rt_object_is_systemobject(rt_object_t object)
{
    /* object check */
    OS_ASSERT(object != OS_NULL);

    if (object->type & OBJECT_CLASS_STATIC)
        return OS_TRUE;

    return OS_FALSE;
}

/**
 * This function will return the type of object without
 * OBJECT_CLASS_STATIC flag.
 *
 * @param object the specified object to be get type.
 *
 * @return the type of object.
 */
rt_uint8_t rt_object_get_type(rt_object_t object)
{
    /* object check */
    OS_ASSERT(object != OS_NULL);

    return object->type & ~OBJECT_CLASS_STATIC;
}

/**
 * This function will find specified name object from object
 * container.
 *
 * @param name the specified name of object.
 * @param type the type of object
 *
 * @return the found object or OS_NULL if there is no this object
 * in object container.
 *
 * @note this function shall not be invoked in interrupt status.
 */
rt_object_t rt_object_find(const char *name, rt_uint8_t type)
{
    struct rt_object *object = OS_NULL;
    struct rt_list_node *node = OS_NULL;
    struct rt_object_information *information = OS_NULL;

    information = rt_object_get_information((enum rt_object_class_type)type);

    /* parameter check */
    if ((name == OS_NULL) || (information == OS_NULL)) return OS_NULL;

    /* which is invoke in interrupt status */
    ENABLE_DEBUG_NOT_IN_INTERRUPT;

    /* enter critical */
    rt_enter_critical();

    /* try to find object */
    rt_list_for_each(node, &(information->object_list))
    {
        object = rt_list_entry(node, struct rt_object, list);
        if (rt_strncmp(object->name, name, OS_NAME_MAX) == 0)
        {
            /* leave critical */
            rt_exit_critical();

            return object;
        }
    }

    /* leave critical */
    rt_exit_critical();

    return OS_NULL;
}

/**@}*/
