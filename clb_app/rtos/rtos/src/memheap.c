/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * File      : memheap.c
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-04-10     Bernard      first implementation
 * 2012-10-16     Bernard      add the mutex lock for heap object.
 * 2012-12-29     Bernard      memheap can be used as system heap.
 *                             change mutex lock to semaphore lock.
 * 2013-04-10     Bernard      add rt_memheap_realloc function.
 * 2013-05-24     Bernard      fix the rt_memheap_realloc issue.
 * 2013-07-11     Grissiom     fix the memory block splitting issue.
 * 2013-07-15     Grissiom     optimize rt_memheap_realloc
 */

#include <rthw.h>
#include <rtthread.h>

#ifdef ENABLE_MEMHEAP

/* dynamic pool magic and mask */
#define MEMORY_HEAP_MAGIC        0x1ea01ea0
#define MEMORY_HEAP_MASK         0xfffffffe
#define MEMORY_HEAP_USED         0x01
#define MEMORY_HEAP_FREED        0x00

#define MEMORY_HEAP_IS_USED(i)   ((i)->magic & MEMORY_HEAP_USED)
#define MEMORY_HEAP_MINIALLOC    12

#define MEMORY_HEAP_SIZE         OS_ALIGN(sizeof(struct rt_memheap_item), OS_ALIGN_SIZE)
#define MEMITEM_SIZE(item)      ((rt_ubase_t)item->next - (rt_ubase_t)item - MEMORY_HEAP_SIZE)

/*
 * The initialized memory pool will be:
 * +-----------------------------------+--------------------------+
 * | whole freed memory block          | Used Memory Block Tailer |
 * +-----------------------------------+--------------------------+
 *
 * block_list --> whole freed memory block
 *
 * The length of Used Memory Block Tailer is 0,
 * which is prevents block merging across list
 */
rt_err_t rt_memheap_init(struct rt_memheap *memheap,
                         const char        *name,
                         void              *start_addr,
                         rt_size_t         size)
{
    struct rt_memheap_item *item;

    OS_ASSERT(memheap != OS_NULL);

    /* initialize pool object */
    rt_object_init(&(memheap->parent), OBJECT_CLASS_MEMORY_HEAP, name);

    memheap->start_addr     = start_addr;
    memheap->pool_size      = OS_ALIGN_DOWN(size, OS_ALIGN_SIZE);
    memheap->available_size = memheap->pool_size - (2 * MEMORY_HEAP_SIZE);
    memheap->max_used_size  = memheap->pool_size - memheap->available_size;

    /* initialize the free list header */
    item            = &(memheap->free_header);
    item->magic     = MEMORY_HEAP_MAGIC;
    item->pool_ptr  = memheap;
    item->next      = OS_NULL;
    item->prev      = OS_NULL;
    item->next_free = item;
    item->prev_free = item;

    /* set the free list to free list header */
    memheap->free_list = item;

    /* initialize the first big memory block */
    item            = (struct rt_memheap_item *)start_addr;
    item->magic     = MEMORY_HEAP_MAGIC;
    item->pool_ptr  = memheap;
    item->next      = OS_NULL;
    item->prev      = OS_NULL;
    item->next_free = item;
    item->prev_free = item;

    item->next = (struct rt_memheap_item *)
                 ((rt_uint8_t *)item + memheap->available_size + MEMORY_HEAP_SIZE);
    item->prev = item->next;

    /* block list header */
    memheap->block_list = item;

    /* place the big memory block to free list */
    item->next_free = memheap->free_list->next_free;
    item->prev_free = memheap->free_list;
    memheap->free_list->next_free->prev_free = item;
    memheap->free_list->next_free            = item;

    /* move to the end of memory pool to build a small tailer block,
     * which prevents block merging
     */
    item = item->next;
    /* it's a used memory block */
    item->magic     = MEMORY_HEAP_MAGIC | MEMORY_HEAP_USED;
    item->pool_ptr  = memheap;
    item->next      = (struct rt_memheap_item *)start_addr;
    item->prev      = (struct rt_memheap_item *)start_addr;
    /* not in free list */
    item->next_free = item->prev_free = OS_NULL;

    /* initialize semaphore lock */
    rt_sem_init(&(memheap->lock), name, 1, IPC_FLAG_FIFO);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                 ("memory heap: start addr 0x%08x, size %d, free list header 0x%08x\n",
                  start_addr, size, &(memheap->free_header)));

    return OS_EOK;
}
OS_MODE_EXPORT(rt_memheap_init);

