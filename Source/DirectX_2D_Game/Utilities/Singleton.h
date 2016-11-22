#pragma once

#include <assert.h>

template <typename T>
class Singleton
{
public:
	Singleton()
	{
		assert(_pInstance == nullptr);
		_pInstance = static_cast<T*>(this);
	}
	virtual ~Singleton()
	{
		assert(_pInstance == this);
		_pInstance = nullptr;
	}

public:
	static T*	GetPtr() { return static_cast<T*>(_pInstance); }
private:
	static T*		_pInstance;
};

template <typename T>
T* Singleton<T>::_pInstance = nullptr;