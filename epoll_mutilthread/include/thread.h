#ifndef _THREAD_H
#define _THREAD_H
#include <pthread.h>

struct thread;
typedef struct thread *thread_t;

typedef void *(*thread_routine)(void*);



thread_t create_thread(int);
void destroy_thread(thread_t*);
void* join(thread_t);
void start_run(thread_t,thread_routine,void*);

#ifndef CREATE_THREAD_RUN
#define CREATE_THREAD_RUN(JOINABLE,ROUTINE,ARG)\
({thread_t __t;__t =create_thread(JOINABLE);\
  start_run(__t,ROUTINE,ARG);__t;})
#endif

void  thread_run(thread_routine,void*);
void thread_suspend(thread_t,int);
void thread_resume(thread_t);


#endif
