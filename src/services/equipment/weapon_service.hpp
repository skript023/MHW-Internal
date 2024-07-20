#pragma once
#include "class/fwddec.hpp"

namespace big
{
	class weapon_service
	{
	public:
		explicit weapon_service();
		~weapon_service() noexcept;

		void init_melee(MeleeWeapon* wep);
		void init_ranged(RangedWeapon* wep);

		MeleeWeapon* get_melee();
		RangedWeapon* get_ranged();

		_NODISCARD std::string get_melee_elemental();
		_NODISCARD std::string get_melee_hidden_elemental();
		_NODISCARD std::string get_ranged_elemental();
		_NODISCARD std::string get_ranged_hidden_elemental();
		std::map<uint8_t, std::string> get_element_data() { return m_element_data; };
	private:
		MeleeWeapon* m_melee_weapon = nullptr;
		RangedWeapon* m_ranged_weapon = nullptr;
		std::map<uint8_t, std::string> m_element_data = {
			{ 0, "None" },
			{ 1, "Fire" },
			{ 2, "Water" },
			{ 3, "Ice" },
			{ 4, "Thunder" },
			{ 5, "Dragon" },
			{ 6, "Poison" },
			{ 7, "Paralysis" },
			{ 8, "Sleep" },
			{ 9, "Blast" }
		};
	};

	inline weapon_service* g_weapon_service{};
}