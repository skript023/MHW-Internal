#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace big::monster_data
{
	struct elemental_weakness
	{
		uint8_t fire = 0;    // 0 = Immune (X), 1 = 1 Star, 2 = 2 Stars, 3 = 3 Stars
		uint8_t water = 0;
		uint8_t thunder = 0;
		uint8_t ice = 0;
		uint8_t dragon = 0;
	};

	struct status_weakness
	{
		uint8_t poison = 0;
		uint8_t sleep = 0;
		uint8_t paralysis = 0;
		uint8_t blast = 0;
		uint8_t stun = 0;
	};

	struct part_hzv_info
	{
		const char* part_name;
		int slash;
		int blunt;
		int shot;
		int elem;
		const char* best_elem;
		bool is_weakpoint;
	};

	struct monster_combat_profile
	{
		elemental_weakness element;
		status_weakness status;
		std::vector<part_hzv_info> parts;
	};

	inline const monster_combat_profile& get_combat_profile(int32_t monster_id)
	{
		static const monster_combat_profile s_default_profile = {
			{1, 1, 1, 1, 1},
			{2, 2, 2, 2, 2},
			{
				{"Head", 60, 60, 50, 20, "Dragon", true},
				{"Body / Torso", 30, 30, 25, 10, "Dragon", false},
				{"Left Foreleg", 40, 40, 30, 15, "Dragon", false},
				{"Right Foreleg", 40, 40, 30, 15, "Dragon", false},
				{"Tail", 55, 45, 40, 20, "Dragon", true}
			}
		};

		static const std::unordered_map<int32_t, monster_combat_profile> s_profile_db = {
			// 0: Anjanath
			{0, {
				{0, 3, 2, 2, 1}, // Fire X, Water 3, Thunder 2, Ice 2, Dragon 1
				{2, 2, 2, 1, 2}, // Poison 2, Sleep 2, Para 2, Blast 1, Stun 2
				{
					{"Head", 75, 70, 60, 30, "Water", true},
					{"Nose", 65, 60, 55, 25, "Water", true},
					{"Body", 30, 30, 25, 15, "Water", false},
					{"Hindlegs", 42, 40, 30, 15, "Water", false},
					{"Tail", 50, 45, 40, 20, "Water", true}
				}
			}},
			// 1: Rathalos
			{1, {
				{0, 1, 2, 1, 3}, // Fire X, Water 1, Thunder 2, Ice 1, Dragon 3
				{1, 2, 2, 1, 2}, // Poison 1, Sleep 2, Para 2, Blast 1, Stun 2
				{
					{"Head", 65, 65, 55, 30, "Dragon", true},
					{"Wings", 45, 40, 45, 20, "Dragon", true},
					{"Body", 25, 25, 20, 15, "Dragon", false},
					{"Legs", 38, 38, 30, 15, "Dragon", false},
					{"Tail", 60, 50, 45, 25, "Dragon", true}
				}
			}},
			// 7: Great Jagras
			{7, {
				{3, 0, 2, 2, 1}, // Fire 3, Water X, Thunder 2, Ice 2, Dragon 1
				{3, 3, 3, 3, 3}, // Poison 3, Sleep 3, Para 3, Blast 3, Stun 3
				{
					{"Head", 80, 75, 70, 30, "Fire", true},
					{"Stomach", 70, 65, 60, 25, "Fire", true},
					{"Forelegs", 50, 50, 45, 20, "Fire", true},
					{"Body", 45, 45, 40, 15, "Fire", true},
					{"Tail", 55, 50, 45, 20, "Fire", true}
				}
			}},
			// 9: Rathian
			{9, {
				{0, 1, 2, 1, 3}, // Fire X, Water 1, Thunder 2, Ice 1, Dragon 3
				{1, 2, 2, 1, 3}, // Poison 1, Sleep 2, Para 2, Blast 1, Stun 3
				{
					{"Head", 70, 70, 60, 30, "Dragon", true},
					{"Wings", 45, 40, 45, 20, "Dragon", true},
					{"Body", 25, 25, 20, 15, "Dragon", false},
					{"Legs", 38, 38, 30, 15, "Dragon", false},
					{"Tail", 60, 50, 45, 25, "Dragon", true}
				}
			}},
			// 10: Pink Rathian
			{10, {
				{0, 1, 2, 1, 3},
				{1, 2, 2, 1, 3},
				{
					{"Head", 65, 65, 55, 25, "Dragon", true},
					{"Wings", 45, 40, 45, 20, "Dragon", true},
					{"Body", 25, 25, 20, 15, "Dragon", false},
					{"Legs", 35, 35, 30, 15, "Dragon", false},
					{"Tail", 55, 45, 40, 20, "Dragon", true}
				}
			}},
			// 11: Azure Rathalos
			{11, {
				{0, 1, 2, 2, 3},
				{1, 2, 2, 1, 2},
				{
					{"Head", 65, 65, 55, 25, "Dragon", true},
					{"Wings", 45, 40, 45, 20, "Dragon", true},
					{"Body", 25, 25, 20, 15, "Dragon", false},
					{"Legs", 35, 35, 30, 15, "Dragon", false},
					{"Tail", 55, 45, 40, 20, "Dragon", true}
				}
			}},
			// 12: Diablos
			{12, {
				{0, 2, 0, 3, 2}, // Fire X, Water 2, Thunder X, Ice 3, Dragon 2
				{2, 2, 3, 2, 1}, // Para 3
				{
					{"Head", 50, 65, 45, 25, "Ice", true},
					{"Chest", 65, 65, 60, 30, "Ice", true},
					{"Wings", 60, 55, 65, 30, "Ice", true},
					{"Tail", 55, 45, 40, 20, "Ice", true},
					{"Legs", 35, 35, 25, 15, "Ice", false}
				}
			}},
			// 13: Black Diablos
			{13, {
				{0, 2, 0, 3, 1},
				{2, 2, 3, 2, 1},
				{
					{"Head", 48, 60, 42, 25, "Ice", true},
					{"Chest", 60, 60, 55, 30, "Ice", true},
					{"Wings", 55, 50, 60, 30, "Ice", true},
					{"Tail", 50, 42, 38, 20, "Ice", true},
					{"Legs", 32, 32, 22, 15, "Ice", false}
				}
			}},
			// 14: Kirin
			{14, {
				{3, 2, 0, 2, 1}, // Fire 3, Thunder X
				{2, 2, 0, 2, 0}, // Para X, Stun X
				{
					{"Horn / Head", 60, 65, 55, 25, "Fire", true},
					{"Body", 20, 20, 15, 10, "Fire", false},
					{"Legs", 25, 25, 20, 10, "Fire", false},
					{"Tail", 25, 25, 20, 10, "Fire", false}
				}
			}},
			// 16: Kushala Daora
			{16, {
				{1, 0, 3, 0, 2}, // Thunder 3, Dragon 2
				{3, 1, 1, 3, 2}, // Poison 3, Blast 3
				{
					{"Head", 65, 65, 50, 25, "Thunder", true},
					{"Forelegs", 45, 40, 35, 15, "Thunder", true},
					{"Tail", 50, 45, 40, 20, "Thunder", true},
					{"Wings", 30, 30, 30, 15, "Thunder", false},
					{"Body", 25, 25, 20, 10, "Thunder", false}
				}
			}},
			// 17: Lunastra
			{17, {
				{0, 2, 1, 3, 1}, // Ice 3, Water 2
				{2, 1, 1, 2, 2},
				{
					{"Head", 60, 70, 40, 25, "Ice", true},
					{"Wings", 55, 40, 60, 25, "Ice", true},
					{"Tail", 50, 45, 40, 20, "Ice", true},
					{"Legs", 30, 30, 25, 15, "Ice", false},
					{"Body", 25, 25, 20, 10, "Ice", false}
				}
			}},
			// 18: Teostra
			{18, {
				{0, 3, 1, 3, 1}, // Water 3, Ice 3
				{3, 2, 2, 1, 2}, // Poison 3
				{
					{"Head", 65, 65, 50, 25, "Water", true},
					{"Wings", 55, 40, 55, 20, "Water", true},
					{"Tail", 50, 45, 40, 20, "Water", true},
					{"Legs", 30, 30, 25, 15, "Water", false},
					{"Body", 25, 25, 20, 10, "Water", false}
				}
			}},
			// 20: Deviljho
			{20, {
				{2, 2, 3, 1, 3}, // Thunder 3, Dragon 3
				{3, 3, 3, 2, 2},
				{
					{"Head", 65, 60, 55, 25, "Thunder", true},
					{"Chest", 55, 50, 45, 20, "Thunder", true},
					{"Tail", 50, 45, 40, 20, "Thunder", true},
					{"Hindlegs", 35, 35, 30, 15, "Thunder", false},
					{"Body", 25, 25, 20, 10, "Thunder", false}
				}
			}},
			// 21: Barroth
			{21, {
				{3, 3, 0, 2, 1}, // Fire 3 (clean) / Water 3 (mud)
				{3, 2, 3, 3, 1}, // Poison 3, Para 3, Blast 3
				{
					{"Head (Crown)", 25, 45, 20, 20, "Fire", true},
					{"Forelegs", 60, 60, 55, 25, "Fire", true},
					{"Tail", 50, 45, 40, 20, "Fire", true},
					{"Body", 35, 35, 30, 15, "Fire", false},
					{"Hindlegs", 35, 35, 30, 15, "Fire", false}
				}
			}},
			// 24: Pukei-Pukei
			{24, {
				{2, 0, 3, 2, 1}, // Thunder 3
				{1, 3, 3, 2, 2}, // Sleep 3, Para 3
				{
					{"Head", 75, 75, 65, 30, "Thunder", true},
					{"Wings", 55, 50, 50, 20, "Thunder", true},
					{"Tail", 55, 50, 45, 20, "Thunder", true},
					{"Body", 35, 35, 30, 15, "Thunder", false}
				}
			}},
			// 25: Nergigante
			{25, {
				{1, 1, 3, 1, 2}, // Thunder 3, Dragon 2
				{2, 2, 2, 2, 2},
				{
					{"Head (White Spikes)", 75, 75, 65, 25, "Thunder", true},
					{"Left Foreleg (Spikes)", 65, 65, 55, 20, "Thunder", true},
					{"Right Foreleg (Spikes)", 65, 65, 55, 20, "Thunder", true},
					{"Wings", 45, 40, 45, 15, "Thunder", true},
					{"Tail", 55, 50, 45, 20, "Thunder", true},
					{"Body", 25, 25, 20, 10, "Thunder", false}
				}
			}},
			// 26: Xeno'jiiva
			{26, {
				{2, 2, 2, 2, 2}, // All 2 stars
				{3, 1, 2, 3, 1}, // Poison 3, Blast 3
				{
					{"Head", 70, 70, 60, 20, "Dragon", true},
					{"Chest (Critical State)", 75, 75, 70, 25, "Dragon", true},
					{"Forelegs", 50, 50, 45, 15, "Dragon", true},
					{"Tail", 55, 50, 45, 20, "Dragon", true},
					{"Wings", 45, 40, 45, 15, "Dragon", true}
				}
			}},
			// 30: Tobi-Kadachi
			{30, {
				{2, 3, 0, 2, 1}, // Water 3
				{3, 2, 2, 2, 2}, // Poison 3
				{
					{"Head", 70, 70, 60, 25, "Water", true},
					{"Tail", 65, 60, 55, 25, "Water", true},
					{"Forelegs", 45, 45, 40, 15, "Water", true},
					{"Body", 35, 35, 30, 15, "Water", false}
				}
			}},
			// 32: Legiana
			{32, {
				{2, 0, 3, 0, 1}, // Thunder 3, Fire 2
				{3, 2, 2, 2, 2}, // Poison 3
				{
					{"Head", 70, 70, 60, 30, "Thunder", true},
					{"Wings", 55, 50, 55, 25, "Thunder", true},
					{"Tail", 55, 50, 45, 20, "Thunder", true},
					{"Body", 30, 30, 25, 15, "Thunder", false}
				}
			}},
			// 34: Odogaron
			{34, {
				{1, 1, 2, 3, 0}, // Ice 3, Thunder 2
				{1, 2, 3, 2, 2}, // Para 3
				{
					{"Head", 70, 70, 60, 25, "Ice", true},
					{"Forelegs", 60, 60, 50, 20, "Ice", true},
					{"Tail", 55, 50, 45, 20, "Ice", true},
					{"Body", 30, 30, 25, 15, "Ice", false}
				}
			}},
			// 36: Vaal Hazak
			{36, {
				{3, 0, 1, 1, 3}, // Fire 3, Dragon 3
				{1, 1, 1, 2, 2},
				{
					{"Head", 65, 65, 50, 30, "Fire", true},
					{"Chest", 55, 50, 45, 25, "Fire", true},
					{"Tail", 50, 45, 40, 20, "Fire", true},
					{"Wings", 30, 30, 30, 15, "Fire", false},
					{"Body", 25, 25, 20, 10, "Fire", false}
				}
			}},
			// 61: Tigrex
			{61, {
				{0, 1, 3, 0, 2}, // Thunder 3, Dragon 2
				{2, 2, 2, 2, 2},
				{
					{"Head", 70, 70, 55, 25, "Thunder", true},
					{"Forelegs", 60, 60, 45, 20, "Thunder", true},
					{"Tail", 50, 45, 40, 15, "Thunder", true},
					{"Body", 30, 30, 25, 10, "Thunder", false}
				}
			}},
			// 62: Nargacuga
			{62, {
				{2, 0, 3, 0, 1}, // Thunder 3, Fire 2
				{2, 2, 2, 2, 2},
				{
					{"Head", 65, 65, 50, 25, "Thunder", true},
					{"Blade Wings", 45, 40, 45, 20, "Thunder", true},
					{"Tail", 55, 50, 45, 20, "Thunder", true},
					{"Body", 30, 30, 25, 10, "Thunder", false}
				}
			}},
			// 63: Barioth
			{63, {
				{3, 0, 2, 0, 1}, // Fire 3, Thunder 2
				{2, 2, 2, 2, 2},
				{
					{"Head", 65, 65, 50, 25, "Fire", true},
					{"Spiked Wings", 50, 50, 45, 20, "Fire", true},
					{"Tail", 50, 45, 40, 15, "Fire", true},
					{"Body", 30, 30, 25, 10, "Fire", false}
				}
			}},
			// 65: Brachydios
			{65, {
				{0, 3, 1, 3, 1}, // Ice 3, Water 3
				{2, 2, 2, 1, 2},
				{
					{"Head", 65, 65, 50, 25, "Ice", true},
					{"Pounders / Arms", 55, 55, 45, 20, "Ice", true},
					{"Tail", 50, 45, 40, 15, "Ice", true},
					{"Body", 30, 30, 25, 10, "Ice", false}
				}
			}},
			// 66: Glavenus
			{66, {
				{0, 3, 1, 2, 1}, // Water 3, Ice 2
				{2, 2, 2, 1, 2},
				{
					{"Head", 65, 65, 50, 25, "Water", true},
					{"Throat (Heated)", 75, 70, 60, 30, "Water", true},
					{"Tail Blade", 65, 55, 45, 25, "Water", true},
					{"Body", 30, 30, 25, 10, "Water", false}
				}
			}},
			// 70: Ruiner Nergigante
			{70, {
				{1, 1, 3, 1, 2},
				{2, 2, 2, 2, 2},
				{
					{"Head (Spikes)", 75, 75, 65, 25, "Thunder", true},
					{"Forelegs (Spikes)", 65, 65, 55, 20, "Thunder", true},
					{"Wings", 45, 40, 45, 15, "Thunder", true},
					{"Tail", 55, 50, 45, 20, "Thunder", true},
					{"Body", 25, 25, 20, 10, "Thunder", false}
				}
			}},
			// 79: Velkhana
			{79, {
				{3, 1, 2, 0, 2}, // Fire 3, Thunder 2, Dragon 2
				{2, 2, 2, 3, 2}, // Blast 3
				{
					{"Head", 65, 65, 50, 30, "Fire", true},
					{"Wings", 55, 45, 50, 20, "Fire", true},
					{"Tail", 55, 50, 45, 25, "Fire", true},
					{"Body", 30, 30, 25, 15, "Fire", false}
				}
			}},
			// 80: Namielle
			{80, {
				{3, 0, 0, 1, 1}, // Fire 3, Ice 1
				{3, 2, 1, 3, 2}, // Poison 3, Blast 3
				{
					{"Head", 65, 65, 50, 35, "Fire", true},
					{"Forelegs", 55, 50, 45, 25, "Fire", true},
					{"Wings", 45, 40, 45, 20, "Fire", true},
					{"Tail", 50, 45, 40, 20, "Fire", true},
					{"Body", 30, 30, 25, 15, "Fire", false}
				}
			}},
			// 87: Alatreon
			{87, {
				{3, 1, 1, 3, 2}, // Fire 3 (in Ice mode) / Ice 3 (in Fire mode), Dragon 2
				{2, 1, 2, 2, 1},
				{
					{"Horns / Head", 85, 80, 60, 25, "Ice / Fire", true},
					{"Forelegs", 60, 55, 45, 20, "Ice / Fire", true},
					{"Wings", 45, 40, 45, 15, "Ice / Fire", true},
					{"Tail", 55, 50, 45, 15, "Ice / Fire", true},
					{"Body", 30, 30, 25, 10, "Ice / Fire", false}
				}
			}},
			// 91: Rajang
			{91, {
				{0, 1, 0, 3, 0}, // Ice 3, Water 1
				{2, 2, 1, 2, 1},
				{
					{"Head", 65, 65, 55, 30, "Ice", true},
					{"Tail", 60, 55, 50, 25, "Ice", true},
					{"Forearms", 45, 45, 35, 15, "Ice", true},
					{"Body / Hindlegs", 45, 45, 40, 15, "Ice", true}
				}
			}},
			// 94: Zinogre
			{94, {
				{1, 2, 0, 3, 1}, // Ice 3, Water 2
				{2, 2, 1, 2, 2},
				{
					{"Head", 70, 70, 55, 30, "Ice", true},
					{"Back", 65, 60, 50, 25, "Ice", true},
					{"Forelegs", 50, 45, 40, 15, "Ice", true},
					{"Tail", 55, 50, 45, 20, "Ice", true},
					{"Body", 30, 30, 25, 10, "Ice", false}
				}
			}},
			// 97: Safi'jiiva
			{97, {
				{2, 2, 2, 2, 2}, // All 2 stars
				{3, 1, 1, 3, 1}, // Poison 3, Blast 3
				{
					{"Head", 75, 75, 60, 20, "Dragon", true},
					{"Forelegs", 55, 50, 45, 15, "Dragon", true},
					{"Hindlegs", 50, 45, 40, 15, "Dragon", true},
					{"Wings", 45, 40, 45, 15, "Dragon", true},
					{"Tail", 60, 50, 45, 15, "Dragon", true},
					{"Chest (Supercritical)", 80, 80, 75, 25, "Dragon", true}
				}
			}},
			// 101: Fatalis
			{101, {
				{0, 0, 1, 2, 3}, // Dragon 3, Ice 2, Thunder 1
				{0, 0, 0, 0, 0}, // Immune to all status
				{
					{"Head", 75, 75, 33, 25, "Dragon", true},
					{"Chest", 45, 45, 45, 15, "Dragon", true},
					{"Forelegs", 45, 45, 25, 10, "Dragon", true},
					{"Wings", 30, 30, 30, 10, "Dragon", false},
					{"Tail", 25, 25, 20, 10, "Dragon", false}
				}
			}}
		};

		if (auto it = s_profile_db.find(monster_id); it != s_profile_db.end())
		{
			return it->second;
		}

		return s_default_profile;
	}
}
