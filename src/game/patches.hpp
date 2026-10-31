#pragma once
#include "core/hooking/byte_patching/byte_patching.hpp"

namespace big
{
	class patches
	{
	public:
		patches();
		~patches();

		void restore_all();

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

	inline patches* g_patches;
}
