/*
 * Disk headers
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

typedef unsigned int PEACHOS_DISK_TYPE;

// Represent a real physical hard disk
#define PEACHOS_DISK_TYPE_REAL 0

struct disk {
    PEACHOS_DISK_TYPE type;
    int sector_size;

    // The id of the disk
    int id;

    struct filesystem *filesytem;

    // The private data of out filesystem
    void *fs_private;
};

void disk_search_and_init(void);
struct disk *disk_get(int index);
int disk_read_block(struct disk *disk, unsigned int lba, int total, void *buf);

#endif // DISK_H