#include "player.hpp"
#include "pointers.hpp"
#include "hook_manager.hpp"

namespace big
{
	void player::invulnerable()
	{
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
		if (g_settings->player.inf_stamina)
		{
			g_hook_manager->m_player_stamina.apply();
		}
		else
		{
			g_hook_manager->m_player_stamina.restore();
		}
	}
	void player::critical_boost()
	{
		auto addr = (*g_pointers->m_critical_boost);
		*(float*)((uintptr_t)addr + 0x7A8) = g_settings->player.critical_boost;
	}
	void player::easy_part_breaker()
	{
		if (g_settings->player.easy_part_breaker)
		{
			g_hook_manager->m_easy_breaker.apply();
		}
		else
		{
			g_hook_manager->m_easy_breaker.restore();
		}
	}
	void player::part_breaker()
	{
		if (g_settings->player.part_breaker)
		{
			g_hook_manager->m_part_breaker.apply();
		}
		else
		{
			g_hook_manager->m_part_breaker.restore();
		}
	}
	void player::max_stat_bonus()
	{
		if (g_settings->player.maxed_stat_set)
		{
			g_hook_manager->m_maxed_attribute.apply();
		}
		else
		{
			g_hook_manager->m_maxed_attribute.restore();
		}
	}
	void player::max_armor_skill()
	{
		if (g_settings->player.maxed_stat_set)
		{
			g_hook_manager->m_maxed_effect_set.apply();
		}
		else
		{
			g_hook_manager->m_maxed_effect_set.restore();
		}
	}
	void player::max_sharpness()
	{
		if (g_settings->player.max_sharpness)
		{
			g_hook_manager->m_max_sharpness.apply();
		}
		else
		{
			g_hook_manager->m_max_sharpness.restore();
		}
	}
	void player::mount_reset()
	{
		if (g_settings->player.mount_reset)
		{
			g_hook_manager->m_mount_reset.apply();
		}
		else
		{
			g_hook_manager->m_mount_reset.restore();
		}
	}
	void player::inf_mount()
	{
		if (g_settings->player.inf_mount)
		{
			g_hook_manager->m_inf_mount.apply();
		}
		else
		{
			g_hook_manager->m_inf_mount.restore();
		}
	}
}