#ifndef _WPACKET_H
#define _WPACKET_H
#include "buffer.h"
#include "link_list.h"
#include <stdint.h>
#include "allocator.h"
#include "sync.h"

struct wpacket;
typedef void (*packet_send_finish)(void*,struct wpacket*);

typedef struct wpacket {
    list_node next;
    uint8_t   type;
    uint32_t *len;      
    buffer_t buf;           
    buffer_t writebuf;       
    uint32_t wpos;
    uint8_t factor;
    uint32_t begin_pos; 
    uint32_t data_size;
    uint8_t  raw;
    uint8_t  mt;
    uint64_t usr_data;
    packet_send_finish _packet_send_finish;
    allocator_t allocator;
    uint32_t send_tick;
}*wpacket_t;
struct rpacket;


typedef struct {
    buffer_t buf;
    uint32_t wpos;
} write_pos;

wpacket_t wpacket_create(uint8_t mt,allocator_t _allo,uint32_t size,uint8_t is_raw);
wpacket_t wpacket_create_by_rpacket(allocator_t _allo,struct rpacket*);//通过rpacket构造
void wpacket_destroy(wpacket_t*);

write_pos wpacket_get_writepos(wpacket_t);
void wpacket_write_uint8(wpacket_t,uint8_t);
void wpacket_write_uint16(wpacket_t,uint16_t);
void wpacket_write_uint32(wpacket_t,uint32_t);
void wpacket_write_uint64(wpacket_t,uint64_t);
void wpacket_write_double(wpacket_t,double);

void wpacket_rewrite_uint8(write_pos*,uint8_t);
void wpacket_rewrite_uint16(write_pos*,uint16_t);
void wpacket_rewrite_uint32(write_pos*,uint32_t);
void wpacket_rewrite_uint64(write_pos*,uint64_t);
void wpacket_rewrite_double(write_pos*,double);

//不提供对非定长数据的rewrite
void wpacket_write_string(wpacket_t,const char*);
void wpacket_write_binary(wpacket_t,const void*,uint32_t);

#endif
