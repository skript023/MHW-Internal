#include "hooking.hpp"

namespace big
{
	int __fastcall hooks::bow_gun_effect(__int64 a1)
	{
		auto rbx = a1;
		auto rsi = *(intptr_t*)(rbx + 2480);
        int64_t eax;

        *(uint32_t*)(rbx + 0x2348 + 0x0) = 4;  // recoil
        *(uint32_t*)(rbx + 0x2348 + 0x4) = 4;  // reload
        *(uint32_t*)(rbx + 0x2348 + 0x8) = 3;  // deviation
        *(uint32_t*)(rbx + 0x2348 + 0xC) = 3;  // close-range
        *(uint32_t*)(rbx + 0x2348 + 0x10) = 3; // ranged atk up
        *(uint32_t*)(rbx + 0x2348 + 0x14) = 4; // shield
        *(uint32_t*)(rbx + 0x2348 + 0x18) = 1; // special scope
        *(uint32_t*)(rbx + 0x2348 + 0x1C) = 3; // evading reload
        *(uint32_t*)(rbx + 0x2348 + 0x20) = 1; // wyvernheart mod
        *(uint32_t*)(rbx + 0x2348 + 0x24) = 1; // wyvernsnipe
        *(uint32_t*)(rbx + 0x2348 + 0x28) = 1; // wyvernblast
        *(uint32_t*)(rbx + 0x2348 + 0x2C) = 1; // power
        *(uint32_t*)(rbx + 0x2348 + 0x30) = 1; // long

        eax = *(int64_t*)(rsi + 0x13874);
        if (eax <= 12) 
        {
            *(int64_t*)(rbx + eax * 4 + 0x2578) -= 1;
        }

        eax = *(int64_t*)(rsi + 0x13878);
        if (eax <= 12) 
        {
            *(int64_t*)(rbx + eax * 4 + 0x2578) -= 1;
        }

        eax = *(int64_t*)(rsi + 0x1387C);
        if (eax <= 12) 
        {
            *(int64_t*)(rbx + eax * 4 + 0x2578) -= 1;
        }

        return g_hooking->m_bow_gun_effect_hook.get_original<decltype(&hooks::bow_gun_effect)>()(rbx);
	}
}