#include "hooking.hpp"

namespace big
{
	unsigned int hooks::glaive_spirit_gauge(__int64 a1, float a2)
	{
		__int64 v2; // rdi@1

		v2 = a1;

		*(float*)(v2 + 0x2368) = 1.f;
		*(DWORD*)(v2 + 0x2370) = 3;
		*(float*)(v2 + 0x2374) = 1.f;

		return;
	}
}