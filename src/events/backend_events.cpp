#include "backend_events.hpp"
#include "script.hpp"
#include <features/player.hpp>
#include <features/weapon.hpp>
#include <server_module.hpp>

namespace big
{
	void backend_events::features()
	{
		
	}

	void backend_events::player_event()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				player::critical_boost();
				player::infinite_stamina();
				player::invulnerable();
				player::easy_part_breaker();
				player::part_breaker();
				player::max_armor_skill();
				player::max_stat_bonus();
				player::max_sharpness();
				player::mount_reset();
				player::inf_mount();
				player::inf_gathering();
				player::inf_lucky_voucher();
				player::cannon_always_loaded();
				player::inf_cannon_fire();
				player::unlock_all_pet();
				player::scoutfly_max_level();
				player::palico_gadget_cd();
				player::reveal_monster();
			}
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}

	void backend_events::weapon_event()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				weapon::infinite_frostcraft();
				weapon::frostcraft_recharge();
				weapon::max_slot_armor();
				weapon::inf_slinger_ammo();

				weapon::instant_charges_greater_sword();
				weapon::inf_spirit_gauge();
				weapon::inf_demon_mode();
				weapon::full_charged_switch_axe();
				weapon::inf_gl_ammo();
				weapon::no_gl_cooldown();
			}
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}

	void backend_events::script_func()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				//g_server_module->run();
				features();
			} 
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}
}