#ifndef _SPINLOCK_H
#define _SPINLOCK_H
#include "atomic.h"
#include <pthread.h>
typedef struct spinlock *spinlock_t;

spinlock_t spin_create();
void spin_destroy(spinlock_t*);
inline int32_t spin_lock(spinlock_t);
inline int32_t spin_unlock(spinlock_t);
#endif