rt_err_t rt_memheap_detach(struct rt_memheap *heap)
{
    OS_ASSERT(heap);
    OS_ASSERT(rt_object_get_type(&heap->parent) == OBJECT_CLASS_MEMORY_HEAP);
    OS_ASSERT(rt_object_is_systemobject(&heap->parent));
    
    rt_sem_detach(&heap->lock);
    rt_object_detach(&(heap->parent));

    /* Return a successful completion. */
    return OS_EOK;
}
OS_MODE_EXPORT(rt_memheap_detach);

void *rt_memheap_alloc(struct rt_memheap *heap, rt_size_t size)
{
    rt_err_t result;
    rt_uint32_t free_size;
    struct rt_memheap_item *header_ptr;

    OS_ASSERT(heap != OS_NULL);
    OS_ASSERT(rt_object_get_type(&heap->parent) == OBJECT_CLASS_MEMORY_HEAP);

    /* align allocated size */
    size = OS_ALIGN(size, OS_ALIGN_SIZE);
    if (size < MEMORY_HEAP_MINIALLOC)
        size = MEMORY_HEAP_MINIALLOC;

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP, ("allocate %d on heap:%8.*s",
                                    size, OS_NAME_MAX, heap->parent.name));

    if (size < heap->available_size)
    {
        /* search on free list */
        free_size = 0;

        /* lock memheap */
        result = rt_sem_take(&(heap->lock), WAITING_FOREVER);
        if (result != OS_EOK)
        {
            rt_set_errno(result);

            return OS_NULL;
        }

        /* get the first free memory block */
        header_ptr = heap->free_list->next_free;
        while (header_ptr != heap->free_list && free_size < size)
        {
            /* get current freed memory block size */
            free_size = MEMITEM_SIZE(header_ptr);
            if (free_size < size)
            {
                /* move to next free memory block */
                header_ptr = header_ptr->next_free;
            }
        }

        /* determine if the memory is available. */
        if (free_size >= size)
        {
            /* a block that satisfies the request has been found. */

            /* determine if the block needs to be split. */
            if (free_size >= (size + MEMORY_HEAP_SIZE + MEMORY_HEAP_MINIALLOC))
            {
                struct rt_memheap_item *new_ptr;

                /* split the block. */
                new_ptr = (struct rt_memheap_item *)
                          (((rt_uint8_t *)header_ptr) + size + MEMORY_HEAP_SIZE);

                ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                             ("split: block[0x%08x] nextm[0x%08x] prevm[0x%08x] to new[0x%08x]\n",
                              header_ptr,
                              header_ptr->next,
                              header_ptr->prev,
                              new_ptr));

                /* mark the new block as a memory block and freed. */
                new_ptr->magic = MEMORY_HEAP_MAGIC;

                /* put the pool pointer into the new block. */
                new_ptr->pool_ptr = heap;

                /* break down the block list */
                new_ptr->prev          = header_ptr;
                new_ptr->next          = header_ptr->next;
                header_ptr->next->prev = new_ptr;
                header_ptr->next       = new_ptr;

                /* remove header ptr from free list */
                header_ptr->next_free->prev_free = header_ptr->prev_free;
                header_ptr->prev_free->next_free = header_ptr->next_free;
                header_ptr->next_free = OS_NULL;
                header_ptr->prev_free = OS_NULL;

                /* insert new_ptr to free list */
                new_ptr->next_free = heap->free_list->next_free;
                new_ptr->prev_free = heap->free_list;
                heap->free_list->next_free->prev_free = new_ptr;
                heap->free_list->next_free            = new_ptr;
                ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP, ("new ptr: next_free 0x%08x, prev_free 0x%08x\n",
                                                new_ptr->next_free,
                                                new_ptr->prev_free));

                /* decrement the available byte count.  */
                heap->available_size = heap->available_size -
                                       size -
                                       MEMORY_HEAP_SIZE;
                if (heap->pool_size - heap->available_size > heap->max_used_size)
                    heap->max_used_size = heap->pool_size - heap->available_size;
            }
            else
            {
                /* decrement the entire free size from the available bytes count. */
                heap->available_size = heap->available_size - free_size;
                if (heap->pool_size - heap->available_size > heap->max_used_size)
                    heap->max_used_size = heap->pool_size - heap->available_size;

                /* remove header_ptr from free list */
                ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                             ("one block: block[0x%08x], next_free 0x%08x, prev_free 0x%08x\n",
                              header_ptr,
                              header_ptr->next_free,
                              header_ptr->prev_free));

                header_ptr->next_free->prev_free = header_ptr->prev_free;
                header_ptr->prev_free->next_free = header_ptr->next_free;
                header_ptr->next_free = OS_NULL;
                header_ptr->prev_free = OS_NULL;
            }

            /* Mark the allocated block as not available. */
            header_ptr->magic |= MEMORY_HEAP_USED;

            /* release lock */
            rt_sem_release(&(heap->lock));

            /* Return a memory address to the caller.  */
            ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                         ("alloc mem: memory[0x%08x], heap[0x%08x], size: %d\n",
                          (void *)((rt_uint8_t *)header_ptr + MEMORY_HEAP_SIZE),
                          header_ptr,
                          size));

            return (void *)((rt_uint8_t *)header_ptr + MEMORY_HEAP_SIZE);
        }

        /* release lock */
        rt_sem_release(&(heap->lock));
    }

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP, ("allocate memory: failed\n"));

    /* Return the completion status.  */
    return OS_NULL;
}
OS_MODE_EXPORT(rt_memheap_alloc);

