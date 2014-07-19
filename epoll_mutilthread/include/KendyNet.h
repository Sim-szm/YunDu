#ifndef _KENDYNET_H
#define _KENDYNET_H
#include <stdint.h>
typedef struct list_node {
    struct list_node *next;
} list_node;

#define LIST_NODE list_node node;

typedef struct {
    LIST_NODE;
    struct iovec *iovec;
    int    iovec_count;
    int    bytes_transfer;
    int    error_code;
} st_io;

int      InitNetSystem();

typedef  void* HANDLE;
HANDLE   CreateEngine();
void     CloseEngine(HANDLE);
int      EngineRun(HANDLE);
void     StopEngine(HANDLE);

int      Bind2Engine(HANDLE,HANDLE);


enum {
    NO_WAIT = -1,
    INFINITE = 0,
};

int      GetQueueEvent(HANDLE,st_io **,int timeout);
int      PutQueueEvent(HANDLE,st_io *);

enum {
    RECV_NOW = 1,
    RECV_POST = 0,
    SEND_NOW = 1,
    SEND_POST = 0,
};

#ifdef _POLL
int WSASend(HANDLE,st_io*,int32_t flag);
int WSARecv(HANDLE,st_io*,int32_t flag);
#else
int WSASend(HANDLE,st_io*);
int WSARecv(HANDLE,st_io*);
#endif


#endif
