#pragma once

#include "..\pch.h"

template <typename Tag, typename T>
class Memory abstract
{
	using SingletonPool = boost::singleton_pool<Tag, sizeof(T), boost::default_user_allocator_malloc_free, boost::details::pool::default_mutex>;
public:
	static void Clear();

	static void* _New();
	static T* New();
	static void Delete(void * p);	

	static void* _OrderedNew(size_t size);
	static T* OrderedNew(size_t size);
	static void OrderedDelete(void * p);		
};

template <typename Tag, typename T>
inline void Memory<Tag, T>::Clear()
{
	SingletonPool::purge_memory();
}

template <typename Tag, typename T>
inline void* Memory<Tag, T>::_New()
{
	T* ret = static_cast<T *>(SingletonPool::malloc());
	new (ret) T();
	return ret;
}

template <typename Tag, typename T>
inline T* Memory<Tag, T>::New()
{
	T* ret = static_cast<T *>(SingletonPool::malloc());
	new (ret) T();
	return ret;
}

template <typename Tag, typename T>
inline void Memory<Tag, T>::Delete(void * p)
{
	static_cast<T *>(p)->~T();
	SingletonPool::free(p);
}

template <typename Tag, typename T>
inline void* Memory<Tag, T>::_OrderedNew(size_t size)
{
	T* ret = static_cast<T *>(SingletonPool::ordered_malloc(size));
	new (ret) T();
	return ret;
}

template <typename Tag, typename T>
inline T* Memory<Tag, T>::OrderedNew(size_t size)
{
	T* ret = static_cast<T *>(SingletonPool::ordered_malloc(size));
	new (ret) T();
	return ret;
}

template <typename Tag, typename T>
inline void Memory<Tag, T>::OrderedDelete(void * p)
{
	static_cast<T *>(p)->~T();
	SingletonPool::ordered_free(p);
}

template <typename T>
inline void SafeDelete(void* p)
{
	if (!p) return;

	T* ptr = static_cast<T*>(p);
	delete ptr;
	p = ptr = nullptr;
}

template <typename T>
inline void SafeDeleteArray(void* p)
{
	if (!p) return;

	T* ptr = static_cast<T*>(p);
	delete[] ptr;
	p = ptr = nullptr;
}

template <typename T>
inline void SafeRelease(T* p)
{
	if (p == NULL) return;

	p->Release();
	p = nullptr;
}