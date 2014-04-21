/*
 * =====================================================================================
 *
 *       Filename:  socket.cpp
 *        Version:  1.0
 *        Created:  2014年04月20日 15时51分24秒
 *       Revision:  none
 *       Compiler:  clang
 *         Author:  sim szm, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include "socket.h"
#include <errno.h>
#include <unistd.h>

#define BACKLOG 100
#define NOT_USE_NONBLOCK 0
typedef struct sockaddr SA;

SA* sockaddr_cast(struct sockaddr_in* addr){
	return static_cast<SA*>(implicit_cast<void*>addr);
}

void set_nonblocking(int sockfd){
	int flags=fcntl(sockfd,F_GETFL,0);
	flags|=O_NONBLOCK;
	int ret=fcntl(sockfd,F_SETFL,flags);
}

void set_close_onexec(int sockfd){
	int flags=fcntl(sockfd,F_GETFD,0);
	flags|=FD_CLOEXEC;
	int ret=fcntl(sockfd,F_SETFD,flags);
}

socket_event::~socket_event(){
	close(m_sockfd);
}

int socket_event::socket_t(){
	int sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd<0){
		perror("socket");
	}
	set_nonblocking(sockfd);
	set_close_onexec(sockfd);
	return sockfd;
}

void socket_event::set_reuseaddr(book on){
	int opt=on ? 1:0 ;
	setsockopt(&m_sockfd,SOL_SOCKET,SO_REUSEADDR,\
				&opt,sizeof(opt));
}

void socket_event::bind_t(const netaddress& localaddr){
	int ret=bind(m_sockfd,sockaddr_cast(&(localaddr.get_sock_netaddress())),\
				sizeof(struct sockaddr) );
	if(ret<0){
		perror("socket bind error");
	}
}

void socket_event::listen_t(){
	int ret=listen(m_sockfd,BACKLOG);
	if(ret<0){
		perror("listen error");
	}
}

int socket_event::accept_t(netaddress* address){
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	socklen_t addrlen=sizeof(addr);
#if NOT_USE_NONBLOCK
	int connfd=accept(m_sockfd,sockaddr_cast(addr),&addrlen);
	set_nonblocking(connfd);
	set_close_onexec(connfd);
#else
	int connfd=accept4(m_sockfd,sockaddr_cast(addr),\
				&addrlen,SOCK_NONBLOCK|SOCK_CLOEXEC);
#endif
	if (connfd<0){
		perror("error");
	}
	if(connfd>=0){
		address->set_sock_netaddress(addr);
	}
	return connfd;
}

void shutdown_write(){
	if(shutdown(m_sockfd,SHUT_WR)<0){
		perror("shutdown_write");
	}
}
