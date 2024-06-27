#include "midfunction_hook.hpp"

namespace big
{
	class midfunc_hook_manager
	{
	public:
		midfunc_hook_manager();
		~midfunc_hook_manager();

	public:
		mid_function_hook m_easy_breaker;
		mid_function_hook m_part_breaker;
		mid_function_hook m_reveal_monster;
	};

	inline midfunc_hook_manager* g_midfunction_manager;
}