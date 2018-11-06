#pragma once

template<typename T>
class singleton
{
public:
	static T* getInstance();

protected:
	singleton() = default;
	singleton(const singleton<T>& target) = delete;
	singleton<T>& operator=(const singleton<T>& target) = delete;

private:
	static std::unique_ptr<T> instance;
	static std::once_flag onceFlag;

};

template<typename T>
std::unique_ptr<T> singleton<T>::instance;

template<typename T>
std::once_flag singleton<T>::onceFlag;

template<typename T>
inline T * singleton<T>::getInstance()
{
	std::call_once(
		onceFlag, [=]() {instance.reset(new T); }
	);

	return instance.get();
}
