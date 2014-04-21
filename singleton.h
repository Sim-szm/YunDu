/*
 * =====================================================================================
 *
 *       Filename:  singleton.h
 *        Version:  1.0
 *        Created:  2014年04月20日 15时07分23秒
 *       Revision:  none
 *       Compiler:  clang
 *         Author:  sim szm, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */
#ifndef SINGLETON_H
#define SINGLETON_H

#include <pthread.h>
#include <stdlib.h> 

tmplate<typename T>
class singleton{
public:
	static T&instance(){
		pthread_once(&m_once,&singleton::init);
		return *m_value;
	}
private:
	static T* m_value;
	static pthread_once_t m_once;
	singleton(){
	}
	~singleton(){
	}
	static void init(){
		m_value=new T();
		::atexit(destory);
	}
	static void destory(){
		delete m_value;
	}
};

template<typename T>
pthread_once_t singleton<T>::m_once=PTHREAD_ONCE_INIT;

template<typename T>
T* singleton<T>::m_value=NULL;

#endif

