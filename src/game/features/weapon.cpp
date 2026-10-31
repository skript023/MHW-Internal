#include "game/settings.hpp"
#include "weapon.hpp"

#include "game/patches.hpp"

float g_frostcraft_drawn;
float g_frostcraft_sheathed;
uint64_t g_equipment_ptr;

namespace big
{
	void weapon::infinite_frostcraft()
	{
		if (!g_patches) return;
		if (g_settings->weapon.inf_frostcraft)
		{
			g_patches->m_inf_frostcraft.apply();
		}
		else
		{
			g_patches->m_inf_frostcraft.restore();
		}
	}

	void weapon::frostcraft_recharge()
	{
	}

	void weapon::inf_spirit_gauge()
	{
	}

	void weapon::inf_demon_mode()
	{
	}

	void weapon::full_charged_switch_axe()
	{
	}

	void weapon::inf_gl_ammo()
	{
	}

	void weapon::no_gl_cooldown()
	{
	}

	void weapon::instant_charges_greater_sword()
	{
	}

	void weapon::max_slot_armor()
	{
	}

	void weapon::inf_slinger_ammo()
	{
		if (!g_patches) return;
		if (g_settings->weapon.inf_slinger_ammo)
		{
			g_patches->m_slinger_max_ammo.apply();
		}
		else
		{
			g_patches->m_slinger_max_ammo.restore();
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
