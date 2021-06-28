/*
 * Heap implementation
 *
 * Author: Claudio Carvalho <claudiodecarvalho@gmail.com>
 */

#include <stdbool.h>

#include "heap.h"
#include "config.h"
#include "status.h"
#include "memory/memory.h"

static bool heap_validate_table(void *ptr, void *end, struct heap_table *table)
{
    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size / PEACHOS_HEAP_BLOCK_SIZE;

    if (table->total != total_blocks)
        return false;

    return true;
}

static bool heap_validate_alignment(void *ptr)
{
    if (((unsigned int) ptr % PEACHOS_HEAP_BLOCK_SIZE) != 0)
        return false;

    return true;
}

static size_t heap_align_up(size_t val)
{
    if ((val % PEACHOS_HEAP_BLOCK_SIZE) == 0)
        return val;

    val = val - (val % PEACHOS_HEAP_BLOCK_SIZE);
    val += PEACHOS_HEAP_BLOCK_SIZE;

    return val;
}

static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry)
{
    return entry & 0x0F;
}

static int heap_get_start_block(struct heap *heap, uint32_t total_blocks)
{
    struct heap_table *table = heap->table;
    int bc = 0;  // block count
    int bs = -1; // block start

    for (size_t i = 0; i < table->total; i++) {
        if (heap_get_entry_type(table->entries[i]) != HEAP_BLOCK_TABLE_ENTRY_FREE) {
            bc = 0;
            bs = -1;
            continue;
        }
        /* Save first block index */
        if (bs == -1)
            bs = i;
        bc++;
        if (bc == total_blocks)
            break;
    }

    if (bs == -1)
        return -ENOMEM;

    return bs;
}

static void *heap_block_to_address(struct heap *heap, int block)
{
    return heap->saddr + (block * PEACHOS_HEAP_BLOCK_SIZE);
}

static int heap_address_to_block(struct heap *heap, void *address)
{
    return ((int)(address - heap->saddr)) / PEACHOS_HEAP_BLOCK_SIZE;
}

static void heap_mark_blocks_free(struct heap *heap, int starting_block)
{
    struct heap_table *table = heap->table;

    for (int i = starting_block; i < (int)table->total; i++) {
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        if (!(entry & HEAP_BLOCK_HAS_NEXT))
            break;
    }
}

static void heap_mark_blocks_taken(struct heap *heap, int start_block, int total_blocks)
{
    int end_block = (start_block + total_blocks) - 1;
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;

    if (total_blocks > 1)
        entry |= HEAP_BLOCK_HAS_NEXT;

    for (int i = start_block; i <= end_block; i++) {
        heap->table->entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if (i != end_block - 1)
            entry |= HEAP_BLOCK_HAS_NEXT;
    }

}

static void *heap_malloc_blocks(struct heap *heap, uint32_t total_blocks)
{
    void *address = NULL;
    int start_block = heap_get_start_block(heap, total_blocks);

    if (start_block < 0)
        return NULL;

    address = heap_block_to_address(heap, start_block);
    heap_mark_blocks_taken(heap, start_block, total_blocks);

    return address;
}

int heap_create(struct heap *heap, void *ptr, void *end, struct heap_table *table)
{
    if (!heap_validate_alignment(ptr) || !heap_validate_alignment(end))
        return -EINVARG;

    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    if (!heap_validate_table(ptr, end, table))
        return -EINVARG;

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);
    
    return 0;
}

void *heap_malloc(struct heap *heap, size_t size)
{
    size_t aligned_size = heap_align_up(size);
    uint32_t total_blocks = aligned_size / PEACHOS_HEAP_BLOCK_SIZE;

    return heap_malloc_blocks(heap, total_blocks);
}

void heap_free(struct heap *heap, void *ptr)
{
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));
}