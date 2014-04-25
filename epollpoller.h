/*
 * =====================================================================================
 *
 *       Filename:  epollpoller.h
 *        Version:  1.0
 *        Created:  2014年04月20日 18时37分46秒
 *       Revision:  none
 *       Compiler:  clang
 *         Author:  sim szm, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */
#ifndef EPOLLPOLLER_H
#define EPOLLPOLLER_H

#include <assert>
#include <sys/epoll.h>
#include <map>
#include <vector>

class epollpoller{
public:
	epollpoller();
	virtual ~epollpoller();
	
private:
	typedef std::vector<struct epoll_event> event_list;

	int m_epollfd;
	event_list m_events;
};

#endif 
