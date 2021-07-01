/*
 * Path parser
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef PATH_PARSER_H
#define PATH_PARSER_H

struct path_root {
    int drive_number;
    struct path_part *first;
};

struct path_part {
    const char *part;
    struct path_part *next;
};

struct path_root * pparser_parse(const char *path, const char *current_directory_path);
void pparser_free(struct path_root *root);

#endif // PATH_PARSER_H