/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * File      : slab.c
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-07-12     Bernard      the first version
 * 2010-07-13     Bernard      fix OS_ALIGN issue found by kuronca
 * 2010-10-23     yi.qiu       add module memory allocator
 * 2010-12-18     yi.qiu       fix zone release bug
 */

/*
 * KERN_SLABALLOC.C - Kernel SLAB memory allocator
 *
 * Copyright (c) 2003,2004 The DragonFly Project.  All rights reserved.
 *
 * This code is derived from software contributed to The DragonFly Project
 * by Matthew Dillon <dillon@backplane.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of The DragonFly Project nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific, prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <rthw.h>
#include <rtthread.h>

#define MEMORY_STATS

#if defined (ENABLE_MEMORY_HEAP) && defined (ENABLE_SLAB)
/* some statistical variable */
#ifdef MEMORY_STATS
static rt_size_t used_mem, max_mem;
#endif

#ifdef ENABLE_HOOK
static void (*rt_malloc_hook)(void *ptr, rt_size_t size);
static void (*rt_free_hook)(void *ptr);

/**
 * @addtogroup Hook
 */

/**@{*/

/**
 * This function will set a hook function, which will be invoked when a memory
 * block is allocated from heap memory.
 *
 * @param hook the hook function
 */
void rt_malloc_sethook(void (*hook)(void *ptr, rt_size_t size))
{
    rt_malloc_hook = hook;
}
OS_MODE_EXPORT(rt_malloc_sethook);

/**
 * This function will set a hook function, which will be invoked when a memory
 * block is released to heap memory.
 *
 * @param hook the hook function
 */
void rt_free_sethook(void (*hook)(void *ptr))
{
    rt_free_hook = hook;
}
OS_MODE_EXPORT(rt_free_sethook);

/**@}*/

#endif

/*
 * slab allocator implementation
 *
 * A slab allocator reserves a ZONE for each chunk size, then lays the
 * chunks out in an array within the zone.  Allocation and deallocation
 * is nearly instantanious, and fragmentation/overhead losses are limited
 * to a fixed worst-case amount.
 *
 * The downside of this slab implementation is in the chunk size
 * multiplied by the number of zones.  ~80 zones * 128K = 10MB of VM per cpu.
 * In a kernel implementation all this memory will be physical so
 * the zone size is adjusted downward on machines with less physical
 * memory.  The upside is that overhead is bounded... this is the *worst*
 * case overhead.
 *
 * Slab management is done on a per-cpu basis and no locking or mutexes
 * are required, only a critical section.  When one cpu frees memory
 * belonging to another cpu's slab manager an asynchronous IPI message
 * will be queued to execute the operation.   In addition, both the
 * high level slab allocator and the low level zone allocator optimize
 * M_ZERO requests, and the slab allocator does not have to pre initialize
 * the linked list of chunks.
 *
 * XXX Balancing is needed between cpus.  Balance will be handled through
 * asynchronous IPIs primarily by reassigning the z_Cpu ownership of chunks.
 *
 * XXX If we have to allocate a new zone and M_USE_RESERVE is set, use of
 * the new zone should be restricted to M_USE_RESERVE requests only.
 *
 *  Alloc Size  Chunking        Number of zones
 *  0-127       8               16
 *  128-255     16              8
 *  256-511     32              8
 *  512-1023    64              8
 *  1024-2047   128             8
 *  2048-4095   256             8
 *  4096-8191   512             8
 *  8192-16383  1024            8
 *  16384-32767 2048            8
 *  (if MEMORY_PAGE_SIZE is 4K the maximum zone allocation is 16383)
 *
 *  Allocations >= zone_limit go directly to kmem.
 *
 *          API REQUIREMENTS AND SIDE EFFECTS
 *
 *    To operate as a drop-in replacement to the FreeBSD-4.x malloc() we
 *    have remained compatible with the following API requirements:
 *
 *    + small power-of-2 sized allocations are power-of-2 aligned (kern_tty)
 *    + all power-of-2 sized allocations are power-of-2 aligned (twe)
 *    + malloc(0) is allowed and returns non-OS_NULL (ahc driver)
 *    + ability to allocate arbitrarily large chunks of memory
 */

