#ifndef _SYNC_H
#define _SYNC_H
#include <pthread.h>
#include <stdio.h>

/*Mutex*/
struct mutex;
typedef struct mutex *mutex_t;

mutex_t mutex_create();
void mutex_destroy(mutex_t *m);
inline int mutex_lock(mutex_t m);
inline int mutex_try_lock(mutex_t m);
inline int mutex_unlock(mutex_t m);

/*Condition*/
struct condition;
typedef struct condition *condition_t;

condition_t condition_create();
void condition_destroy(condition_t *c);
inline int condition_wait(condition_t c,mutex_t m);
int condition_timedwait(condition_t c,mutex_t m,long ms);
inline int condition_signal(condition_t c);
inline int condition_broadcast(condition_t c);

/*Barrior*/
struct barrior;
typedef struct barrior *barrior_t;

barrior_t barrior_create(int);
void barrior_destroy(barrior_t*);
void barrior_wait(barrior_t);

#endif
