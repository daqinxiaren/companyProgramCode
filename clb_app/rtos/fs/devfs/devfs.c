/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-02-11     Bernard      Ignore O_CREAT flag in open.
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <dfs.h>
#include <dfs_fs.h>
#include <dfs_file.h>

#include "devfs.h"

#ifdef ENABLE_DFS_DEVFS

struct device_dirent
{
    rt_device_t *devices;
    rt_uint16_t read_index;
    rt_uint16_t device_count;
};

int dfs_device_fs_mount(struct dfs_filesystem *fs, unsigned long rwflag, const void *data)
{
    return OS_EOK;
}

int dfs_device_fs_ioctl(struct dfs_fd *file, int cmd, void *args)
{
    rt_err_t result;
    rt_device_t dev_id;

    OS_ASSERT(file != OS_NULL);

    /* get device handler */
    dev_id = (rt_device_t)file->data;
    OS_ASSERT(dev_id != OS_NULL);

    /* close device handler */
    result = rt_device_control(dev_id, cmd, args);
    if (result == OS_EOK)
        return OS_EOK;

    return result;
}

int dfs_device_fs_read(struct dfs_fd *file, void *buf, size_t count)
{
    int result;
    rt_device_t dev_id;

    OS_ASSERT(file != OS_NULL);

    /* get device handler */
    dev_id = (rt_device_t)file->data;
    OS_ASSERT(dev_id != OS_NULL);

    /* read device data */
    result = rt_device_read(dev_id, file->pos, buf, count);
    file->pos += result;

    return result;
}

int dfs_device_fs_write(struct dfs_fd *file, const void *buf, size_t count)
{
    int result;
    rt_device_t dev_id;

    OS_ASSERT(file != OS_NULL);

    /* get device handler */
    dev_id = (rt_device_t)file->data;
    OS_ASSERT(dev_id != OS_NULL);

    /* read device data */
    result = rt_device_write(dev_id, file->pos, buf, count);
    file->pos += result;

    return result;
}

int dfs_device_fs_close(struct dfs_fd *file)
{
    rt_err_t result;
    rt_device_t dev_id;

    OS_ASSERT(file != OS_NULL);

    if (file->type == FT_DIRECTORY)
    {
        struct device_dirent *root_dirent;

        root_dirent = (struct device_dirent *)file->data;
        OS_ASSERT(root_dirent != OS_NULL);

        /* release dirent */
        rt_free(root_dirent);
        return OS_EOK;
    }

    /* get device handler */
    dev_id = (rt_device_t)file->data;
    OS_ASSERT(dev_id != OS_NULL);

    /* close device handler */
    result = rt_device_close(dev_id);
    if (result == OS_EOK)
    {
        file->data = OS_NULL;

        return OS_EOK;
    }

    return -EIO;
}

int dfs_device_fs_open(struct dfs_fd *file)
{
    rt_err_t result;
    rt_device_t device;

    /* open root directory */
    if ((file->path[0] == '/') && (file->path[1] == '\0') &&
        (file->flags & O_DIRECTORY))
    {
        struct rt_object *object;
        struct rt_list_node *node;
        struct rt_object_information *information;
        struct device_dirent *root_dirent;
        rt_uint32_t count = 0;

        /* lock scheduler */
        rt_enter_critical();

        /* traverse device object */
        information = rt_object_get_information(OBJECT_CLASS_DEVICE);
        OS_ASSERT(information != OS_NULL);
        for (node = information->object_list.next; node != &(information->object_list); node = node->next)
        {
            count ++;
        }

        root_dirent = (struct device_dirent *)rt_malloc(sizeof(struct device_dirent) +
                      count * sizeof(rt_device_t));
        if (root_dirent != OS_NULL)
        {
            root_dirent->devices = (rt_device_t *)(root_dirent + 1);
            root_dirent->read_index = 0;
            root_dirent->device_count = count;
            count = 0;
            /* get all device node */
            for (node = information->object_list.next; node != &(information->object_list); node = node->next)
            {
                object = rt_list_entry(node, struct rt_object, list);
                root_dirent->devices[count] = (rt_device_t)object;
                count ++;
            }
        }
        rt_exit_critical();

        /* set data */
        file->data = root_dirent;

        return OS_EOK;
    }

    device = rt_device_find(&file->path[1]);
    if (device == OS_NULL)
        return -ENODEV;

#ifdef ENABLE_POSIX
    if (device->fops)
    {
        /* use device fops */
        file->fops = device->fops;
        file->data = (void *)device;

        /* use fops */
        if (file->fops->open)
        {
            result = file->fops->open(file);
            if (result == OS_EOK || result == -OS_ENOSYS)
            {
                file->type = FT_DEVICE;
                return 0;
            }
        }
    }
    else
#endif
    {
        result = rt_device_open(device, DEVICE_OFLAG_RDWR);
        if (result == OS_EOK || result == -OS_ENOSYS)
        {
            file->data = device;
            file->type = FT_DEVICE;
            return OS_EOK;
        }
    }

    file->data = OS_NULL;
    /* open device failed. */
    return -EIO;
}