/*
 * Chunk structure for free elements
 */
typedef struct slab_chunk
{
    struct slab_chunk *c_next;
} slab_chunk;

/*
 * The IN-BAND zone header is placed at the beginning of each zone.
 */
typedef struct slab_zone
{
    rt_int32_t  z_magic;        /* magic number for sanity check */
    rt_int32_t  z_nfree;        /* total free chunks / ualloc space in zone */
    rt_int32_t  z_nmax;         /* maximum free chunks */

    struct slab_zone *z_next;   /* zoneary[] link if z_nfree non-zero */
    rt_uint8_t  *z_baseptr;     /* pointer to start of chunk array */

    rt_int32_t  z_uindex;       /* current initial allocation index */
    rt_int32_t  z_chunksize;    /* chunk size for validation */

    rt_int32_t  z_zoneindex;    /* zone index */
    slab_chunk  *z_freechunk;   /* free chunk list */
} slab_zone;

#define ZALLOC_SLAB_MAGIC       0x51ab51ab
#define ZALLOC_ZONE_LIMIT       (16 * 1024)     /* max slab-managed alloc */
#define ZALLOC_MIN_ZONE_SIZE    (32 * 1024)     /* minimum zone size */
#define ZALLOC_MAX_ZONE_SIZE    (128 * 1024)    /* maximum zone size */
#define NZONES                  72              /* number of zones */
#define ZONE_RELEASE_THRESH     2               /* threshold number of zones */

static slab_zone *zone_array[NZONES];   /* linked list of zones NFree > 0 */
static slab_zone *zone_free;            /* whole zones that have become free */

static int zone_free_cnt;
static int zone_size;
static int zone_limit;
static int zone_page_cnt;

/*
 * Misc constants.  Note that allocations that are exact multiples of
 * MEMORY_PAGE_SIZE, or exceed the zone limit, fall through to the kmem module.
 */
#define MIN_CHUNK_SIZE      8       /* in bytes */
#define MIN_CHUNK_MASK      (MIN_CHUNK_SIZE - 1)

/*
 * Array of descriptors that describe the contents of each page
 */
#define PAGE_TYPE_FREE      0x00
#define PAGE_TYPE_SMALL     0x01
#define PAGE_TYPE_LARGE     0x02
struct memusage
{
    rt_uint32_t type: 2 ;       /* page type */
    rt_uint32_t size: 30;       /* pages allocated or offset from zone */
};
static struct memusage *memusage = OS_NULL;
#define btokup(addr)    \
    (&memusage[((rt_ubase_t)(addr) - heap_start) >> MEMORY_PAGE_BITS])

static rt_ubase_t heap_start, heap_end;

/* page allocator */
struct rt_page_head
{
    struct rt_page_head *next;      /* next valid page */
    rt_size_t page;                 /* number of page  */

    /* dummy */
    char dummy[MEMORY_PAGE_SIZE - (sizeof(struct rt_page_head *) + sizeof(rt_size_t))];
};
static struct rt_page_head *rt_page_list;
static struct rt_semaphore heap_sem;

void *rt_page_alloc(rt_size_t npages)
{
    struct rt_page_head *b, *n;
    struct rt_page_head **prev;

    if (npages == 0)
        return OS_NULL;

    /* lock heap */
    rt_sem_take(&heap_sem, WAITING_FOREVER);
    for (prev = &rt_page_list; (b = *prev) != OS_NULL; prev = &(b->next))
    {
        if (b->page > npages)
        {
            /* splite pages */
            n       = b + npages;
            n->next = b->next;
            n->page = b->page - npages;
            *prev   = n;
            break;
        }

        if (b->page == npages)
        {
            /* this node fit, remove this node */
            *prev = b->next;
            break;
        }
    }

    /* unlock heap */
    rt_sem_release(&heap_sem);

    return b;
}

