#include "hooking.hpp"

namespace big
{
	void __fastcall hooks::research_experience(__int64 a1, uint32_t a2, float a3)
	{
		return base_hook::get_original<research_experience>()(a1, a2, a3 * g_settings->player.research_exp);
	}
}