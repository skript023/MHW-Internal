#include "hooking.hpp"

namespace big
{
    unsigned int hooks::monster_part_breaking(__int64 a1, unsigned int a2)
    {
        if (*(uint32_t *)(a1 + 0x10) <= a2) 
        {
            return 0;
        }

        *(uint32_t*)(*(int64_t*)(*(int64_t*)(a2 + 0x20) + (uint64_t)a2 * 8) + 0x10) = 0;

        return *(uint32_t*)(*(int64_t*)(*(int64_t*)(a2 + 0x20) + (uint64_t)a2 * 8) + 0x10);
    }
}