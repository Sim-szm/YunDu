/*
 * =====================================================================================
 *
 *       Filename:  condition.h
 *        Version:  1.0
 *        Created:  2014年04月20日 14时25分22秒
 *       Revision:  none
 *       Compiler:  clang
 *         Author:  sim szm, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */
#ifndef CONDITION_H
#define CONDITION_h

#include <pthread.h>
#include "mutex.h"

class condition{
public:
	condition(mutex_lock& mutex):m_mutex(mutex){
		pthread_cond_init(&m_cond,NULL);
	}
	~condition();
	void condition_wait();
	void condition_notify();
	void condition_notifyAll();
private:
	pthread_cond_t m_cond;
	mutex_lock& m_mutex;
};

condition::~condition(){
	pthread_cond_destroy(&m_cond);
}

void condition::condition_wait(){
	pthread_cond_wait(&m_cond,m_mutex.get_mutex());
}

void condition::condition_notify(){
	pthread_cond_signal(&m_cond);
}

void condition::condition_notifyAll(){
	pthread_cond_broadcast(&m_cond);
}

#endif

