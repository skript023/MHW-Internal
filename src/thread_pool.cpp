#include "thread_pool.hpp"

namespace big
{
	thread_pool::thread_pool() :
	    m_accept_jobs(true)
	{
		create();
		g_thread_pool = this;
	}

	thread_pool::~thread_pool()
	{
		destroy();
		g_thread_pool = nullptr;
	}

	void thread_pool::create()
	{
		const auto cap = std::thread::hardware_concurrency();

		LOG(INFO) << "Allocating " << cap << " threads in thread pool.";

		m_managing_thread = std::thread(&thread_pool::run, this);
	}

	void thread_pool::destroy()
	{
		if (this->m_managing_thread.joinable())
			this->m_managing_thread.join();

		{
			std::unique_lock lock(m_lock);
			this->m_accept_jobs = false;
		}
		this->m_data_condition.notify_all();

		for (auto& thread : m_thread_pool)
			if (thread.joinable())
				thread.join();

		m_thread_pool.clear();
	}

	void thread_pool::push(std::function<void()> func)
	{
		if (func)
		{
			{
				std::unique_lock lock(this->m_lock);
				if (!m_accept_jobs)
					return;
				this->m_job_stack.push(std::move(func));
			}
			this->m_data_condition.notify_all();
		}
	}

	void thread_pool::run()
	{
		const auto cap = std::thread::hardware_concurrency();

		for (size_t i = 0; i < cap; i++)
			m_thread_pool.push_back(std::thread(&thread_pool::done, this));
	}

	void thread_pool::done()
	{
		while (m_accept_jobs)
		{
			try
			{
				std::unique_lock lock(m_lock);

				m_data_condition.wait(lock, [this]() {
					return !m_job_stack.empty() || !m_accept_jobs;
				});

				if (!m_accept_jobs)
					break;

				auto job = std::move(m_job_stack.top());
				m_job_stack.pop();
				lock.unlock();

				job();
			}
			catch (const std::exception& e)
			{
				LOG(WARNING) << "Exception thrown while executing job in thread:" << std::endl << e.what();
			}
		}

		LOG(INFO) << "Thread " << std::this_thread::get_id() << " exiting...";
	}
}
