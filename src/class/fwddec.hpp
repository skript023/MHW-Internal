#pragma once
#include <common.hpp>
#include "vector.hpp"

namespace big
{
	class GatheringMonster
	{
	public:
		char padding[199];
		uint64_t m_material_count; //0xC8
		uint64_t m_body_count; //0XD0
	};
	static_assert(sizeof(GatheringMonster) == 0xD8);

	class PlayerStat
	{
	public:
		char m_pad0x18F[399];
		float m_frostcraft_recharges_drawn; //0x190
		char padding[1556]; //0x194
		float m_critical_boost; //0x7A8
	};
	static_assert(sizeof(PlayerStat) == 0x7AC);

	class Consumeable
	{
	public:
		uint32_t field_04; // corresponds to [rax + 0x04]
		uint32_t field_08; // corresponds to [rax + 0x08]
		uint32_t field_0C; // corresponds to [rax + 0x0C]
		uint32_t field_10; // corresponds to [rax + 0x10]
		uintptr_t field_14; // corresponds to [rax + 0x14]
	};
	static_assert(sizeof(Consumeable) == 0x18);

	class Character
	{
	public:
		char pad_0000[0x50]; //0x0000
		char m_name[16]; //0x0050
		char pad_0066[42]; //0x0066
		uint32_t m_high_rank_level; //0x0090
		uint32_t m_zeni; //0x0094
		uint32_t m_research_points; //0x0098
		uint32_t m_high_rank_experience; //0x009C
		uint32_t m_time_playing; //0x00A0
		char pad_00A4[48]; //0x00A4
		uint32_t m_master_rank_level; //0x00D4
		uint32_t m_master_rank_experience; //0x00D8
		char pad_00DC[947032]; //0x00DC
		struct LayeredArmour
		{
			uint32_t m_layered_head; //0xE7434
			uint32_t m_layered_chest; //0xE7438
			uint32_t m_layered_arms; //0xE743C
			uint32_t m_layered_waist; //0xE7440
			uint32_t m_layered_legs; //0xE7444
		} m_layered_armour;
		char pad_E74448[111060]; //0xE7448
		struct Palico
		{
			char m_name[20]; //0x10261C
			char pad_102630[44]; //0x102630
			uint32_t m_experience; //0x10265C
			uint32_t m_vigorwasp; //0x102660
			uint32_t m_flashfly; //0x102664
			uint32_t m_shieldspire; //0x102668
			uint32_t m_coral; //0x10266C
			uint32_t m_plunder_blade; //0x102670
			uint32_t m_cocktail; //0x102674
			char pad_x102678[40]; //0x102678
			uint32_t m_bugtrappers; //0x1026A0
			uint32_t m_protectors	; //0x1026A4
			uint32_t m_troupers; //0x1026A8
			uint32_t m_plunderers; //0x1026AC
			uint32_t m_linguist; //0x1026B0
			uint32_t m_unk; //0x1026B4
			uint32_t m_unity; //0x1026B8
		} m_palico;
		char pad_1026BC[1541640]; //0x1026BC
		uint32_t m_lucky_voucher; //0x27ACC4
	};
	static_assert(sizeof(Character) == 0x27ACC4);

	class Equipment
	{
	public:
		char pad_0000[11]; //0x00
		int8_t m_num_active_slot; //0x0B
		char pad_000C[8]; //0x0C
		short m_id_layered; //0x14
		short m_defense; //0x16
		char pad_0018[6]; //0x18
		short m_id_ed; //0x1E
		char pad_0020[12]; //0x20
		int8_t m_fire_res; //0x2C
		int8_t m_water_res; //0x2D
		int8_t m_thunder_res; //0x2E
		int8_t m_ice_res; //0x2F
		int8_t m_dragon_res; //0x30
		int8_t m_bonus_level; //0x31
		char pad_0032; //0x32
		int8_t m_skill_level_a; //0x33
		int8_t m_skill_level_b; //0x34
		int8_t m_skill_level_c; //0x35
		int8_t m_set_bonus; //0x36
		char m_pad_0037[3]; //0x37
		int8_t m_skill_a; //0x3A
		char pad_003B; //0x3B
		int8_t m_skill_b; //0x3C
		char pad_003D; //0x3D
		int8_t m_skill_c; //0x3E
		int8_t m_slot_a_level; //0x40
		int8_t m_slot_b_level; //0x41
		int8_t m_slot_c_level; //0x42
	};
	static_assert(sizeof(Equipment) == 0x42);

