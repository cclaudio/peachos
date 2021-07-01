/*
 * FAT16 headers
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef FAT16_H
#define FAT16_H

#include "file.h"

struct filesystem *fat16_init(void);

#endif // FAT16_H