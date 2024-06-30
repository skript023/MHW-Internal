#include "weapon.hpp"
#include "hook_manager.hpp"

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
}