/*
 * =====================================================================================
 *
 *       Filename:  netaddress.h
 *        Version:  1.0
 *        Created:  2014年04月20日 16时02分20秒
 *       Revision:  none
 *       Compiler:  clang
 *         Author:  sim szm, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */
#ifndef NETADDRESS_H
#define NETADDRESS_H

#include <iostream>
#include <netinet/in.h>

class netaddress{
public:
	explicit netaddress(uint16_t port);
	netaddress(const std::string&ip_addr,uint16_t port);
	netaddress(const struct sockaddr_in& addr):m_addr(addr){ 
	}
	const struct sockaddr_in& get_sock_netaddress() const{
		return m_addr;
	}
	void set_sock_netaddress(const struct sockaddr_in& addr){
		m_addr=addr;
	}
private:
	struct sockaddr_in m_addr;
};

netaddress::netaddress(uint16_t port){
	bzero(&m_addr,sizeof(m_addr));
	m_addr.sin_family=AF_INET;
	m_addr.sin_addr.s_addr=hontl(INADDR_ANY);
	m_addr.sin_port=honts(port);
}
netaddress::netaddress(const std::string&ip_addr,uint16_t port){
	bzero(&m_addr,sizeof(m_addr));
	m_addr.sin_family=AF_INET;
	m_addr.sin_port=honts(port);
	inet_pton(AF_INET,ip_addr.c_str(),&(m_addr.sin_addr));
}
#endif

