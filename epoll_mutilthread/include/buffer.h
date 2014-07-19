#ifndef _BUFFER_H
#define _BUFFER_H

#include "KendyNet.h"
#include <stdint.h>
#include "refbase.h"
#include "sync.h"
#include "link_list.h"
#include "spinlock.h"

typedef struct buffer {
    struct refbase _refbase;
    uint32_t capacity;
    uint32_t size;
    struct buffer *next;
    int8_t   buf[0];
}*buffer_t;


inline buffer_t buffer_create_and_acquire(uint8_t mt,buffer_t,uint32_t);
inline buffer_t buffer_acquire(buffer_t,buffer_t);
inline void     buffer_release(buffer_t*);
int32_t  buffer_read(buffer_t,uint32_t,int8_t*,uint32_t);

/*
struct _mem_block
{
	list_node next;
	void *mem_block;
};

struct buffer_block
{
	list_node next;
};

struct buffer_mgr
{
	int32_t factor;
	int32_t free_size;
	list_node *head;
	list_node *tail;
	struct link_list *blocks;
	int32_t create_block_size;
	//mutex_t mtx;
	spinlock_t mtx;
};

typedef struct buffer_allocator
{
	IMPLEMEMT(allocator);
	struct buffer_mgr bf_mgr[11];//2^5~2^16
}*buffer_allocator_t;


buffer_allocator_t create_buffer_allocator(int8_t mt);
*/

#endif