void rt_page_free(void *addr, rt_size_t npages)
{
    struct rt_page_head *b, *n;
    struct rt_page_head **prev;

    OS_ASSERT(addr != OS_NULL);
    OS_ASSERT((rt_ubase_t)addr % MEMORY_PAGE_SIZE == 0);
    OS_ASSERT(npages != 0);

    n = (struct rt_page_head *)addr;

    /* lock heap */
    rt_sem_take(&heap_sem, WAITING_FOREVER);

    for (prev = &rt_page_list; (b = *prev) != OS_NULL; prev = &(b->next))
    {
        OS_ASSERT(b->page > 0);
        OS_ASSERT(b > n || b + b->page <= n);

        if (b + b->page == n)
        {
            if (b + (b->page += npages) == b->next)
            {
                b->page += b->next->page;
                b->next  = b->next->next;
            }

            goto _return;
        }

        if (b == n + npages)
        {
            n->page = b->page + npages;
            n->next = b->next;
            *prev   = n;

            goto _return;
        }

        if (b > n + npages)
            break;
    }

    n->page = npages;
    n->next = b;
    *prev   = n;

_return:
    /* unlock heap */
    rt_sem_release(&heap_sem);
}

/*
 * Initialize the page allocator
 */
static void rt_page_init(void *addr, rt_size_t npages)
{
    OS_ASSERT(addr != OS_NULL);
    OS_ASSERT(npages != 0);

    rt_page_list = OS_NULL;
    rt_page_free(addr, npages);
}

/**
 * @ingroup SystemInit
 *
 * This function will init system heap
 *
 * @param begin_addr the beginning address of system page
 * @param end_addr the end address of system page
 */
void rt_system_heap_init(void *begin_addr, void *end_addr)
{
    rt_uint32_t limsize, npages;

    ENABLE_DEBUG_NOT_IN_INTERRUPT;

    /* align begin and end addr to page */
    heap_start = OS_ALIGN((rt_ubase_t)begin_addr, MEMORY_PAGE_SIZE);
    heap_end   = OS_ALIGN_DOWN((rt_ubase_t)end_addr, MEMORY_PAGE_SIZE);

    if (heap_start >= heap_end)
    {
        rt_kprintf("rt_system_heap_init, wrong address[0x%x - 0x%x]\n",
                   (rt_ubase_t)begin_addr, (rt_ubase_t)end_addr);

        return;
    }

    limsize = heap_end - heap_start;
    npages  = limsize / MEMORY_PAGE_SIZE;

    /* initialize heap semaphore */
    rt_sem_init(&heap_sem, "heap", 1, IPC_FLAG_FIFO);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_SLAB, ("heap[0x%x - 0x%x], size 0x%x, 0x%x pages\n",
                                 heap_start, heap_end, limsize, npages));

    /* init pages */
    rt_page_init((void *)heap_start, npages);

    /* calculate zone size */
    zone_size = ZALLOC_MIN_ZONE_SIZE;
    while (zone_size < ZALLOC_MAX_ZONE_SIZE && (zone_size << 1) < (limsize / 1024))
        zone_size <<= 1;

    zone_limit = zone_size / 4;
    if (zone_limit > ZALLOC_ZONE_LIMIT)
        zone_limit = ZALLOC_ZONE_LIMIT;

    zone_page_cnt = zone_size / MEMORY_PAGE_SIZE;

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_SLAB, ("zone size 0x%x, zone page count 0x%x\n",
                                 zone_size, zone_page_cnt));

    /* allocate memusage array */
    limsize  = npages * sizeof(struct memusage);
    limsize  = OS_ALIGN(limsize, MEMORY_PAGE_SIZE);
    memusage = rt_page_alloc(limsize / MEMORY_PAGE_SIZE);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_SLAB, ("memusage 0x%x, size 0x%x\n",
                                 (rt_ubase_t)memusage, limsize));
}