int dfs_device_fs_stat(struct dfs_filesystem *fs, const char *path, struct stat *st)
{
    /* stat root directory */
    if ((path[0] == '/') && (path[1] == '\0'))
    {
        st->st_dev = 0;

        st->st_mode = S_IFREG | S_IRUSR | S_IRGRP | S_IROTH |
                      S_IWUSR | S_IWGRP | S_IWOTH;
        st->st_mode &= ~S_IFREG;
        st->st_mode |= S_IFDIR | S_IXUSR | S_IXGRP | S_IXOTH;

        st->st_size  = 0;
        st->st_mtime = 0;

        return OS_EOK;
    }
    else
    {
        rt_device_t dev_id;

        dev_id = rt_device_find(&path[1]);
        if (dev_id != OS_NULL)
        {
            st->st_dev = 0;

            st->st_mode = S_IRUSR | S_IRGRP | S_IROTH |
                          S_IWUSR | S_IWGRP | S_IWOTH;

            if (dev_id->type == DEVICE_CLASS_CHAR)
                st->st_mode |= S_IFCHR;
            else if (dev_id->type == DEVICE_CLASS_BLOCK)
                st->st_mode |= S_IFBLK;
            else if (dev_id->type == DEVICE_CLASS_PIPE)
                st->st_mode |= S_IFIFO;
            else
                st->st_mode |= S_IFREG;

            st->st_size  = 0;
            st->st_mtime = 0;

            return OS_EOK;
        }
    }

    return -ENOENT;
}

int dfs_device_fs_getdents(struct dfs_fd *file, struct dirent *dirp, uint32_t count)
{
    rt_uint32_t index;
    rt_object_t object;
    struct dirent *d;
    struct device_dirent *root_dirent;

    root_dirent = (struct device_dirent *)file->data;
    OS_ASSERT(root_dirent != OS_NULL);

    /* make integer count */
    count = (count / sizeof(struct dirent));
    if (count == 0)
        return -EINVAL;

    for (index = 0; index < count && index + root_dirent->read_index < root_dirent->device_count; 
        index ++)
    {
        object = (rt_object_t)root_dirent->devices[root_dirent->read_index + index];

        d = dirp + index;
        d->d_type = DT_REG;
        d->d_namlen = OS_NAME_MAX;
        d->d_reclen = (rt_uint16_t)sizeof(struct dirent);
        rt_strncpy(d->d_name, object->name, OS_NAME_MAX);
    }

    root_dirent->read_index += index;

    return index * sizeof(struct dirent);
}

static int dfs_device_fs_poll(struct dfs_fd *fd, struct rt_pollreq *req)
{
    int mask = 0;

    return mask;
}

static const struct dfs_file_ops _device_fops =
{
    dfs_device_fs_open,
    dfs_device_fs_close,
    dfs_device_fs_ioctl,
    dfs_device_fs_read,
    dfs_device_fs_write,
    OS_NULL,                    /* flush */
    OS_NULL,                    /* lseek */
    dfs_device_fs_getdents,
    dfs_device_fs_poll,
};

static const struct dfs_filesystem_ops _device_fs =
{
    "devfs",
    DFS_FS_FLAG_DEFAULT,
    &_device_fops,

    dfs_device_fs_mount,
    OS_NULL,
    OS_NULL,
    OS_NULL,

    OS_NULL,
    dfs_device_fs_stat,
    OS_NULL,
};

int devfs_init(void)
{
    /* register device file system */
    dfs_register(&_device_fs);

    return 0;
}

#endif
