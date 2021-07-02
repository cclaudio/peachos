/*
 * Disk functions
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

// ATA commands: https://wiki.osdev.org/ATA_Command_Matrix

#include "io/io.h"
#include "disk/disk.h"
#include "memory/memory.h"
#include "config.h"
#include "status.h"

struct disk primary_disk;

/*
 * lba = logical block address
 * total = total number of blocks to read from the lba
 * buf
 */
int  disk_read_sector(int lba, int total, void *buf)
{
    /* https://wiki.osdev.org/ATA_read/write_sectors */
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char) (lba & 0xff));
    outb(0x1F4, (unsigned char) (lba >> 8));
    outb(0x1F5, (unsigned char) (lba >> 16));
    outb(0x1F7, 0x20);

    /* Read two bytes at a time */
    unsigned short *ptr = (unsigned short *) buf;
    for (int b = 0; b < total; b++) {

        // Wait for the buffer to be ready
        char c = insb(0x1F7);
        while (!(c & 0x08))
            c = insb(0x1F7);

        // Copy from primary hard disk to memory
        for (int i = 0; i < 256; i++) {
            *ptr = insw(0x1F0);
            ptr++;
        }
    }
    return 0;
}

void disk_search_and_init(void)
{
    memset(&primary_disk, 0, sizeof(primary_disk));
    primary_disk.type = PEACHOS_DISK_TYPE_REAL;
    primary_disk.id = 0;
    primary_disk.sector_size = PEACHOS_SECTOR_SIZE;
    primary_disk.filesytem = fs_resolve(&primary_disk);
}

struct disk *disk_get(int index)
{
    if (index != 0)
        return 0;

    return &primary_disk;
}

int disk_read_block(struct disk *disk, unsigned int lba, int total, void *buf)
{
    if (disk != &primary_disk)
        return -EIO;

    return disk_read_sector(lba, total, buf);
}