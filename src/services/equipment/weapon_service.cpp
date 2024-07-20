#include "weapon_service.hpp"

namespace big
{
	weapon_service::weapon_service()
	{
		g_weapon_service = this;
	}
	weapon_service::~weapon_service() noexcept
	{
		g_weapon_service = nullptr;
	}
	void weapon_service::init_melee(MeleeWeapon* wep)
	{
		m_melee_weapon = wep;
	}
	void weapon_service::init_ranged(RangedWeapon* wep)
	{
		m_ranged_weapon = wep;
	}
	MeleeWeapon* weapon_service::get_melee()
	{
		return m_melee_weapon == nullptr ? nullptr : m_melee_weapon;
	}
	RangedWeapon* weapon_service::get_ranged()
	{
		return m_ranged_weapon == nullptr ? nullptr : m_ranged_weapon;
	}
	std::string weapon_service::get_melee_elemental()
	{
		if (m_melee_weapon)
		{
			if (auto it = m_element_data.find(m_melee_weapon->m_element_type); it != m_element_data.end())
			{
				return it->second;
			}
		}

		return "No element found";
	}
	std::string weapon_service::get_melee_hidden_elemental()
	{
		if (m_melee_weapon)
		{
			if (auto it = m_element_data.find(m_melee_weapon->m_hidden_element_type); it != m_element_data.end())
			{
				return it->second;
			}
		}

		return "No element found";
	}
	std::string weapon_service::get_melee_skill()
	{
		if (m_melee_weapon)
		{
			if (auto it = m_skill_data.find(m_melee_weapon->m_skill); it != m_skill_data.end())
			{
				return it->second;
			}
		}

		return "No skill found";
	}
	std::string weapon_service::get_ranged_elemental()
	{
		if (m_ranged_weapon)
		{
			if (auto it = m_element_data.find(m_ranged_weapon->m_element_type); it != m_element_data.end())
			{
				return it->second;
			}
		}

		return "No element found";
	}
	std::string weapon_service::get_ranged_hidden_elemental()
	{
		if (m_ranged_weapon)
		{
			if (auto it = m_element_data.find(m_ranged_weapon->m_hidden_element_type); it != m_element_data.end())
			{
				return it->second;
			}
		}

		return "No element found";
	}
	std::string weapon_service::get_ranged_skill()
	{
		if (m_ranged_weapon)
		{
			if (auto it = m_skill_data.find(m_ranged_weapon->m_skill); it != m_skill_data.end())
			{
				return it->second;
			}
		}

		return "No skill found";
	}
}