void *rt_memheap_realloc(struct rt_memheap *heap, void *ptr, rt_size_t newsize)
{
    rt_err_t result;
    rt_size_t oldsize;
    struct rt_memheap_item *header_ptr;
    struct rt_memheap_item *new_ptr;

    OS_ASSERT(heap);
    OS_ASSERT(rt_object_get_type(&heap->parent) == OBJECT_CLASS_MEMORY_HEAP);

    if (newsize == 0)
    {
        rt_memheap_free(ptr);

        return OS_NULL;
    }
    /* align allocated size */
    newsize = OS_ALIGN(newsize, OS_ALIGN_SIZE);
    if (newsize < MEMORY_HEAP_MINIALLOC)
        newsize = MEMORY_HEAP_MINIALLOC;

    if (ptr == OS_NULL)
    {
        return rt_memheap_alloc(heap, newsize);
    }

    /* get memory block header and get the size of memory block */
    header_ptr = (struct rt_memheap_item *)
                 ((rt_uint8_t *)ptr - MEMORY_HEAP_SIZE);
    oldsize = MEMITEM_SIZE(header_ptr);
    /* re-allocate memory */
    if (newsize > oldsize)
    {
        void *new_ptr;
        struct rt_memheap_item *next_ptr;

        /* lock memheap */
        result = rt_sem_take(&(heap->lock), WAITING_FOREVER);
        if (result != OS_EOK)
        {
            rt_set_errno(result);
            return OS_NULL;
        }

        next_ptr = header_ptr->next;

        /* header_ptr should not be the tail */
        OS_ASSERT(next_ptr > header_ptr);

        /* check whether the following free space is enough to expand */
        if (!MEMORY_HEAP_IS_USED(next_ptr))
        {
            rt_int32_t nextsize;

            nextsize = MEMITEM_SIZE(next_ptr);
            OS_ASSERT(next_ptr > 0);

            /* Here is the ASCII art of the situation that we can make use of
             * the next free node without alloc/memcpy, |*| is the control
             * block:
             *
             *      oldsize           free node
             * |*|-----------|*|----------------------|*|
             *         newsize          >= minialloc
             * |*|----------------|*|-----------------|*|
             */
            if (nextsize + oldsize > newsize + MEMORY_HEAP_MINIALLOC)
            {
                /* decrement the entire free size from the available bytes count. */
                heap->available_size = heap->available_size - (newsize - oldsize);
                if (heap->pool_size - heap->available_size > heap->max_used_size)
                    heap->max_used_size = heap->pool_size - heap->available_size;

                /* remove next_ptr from free list */
                ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                             ("remove block: block[0x%08x], next_free 0x%08x, prev_free 0x%08x",
                              next_ptr,
                              next_ptr->next_free,
                              next_ptr->prev_free));

                next_ptr->next_free->prev_free = next_ptr->prev_free;
                next_ptr->prev_free->next_free = next_ptr->next_free;
                next_ptr->next->prev = next_ptr->prev;
                next_ptr->prev->next = next_ptr->next;

                /* build a new one on the right place */
                next_ptr = (struct rt_memheap_item *)((char *)ptr + newsize);

                ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                             ("new free block: block[0x%08x] nextm[0x%08x] prevm[0x%08x]",
                              next_ptr,
                              next_ptr->next,
                              next_ptr->prev));

                /* mark the new block as a memory block and freed. */
                next_ptr->magic = MEMORY_HEAP_MAGIC;

                /* put the pool pointer into the new block. */
                next_ptr->pool_ptr = heap;

                next_ptr->prev          = header_ptr;
                next_ptr->next          = header_ptr->next;
                header_ptr->next->prev = next_ptr;
                header_ptr->next       = next_ptr;

                /* insert next_ptr to free list */
                next_ptr->next_free = heap->free_list->next_free;
                next_ptr->prev_free = heap->free_list;
                heap->free_list->next_free->prev_free = next_ptr;
                heap->free_list->next_free            = next_ptr;
                ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP, ("new ptr: next_free 0x%08x, prev_free 0x%08x",
                                                next_ptr->next_free,
                                                next_ptr->prev_free));

                /* release lock */
                rt_sem_release(&(heap->lock));

                return ptr;
            }
        }

        /* release lock */
        rt_sem_release(&(heap->lock));

        /* re-allocate a memory block */
        new_ptr = (void *)rt_memheap_alloc(heap, newsize);
        if (new_ptr != OS_NULL)
        {
            rt_memcpy(new_ptr, ptr, oldsize < newsize ? oldsize : newsize);
            rt_memheap_free(ptr);
        }

        return new_ptr;
    }

    /* don't split when there is less than one node space left */
    if (newsize + MEMORY_HEAP_SIZE + MEMORY_HEAP_MINIALLOC >= oldsize)
        return ptr;

    /* lock memheap */
    result = rt_sem_take(&(heap->lock), WAITING_FOREVER);
    if (result != OS_EOK)
    {
        rt_set_errno(result);

        return OS_NULL;
    }

    /* split the block. */
    new_ptr = (struct rt_memheap_item *)
              (((rt_uint8_t *)header_ptr) + newsize + MEMORY_HEAP_SIZE);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                 ("split: block[0x%08x] nextm[0x%08x] prevm[0x%08x] to new[0x%08x]\n",
                  header_ptr,
                  header_ptr->next,
                  header_ptr->prev,
                  new_ptr));

    /* mark the new block as a memory block and freed. */
    new_ptr->magic = MEMORY_HEAP_MAGIC;
    /* put the pool pointer into the new block. */
    new_ptr->pool_ptr = heap;

    /* break down the block list */
    new_ptr->prev          = header_ptr;
    new_ptr->next          = header_ptr->next;
    header_ptr->next->prev = new_ptr;
    header_ptr->next       = new_ptr;

    /* determine if the block can be merged with the next neighbor. */
    if (!MEMORY_HEAP_IS_USED(new_ptr->next))
    {
        struct rt_memheap_item *free_ptr;

        /* merge block with next neighbor. */
        free_ptr = new_ptr->next;
        heap->available_size = heap->available_size - MEMITEM_SIZE(free_ptr);

        ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                     ("merge: right node 0x%08x, next_free 0x%08x, prev_free 0x%08x\n",
                      header_ptr, header_ptr->next_free, header_ptr->prev_free));

        free_ptr->next->prev = new_ptr;
        new_ptr->next   = free_ptr->next;

        /* remove free ptr from free list */
        free_ptr->next_free->prev_free = free_ptr->prev_free;
        free_ptr->prev_free->next_free = free_ptr->next_free;
    }

    /* insert the split block to free list */
    new_ptr->next_free = heap->free_list->next_free;
    new_ptr->prev_free = heap->free_list;
    heap->free_list->next_free->prev_free = new_ptr;
    heap->free_list->next_free            = new_ptr;
    ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP, ("new free ptr: next_free 0x%08x, prev_free 0x%08x\n",
                                    new_ptr->next_free,
                                    new_ptr->prev_free));

    /* increment the available byte count.  */
    heap->available_size = heap->available_size + MEMITEM_SIZE(new_ptr);

    /* release lock */
    rt_sem_release(&(heap->lock));

    /* return the old memory block */
    return ptr;
}
OS_MODE_EXPORT(rt_memheap_realloc);

