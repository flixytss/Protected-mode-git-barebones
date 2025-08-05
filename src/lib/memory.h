#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_SIZE 1024*8
#define NULL ((void*)0)
#define BLOCK_SIZE sizeof(Block)

typedef unsigned long size_t;

static char memory[MEMORY_SIZE];

typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

static Block* free_list = (Block*)memory;

static inline void init_mem(){
    free_list->size=MEMORY_SIZE-BLOCK_SIZE;
    free_list->free=1;
    free_list->next=(Block*)NULL;
}
static inline void* malloc(size_t size) {
    Block* curr = free_list;
    while (curr != NULL) {
        if (curr->free && curr->size >= size) {
            if (curr->size >= size + BLOCK_SIZE + 1) {
                Block* new_block = (Block*)((char*)curr + BLOCK_SIZE + size);
                new_block->size = curr->size - size - BLOCK_SIZE;
                new_block->free = 1;
                new_block->next = curr->next;
                curr->next = new_block;
                curr->size = size;
            }

            curr->free = 0;
            return (char*)curr + BLOCK_SIZE;
        }
        curr = curr->next;
    }
    return NULL;
}
static inline void free(void* ptr) {
    if (!ptr) return;

    Block* block = (Block*)((char*)ptr - BLOCK_SIZE);
    block->free = 1;

    Block* curr = free_list;
    while (curr != NULL && curr->next != NULL) {
        if (curr->free && curr->next->free) {
            curr->size += BLOCK_SIZE + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

#endif