/*
 * Calculate the zone index for the allocation request size and set the
 * allocation request size to that particular zone's chunk size.
 */
rt_inline int zoneindex(rt_size_t *bytes)
{
    /* unsigned for shift opt */
    rt_ubase_t n = (rt_ubase_t)(*bytes);

    if (n < 128)
    {
        *bytes = n = (n + 7) & ~7;

        /* 8 byte chunks, 16 zones */
        return (n / 8 - 1);
    }
    if (n < 256)
    {
        *bytes = n = (n + 15) & ~15;

        return (n / 16 + 7);
    }
    if (n < 8192)
    {
        if (n < 512)
        {
            *bytes = n = (n + 31) & ~31;

            return (n / 32 + 15);
        }
        if (n < 1024)
        {
            *bytes = n = (n + 63) & ~63;

            return (n / 64 + 23);
        }
        if (n < 2048)
        {
            *bytes = n = (n + 127) & ~127;

            return (n / 128 + 31);
        }
        if (n < 4096)
        {
            *bytes = n = (n + 255) & ~255;

            return (n / 256 + 39);
        }
        *bytes = n = (n + 511) & ~511;

        return (n / 512 + 47);
    }
    if (n < 16384)
    {
        *bytes = n = (n + 1023) & ~1023;

        return (n / 1024 + 55);
    }

    rt_kprintf("Unexpected byte count %d", n);

    return 0;
}

/**
 * @addtogroup MM
 */

/**@{*/

/**
 * This function will allocate a block from system heap memory.
 * - If the nbytes is less than zero,
 * or
 * - If there is no nbytes sized memory valid in system,
 * the OS_NULL is returned.
 *
 * @param size the size of memory to be allocated
 *
 * @return the allocated memory
 */
