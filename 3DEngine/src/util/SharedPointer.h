#pragma once

#include <memory>

#define SHARED_PTR_TYPE_DECL(T) \
	typedef std::shared_ptr<T> T##_ptr; \
	typedef std::shared_ptr<T const > T##_cptr; \
	typedef std::weak_ptr<T > T##_wptr; \
	typedef std::weak_ptr<T const > T##_cwptr;

#define SHARED_PTR_CLASS_DECL(T) \
	class T; \
	typedef std::shared_ptr<T> T##_ptr; \
	typedef std::shared_ptr<T const > T##_cptr; \
	typedef std::weak_ptr<T > T##_wptr; \
	typedef std::weak_ptr<T const > T##_cwptr;

#define SHARED_PTR_FACTORY(T) \
	static std::shared_ptr<T> Create() \
	{ \
	return std::shared_ptr<T>(new T(),[](const T* p) {delete p;}); \
	};