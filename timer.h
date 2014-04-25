/*
 * =====================================================================================
 *
 *       Filename:  timer.h
 *        Version:  1.0
 *        Created:  2014年04月25日 14时54分31秒
 *       Revision:  none
 *       Compiler:  clang
 *         Author:  sim szm, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#ifndef TIME_H
#define TIME_H

#include <unistd.h>
#include <assert.h>
#include <iostream>
#include <netinet/in.h>
#include <time.h>

class timer_t;

typedef struct{
	char *buf;
	uint32_t size;
	uint32_t len;
}xbuffer_t;

typedef struct cli_data{
	int sockfd;
	struct sockaddr_in address;
	xbuffer_t buffer;
	timer_t* timer;
}cli_data_t;

class timer_t{
public:
	timer_t(int delay){
		expire=time(NULL)+delay;
	}
public:
	time_t expire;
	void (*callback_func)(cli_data_t*);
	cli_data_t* user_data;
};

class time_heap{
public:
	time_heap();
private:
	timer_t** array;
	int capacity_t;
	int cur_size;
};