	class MeleeWeapon
	{
	public:
		char pad_0000[0x1C]; //0x0000
		uint8_t m_rarity; //0x001C
		uint8_t m_sharpness_quality; //0x001D
		uint8_t m_sharpness_amount; //0x001E
		uint8_t pad_001F; //0x001F
		unsigned short m_attack; //0x0020
		unsigned short m_defense; //0x0022
		uint8_t m_affinity; //0x0024
		uint8_t m_element_type; //0x0025
		unsigned short m_elemental_atk; //0x0026
		uint8_t m_hidden_element_type; //0x0028
		uint8_t m_pad_0029; //0x0029
		unsigned short m_hidden_element_atk; //0x002A
		uint8_t m_elderseal; //0x002C
		uint8_t m_active_slot; //0x002D
		uint8_t m_slot_a_level; //0x002E
		uint8_t m_slot_b_level; //0x002F
		uint8_t m_slot_c_level; //0x0030
		uint8_t m_special_bonus; //0x0031 Shelling Level Phial Type (CB) Phial Type (SA)
		char pad_0032[16]; //0x0032
		unsigned short m_weapon_id; //0x0042
		char pad_0044[4]; //0x0044
		uint8_t m_skill; //0x0048
	};
	static_assert(sizeof(MeleeWeapon) == 0x4A);

	class RangedWeapon
	{
	public:
		char pad_0000[32]; //0x0000
		uint8_t m_rarity; //0x0020
		uint8_t m_unk_0021; //0x0021
		uint16_t m_attack; //0x0022
		uint16_t m_defense; //0x0024
		uint8_t m_affinity; //0x0026
		uint8_t m_element_type; //0x0027
		uint16_t m_element_atk; //0x0028
		uint8_t m_hidden_element_type; //0x002A
		uint8_t m_unk_002B; //0x002B
		uint16_t m_hidden_element_atk; //0x002C
		char pad_002E[4]; //0x002E
		uint8_t m_deviation; //0x0032
		uint8_t m_active_slot; //0x0033
		uint8_t m_slot_a_level; //0x0034
		uint8_t m_slot_b_level; //0x0035
		uint8_t m_slot_c_level; //0x0036
		char pad_0037[14]; //0x0037
		uint8_t m_bowgun_special_ammo; //0x0045
		char pad_0046[2]; //0x0046
		uint16_t m_weapon_id; //0x0048
		char pad_004A[6]; //0x004A
		uint8_t m_skill; //0x0050
	};
	static_assert(sizeof(RangedWeapon) == 0x52);

	class PlayerForward
	{
	public:
		char pad_0000[0x7D30]; //0x0000
		Vector3 m_forward_pos; //0x7D30
	};
	static_assert(sizeof(PlayerForward) == 0x7D3C);

	class PlayerPosition
	{
	public:
		char pad_0000[352]; //0x0000
		Vector3 m_position; //0x0160
		char pad_016C[2276]; //0x016C
		Vector3 m_psycal_pos; //0x0A50
	};
	static_assert(sizeof(PlayerPosition) == 0xA5C);

	class Waypoint
	{
	public:
		char pad_0000[720]; //0x0000
		Vector3 m_waypoint; //0x02D0
	};
	static_assert(sizeof(Waypoint) == 0x2DC);

#pragma pack(push, 1)
	struct MealIngredientSlot
	{
		uint32_t m_ingredient[5];
	};
	static_assert(sizeof(MealIngredientSlot) == 0x14);

