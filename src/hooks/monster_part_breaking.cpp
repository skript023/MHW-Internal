#include "hooking.hpp"

namespace big
{
    __int64 __fastcall hooks::monster_part_breaking(__int64 a1, unsigned int a2)
    {
        __int64 result; // rax@1

        if (g_settings->player.easy_part_breaker)
        {
            result = a1;

            if (a2 < *(DWORD*)(a1 + 16))
            {
                result = *(uintptr_t*)(a1 + 32);

                *(DWORD*)(*(uintptr_t*)(result + 8i64 * a2) + 16i64) = 0;
            }

            return result;
        }

        return base_hook::get_original<hooks::monster_part_breaking>()(a1, a2);
    }
}