#ifndef _ATOMIC_H
#define _ATOMIC_H
#include <stdint.h>

typedef volatile int8_t  atomic_8_t;
typedef volatile int16_t atomic_16_t;
typedef volatile int32_t atomic_32_t;
typedef volatile int64_t atomic_64_t;

#define COMPARE_AND_SWAP(PTR,OLD,NEW) __sync_val_compare_and_swap(PTR,OLD,NEW)
#define ATOMIC_INCREASE(PTR) __sync_add_and_fetch(PTR,1) 
#define ATOMIC_DECREASE(PTR) __sync_sub_and_fetch(PTR,1) 

#endif
