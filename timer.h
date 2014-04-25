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
	time_heap(int capacity):capacity_t(capacity),cur_size(0){
		array=new timer_t* [capacity_t];
		assert(array!=NULL);
		for(int i=0;i<capacity_t;i++){
			array[i]=NULL;
		}
	}

	time_heap(timer_t** init,int size ,int capacity):\
		capacity_t(capacity),cur_size(size){
		array=new timer_t* [capacity_t];
		assert(array!=NULL);
		for(int i=0;i<capacity_t;i++){
			array[i]=NULL;
		}
		if(size!=0){
			for(int i=0;i<size;i++){
				array[i]=init[i];
			}
			for(int i=(cur_size-1)/2;i>=0;i--){
				//......
			}
		}
	}

	~time_heap(){
		for(int i=0;i<cur_size;i++){
			delete array[i];
		}
		delete [] array;
	}

	void add_timer(timer_t* timer);
	
	void delete_timer(timer_t* timer);

	timer_t* get_top()const{
		if(cur_size==0){
			return NULL;
		}
		return array[0];
	}

	void pop_timer();

	void tick();

private:
	timer_t** array;

	int capacity_t;

	int cur_size;

	void percolate(int hole);

	void resize_heap();
};

#endif
