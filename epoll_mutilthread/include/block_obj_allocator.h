#ifndef _BLOCK_OBJ_ALLOCATOR
#define _BLOCK_OBJ_ALLOCATOR

#include <stdint.h>

#define DEFAULT_BLOCK_SIZE 1024*1024

typedef struct block_obj_allocator *block_obj_allocator_t;

block_obj_allocator_t create_block_obj_allocator(uint8_t mt,uint32_t obj_size);

#endif
