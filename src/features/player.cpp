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
	void player::reveal_monster()
	{
		if (g_settings->player.reveal_monster)
			g_hook_manager->m_reveal_monster.apply();
		else
			g_hook_manager->m_reveal_monster.restore();
	}
	void player::critical_boost()
	{
		auto addr = (*g_pointers->m_player_stat);
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
	void player::inf_gathering()
	{
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
		if (g_settings->player.inf_lucky_voucher)
		{
			g_hook_manager->m_lucky_voucher.apply();
		}
		else
		{
			g_hook_manager->m_lucky_voucher.restore();
		}
	}
	void player::cannon_always_loaded()
	{
		if (g_settings->player.cannon_always_loaded)
		{
			g_hook_manager->m_load_cannon.apply();
		}
		else
		{
			g_hook_manager->m_load_cannon.restore();
		}
	}
	void player::inf_cannon_fire()
	{
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
		if (g_settings->player.unlock_all_pet)
		{
			g_hook_manager->m_pet_mgr.apply();
			g_hook_manager->m_pet_unlocker.apply();
		}
		else
		{
			g_hook_manager->m_pet_mgr.restore();
			g_hook_manager->m_pet_unlocker.restore();
		}
	}
	void player::scoutfly_max_level()
	{
		if (g_settings->player.scoutfly_max_level)
		{
			g_hook_manager->m_scoutfly_max.apply();
			g_hook_manager->m_scoutfly_level.apply();
		}
		else
		{
			g_hook_manager->m_scoutfly_max.restore();
			g_hook_manager->m_scoutfly_level.restore();
		}
	}
	void player::palico_gadget_cd()
	{
		if (g_settings->player.palico_gadget_cd)
		{
			g_hook_manager->m_palico_gadget_cd.apply();
		}
		else
		{
			g_hook_manager->m_palico_gadget_cd.restore();
		}
	}
}