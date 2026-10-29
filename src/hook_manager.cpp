#include "hook_manager.hpp"
#include "pointers.hpp"

namespace big
{
	hook_manager::hook_manager() :
	    m_inf_frostcraft(g_pointers->m_inf_frostcraft, {0x90, 0x90, 0x90, 0x90}),
	    m_player_state(g_pointers->m_player_state, {0x90, 0x90, 0x90, 0x90, 0x90}),
	    m_player_anim(g_pointers->m_player_anim, {0x90, 0x90, 0x90, 0x90, 0x90}),
	    m_player_health(g_pointers->m_player_health, {0x90, 0x90, 0x90, 0x90, 0x90}),
	    m_player_stamina(g_pointers->m_player_stamina, {0x90, 0x90, 0x90, 0x90}),
	    m_inf_mount(g_pointers->m_inf_mount, {0x90, 0x90}),
	    m_gathering_resource(g_pointers->m_gathering_resource, {0x90, 0x90}),
	    m_cannon_fire(g_pointers->m_cannon_fire, {0xEB}),
	    m_barrel_placement_max(g_pointers->m_barrel_placement_max, {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}),
	    m_barrel_placement_count(g_pointers->m_barrel_placement_count, {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90}),
	    m_slinger_max_ammo(g_pointers->m_slinger_max_ammo, {0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90})
	{
		g_hook_manager = this;
	}

	hook_manager::~hook_manager()
	{
		restore_all();
		g_hook_manager = nullptr;
	}

	void hook_manager::restore_all()
	{
		m_inf_frostcraft.restore();
		m_player_state.restore();
		m_player_anim.restore();
		m_player_health.restore();
		m_player_stamina.restore();
		m_inf_mount.restore();
		m_gathering_resource.restore();
		m_cannon_fire.restore();
		m_barrel_placement_max.restore();
		m_barrel_placement_count.restore();
		m_slinger_max_ammo.restore();
	}
}
