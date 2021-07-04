/*
 * Path parser
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#include "pparser.h"
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "status.h"
#include "config.h"

static int pparser_path_valid_format(const char *filename)
{
    int len = strnlen(filename, PEACHOS_MAX_PATH);
    return (len >= 3 && is_digit(filename[0]) && memcmp((void *) &filename[1], ":/", 2) == 0);
}

static int pparser_get_drive_by_path(const char **path)
{
    if (!pparser_path_valid_format(*path))
        return -EBADPATH;
    
    int drive_number = to_numeric_digit(*path[0]);

    // Add 3 bytes to skip the drive number 0:/ 1:/ 2:/
    *path += 3;

    return drive_number;
}

static struct path_root *pparser_create_root(int drive_number)
{
    struct path_root *path_r = kzalloc(sizeof(struct path_root));

    path_r->drive_number = drive_number;
    path_r->first = 0;

    return path_r;
}

static const char *pparser_get_path_part(const char **path)
{
    char *result_path_part = kzalloc(PEACHOS_MAX_PATH);
    int i = 0;

    while(**path != '/' && **path != 0x00) {
        result_path_part[i] = **path;
        *path += 1;
        i++;
    }

    // Skip the forward slash to prevent from loop in the next call
    if (**path == '/')
        *path += 1;
    
    if (i == 0) {
        kfree(result_path_part);
        return NULL;
    }

    return result_path_part;
}

static struct path_part *pparser_parse_path_part(struct path_part *last_part, const char ** path)
{
    const char *path_part_str = pparser_get_path_part(path);

    if (!path_part_str)
        return NULL;

    struct path_part *part = kzalloc(sizeof(struct path_part));
    part->part = path_part_str;
    part->next = 0x00;

    if (last_part)
        last_part->next = part;
    
    return part;
}

void pparser_free(struct path_root *root)
{
    struct path_part *part = root->first;

    while (part) {
        struct path_part *next_part = part->next;
        kfree((void *) part->part);
        kfree(part);
        part = next_part;
    }

    kfree(root);
}

struct path_root * pparser_parse(const char *path, const char *current_directory_path)
{
    int res = 0;
    const char *tmp_path = path;
    struct path_root *path_root = NULL;
    struct path_part *part = NULL;

    if (strlen(path) > PEACHOS_MAX_PATH)
        goto out;

    res = pparser_get_drive_by_path(&tmp_path);
    if (res < 0)
        goto out;

    path_root = pparser_create_root(res);
    if (!path_root)
        goto out;

    struct path_part *first_part = pparser_parse_path_part(NULL, &tmp_path);
    if (!first_part)
        goto out;

    path_root->first = first_part;
    part = pparser_parse_path_part(part, &tmp_path);
    while (part)
        part = pparser_parse_path_part(part, &tmp_path);

out:
    return path_root;
}