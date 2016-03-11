#pragma once
#include <iostream>
#include <functional>
#include <thread>
#include <string>

#include "ThreadPool.h"

using namespace std;
enum THREADAPITYPE
{
	NO_THREAD,
	ADHOC_THREAD,
	THREAD_POOL,
	THREADAPITYPECOUNT
};

template<THREADAPITYPE T>
class ThreadAPI
{
	int m_nb;
public:

	ThreadAPI(int nb) { static_assert(false, "Unhandled Value"); }
	~ThreadAPI() = default;
	template<class f, class ... Args>
	void execute(f&& func, Args && ... args){}

};

template<>
class ThreadAPI<THREADAPITYPE::NO_THREAD>
{
	int m_nb;
public:

	ThreadAPI() {  }
	~ThreadAPI() = default;
	template<class f,class ... Args>
	void execute(f&& func , Args && ... args)
	{
		func(forward<Args>(args)...);
	}

};

template<>
class ThreadAPI<THREADAPITYPE::ADHOC_THREAD>
{
	int m_nb;
public:

	ThreadAPI() { static_assert(true, "NO_THREAD Value"); }
	~ThreadAPI() = default;
	template<class f, class ... Args>
	thread execute(f&& func, Args && ... args)
	{
		return thread(func,forward<Args>(args)...);
	}

};

template<>
class ThreadAPI<THREADAPITYPE::THREAD_POOL>
{
	ThreadPool m_TP;
public:

	ThreadAPI(unsigned int max_thread = thread::hardware_concurrency()) :m_TP{ max_thread } {  }
	~ThreadAPI() = default;
	template<class f>
	void execute(f&& func)
	{
		m_TP.addTask(func);
	}
	void wait() 
	{
		m_TP.halt();
		m_TP.wait_end(); 
	}

};

//template<THREADAPITYPE T>
//ThreadAPI<T>::ThreadAPI(int nb) :m_nb(0)
//{
//	Initialize(nb);
//	cout << "Random" << endl;
//}
//
//template<THREADAPITYPE T>
//void ThreadAPI<T>::Initialize(int nb)
//{
//	static_assert(true, "Unhandled Value");
//}
//
////template<>
////void ThreadAPI<THREADAPITYPE::NO_THREAD>::Initialize(int nb)
////{
////	cout << "NO_THREAD" << endl;
////}
////
////template<>
////void ThreadAPI<THREADAPITYPE::ADHOC_THREAD>::Initialize(int nb)
////{
////	cout << "ADHOC_THREAD" << endl;
////
////}
////
////template<>
////void ThreadAPI<THREADAPITYPE::THREAD_POOL>::Initialize(int nb)
////{
////	cout << "THREAD_POOL" << endl;
////
////}
//
//template<THREADAPITYPE T>
//void ThreadAPI<T>::execute()
//{
//
//	cout << "exe" << endl;
//}
//
//template<THREADAPITYPE T>
//int ThreadAPI<T>::get()
//{
//	cout << "random get" << endl;
//	return m_nb;
//}
//
//template<>
//int ThreadAPI<THREADAPITYPE::NO_THREAD>::get()
//{
//	cout << "NO_THREAD get" << endl;
//	return m_nb;
//}
//
//template<THREADAPITYPE T>
//ThreadAPI<T>::~ThreadAPI()
//{
//}