void rt_memheap_free(void *ptr)
{
    rt_err_t result;
    struct rt_memheap *heap;
    struct rt_memheap_item *header_ptr, *new_ptr;
    rt_uint32_t insert_header;

    /* NULL check */
    if (ptr == OS_NULL) return;

    /* set initial status as OK */
    insert_header = 1;
    new_ptr       = OS_NULL;
    header_ptr    = (struct rt_memheap_item *)
                    ((rt_uint8_t *)ptr - MEMORY_HEAP_SIZE);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP, ("free memory: memory[0x%08x], block[0x%08x]\n",
                                    ptr, header_ptr));

    /* check magic */
    OS_ASSERT((header_ptr->magic & MEMORY_HEAP_MASK) == MEMORY_HEAP_MAGIC);
    OS_ASSERT(header_ptr->magic & MEMORY_HEAP_USED);
    /* check whether this block of memory has been over-written. */
    OS_ASSERT((header_ptr->next->magic & MEMORY_HEAP_MASK) == MEMORY_HEAP_MAGIC);

    /* get pool ptr */
    heap = header_ptr->pool_ptr;

    OS_ASSERT(heap);
    OS_ASSERT(rt_object_get_type(&heap->parent) == OBJECT_CLASS_MEMORY_HEAP);

    /* lock memheap */
    result = rt_sem_take(&(heap->lock), WAITING_FOREVER);
    if (result != OS_EOK)
    {
        rt_set_errno(result);

        return ;
    }

    /* Mark the memory as available. */
    header_ptr->magic &= ~MEMORY_HEAP_USED;
    /* Adjust the available number of bytes. */
    heap->available_size = heap->available_size + MEMITEM_SIZE(header_ptr);

    /* Determine if the block can be merged with the previous neighbor. */
    if (!MEMORY_HEAP_IS_USED(header_ptr->prev))
    {
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP, ("merge: left node 0x%08x\n",
                                        header_ptr->prev));

        /* adjust the available number of bytes. */
        heap->available_size = heap->available_size + MEMORY_HEAP_SIZE;

        /* yes, merge block with previous neighbor. */
        (header_ptr->prev)->next = header_ptr->next;
        (header_ptr->next)->prev = header_ptr->prev;

        /* move header pointer to previous. */
        header_ptr = header_ptr->prev;
        /* don't insert header to free list */
        insert_header = 0;
    }

    /* determine if the block can be merged with the next neighbor. */
    if (!MEMORY_HEAP_IS_USED(header_ptr->next))
    {
        /* adjust the available number of bytes. */
        heap->available_size = heap->available_size + MEMORY_HEAP_SIZE;

        /* merge block with next neighbor. */
        new_ptr = header_ptr->next;

        ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                     ("merge: right node 0x%08x, next_free 0x%08x, prev_free 0x%08x\n",
                      new_ptr, new_ptr->next_free, new_ptr->prev_free));

        new_ptr->next->prev = header_ptr;
        header_ptr->next    = new_ptr->next;

        /* remove new ptr from free list */
        new_ptr->next_free->prev_free = new_ptr->prev_free;
        new_ptr->prev_free->next_free = new_ptr->next_free;
    }

    if (insert_header)
    {
        /* no left merge, insert to free list */
        header_ptr->next_free = heap->free_list->next_free;
        header_ptr->prev_free = heap->free_list;
        heap->free_list->next_free->prev_free = header_ptr;
        heap->free_list->next_free            = header_ptr;

        ENABLE_DEBUG_LOG(ENABLE_DEBUG_MEMHEAP,
                     ("insert to free list: next_free 0x%08x, prev_free 0x%08x\n",
                      header_ptr->next_free, header_ptr->prev_free));
    }

    /* release lock */
    rt_sem_release(&(heap->lock));
}
OS_MODE_EXPORT(rt_memheap_free);

