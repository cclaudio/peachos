/*
 * Paging
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Page Table entry bits
#define PAGING_CACHE_DISABLED   0b00010000
#define PAGING WRITE_THROUGH    0b00001000
#define PAGING_ACCESS_FROM_ALL  0b00000100  // User/Supervisor b  it
#define PAGING_IS_WRITEABLE     0b00000010
#define PAGING_IS_PRESENT       0b00000001

#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096

struct paging_4gb_chunk {
    uint32_t *directory_entry;
};

uint32_t *paging_4gb_chunk_get_directory(struct paging_4gb_chunk * chunk);
struct paging_4gb_chunk *paging_new_4gb(uint8_t flags);
void paging_switch(uint32_t *directory);
void enable_paging(void);
bool paging_is_aligned(void *addr);
int paging_set(uint32_t *directory, void *virt, uint32_t val);
int paging_get_indexes(void *virtual_address, uint32_t *directory_index_out, uint32_t *table_index_out);
void paging_free_4gb(struct paging_4gb_chunk *chunk);

#endif // PAGING_H