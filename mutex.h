/*
 * =====================================================================================
 *
 *       Filename:  mutex.h
 *        Version:  1.0
 *        Created:  2014年04月20日 14时05分32秒
 *       Revision:  none
 *       Compiler:  clang
 *         Author:  sim szm, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
*/
#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class mutex_lock{
public:
	mutex_lock();
	~mutex_lock();
	void lock_mutex();
	void unlock_mutex();
	pthread_mutex_t* get_mutex();
private:
	pthread_mutex_t m_mutex;
};

mutex_lock::mutex_lock(){
	pthread_mutex_init(&m_mutex,NULL);
}

mutex_lock::~mutex_lock(){
	pthread_mutex_destroy(&m_mutex);
}

void mutex_lock::lock_mutex(){
	pthread_mutex_lock(&m_mutex);
}

void mutex_lock::unlock_mutex(){
	pthread_mutex_unlock(&m_mutex);
}

pthread_mutex_t* mutex_lock::get_mutex(){
	return &m_mutex;
}

class mutex_lock_guard{
public:
	explicit mutex_lock_guard(mutex_lock& mutex):m_mutex(mutex){
		m_mutex.lock_mutex();
	}
	~mutex_lock_guard(){
		m_mutex.unlock_mutex();
	}
private:
	mutex_lock& m_mutex;
};

#endif


