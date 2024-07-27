#include "weapon.hpp"
#include "hook_manager.hpp"

float g_frostcraft_drawn;
float g_frostcraft_sheathed;
uint64_t g_equipment_ptr;
 //bool g_is_frostcraft_drawn;
 //bool g_is_frostcraft_sheathed;

namespace big
{
	void weapon::infinite_frostcraft()
	{
		if (g_settings->weapon.inf_frostcraft)
		{
			g_hook_manager->m_inf_frostcraft.apply();
		}
		else
		{
			g_hook_manager->m_inf_frostcraft.restore();
		}
	}
	void weapon::frostcraft_recharge()
	{
		if (g_settings->weapon.frostcraft_recharge)
		{
			g_hook_manager->m_frostcraft_drawn.apply();
			g_hook_manager->m_frostcraft_heat.apply();
		}
		else
		{
			g_hook_manager->m_frostcraft_drawn.restore();
			g_hook_manager->m_frostcraft_heat.restore();
		}
	}
	void weapon::inf_spirit_gauge()
	{
		if (g_settings->weapon.inf_spirit_gauge)
		{
			g_hook_manager->m_spirit_gauge.apply();
		}
		else
		{
			g_hook_manager->m_spirit_gauge.restore();
		}
	}
	void weapon::inf_demon_mode()
	{
		if (g_settings->weapon.inf_demon_mode)
		{
			g_hook_manager->m_demon_mode.apply();
		}
		else
		{
			g_hook_manager->m_demon_mode.restore();
		}
	}
	void weapon::full_charged_switch_axe()
	{
		if (g_settings->weapon.full_charged_switch_axe)
		{
			g_hook_manager->m_fully_charged_switch_axe.apply();
		}
		else
		{
			g_hook_manager->m_fully_charged_switch_axe.restore();
		}
	}
	void weapon::inf_gl_ammo()
	{
		if (g_settings->weapon.inf_gl_ammo)
		{
			g_hook_manager->m_gl_ammo.apply();
		}
		else
		{
			g_hook_manager->m_gl_ammo.restore();
		}
	}
	void weapon::no_gl_cooldown()
	{
		if (g_settings->weapon.no_gl_cooldown)
		{
			g_hook_manager->m_gl_cooldown.apply();
		}
		else
		{
			g_hook_manager->m_gl_cooldown.restore();
		}
	}
	void weapon::instant_charges_greater_sword()
	{
		if (g_settings->weapon.inst_chrgs_gs)
		{
			g_hook_manager->m_gs_inst_chrgs.apply();
		}
		else
		{
			g_hook_manager->m_gs_inst_chrgs.restore();
		}
	}
	void weapon::max_slot_armor()
	{
		if (g_settings->weapon.max_slot_armor)
		{
			g_hook_manager->m_max_slot_armor.apply();
			g_hook_manager->m_max_slot_armor_melee.apply();
			g_hook_manager->m_max_slot_armor_ranged.apply();
		}
		else
		{
			g_hook_manager->m_max_slot_armor.restore();
			g_hook_manager->m_max_slot_armor_melee.restore();
			g_hook_manager->m_max_slot_armor_ranged.restore();
		}
	}
	void weapon::inf_slinger_ammo()
	{
		if (g_settings->weapon.inf_slinger_ammo)
		{
			g_hook_manager->m_slinger_ammo.apply();
			g_hook_manager->m_slinger_max_ammo.apply();
		}
		else
		{
			g_hook_manager->m_slinger_ammo.restore();
			g_hook_manager->m_slinger_max_ammo.restore();
		}
	}
	Equipment* weapon::eqipment()
	{
		auto ptr = reinterpret_cast<Equipment*>(g_equipment_ptr);

		if (!ptr)
			return nullptr;

		return ptr;
	}
}