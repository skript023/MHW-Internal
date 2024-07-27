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
		mid_function_hook m_load_cannon;
		mid_function_hook m_pet_mgr;
		mid_function_hook m_pet_unlocker;
		mid_function_hook m_max_slot_armor;
		mid_function_hook m_max_slot_armor_ranged;
		mid_function_hook m_max_slot_armor_melee;
		mid_function_hook m_scoutfly_level;
		mid_function_hook m_scoutfly_max;
		mid_function_hook m_palico_gadget_cd;
		mid_function_hook m_slinger_ammo;
		mid_function_hook m_spirit_gauge;
		mid_function_hook m_demon_mode;
		mid_function_hook m_fully_charged_switch_axe;
		mid_function_hook m_gl_ammo;
		mid_function_hook m_gl_cooldown;
	public:
		byte_patching m_inf_frostcraft;
		byte_patching m_player_state;
		byte_patching m_player_anim;
		byte_patching m_player_health;
		byte_patching m_player_stamina;
		byte_patching m_inf_mount;
		byte_patching m_gathering_resource;
		byte_patching m_cannon_fire;
		byte_patching m_barrel_placement_max;
		byte_patching m_barrel_placement_count;
		byte_patching m_slinger_max_ammo;
	};

	inline hook_manager* g_hook_manager;
}