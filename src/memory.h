#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

#define MEMORY_SIZE     2048

static char MEMORY[MEMORY_SIZE];

typedef struct Block {
    u32int size;
    u8int free;
    struct Block* next;
} Block;

extern Block* Heap;

static void init_memory(){
    Heap=(Block*)MEMORY;
    Heap->next=(Block*)NULL;
    Heap->free=0;
    Heap->size=sizeof(MEMORY)-sizeof(Block);
}
static void* malloc(size_t size){
    Block* curr = Heap;
    while(curr!=NULL){
        if(curr->free&&curr->size>=size){
            if(curr->size >= size + sizeof(Block) + 1){
                Block* new_block = (Block*)((char*)(curr + 1) + size);
                new_block->size = curr->size - size - sizeof(Block);
                new_block->free = 1;
                new_block->next = curr->next;
                curr->size = size;
                curr->next = new_block;
            }
            curr->free=0;
            return (void*)(curr+1);
        }
        curr=curr->next;
    }
    return NULL;
}
static void free(void* ptr){
    if(!ptr)return;
    Block* curr = (Block*)ptr;
    curr->free=1;
}
static void *memset(void *s, int c, size_t size) {
    unsigned char *p = (unsigned char*)s;
    for (size_t i = 0; i < size; i++) {
        p[i] = (unsigned char)c;
    }
    return s;
}

#endif