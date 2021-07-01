/*
 * Disk streamer
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#include "streamer.h"
#include "memory/heap/kheap.h"
#include "config.h"

struct disk_stream *dstreamer_new(int disk_id)
{
    struct disk *disk = disk_get(disk_id);
    
    if (!disk)
        return NULL;

    struct disk_stream *streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;

    return streamer;
}

/*
 * @stream stream handler returned by dstreamer_new()
 * @pos = stream position in bytes
 */
int dstreamer_seek(struct disk_stream *stream, int pos)
{
    stream->pos = pos;
    return 0;
}

int dstreamer_read(struct disk_stream *stream, void *out, int total)
{
    while (total > 0) {
        int sector = stream->pos / PEACHOS_SECTOR_SIZE;
        int offset = stream->pos % PEACHOS_SECTOR_SIZE;
        char buf[PEACHOS_SECTOR_SIZE];

        /* This assumes that the function is always able to read the number
         * of blocks requested */
        int res = disk_read_block(stream->disk, sector, 1, buf);
        if (res < 0)
            return res;

        int bytes_to_read = total > PEACHOS_SECTOR_SIZE ? PEACHOS_SECTOR_SIZE : total;

        for (int i = 0; i < bytes_to_read; i++)
            *(char *)out++ = buf[offset+i];
    
        // Adjust the stream
        stream->pos += bytes_to_read;
        total -= bytes_to_read;
    }
    
    return 0;
}

void dstreamer_close(struct disk_stream *stream)
{
    kfree(stream);
}