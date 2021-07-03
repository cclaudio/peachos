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
#include "fs/fat/fat16.h"
#include "disk/disk.h"
#include "string/string.h"
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
    fs_insert_filesystem(fat16_init());
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

FILE_MODE file_get_mode_by_string(const char *str)
{
    FILE_MODE mode = FILE_MODE_INVALID;

    if (strncmp(str, "r", 1) == 0)
        mode = FILE_MODE_READ;
    else if (strncmp(str, "w", 1) == 0)
        mode = FILE_MODE_WRITE;
    else if (strncmp(str, "a", 1) == 0)
        mode = FILE_MODE_APPEND;

    return mode;
}

int fopen(const char *filename, const char *mode_str)
{
    struct path_root *root_path;
    struct disk *disk;
    void *descriptor_private_data;
    struct file_descriptor *desc;
    int res = 0;

    root_path = pparser_parse(filename, NULL);
    if (!root_path) {
        res = -EINVARG;
        goto out;
    }

    // We cannot have just a root path 0:/ 0:/test.txt
    if (!root_path->first) {
        res = -EINVARG;
        goto out;
    }

    // Ensure the disk we are reading from exists
    disk = disk_get(root_path->drive_number);
    if (!disk) {
        res = -EIO;
        goto out;
    }

    if (!disk->filesystem) {
        res = -EIO;
        goto out;
    }

    FILE_MODE mode = file_get_mode_by_string(mode_str);
    if (mode == FILE_MODE_INVALID) {
        res = -EINVARG;
        goto out;
    }

    descriptor_private_data = disk->filesystem->open(disk, root_path->first, mode);
    if (ISERR(descriptor_private_data)) {
        res = ERROR_I(descriptor_private_data);
        goto out;
    }

    desc = NULL;
    res = file_new_descriptor(&desc);
    if (res < 0)
        goto out;

    desc->filesystem = disk->filesystem;
    desc->private = descriptor_private_data;
    desc->disk = disk;

    res = desc->index;

out:
    // fopen should not return negative values
    if (res < 0)
        res = 0;

    return res;
}

int fread(void *ptr, uint32_t size, uint32_t nmemb, int fd)
{
    struct file_descriptor *desc;

    if (size == 0 || nmemb == 0 || fd < 1)
        return -EINVARG;

    desc = file_get_descriptor(fd);
    if (!desc)
        return -EINVARG;

    return desc->filesystem->read(desc->disk, desc->private, size, nmemb, (char *) ptr);
}