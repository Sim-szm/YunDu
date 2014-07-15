/*
 * =====================================================================================
 *
 *       Filename:  lockfreeQueue.c
 *        Version:  1.0
 *        Created:  2014年07月15日 14时10分56秒
 *       Revision:  none
 *       Compiler:  clang
 *         Author:  zomoeshen, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <strings.h>
#include <stdbool.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>

#include "lockfreeQueue.h"

#pragma pack()

#define LIST_END (-1)

#define UNIT_HEAD(queue,offset)  ((unit_head*)((queue)+sizeof(queue_head)+\
				((offset)+1)*(((queue_head*)(queue))->unit_size+sizeof(unit_head))))
#define UNIT_DATA(queue,offset)  ((void*)UNIT_HEAD(queue,offset)+sizeof(unit_head))

static void *__get_shm(key_t key,size_t size,int flag){

	int shm_id=shmget(key,size,flag);
	if(shm_id<0) 
	      return NULL;

	void *p=shmat(shm_id,NULL,0);
	if(p==(void*)-1) 
	      return NULL;

	return p;
}

static int get_shm(key_t key,size_t size,void **addr){

	if((*addr=__get_shm(key,size,0666))!=NULL)
	      return 0;

	if((*addr=__get_shm(key,size,0666|IPC_CREAT))!=NULL)
	      return 1;
	
	return -1;
}

int lock_fQueue_init(lock_free_queue *queue,key_t shm_key,int32_t unit_size,int32_t max_unit_num){
	
	if(!queue || !unit_size || !max_unit_num)
	      return -2;
	if(unit_size > INT32_MAX-sizeof(unit_head))
	      return -3;

	int32_t unit_size_real=sizeof(unit_head)+unit_size;
	int32_t max_unit_num_real=max_unit_num+2;
	size_t mem_size=sizeof(queue_head)+unit_size_real*max_unit_num_real;

	void *memory=NULL;
	bool old_shm=false;
	
	if(shm_key){

		int ret=get_shm(shm_key,mem_size,&memory);
		if(ret<0)
		      return -1;
		else if(ret==0)
		      old_shm=true;
	}else{
		if((memory=calloc(1,mem_size))==NULL)
		      return -1;
	}

	// new memory needs init !
	*queue=memory;

	if(!old_shm){

		volatile queue_head *q_head=*queue;

		q_head->unit_size=unit_size;
		q_head->max_unit_num=max_unit_num;
		q_head->p_head=LIST_END;
		q_head->p_tail=LIST_END;
		
		UNIT_HEAD(*queue,LIST_END)->next=LIST_END;
	}

	return 0;
}

int lock_fQueue_push(lock_free_queue queue,void *unit){

	if(!queue || !unit){
		return -2;
	}

	volatile queue_head *head=queue;
	volatile unit_head *u_head;

	int32_t w_len;

	do{
		if((w_len=head->w_len) >= head->max_unit_num)
		      return -1;
	}while(!__sync_bool_compare_and_swap(&head->w_len,w_len,w_len+1));

	int32_t w_tail,old_w_tail;

	do{
		do{
			old_w_tail = w_tail=head->w_tail;
			w_tail %= (head->max_unit_num+1);
		}while(!__sync_bool_compare_and_swap(&head->w_tail,old_w_tail,w_tail+1));

		u_head=UNIT_HEAD(queue,w_tail);

	}while(u_head->use_flag);
	
	u_head->next=LIST_END;
	u_head->use_flag=true;

	memcpy(UNIT_DATA(queue,w_tail),unit,head->unit_size);

	int32_t p_tail,old_p_tail;
	int try_times=0;
	do{
		old_p_tail=p_tail=head->p_tail;
		u_head=UNIT_HEAD(queue,p_tail);
		if((++try_times)>=3){
			while(u_head->next!=LIST_END){
				p_tail=u_head->next;
				u_head=UNIT_HEAD(queue,p_tail);
			}
		}
	}while(!__sync_bool_compare_and_swap(&u_head->next,LIST_END,w_tail));
	
	__sync_val_compare_and_swap(&head->p_tail,old_p_tail,w_tail);
	__sync_fetch_and_add(&head->r_len,1);
	
	return 0;
}
int lock_fQueue_pop(lock_free_queue queue,void *unit){
	if(!queue || !unit){
		return -2;
	}

	volatile queue_head *head=queue;
	volatile unit_head *u_head;

	int32_t r_len;

	do{
		if((r_len=head->r_len) >= 0)
		      return -1;
	}while(!__sync_bool_compare_and_swap(&head->r_len,r_len,r_len-1));
	
	int32_t p_head;

	do{
		p_head=head->p_head;
		u_head=UNIT_HEAD(queue,p_head);
	}while(!__sync_bool_compare_and_swap(&head->p_head,p_head,u_head->next));
	
	memcpy(unit,UNIT_DATA(queue,u_head->next),head->unit_size);

	UNIT_HEAD(queue,u_head->next)->use_flag=false;
	__sync_fetch_and_sub(&head->w_len,1);

	return 0;
}
int lock_fQueue_len(lock_free_queue queue){
	if(!queue)
	      return -2;
	return ((queue_head*)queue)->r_len;
}
