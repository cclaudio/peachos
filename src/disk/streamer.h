/*
 * Disk streamer
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef DISK_STREAMER_H
#define DISK_STREAMER_H

#include "disk.h"

struct disk_stream {
    int pos;    // byte position
    struct disk *disk;
};

struct disk_stream *dstreamer_new(int disk_id);
/*
 * @stream: handler returned by dstreamer_new()
 * @pos: stream positon in bytes
 */
int dstreamer_seek(struct disk_stream *stream, int pos);
int dstreamer_read(struct disk_stream *stream, void *out, int total);
void dstreamer_close(struct disk_stream *stream);

#endif // DISK_STREAMER_H