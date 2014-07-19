#ifndef _SOCKETWRAPPER_H
#define _SOCKETWRAPPER_H

#include "Engine.h"
#include "spinlock.h"
typedef struct socket_wrapper {
    //mutex_t  mtx;
    spinlock_t mtx;
    volatile int status;
    engine_t  engine;

    volatile int readable;
    volatile int writeable;
    volatile int active_read_count;
    volatile int active_write_count;
    int fd;

    struct link_list *pending_send;
    struct link_list *pending_recv;

    spinlock_t   recv_mtx;
    spinlock_t   send_mtx;

}*socket_t;


void on_read_active(socket_t);
void on_write_active(socket_t);
socket_t create_socket();
void free_socket(socket_t*);

enum {
    NOTIFY_ON_COMPLETE = 1,
    SLIENT_ON_COMPLETE = 0,
};

#ifdef _POLL
int _recv(socket_t,st_io*,int);
int _send(socket_t,st_io*,int);
#else
int _recv(socket_t,st_io*,int,int notify);
int _send(socket_t,st_io*,int,int notify);
#endif

#endif
