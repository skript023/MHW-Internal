#include "hooking.hpp"
#include <pointers.hpp>

namespace big
{
	int WINAPI hooks::great_sword_charges(__int64 a1, uint32_t a2, float a3)
	{
		if (g_settings->weapon.inst_chrgs_gs)
		{

		}

		return 0;
	}
}