void *rt_malloc(rt_size_t size)
{
    slab_zone *z;
    rt_int32_t zi;
    slab_chunk *chunk;
    struct memusage *kup;

    /* zero size, return OS_NULL */
    if (size == 0)
        return OS_NULL;

    /*
     * Handle large allocations directly.  There should not be very many of
     * these so performance is not a big issue.
     */
    if (size >= zone_limit)
    {
        size = OS_ALIGN(size, MEMORY_PAGE_SIZE);

        chunk = rt_page_alloc(size >> MEMORY_PAGE_BITS);
        if (chunk == OS_NULL)
            return OS_NULL;

        /* set kup */
        kup = btokup(chunk);
        kup->type = PAGE_TYPE_LARGE;
        kup->size = size >> MEMORY_PAGE_BITS;

        ENABLE_DEBUG_LOG(ENABLE_DEBUG_SLAB,
                     ("malloc a large memory 0x%x, page cnt %d, kup %d\n",
                      size,
                      size >> MEMORY_PAGE_BITS,
                      ((rt_ubase_t)chunk - heap_start) >> MEMORY_PAGE_BITS));

        /* lock heap */
        rt_sem_take(&heap_sem, WAITING_FOREVER);

#ifdef MEMORY_STATS
        used_mem += size;
        if (used_mem > max_mem)
            max_mem = used_mem;
#endif
        goto done;
    }

    /* lock heap */
    rt_sem_take(&heap_sem, WAITING_FOREVER);

    /*
     * Attempt to allocate out of an existing zone.  First try the free list,
     * then allocate out of unallocated space.  If we find a good zone move
     * it to the head of the list so later allocations find it quickly
     * (we might have thousands of zones in the list).
     *
     * Note: zoneindex() will panic of size is too large.
     */
    zi = zoneindex(&size);
    OS_ASSERT(zi < NZONES);

    ENABLE_DEBUG_LOG(ENABLE_DEBUG_SLAB, ("try to malloc 0x%x on zone: %d\n", size, zi));

    if ((z = zone_array[zi]) != OS_NULL)
    {
        OS_ASSERT(z->z_nfree > 0);

        /* Remove us from the zone_array[] when we become empty */
        if (--z->z_nfree == 0)
        {
            zone_array[zi] = z->z_next;
            z->z_next = OS_NULL;
        }

        /*
         * No chunks are available but nfree said we had some memory, so
         * it must be available in the never-before-used-memory area
         * governed by uindex.  The consequences are very serious if our zone
         * got corrupted so we use an explicit rt_kprintf rather then a KASSERT.
         */
        if (z->z_uindex + 1 != z->z_nmax)
        {
            z->z_uindex = z->z_uindex + 1;
            chunk = (slab_chunk *)(z->z_baseptr + z->z_uindex * size);
        }
        else
        {
            /* find on free chunk list */
            chunk = z->z_freechunk;

            /* remove this chunk from list */
            z->z_freechunk = z->z_freechunk->c_next;
        }

#ifdef MEMORY_STATS
        used_mem += z->z_chunksize;
        if (used_mem > max_mem)
            max_mem = used_mem;
#endif

        goto done;
    }

    /*
     * If all zones are exhausted we need to allocate a new zone for this
     * index.
     *
     * At least one subsystem, the tty code (see CROUND) expects power-of-2
     * allocations to be power-of-2 aligned.  We maintain compatibility by
     * adjusting the base offset below.
     */
    {
        rt_int32_t off;

        if ((z = zone_free) != OS_NULL)
        {
            /* remove zone from free zone list */
            zone_free = z->z_next;
            -- zone_free_cnt;
        }
        else
        {
            /* unlock heap, since page allocator will think about lock */
            rt_sem_release(&heap_sem);

            /* allocate a zone from page */
            z = rt_page_alloc(zone_size / MEMORY_PAGE_SIZE);
            if (z == OS_NULL)
            {
                chunk = OS_NULL;
                goto __exit;
            }

            /* lock heap */
            rt_sem_take(&heap_sem, WAITING_FOREVER);

            ENABLE_DEBUG_LOG(ENABLE_DEBUG_SLAB, ("alloc a new zone: 0x%x\n",
                                         (rt_ubase_t)z));

            /* set message usage */
            for (off = 0, kup = btokup(z); off < zone_page_cnt; off ++)
            {
                kup->type = PAGE_TYPE_SMALL;
                kup->size = off;

                kup ++;
            }
        }

        /* clear to zero */
        rt_memset(z, 0, sizeof(slab_zone));

        /* offset of slab zone struct in zone */
        off = sizeof(slab_zone);

        /*
         * Guarentee power-of-2 alignment for power-of-2-sized chunks.
         * Otherwise just 8-byte align the data.
         */
        if ((size | (size - 1)) + 1 == (size << 1))
            off = (off + size - 1) & ~(size - 1);
        else
            off = (off + MIN_CHUNK_MASK) & ~MIN_CHUNK_MASK;

        z->z_magic     = ZALLOC_SLAB_MAGIC;
        z->z_zoneindex = zi;
        z->z_nmax      = (zone_size - off) / size;
        z->z_nfree     = z->z_nmax - 1;
        z->z_baseptr   = (rt_uint8_t *)z + off;
        z->z_uindex    = 0;
        z->z_chunksize = size;

        chunk = (slab_chunk *)(z->z_baseptr + z->z_uindex * size);

        /* link to zone array */
        z->z_next = zone_array[zi];
        zone_array[zi] = z;

#ifdef MEMORY_STATS
        used_mem += z->z_chunksize;
        if (used_mem > max_mem)
            max_mem = used_mem;
#endif
    }

done:
    rt_sem_release(&heap_sem);
    OBJECT_HOOK_CALL(rt_malloc_hook, ((char *)chunk, size));

__exit:
    return chunk;
}
OS_MODE_EXPORT(rt_malloc);

/**
 * This function will change the size of previously allocated memory block.
 *
 * @param ptr the previously allocated memory block
 * @param size the new size of memory block
 *
 * @return the allocated memory
 */
