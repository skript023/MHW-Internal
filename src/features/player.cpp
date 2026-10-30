#include <mhw/fwddec.hpp>
#include "player.hpp"

#include "hook_manager.hpp"
#include "pointers.hpp"

namespace big
{
	void player::invulnerable()
	{
		if (!g_hook_manager) return;
		if (g_settings->player.invulnerable)
		{
			g_hook_manager->m_player_anim.apply();
			g_hook_manager->m_player_state.apply();
			g_hook_manager->m_player_health.apply();
		}
		else
		{
			g_hook_manager->m_player_anim.restore();
			g_hook_manager->m_player_state.restore();
			g_hook_manager->m_player_health.restore();
		}
	}

	void player::infinite_stamina()
	{
		if (!g_hook_manager) return;
		if (g_settings->player.inf_stamina)
		{
			g_hook_manager->m_player_stamina.apply();
		}
		else
		{
			g_hook_manager->m_player_stamina.restore();
		}
	}

	void player::reveal_monster()
	{
	}

	void player::critical_boost()
	{
		if (g_pointers->m_player_stat && *g_pointers->m_player_stat)
		{
			(*g_pointers->m_player_stat)->m_critical_boost = g_settings->player.critical_boost;
		}
	}

	void player::easy_part_breaker()
	{
	}

	void player::part_breaker()
	{
	}

	void player::max_stat_bonus()
	{
	}

	void player::max_armor_skill()
	{
	}

	void player::max_sharpness()
	{
	}

	void player::mount_reset()
	{
	}

	void player::inf_mount()
	{
		if (!g_hook_manager) return;
		if (g_settings->player.inf_mount)
		{
			g_hook_manager->m_inf_mount.apply();
		}
		else
		{
			g_hook_manager->m_inf_mount.restore();
		}
	}

	void player::inf_gathering()
	{
		if (!g_hook_manager) return;
		if (g_settings->player.inf_gathering)
		{
			g_hook_manager->m_gathering_resource.apply();
		}
		else
		{
			g_hook_manager->m_gathering_resource.restore();
		}
	}

	void player::inf_lucky_voucher()
	{
	}

	void player::cannon_always_loaded()
	{
	}

	void player::inf_cannon_fire()
	{
		if (!g_hook_manager) return;
		if (g_settings->player.inf_cannon_fire)
		{
			g_hook_manager->m_cannon_fire.apply();
		}
		else
		{
			g_hook_manager->m_cannon_fire.restore();
		}
	}

	void player::unlock_all_pet()
	{
	}

	void player::scoutfly_max_level()
	{
	}

	void player::palico_gadget_cd()
	{
	}
}
