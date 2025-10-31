#pragma once
#include <memory>
#include <mutex>

template <typename T>
class Singleton : public std::enable_shared_from_this<T>
{
public:
	static std::shared_ptr<T> Instance()
	{
		std::call_once(once_flag_, []() {
			instance_ = std::make_shared<T>();
		});
		return instance_;
	}

protected:
	Singleton() = default;
	virtual ~Singleton() = default;

private:
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;

private:
	inline static std::shared_ptr<T> instance_ = nullptr;
	inline static std::once_flag once_flag_;
};