void *rt_realloc(void *ptr, rt_size_t size)
{
    void *nptr;
    slab_zone *z;
    struct memusage *kup;

    if (ptr == OS_NULL)
        return rt_malloc(size);
    if (size == 0)
    {
        rt_free(ptr);

        return OS_NULL;
    }

    /*
     * Get the original allocation's zone.  If the new request winds up
     * using the same chunk size we do not have to do anything.
     */
    kup = btokup((rt_ubase_t)ptr & ~MEMORY_PAGE_MASK);
    if (kup->type == PAGE_TYPE_LARGE)
    {
        rt_size_t osize;

        osize = kup->size << MEMORY_PAGE_BITS;
        if ((nptr = rt_malloc(size)) == OS_NULL)
            return OS_NULL;
        rt_memcpy(nptr, ptr, size > osize ? osize : size);
        rt_free(ptr);

        return nptr;
    }
    else if (kup->type == PAGE_TYPE_SMALL)
    {
        z = (slab_zone *)(((rt_ubase_t)ptr & ~MEMORY_PAGE_MASK) -
                          kup->size * MEMORY_PAGE_SIZE);
        OS_ASSERT(z->z_magic == ZALLOC_SLAB_MAGIC);

        zoneindex(&size);
        if (z->z_chunksize == size)
            return (ptr); /* same chunk */

        /*
         * Allocate memory for the new request size.  Note that zoneindex has
         * already adjusted the request size to the appropriate chunk size, which
         * should optimize our bcopy().  Then copy and return the new pointer.
         */
        if ((nptr = rt_malloc(size)) == OS_NULL)
            return OS_NULL;

        rt_memcpy(nptr, ptr, size > z->z_chunksize ? z->z_chunksize : size);
        rt_free(ptr);

        return nptr;
    }

    return OS_NULL;
}
OS_MODE_EXPORT(rt_realloc);

/**
 * This function will contiguously allocate enough space for count objects
 * that are size bytes of memory each and returns a pointer to the allocated
 * memory.
 *
 * The allocated memory is filled with bytes of value zero.
 *
 * @param count number of objects to allocate
 * @param size size of the objects to allocate
 *
 * @return pointer to allocated memory / NULL pointer if there is an error
 */
void *rt_calloc(rt_size_t count, rt_size_t size)
{
    void *p;

    /* allocate 'count' objects of size 'size' */
    p = rt_malloc(count * size);

    /* zero the memory */
    if (p)
        rt_memset(p, 0, count * size);

    return p;
}
OS_MODE_EXPORT(rt_calloc);

/**
 * This function will release the previous allocated memory block by rt_malloc.
 * The released memory block is taken back to system heap.
 *
 * @param ptr the address of memory which will be released
 */
