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

class heap_timer;

typedef struct{
	char *buf;
	uint32_t size;
	uint32_t len;
}xbuffer_t;

typedef struct cli_data{
	int sockfd;
	struct sockaddr_in address;
	xbuffer_t buffer;
	heap_timer* timer;
}cli_data_t;

class heap_timer{
public:
	heap_timer(int delay){
		expire=time(NULL)+delay;
	}
	heap_timer(){
		expire=time(NULL);
	}
public:
	time_t expire;
	void (*callback_func)(cli_data_t*);
	cli_data_t* user_data;
};

class time_heap{
public:
	time_heap(int capacity):capacity_t(capacity),cur_size(0){
		array=new heap_timer* [capacity_t];
		assert(array!=NULL);
		for(int i=0;i<capacity_t;i++){
			array[i]=NULL;
		}
	}

	time_heap(heap_timer** init,int size ,int capacity)
		:capacity_t(capacity),cur_size(size){
		array=new heap_timer* [capacity_t];
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

	void add_timer(heap_timer* timer);
	
	void delete_timer(heap_timer* timer);

	heap_timer* get_top()const{
		if(cur_size==0){
			return NULL;
		}
		return array[0];
	}

	void pop_timer();

	void tick();

private:
	heap_timer** array;

	int capacity_t;

	int cur_size;

	void percolate(int hole_index);

	void resize_heap();
};

void time_heap::add_timer(heap_timer* timer){
	assert(timer!=NULL);
	if(capacity_t <= cur_size){
		resize_heap();
	}
	int hole_index=cur_size++;
	int parent=0;
	for(; hole_index>0;hole_index=parent){
		parent=(hole_index-1)/2;
		if((array[parent]->expire) <=( timer->expire)){
			break;
		}
		array[hole_index]=array[parent];
	}
	array[hole_index]=timer;
}

void time_heap::delete_timer(heap_timer* timer){
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
	heap_timer* tmp=array[0];
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

void time_heap::percolate(int hole_index){
	heap_timer* tmp=array[hole_index];
	int child=0;
	for( ; ((hole_index*2+1)<=(cur_size-1)) ; hole_index=child){
		child=hole_index*2+1;
		if( (child<(cur_size-1)) && \
					(array[child+1]->expire < array[child]->expire)){
			++child;
		}
		if(array[child]->expire < tmp->expire){
			array[hole_index]=array[child];
		}
		else
		      break;
	}
	array[hole_index]=tmp;
}

void time_heap::resize_heap(){
	heap_timer** tmp=new heap_timer* [capacity_t*2];
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
