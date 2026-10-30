#include "weapon_service.hpp"

namespace big
{
	void weapon_service::init_melee_impl(MeleeWeapon* wep)
	{
		m_melee_weapon = wep;
	}

	void weapon_service::init_ranged_impl(RangedWeapon* wep)
	{
		m_ranged_weapon = wep;
	}

	MeleeWeapon* weapon_service::get_melee_impl()
	{
		return m_melee_weapon == nullptr ? nullptr : m_melee_weapon;
	}

	RangedWeapon* weapon_service::get_ranged_impl()
	{
		return m_ranged_weapon == nullptr ? nullptr : m_ranged_weapon;
	}

	std::string weapon_service::get_melee_elemental_impl()
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

	std::string weapon_service::get_melee_hidden_elemental_impl()
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

	std::string weapon_service::get_melee_skill_impl()
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

	std::string weapon_service::get_ranged_elemental_impl()
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

	std::string weapon_service::get_ranged_hidden_elemental_impl()
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

	std::string weapon_service::get_ranged_skill_impl()
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
