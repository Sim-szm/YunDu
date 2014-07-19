
#ifndef _SOCK_WRAPPER_H
#define _SOCK_WRAPPER_H
 
#include <sys/types.h>	
#include <sys/socket.h>	
#include <sys/time.h>	
#include <time.h>		
#include <netinet/in.h>	
#include <arpa/inet.h>	
#include <errno.h>
#include <fcntl.h>		
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>	
#include <sys/uio.h>		
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>		/* for Unix domain sockets */
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <fcntl.h>

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(expression)\
   ({ long int __result;\
       do __result = (long int)(expression);\
       while(__result == -1L&& errno == EINTR);\
       __result;})
#endif

enum sock_family {
    INET = AF_INET,
    INET6 = AF_INET6,
    LOCAL = AF_LOCAL,
    ROUTE = AF_ROUTE,
#ifdef _LINUX
    KEY = AF_KEY,
#endif
};

enum sock_type {
    STREAM = SOCK_STREAM,
    DGRAM  = SOCK_STREAM,
    SEQPACKET = SOCK_SEQPACKET,
    RAW = SOCK_RAW,
};

enum sock_protocol {
    TCP = IPPROTO_TCP,
    UDP = IPPROTO_UDP,
    SCTP = IPPROTO_SCTP,
};

//typedef sock_wrapper *socket_t;
#include "KendyNet.h"
HANDLE  OpenSocket(int family,int type,int protocol);

int  CloseSocket(HANDLE);

int Connect(HANDLE sock,const struct sockaddr *servaddr,socklen_t addrlen);

HANDLE Tcp_Connect(const char *ip,unsigned short port,struct sockaddr_in *servaddr,int retry);

int Bind(HANDLE sock,const struct sockaddr *myaddr,socklen_t addrlen);

int Listen(HANDLE sock,int backlog);

HANDLE Tcp_Listen(const char *ip,unsigned short port,struct sockaddr_in *servaddr,int backlog);


HANDLE Accept(HANDLE,struct sockaddr *sa,socklen_t *salen);

int getRemoteAddrPort(HANDLE sock,char *buf,unsigned short *port);
int getLocalAddrPort(HANDLE sock,struct sockaddr_in *remoAddr,socklen_t *len,char *buf,unsigned short *port);

/*
ssize_t write_fd(int fd,void *ptr,size_t nbytes,int sendfd);

int create_un_execl(const char *path,const char *child);

ssize_t read_fd(int fd,void *ptr,size_t nbytes,int *recvfd);
*/

struct hostent *Gethostbyaddr(const char *ip,int family);

int setNonblock(HANDLE sock);

#endif



