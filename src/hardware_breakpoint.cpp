#include "hardware_breakpoint.hpp"

namespace big
{
	hardware_breakpoint_hook::hardware_breakpoint_hook(void* address):
		m_address(address), m_is_set(false)
	{}
	hardware_breakpoint_hook::~hardware_breakpoint_hook()
	{
		clear();
	}

	void hardware_breakpoint_hook::set()
	{
        if (m_is_set) 
        {
            return; // Breakpoint already set
        }

        CONTEXT context = {};
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;

        HANDLE thread = GetCurrentThread();
        if (GetThreadContext(thread, &context)) 
        {
            context.Dr0 = reinterpret_cast<DWORD_PTR>(m_address);
            context.Dr7 |= (1 << 0); // Enable local breakpoint in DR0

            if (SetThreadContext(thread, &context))
            {
                m_is_set = true;
                LOG(INFO) << "Hardware breakpoint set at address: " << m_address;
            }
            else 
            {
                LOG(WARNING) << "Failed to set hardware breakpoint.";
            }
        }
        else 
        {
            LOG(WARNING) << "Failed to get thread context.";
        }
	}
    void hardware_breakpoint_hook::clear()
    {
        if (!m_is_set) 
        {
            return; // Breakpoint already cleared or never set
        }

        CONTEXT context = {};
        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;

        HANDLE thread = GetCurrentThread();
        if (GetThreadContext(thread, &context)) 
        {
            context.Dr0 = 0;
            context.Dr7 &= ~(1 << 0); // Disable local breakpoint in DR0

            if (SetThreadContext(thread, &context)) 
            {
                m_is_set = false;
                LOG(INFO) << "Hardware breakpoint cleared.";
            }
            else 
            {
                LOG(WARNING) << "Failed to clear hardware breakpoint.";
            }
        }
        else 
        {
            LOG(WARNING) << "Failed to get thread context.";
        }
    }
}