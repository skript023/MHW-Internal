#include "exception_handler.hpp"

#include "stack_trace.hpp"

#include <mutex>
#include <set>

namespace big
{
	exception_handler::exception_handler()
	{
		m_old_error_mode = SetErrorMode(0);
		m_exception_handler = SetUnhandledExceptionFilter(&vectored_exception_handler);
	}

	exception_handler::~exception_handler()
	{
		SetErrorMode(m_old_error_mode);
		SetUnhandledExceptionFilter(reinterpret_cast<decltype(&vectored_exception_handler)>(m_exception_handler));
	}

	inline static stack_trace trace;
	LONG vectored_exception_handler(EXCEPTION_POINTERS* exception_info)
	{
		const auto exception_code = exception_info->ExceptionRecord->ExceptionCode;
		if (exception_code == EXCEPTION_BREAKPOINT || exception_code == DBG_PRINTEXCEPTION_C || exception_code == DBG_PRINTEXCEPTION_WIDE_C)
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}

		static std::mutex logged_mutex;
		static std::set<std::pair<uintptr_t, DWORD>> logged_exceptions;
		bool first_time = false;
		{
			std::lock_guard lock(logged_mutex);
			first_time = logged_exceptions
			                 .emplace(reinterpret_cast<uintptr_t>(exception_info->ExceptionRecord->ExceptionAddress), exception_code)
			                 .second;
		}
		if (first_time)
		{
			trace.new_stack_trace(exception_info);
			LOG(FATAL) << trace;
			Logger::FlushQueue();
		}

		// Advancing past an access violation skips an arbitrary memory operation
		// without repairing its inputs. Preserve the original fault rather than
		// continuing with corrupted state.
		if (exception_code == EXCEPTION_ACCESS_VIOLATION)
		{
			return EXCEPTION_CONTINUE_SEARCH;
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
}
