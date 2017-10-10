#pragma once

// http://vallista.tistory.com/entry/1-Singleton-Pattern-in-C

template < typename T >
class SingletonBase
{
protected:
	SingletonBase() {}	
	virtual ~SingletonBase() {}
	
public:
	static shared_ptr<T> Instance()
	{
		if (!_instance) _instance.reset(new T());
			
		return _instance;
	};

	virtual bool Init() abstract;
	virtual void Release() abstract;

private:
	static shared_ptr<T> _instance;
};

template <typename T> shared_ptr<T> SingletonBase<T>::_instance = 0;

