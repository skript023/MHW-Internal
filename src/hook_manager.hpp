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
		mid_function_hook m_maxed_effect_set;
		mid_function_hook m_maxed_attribute;
		mid_function_hook m_max_sharpness;
		mid_function_hook m_gs_inst_chrgs;
		mid_function_hook m_mount_reset;
		mid_function_hook m_frostcraft_drawn;
		mid_function_hook m_frostcraft_heat;
		mid_function_hook m_lucky_voucher;
	public:
		byte_patching m_inf_frostcraft;
		byte_patching m_player_state;
		byte_patching m_player_anim;
		byte_patching m_player_health;
		byte_patching m_player_stamina;
		byte_patching m_inf_mount;
	};

	inline hook_manager* g_hook_manager;
}