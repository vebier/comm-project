#pragma once
#include<memory>
#include<mutex>
#include<iostream>
template<typename T>
class Singleton {
public:
	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static std::shared_ptr<T> GetInstance()
	{
		static std::once_flag flag;
		std::call_once(flag, [&]() {
			_instance = std::shared_ptr<T>(new T);
			});
		return _instance;
	}

	void PrintAddress() {
		std::cout << _instance.get() << std::endl;
	}

	~Singleton()
	{
		std::cout << "this is singleton destruct" << std::endl;
	}
protected:
	static std::shared_ptr<T> _instance;
};
template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;