	class MealStats
	{
	public:
		int64_t pad_0000; //0x0000
		int32_t m_ingredient_slots[6]; //0x0008
		char pad_0020[48]; //0x0020
		int32_t m_skill_a; //0x0050
		int32_t m_skill_category; //0x0054
		int32_t pad_0058; //0x0058
		int32_t m_skill_b; //0x005C
		char pad_0060[2]; //0x0060
		int32_t m_skill_c; //0x0062
		char pad_0066[58]; //0x0066
		int32_t m_fresh_ingredients; //0x00A0
		char pad_00A4[12]; //0x00A4
		int32_t m_health_effect; //0x00B0
		int32_t m_stamina_effect; //0x00B4
		int32_t m_attack_effect; //0x00B8
		int32_t m_defense_effect; //0x00BC
		int32_t m_elemental_res; //0x00C0
		char pad_00C4[20]; //0x00C4
		int32_t m_palico_health; //0x00D8
	};
	static_assert(sizeof(MealStats) == 0xDC);
#pragma pack(pop)

	class PlayerParams
	{
	public:
		uint32_t m_base; //0x00
		char pad_0004[180]; //0x04
		float m_gravity; //0xB8
		float m_gravity_speed_rate; //0xBC
		char pad_00C0[600]; //0xC0
		float m_health_initial; //0x318
		float m_health_max; //0x31C
		float m_health_recover_ratio; //0x320
		float m_health_recover_delay; //0x324
		float m_health_recover_interval; //0x328
		float m_health_recover_amount; //0x32C
		float m_stamina_initial; //0x330
		float m_stamina_max; //0x334
		float m_stamina_tired; //0x338
		float m_stamina_recover; //0x33C
		float m_stamina_regen_rate; //0x340
		float m_stamina_reduction_eat; //0x344
		float m_stamina_reduction_timer; //0x348
		float m_stamina_escape_dash_cost; //0x34C
		float m_stamina_cost_rate; //0x350
		float unk_354; //0x354
		float m_stamina_multiplier_action; //0x358
		float m_stamina_multiplier_bow; //0x35C
		float m_stamina_cost_action; //0x360
		char pad_364[12]; //0x364
		float m_dp_cost; //0x370
		char pad_0374[14]; //0x374
		float unk_388; //0x388
		char pad_38C[212]; //0x38C
		float m_bow_charge_step; //0x460
	};
	static_assert(sizeof(PlayerParams) == 0x460, "PlayerParams not properly sized.");

	class MonsterWorldStatus
	{
	public:
		char pad_0000[120328]; //0x0000
		float m_timer; //0x1D608
		float m_max_timer; //0x1D60C
		char pad_1D610[424]; //0x1D610
		uint32_t m_monster_id; //0x1D7B8
	};
	static_assert(sizeof(MonsterWorldStatus) == 0x1D7BC, "MonsterWorldStatus not properly sized.");

	class MonsterStats
	{
	public:
		char pad_0000[96];
		float m_max;
		float m_health;
	};
	static_assert(sizeof(MonsterStats) == 0x68, "MonsterStats not properly sized.");

	class MonsterEntry
	{
	public:
		char pad_0000[392]; //0x0000
		float m_size; //0x0188
		char pad_018C[29924]; //0x018C
		MonsterStats* m_monster_stats; //0x7670
		char pad_7678[44032]; //0x7678
		MonsterWorldStatus* m_monster_world_stats; //0x12278
	};
	static_assert(sizeof(MonsterEntry) == 0x12280, "MonsterEntry not properly sized.");

	class Monster
	{
	public:
		char pad_0000[3672];
		MonsterEntry* m_first_world_monster; //0xE58
		char pad_0E60[72];
		MonsterEntry* m_second_world_monster; //0xEA8
		char pad_0EB0[72];
		MonsterEntry* m_third_world_monster; //0xEF8
	};
	static_assert(sizeof(Monster) == 0xF00, "Monster not properly sized.");
}