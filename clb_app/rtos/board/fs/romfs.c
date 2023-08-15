/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rtthread.h>
#include <dfs_romfs.h>

#include <romfs_config.h>
#if defined(ENABLE_DFS_ROMFS) && defined(ENABLE_DFS)

#if 0

const static unsigned char _dummy_dummy_txt[] =
{
    0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x21, 0x0d, 0x0a,
};

const static struct romfs_dirent _dummy[] =
{
    {ROMFS_DIRENT_FILE, "dummy.txt", _dummy_dummy_txt, sizeof(_dummy_dummy_txt)},
};

const static unsigned char _dummy_txt[] =
{
    0x74, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x66, 0x69, 0x6c, 0x65, 0x21, 0x0d, 0x0a,
};

OS_WEAK const struct romfs_dirent _root_dirent[] =
{
    {ROMFS_DIRENT_DIR, "dummy", (rt_uint8_t *)_dummy, sizeof(_dummy) / sizeof(_dummy[0])},
    {ROMFS_DIRENT_FILE, "dummy.txt", _dummy_txt, sizeof(_dummy_txt)},
};

OS_WEAK const struct romfs_dirent romfs_root =
{
    ROMFS_DIRENT_DIR, "/", (rt_uint8_t *)_root_dirent, sizeof(_root_dirent) / sizeof(_root_dirent[0])
};
#endif

static const struct romfs_dirent _romfs_root[] = {
    {ROMFS_DIRENT_DIR, ROMFS_DIR1, OS_NULL, 0},
    {ROMFS_DIRENT_DIR, ROMFS_DIR2, OS_NULL, 0},
    {ROMFS_DIRENT_DIR, ROMFS_DIR3, OS_NULL, 0},
    {ROMFS_DIRENT_DIR, ROMFS_DIR4, OS_NULL, 0},
};

OS_WEAK const struct romfs_dirent romfs_root =
{
	ROMFS_DIRENT_DIR, "/", (rt_uint8_t *)_romfs_root, sizeof(_romfs_root) / sizeof(_romfs_root[0])
};

#endif
