#pragma once
#include "core/threading/fiber_pool.hpp"
#include <functional>

namespace big
{
	template<typename F, typename... Args>
	void async_run(F&& f, Args&&... args)
	{
		auto job = [fn = std::forward<F>(f), ...a = std::forward<Args>(args)]() mutable {
			fn(std::forward<Args>(a)...);
		};
		if (g_fiber_pool)
		{
			g_fiber_pool->queue_job(std::move(job));
		}
		else
		{
			job();
		}
	}

	template<typename F>
	inline std::function<void()> async_func(F&& f)
	{
		return [fn = std::forward<F>(f)]() mutable {
			async_run(fn);
		};
	}
}
