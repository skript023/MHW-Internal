#include "hook_manager.hpp"
#include "pointers.hpp"

namespace big
{
	hook_manager::hook_manager() :
		/*mid function hook*/
		m_easy_breaker(g_pointers->m_easy_breaker, { 0x48, 0xC7, 0x41, 0x10, 0x00, 0x00, 0x00, 0x00 }),
		m_part_breaker(g_pointers->m_part_breaker, { 0xC7, 0x84, 0x24, 0xA8, 0x01, 0x00, 0x00 }, 990.f),
		m_reveal_monster(g_pointers->m_reveal_monster, { 0xC7, 0x84, 0x24, 0xA8, 0x01, 0x00, 0x00 }, 100.f),
		m_critical_boost(g_pointers->m_critical_boost, { 0xC7, 0x84, 0x24, 0xA8, 0x07, 0x00, 0x00 }, 990.f),
		/*byte patching*/
		m_player_state(g_pointers->m_player_state, { 0x90, 0x90, 0x90, 0x90, 0x90 }),
		m_player_anim(g_pointers->m_player_anim, { 0x90, 0x90, 0x90, 0x90, 0x90 }),
		m_player_health(g_pointers->m_player_health, { 0x90, 0x90, 0x90, 0x90, 0x90 }),
		m_player_stamina(g_pointers->m_player_stamina, { 0x90, 0x90, 0x90, 0x90 })
	{
		m_player_stamina.apply();
		m_reveal_monster.apply();

		g_hook_manager = this;
	}

	hook_manager::~hook_manager()
	{
		/*mid function hook*/
		m_easy_breaker.restore();
		m_part_breaker.restore();
		m_reveal_monster.restore();
		m_critical_boost.restore();
		/*byte patching*/
		m_player_anim.restore();
		m_player_health.restore();
		m_player_state.restore();
		m_player_stamina.restore();

		g_hook_manager = nullptr;
	}
}