#ifdef ENABLE_MEMHEAP_AS_HEAP
static struct rt_memheap _heap;

void rt_system_heap_init(void *begin_addr, void *end_addr)
{
    /* initialize a default heap in the system */
    rt_memheap_init(&_heap,
                    "heap",
                    begin_addr,
                    (rt_uint32_t)end_addr - (rt_uint32_t)begin_addr);
}

void *rt_malloc(rt_size_t size)
{
    void *ptr;

    /* try to allocate in system heap */
    ptr = rt_memheap_alloc(&_heap, size);
    if (ptr == OS_NULL)
    {
        struct rt_object *object;
        struct rt_list_node *node;
        struct rt_memheap *heap;
        struct rt_object_information *information;

        /* try to allocate on other memory heap */
        information = rt_object_get_information(OBJECT_CLASS_MEMORY_HEAP);
        OS_ASSERT(information != OS_NULL);
        for (node  = information->object_list.next;
             node != &(information->object_list);
             node  = node->next)
        {
            object = rt_list_entry(node, struct rt_object, list);
            heap   = (struct rt_memheap *)object;

            OS_ASSERT(heap);
            OS_ASSERT(rt_object_get_type(&heap->parent) == OBJECT_CLASS_MEMORY_HEAP);

            /* not allocate in the default system heap */
            if (heap == &_heap)
                continue;

            ptr = rt_memheap_alloc(heap, size);
            if (ptr != OS_NULL)
                break;
        }
    }

    return ptr;
}
OS_MODE_EXPORT(rt_malloc);

