/*
 * File implementation (Virtual Filesystem)
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "status.h"
#include "memory/heap/kheap.h"
#include "kernel.h"

struct filesystem *filesystems[PEACHOS_MAX_FILESYSTEMS];
struct file_descriptor *file_descriptors[PEACHOS_MAX_FILE_DESCRIPTORS];

static struct filesystem **fs_get_free_filesystem(void)
{
    for (int i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++)
        if (filesystems[i] == 0)
            return &filesystems[i];

    return NULL;
}

void fs_insert_filesystem(struct filesystem *filesystem)
{
    struct filesystem **fs;

    // TODO: panic if no filesystem

    fs = fs_get_free_filesystem();
    if (!fs) {
        print("Problem inserting filesystem");
        while (1) {}
    }

    *fs = filesystem;
}

static void fs_static_load(void)
{
    //fs_insert_filesystem(fat16_init());
}

void fs_load(void)
{
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

void fs_init(void)
{
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static int file_new_descriptor(struct file_descriptor **desc_out)
{
    for (int i = 0; i < PEACHOS_MAX_FILE_DESCRIPTORS; i++) {
        if (file_descriptors[i] == 0) {
            struct file_descriptor *desc = kzalloc(sizeof(struct file_descriptor));
            // Descriptors start at 1
            desc->index = i + 1;
            file_descriptors[i] = desc;
            *desc_out = desc;
            return 0;
        }
    }

    return -ENOMEM;
}

static struct file_descriptor *file_get_descriptor(int fd)
{
    if (fd <= 0 || fd >= PEACHOS_MAX_FILE_DESCRIPTORS)
        return 0;

    // Descriptors start at 1
    return file_descriptors[fd - 1];
}

struct filesystem *fs_resolve(struct disk *disk)
{
    struct filesystem *fs = NULL;

    for (int i = 0; i < PEACHOS_MAX_FILESYSTEMS; i++) {
        if (filesystems[i] != 0 && filesystems[i]->resolve(disk) == 0) {
            fs = filesystems[i];
            return fs;
        }
    }
    return NULL;
}

int fopen(const char *filename, char *mode)
{
    return -EIO;
}