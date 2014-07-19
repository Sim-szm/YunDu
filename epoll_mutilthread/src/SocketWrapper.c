#include "SocketWrapper.h"
#include "Socket.h"
#include "HandleMgr.h"

static void InitSocket(HANDLE sock,int fd) {
    socket_t s = GetSocketByHandle(sock);
    s->fd = fd;
    LINK_LIST_CLEAR(s->pending_send);
    LINK_LIST_CLEAR(s->pending_recv);
    s->readable = s->writeable = 0;
    s->active_read_count = s->active_write_count = 0;
    s->engine = 0;
}

HANDLE OpenSocket(int family,int type,int protocol) {
    int sockfd;
    if( (sockfd = socket(family,type,protocol)) < 0) {
        return NULL;
    }
    HANDLE sock = NewSocketWrapper();
    if(sock < 0) {
        close(sockfd);
        return NULL;
    }
    InitSocket(sock,sockfd);
    return sock;
}

int CloseSocket(HANDLE sock) {
    return ReleaseSocketWrapper(sock);
}

int Connect(HANDLE sock,const struct sockaddr *servaddr,socklen_t addrlen) {
    socket_t s = GetSocketByHandle(sock);
    if(s) {
        if(connect(s->fd,servaddr,addrlen) < 0) {
            printf("%s\n",strerror(errno));
            return -1;
        }
        return 0;
    }
    return -1;

}

HANDLE Tcp_Connect(const char *ip,unsigned short port,struct sockaddr_in *servaddr,int retry) {
    if(!ip)
        return NULL;

    bzero(servaddr,sizeof(*servaddr));
    servaddr->sin_family = INET;
    servaddr->sin_port = htons(port);
    if(inet_pton(INET,ip,&servaddr->sin_addr) < 0) {

        printf("%s\n",strerror(errno));
        return NULL;
    }

    HANDLE sock = OpenSocket(INET,STREAM,TCP);
    if(sock) {
        while(1) {
            if(0 == Connect(sock,(struct sockaddr*)servaddr,sizeof(*servaddr)))
                return sock;
            if(!retry)
                break;
        }
        CloseSocket(sock);
    }
    return NULL;
}

int Bind(HANDLE sock,const struct sockaddr *myaddr,socklen_t addrlen) {
    socket_t s = GetSocketByHandle(sock);
    if(s) {
        if(bind(s->fd,myaddr,addrlen) < 0) {
            printf("%s\n",strerror(errno));
            return -1;
        }
        return 0;
    }
    return -1;
}

int Listen(HANDLE sock,int backlog) {
    socket_t s = GetSocketByHandle(sock);
    if(s) {
        if(listen(s->fd,backlog) < 0) {
            printf("%s\n",strerror(errno));
            return -1;
        }
        return 0;
    }
    return -1;
}

HANDLE Tcp_Listen(const char *ip,unsigned short port,struct sockaddr_in *servaddr,int backlog)

{
    HANDLE sock;
    sock = OpenSocket(INET,STREAM,TCP);
    if(sock < 0)
        return NULL;

    bzero(servaddr,sizeof(*servaddr));
    servaddr->sin_family = INET;
    if(ip) {
        if(inet_pton(INET,ip,&servaddr->sin_addr) < 0) {

            printf("%s\n",strerror(errno));
            return NULL;
        }
    } else
        servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr->sin_port = htons(port);

    if(Bind(sock,(struct sockaddr*)servaddr,sizeof(*servaddr)) < 0) {
        CloseSocket(sock);
        return NULL;
    }

    if(Listen(sock,backlog) == 0)
        return sock;
    else {
        CloseSocket(sock);
        return NULL;
    }
}

HANDLE Accept(HANDLE sock,struct sockaddr *sa,socklen_t *salen) {
    socket_t s = GetSocketByHandle(sock);
    if(s) {
        int n;
again:
        if((n = accept(s->fd,sa,salen)) < 0) {
#ifdef EPROTO
            if(errno == EPROTO || errno == ECONNABORTED)
#else
            if(errno == ECONNABORTED)
#endif
                goto again;
            else {
                printf("%s\n",strerror(errno));
                return NULL;
            }
        }
        HANDLE newsock = NewSocketWrapper();
        if(newsock < 0) {
            close(n);
            return NULL;
        }
        InitSocket(newsock,n);
        return newsock;
    }
    return NULL;
}

int getLocalAddrPort(HANDLE sock,struct sockaddr_in *remoAddr,socklen_t *len,char *buf,unsigned short *port) {

    socket_t s = GetSocketByHandle(sock);
    if(s) {
        if(0 == buf)
            return -1;
        int ret = getsockname(s->fd, (struct sockaddr*)remoAddr,len);
        if(ret != 0)
            return -1;
        if(0 == inet_ntop(INET,&remoAddr->sin_addr,buf,15))
            return -1;
        *port = ntohs(remoAddr->sin_port);
        return 0;
    }
    return -1;
}


int getRemoteAddrPort(HANDLE sock,char *buf,unsigned short *port) {
    socket_t s = GetSocketByHandle(sock);
    if(s) {
        if(0 == buf)
            return -1;
        struct sockaddr_in remoAddr;
        memset(&remoAddr,0,sizeof(remoAddr));
        remoAddr.sin_family = INET;
        socklen_t len = sizeof(remoAddr);
        int ret = getpeername(s->fd,(struct sockaddr*)&remoAddr,&len);
        if(ret != 0)
            return -1;
        if(0 == inet_ntop(INET,&remoAddr.sin_addr,buf,15))
            return -1;
        *port = ntohs(remoAddr.sin_port);
        return 0;
    }
    return -1;
}
struct hostent *Gethostbyaddr(const char *ip,int family) {

    if(!ip)
        return NULL;
    struct sockaddr_in servaddr;
    struct hostent	*hptr;

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if(inet_pton(family,ip,&servaddr.sin_addr) < 0) {
        return NULL;
    }
    if ( (hptr = gethostbyaddr(&servaddr.sin_addr,sizeof(servaddr.sin_addr),family)) == NULL) {
        return NULL;
    }

    return hptr;
}

int setNonblock(HANDLE sock) {

    socket_t s = GetSocketByHandle(sock);
    if(s) {
        int fd_flags;
        int nodelay = 1;

        if (setsockopt(s->fd, IPPROTO_TCP, TCP_NODELAY, (void *)&nodelay, sizeof(nodelay)))
            return -1;

        fd_flags = fcntl(s->fd, F_GETFL, 0);

#if defined(O_NONBLOCK)
        fd_flags |= O_NONBLOCK;
#elif defined(O_NDELAY)
        fd_flags |= O_NDELAY;
#elif defined(FNDELAY)
        fd_flags |= O_FNDELAY;
#else
        /* XXXX: this breaks things, but an alternative isn't obvious...*/
        return -1;
#endif

        if (fcntl(s->fd, F_SETFL, fd_flags) == -1)
            return -1;

        return 0;
    }
    return -1;
}

