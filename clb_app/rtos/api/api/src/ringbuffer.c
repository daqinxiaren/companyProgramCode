/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-09-30     Bernard      first version.
 * 2013-05-08     Grissiom     reimplement
 * 2016-08-18     heyuanjie    add interface
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>

rt_inline enum rt_ringbuffer_state rt_ringbuffer_status(struct rt_ringbuffer *rb)
{
    if (rb->read_index == rb->write_index)
    {
        if (rb->read_mirror == rb->write_mirror)
            return RING_BUFFER_EMPTY;
        else
            return RING_BUFFER_FULL;
    }
    return RING_BUFFER_HALFFULL;
}

void rt_ringbuffer_init(struct rt_ringbuffer *rb,
                        rt_uint8_t           *pool,
                        rt_int16_t            size)
{
    OS_ASSERT(rb != OS_NULL);
    OS_ASSERT(size > 0);

    /* initialize read and write index */
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    /* set buffer pool and size */
    rb->buffer_ptr = pool;
    rb->buffer_size = OS_ALIGN_DOWN(size, OS_ALIGN_SIZE);
}
OS_MODE_EXPORT(rt_ringbuffer_init);

/**
 * put a block of data into ring buffer
 */
rt_size_t rt_ringbuffer_put(struct rt_ringbuffer *rb,
                            const rt_uint8_t     *ptr,
                            rt_uint16_t           length)
{
    rt_uint16_t size;

    OS_ASSERT(rb != OS_NULL);

    /* whether has enough space */
    size = rt_ringbuffer_space_len(rb);

    /* no space */
    if (size == 0)
        return 0;

    /* drop some data */
    if (size < length)
        length = size;

    if (rb->buffer_size - rb->write_index > length)
    {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->write_index += length;
        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index],
           &ptr[0],
           rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0],
           &ptr[rb->buffer_size - rb->write_index],
           length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);

    return length;
}
OS_MODE_EXPORT(rt_ringbuffer_put);

/**
 * put a block of data into ring buffer
 *
 * When the buffer is full, it will discard the old data.
 */
rt_size_t rt_ringbuffer_put_force(struct rt_ringbuffer *rb,
                            const rt_uint8_t     *ptr,
                            rt_uint16_t           length)
{
    rt_uint16_t space_length;

    OS_ASSERT(rb != OS_NULL);

    space_length = rt_ringbuffer_space_len(rb);

    if (length > rb->buffer_size)
    {
        ptr = &ptr[length - rb->buffer_size];
        length = rb->buffer_size;
    }

    if (rb->buffer_size - rb->write_index > length)
    {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->write_index += length;

        if (length > space_length)
            rb->read_index = rb->write_index;

        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index],
           &ptr[0],
           rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0],
           &ptr[rb->buffer_size - rb->write_index],
           length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);

    if (length > space_length)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = rb->write_index;
    }

    return length;
}
OS_MODE_EXPORT(rt_ringbuffer_put_force);

/**
 *  get data from ring buffer
 */
rt_size_t rt_ringbuffer_get(struct rt_ringbuffer *rb,
                            rt_uint8_t           *ptr,
                            rt_uint16_t           length)
{
    rt_size_t size;

    OS_ASSERT(rb != OS_NULL);

    /* whether has enough data  */
    size = rt_ringbuffer_data_len(rb);

    /* no data */
    if (size == 0)
        return 0;

    /* less data */
    if (size < length)
        length = size;

    if (rb->buffer_size - rb->read_index > length)
    {
        /* copy all of data */
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->read_index += length;
        return length;
    }

    memcpy(&ptr[0],
           &rb->buffer_ptr[rb->read_index],
           rb->buffer_size - rb->read_index);
    memcpy(&ptr[rb->buffer_size - rb->read_index],
           &rb->buffer_ptr[0],
           length - (rb->buffer_size - rb->read_index));

    /* we are going into the other side of the mirror */
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index = length - (rb->buffer_size - rb->read_index);

    return length;
}
OS_MODE_EXPORT(rt_ringbuffer_get);

/**
 *  peak data from ring buffer
 */
