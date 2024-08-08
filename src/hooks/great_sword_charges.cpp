#include "hooking.hpp"

namespace big
{
	int hooks::great_sword_charges(unsigned int a1, __int64 a2, float a3)
	{
		unsigned int v3; // edi@1
		__int64 v4; // rbx@1

		v3 = a1;
		v4 = a2;

		if (g_settings->weapon.inst_chrgs_gs)
		{
			*(DWORD*)(v4 + 9048) = 3;
		}

		return;
	}
}