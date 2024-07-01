#include "hooking.hpp"

namespace big
{
	void __fastcall hooks::research_experience(__int64 a1, unsigned int a2, float a3)
	{
		return g_hooking->m_research_exp_hook.get_original<decltype(&research_experience)>()(a1, a2, a3 * g_settings->player.research_exp);
	}
}