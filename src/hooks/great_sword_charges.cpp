#include "hooking.hpp"
#include <pointers.hpp>

namespace big
{
	int WINAPI hooks::great_sword_charges(__int64 a1, uint32_t a2, float a3)
	{
		if (g_settings->weapon.inst_chrgs_gs)
		{

		}

		return base_hook::get_original<great_sword_charges>()(a1, a2, a3);
	}
}