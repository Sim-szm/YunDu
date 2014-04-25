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

	time_heap(timer_t** init,int size ,int capacity)
		:capacity_t(capacity),cur_size(size){
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
				percolate(i);
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

	void percolate(int index);

	void resize_heap();
};

void time_heap::add_timer(timer_t* timer){
	assert(timer!=NULL);
	if(capacity_t <= cur_size){
		resize_heap();
	}
	int index=cur_size++;
	int parent=0;
	for(; index>0;index=parent){
		parent=(index-1)/2;
		if((array[parent]->expire) <=( timer->expire)){
			break;
		}
		array[index]=array[parent];
	}
	array[index]=timer;
}

void time_heap::delete_timer(timer_t* timer){
	assert(timer!=NULL);
	timer->callback_func=NULL;
	//just not callback ! not really delete !
}

void time_heap::pop_timer(){
	if(cur_size==0)
	      return ;
	if(NULL!=array[0]){
		delete array[0];
		array[0]=array[--cur_size];
		percolate(0);
	}
}

void time_heap::tick(){
	timer_t* tmp=array[0];
	time_t cur_time=time(NULL);
	while(cur_size!=0){
		if(!tmp)
		      break;
		if(tmp->expire>cur_time)
		      break;
		if(array[0]->callback_func){
			array[0]->callback_func(array[0]->user_data);
		}
		pop_timer();
		tmp=array[0];
	}
}

void time_heap::percolate(int index){
	timer_t* tmp=array[index];
	int child=0;
	for( ; ((index*2+1)<=(cur_size-1)) ; index=child){
		child=index*2+1;
		if( (child<(cur_size-1)) && \
					(array[child+1]->expire < array[child]->expire)){
			++child;
		}
		if(array[child]->expire < tmp->expire){
			array[index]=array[child];
		}
		else
		      break;
	}
	array[index]=tmp;
}

void time_heap::resize_heap(){
	timer_t** tmp=new timer_t* [capacity_t*2];
	for(int i=0;i<capacity_t*2;i++){
		tmp[i]=NULL;
	}
	assert(tmp!=NULL);
	capacity_t=capacity_t*2;
	for(int i=0;i<cur_size;i++){
		tmp[i]=array[i];
	}
	delete [] array;
	array=tmp;
}

#endif