void rt_free(void *ptr)
{
    slab_zone *z;
    slab_chunk *chunk;
    struct memusage *kup;

    /* free a OS_NULL pointer */
    if (ptr == OS_NULL)
        return ;

    OBJECT_HOOK_CALL(rt_free_hook, (ptr));

    /* get memory usage */
#if ENABLE_DEBUG_SLAB
    {
        rt_ubase_t addr = ((rt_ubase_t)ptr & ~MEMORY_PAGE_MASK);
        ENABLE_DEBUG_LOG(ENABLE_DEBUG_SLAB,
                     ("free a memory 0x%x and align to 0x%x, kup index %d\n",
                      (rt_ubase_t)ptr,
                      (rt_ubase_t)addr,
                      ((rt_ubase_t)(addr) - heap_start) >> MEMORY_PAGE_BITS));
    }
#endif

    kup = btokup((rt_ubase_t)ptr & ~MEMORY_PAGE_MASK);
    /* release large allocation */
    if (kup->type == PAGE_TYPE_LARGE)
    {
        rt_ubase_t size;

        /* lock heap */
        rt_sem_take(&heap_sem, WAITING_FOREVER);
        /* clear page counter */
        size = kup->size;
        kup->size = 0;

#ifdef MEMORY_STATS
        used_mem -= size * MEMORY_PAGE_SIZE;
#endif
        rt_sem_release(&heap_sem);

        ENABLE_DEBUG_LOG(ENABLE_DEBUG_SLAB,
                     ("free large memory block 0x%x, page count %d\n",
                      (rt_ubase_t)ptr, size));

        /* free this page */
        rt_page_free(ptr, size);

        return;
    }

    /* lock heap */
    rt_sem_take(&heap_sem, WAITING_FOREVER);

    /* zone case. get out zone. */
    z = (slab_zone *)(((rt_ubase_t)ptr & ~MEMORY_PAGE_MASK) -
                      kup->size * MEMORY_PAGE_SIZE);
    OS_ASSERT(z->z_magic == ZALLOC_SLAB_MAGIC);

    chunk          = (slab_chunk *)ptr;
    chunk->c_next  = z->z_freechunk;
    z->z_freechunk = chunk;

#ifdef MEMORY_STATS
    used_mem -= z->z_chunksize;
#endif

    /*
     * Bump the number of free chunks.  If it becomes non-zero the zone
     * must be added back onto the appropriate list.
     */
    if (z->z_nfree++ == 0)
    {
        z->z_next = zone_array[z->z_zoneindex];
        zone_array[z->z_zoneindex] = z;
    }

    /*
     * If the zone becomes totally free, and there are other zones we
     * can allocate from, move this zone to the FreeZones list.  Since
     * this code can be called from an IPI callback, do *NOT* try to mess
     * with kernel_map here.  Hysteresis will be performed at malloc() time.
     */
    if (z->z_nfree == z->z_nmax &&
        (z->z_next || zone_array[z->z_zoneindex] != z))
    {
        slab_zone **pz;

        ENABLE_DEBUG_LOG(ENABLE_DEBUG_SLAB, ("free zone 0x%x\n",
                                     (rt_ubase_t)z, z->z_zoneindex));

        /* remove zone from zone array list */
        for (pz = &zone_array[z->z_zoneindex]; z != *pz; pz = &(*pz)->z_next)
            ;
        *pz = z->z_next;

        /* reset zone */
        z->z_magic = -1;

        /* insert to free zone list */
        z->z_next = zone_free;
        zone_free = z;

        ++ zone_free_cnt;

        /* release zone to page allocator */
        if (zone_free_cnt > ZONE_RELEASE_THRESH)
        {
            register rt_base_t i;

            z         = zone_free;
            zone_free = z->z_next;
            -- zone_free_cnt;

            /* set message usage */
            for (i = 0, kup = btokup(z); i < zone_page_cnt; i ++)
            {
                kup->type = PAGE_TYPE_FREE;
                kup->size = 0;
                kup ++;
            }

            /* unlock heap */
            rt_sem_release(&heap_sem);

            /* release pages */
            rt_page_free(z, zone_size / MEMORY_PAGE_SIZE);

            return;
        }
    }
    /* unlock heap */
    rt_sem_release(&heap_sem);
}
OS_MODE_EXPORT(rt_free);

#ifdef MEMORY_STATS
void rt_memory_info(rt_uint32_t *total,
                    rt_uint32_t *used,
                    rt_uint32_t *max_used)
{
    if (total != OS_NULL)
        *total = heap_end - heap_start;

    if (used  != OS_NULL)
        *used = used_mem;

    if (max_used != OS_NULL)
        *max_used = max_mem;
}

#ifdef ENABLE_FINSH
#include <finsh.h>

void list_mem(void)
{
    rt_kprintf("total memory: %d\n", heap_end - heap_start);
    rt_kprintf("used memory : %d\n", used_mem);
    rt_kprintf("maximum allocated memory: %d\n", max_mem);
}
FINSH_FUNCTION_EXPORT(list_mem, list memory usage information)
#endif
#endif

/**@}*/

#endif
