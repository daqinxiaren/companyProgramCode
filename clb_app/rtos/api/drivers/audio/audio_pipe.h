/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */
#ifndef __AUDIO_PIPE_H__
#define __AUDIO_PIPE_H__

/**
 * Pipe Device
 */
#include <rtdevice.h>

#ifndef PIPE_BUFFER_SIZE
#define PIPE_BUFFER_SIZE    512
#else
#define PIPE_BUFFER_SIZE    PIPE_BUFFER_SIZE
#endif

/* portal device */
struct rt_audio_portal_device
{
    struct rt_device parent;
    struct rt_device *write_dev;
    struct rt_device *read_dev;
};

enum rt_audio_pipe_flag
{
    /* both read and write won't block */
    PIPE_FLAG_NONBLOCK_RDWR = 0x00,
    /* read would block */
    PIPE_FLAG_BLOCK_RD = 0x01,
    /* write would block */
    PIPE_FLAG_BLOCK_WR = 0x02,
    /* write to this pipe will discard some data when the pipe is full.
     * When this flag is set, PIPE_FLAG_BLOCK_WR will be ignored since write
     * operation will always be success. */
    PIPE_FLAG_FORCE_WR = 0x04,
};

struct rt_audio_pipe
{
    struct rt_device parent;

    /* ring buffer in pipe device */
    struct rt_ringbuffer ringbuffer;

    rt_int32_t flag;

    /* suspended list */
    rt_list_t suspended_read_list;
    rt_list_t suspended_write_list;

    struct rt_audio_portal_device *write_portal;
    struct rt_audio_portal_device *read_portal;
};

#define PIPE_CTRL_GET_SPACE          0x14            /**< get the remaining size of a pipe device */

rt_err_t rt_audio_pipe_init(struct rt_audio_pipe *pipe,
                      const char *name,
                      rt_int32_t flag,
                      rt_uint8_t *buf,
                      rt_size_t size);
rt_err_t rt_audio_pipe_detach(struct rt_audio_pipe *pipe);
#ifdef ENABLE_MEMORY_HEAP
rt_err_t rt_audio_pipe_create(const char *name, rt_int32_t flag, rt_size_t size);
void rt_audio_pipe_destroy(struct rt_audio_pipe *pipe);
#endif /* ENABLE_MEMORY_HEAP */

#endif /* __AUDIO_PIPE_H__ */