rt_size_t rt_ringbuffer_peak(struct rt_ringbuffer *rb, rt_uint8_t **ptr)
{
    OS_ASSERT(rb != OS_NULL);

    *ptr = OS_NULL;

    /* whether has enough data  */
    rt_size_t size = rt_ringbuffer_data_len(rb);

    /* no data */
    if (size == 0)
        return 0;

    *ptr = &rb->buffer_ptr[rb->read_index];

    if(rb->buffer_size - rb->read_index > size)
    {
        rb->read_index += size;
        return size;
    }

    size = rb->buffer_size - rb->read_index;

    /* we are going into the other side of the mirror */
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index = 0;

    return size;
}
OS_MODE_EXPORT(rt_ringbuffer_peak);

/**
 * put a character into ring buffer
 */
rt_size_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const rt_uint8_t ch)
{
    OS_ASSERT(rb != OS_NULL);

    /* whether has enough space */
    if (!rt_ringbuffer_space_len(rb))
        return 0;

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    }
    else
    {
        rb->write_index++;
    }

    return 1;
}
OS_MODE_EXPORT(rt_ringbuffer_putchar);

/**
 * put a character into ring buffer
 *
 * When the buffer is full, it will discard one old data.
 */
rt_size_t rt_ringbuffer_putchar_force(struct rt_ringbuffer *rb, const rt_uint8_t ch)
{
    enum rt_ringbuffer_state old_state;

    OS_ASSERT(rb != OS_NULL);

    old_state = rt_ringbuffer_status(rb);

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
        if (old_state == RING_BUFFER_FULL)
        {
            rb->read_mirror = ~rb->read_mirror;
            rb->read_index = rb->write_index;
        }
    }
    else
    {
        rb->write_index++;
        if (old_state == RING_BUFFER_FULL)
            rb->read_index = rb->write_index;
    }

    return 1;
}
OS_MODE_EXPORT(rt_ringbuffer_putchar_force);

/**
 * get a character from a ringbuffer
 */
rt_size_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, rt_uint8_t *ch)
{
    OS_ASSERT(rb != OS_NULL);

    /* ringbuffer is empty */
    if (!rt_ringbuffer_data_len(rb))
        return 0;

    /* put character */
    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index == rb->buffer_size-1)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    }
    else
    {
        rb->read_index++;
    }

    return 1;
}
OS_MODE_EXPORT(rt_ringbuffer_getchar);

/** 
 * get the size of data in rb 
 */
rt_size_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb)
{
    switch (rt_ringbuffer_status(rb))
    {
    case RING_BUFFER_EMPTY:
        return 0;
    case RING_BUFFER_FULL:
        return rb->buffer_size;
    case RING_BUFFER_HALFFULL:
    default:
        if (rb->write_index > rb->read_index)
            return rb->write_index - rb->read_index;
        else
            return rb->buffer_size - (rb->read_index - rb->write_index);
    };
}
OS_MODE_EXPORT(rt_ringbuffer_data_len);

/** 
 * empty the rb 
 */
void rt_ringbuffer_reset(struct rt_ringbuffer *rb)
{
    OS_ASSERT(rb != OS_NULL);

    rb->read_mirror = 0;
    rb->read_index = 0;
    rb->write_mirror = 0;
    rb->write_index = 0;
}
OS_MODE_EXPORT(rt_ringbuffer_reset);

#ifdef ENABLE_MEMORY_HEAP

struct rt_ringbuffer* rt_ringbuffer_create(rt_uint16_t size)
{
    struct rt_ringbuffer *rb;
    rt_uint8_t *pool;

	OS_ASSERT(size > 0);

    size = OS_ALIGN_DOWN(size, OS_ALIGN_SIZE);

    rb = (struct rt_ringbuffer *)rt_malloc(sizeof(struct rt_ringbuffer));
    if (rb == OS_NULL)
        goto exit;

    pool = (rt_uint8_t *)rt_malloc(size);
    if (pool == OS_NULL)
    {
        rt_free(rb);
        rb = OS_NULL;
        goto exit;
    }
    rt_ringbuffer_init(rb, pool, size);

exit:
    return rb;
}
OS_MODE_EXPORT(rt_ringbuffer_create);

void rt_ringbuffer_destroy(struct rt_ringbuffer *rb)
{
    OS_ASSERT(rb != OS_NULL);

    rt_free(rb->buffer_ptr);
    rt_free(rb);
}
OS_MODE_EXPORT(rt_ringbuffer_destroy);

#endif
