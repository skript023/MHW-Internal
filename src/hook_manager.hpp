#include "midfunction_hook.hpp"

namespace big
{
	class hook_manager
	{
	public:
		hook_manager();
		~hook_manager();

	public:
		mid_function_hook m_easy_breaker;
		mid_function_hook m_part_breaker;
		mid_function_hook m_reveal_monster;
		mid_function_hook m_critical_boost;
	public:
		byte_patching m_player_state;
		byte_patching m_player_anim;
		byte_patching m_player_health;
		byte_patching m_player_stamina;
	};

	inline hook_manager* g_hook_manager;
}