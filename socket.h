/*
 * =====================================================================================
 *
 *       Filename:  socket.h
 *        Version:  1.0
 *        Created:  2014年04月20日 15时36分37秒
 *       Revision:  none
 *       Compiler:  clang
 *         Author:  sim szm, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */
#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>


class netaddress;
class socket_event{
public:
	explicit socket_event(int sockfd):m_sockfd(sockfd){
	}
	~socket_event(){
		close(m_sockfd);
	}
	int get_sockfd() const{
		return m_sockfd;
	}
	int socket_t();
	void set_reuseaddr(bool on);
	void bind_t(const netaddress& localaddr);
	void listen_t();
	int accept_t(netaddress* address);
	void shutdown_write();

private:
	const int m_sockfd;
};
#endif 