void rt_free(void *rmem)
{
    rt_memheap_free(rmem);
}
OS_MODE_EXPORT(rt_free);

void *rt_realloc(void *rmem, rt_size_t newsize)
{
    void *new_ptr;
    struct rt_memheap_item *header_ptr;

    if (rmem == OS_NULL)
        return rt_malloc(newsize);

    if (newsize == 0)
    {
        rt_free(rmem);
        return OS_NULL;
    }

    /* get old memory item */
    header_ptr = (struct rt_memheap_item *)
                 ((rt_uint8_t *)rmem - MEMORY_HEAP_SIZE);

    new_ptr = rt_memheap_realloc(header_ptr->pool_ptr, rmem, newsize);
    if (new_ptr == OS_NULL && newsize != 0)
    {
        /* allocate memory block from other memheap */
        new_ptr = rt_malloc(newsize);
        if (new_ptr != OS_NULL && rmem != OS_NULL)
        {
            rt_size_t oldsize;

            /* get the size of old memory block */
            oldsize = MEMITEM_SIZE(header_ptr);
            if (newsize > oldsize)
                rt_memcpy(new_ptr, rmem, oldsize);
            else
                rt_memcpy(new_ptr, rmem, newsize);

            rt_free(rmem);
        }
    }

    return new_ptr;
}
OS_MODE_EXPORT(rt_realloc);

void *rt_calloc(rt_size_t count, rt_size_t size)
{
    void *ptr;
    rt_size_t total_size;

    total_size = count * size;
    ptr = rt_malloc(total_size);
    if (ptr != OS_NULL)
    {
        /* clean memory */
        rt_memset(ptr, 0, total_size);
    }

    return ptr;
}
OS_MODE_EXPORT(rt_calloc);

void rt_memory_info(rt_uint32_t *total,
                    rt_uint32_t *used,
                    rt_uint32_t *max_used)
{
    if (total != OS_NULL)
        *total = _heap.pool_size;

    if (used  != OS_NULL)
        *used = _heap.pool_size - _heap.available_size;

    if (max_used != OS_NULL)
        *max_used = _heap.max_used_size;
}

#endif

#endif
