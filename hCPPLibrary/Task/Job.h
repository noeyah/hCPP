#pragma once
#include <memory>
#include <type_traits>
#include <utility>
#include <concepts>
#include "IJobQueue.h"

namespace hlib
{
	template<typename F>
	class Job : public IJob
	{
	public:
		explicit Job(F&& fn) : func_(std::forward<F>(fn))
		{
		}

		virtual void Run() override
		{
			func_();
		}

	private:
		std::decay_t<F> func_;
	};
}

