#pragma once
#include "common.hpp"

namespace big
{
	class hardware_breakpoint_hook
	{
    public:
        hardware_breakpoint_hook(void* address);
        ~hardware_breakpoint_hook();

        void set();
        void clear();

    private:
        void* m_address;
        bool m_is_set;
	};
}