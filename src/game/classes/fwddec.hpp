#pragma once
#include "core/utility/vector.hpp"

#include "core/common.hpp"

namespace big
{
	class GatheringMonster
	{
	public:
		char padding[199];
		uint64_t m_material_count; //0xC8
		uint64_t m_body_count;     //0XD0
	};
	static_assert(sizeof(GatheringMonster) == 0xD8);

	class PlayerStat
	{
	public:
		char m_pad0x18F[399];
		float m_frostcraft_recharges_drawn; //0x190
		char padding[1556];                 //0x194
		float m_critical_boost;             //0x7A8
	};
	static_assert(sizeof(PlayerStat) == 0x7AC);

	class Consumeable
	{
	public:
		uint32_t field_04;  // corresponds to [rax + 0x04]
		uint32_t field_08;  // corresponds to [rax + 0x08]
		uint32_t field_0C;  // corresponds to [rax + 0x0C]
		uint32_t field_10;  // corresponds to [rax + 0x10]
		uintptr_t field_14; // corresponds to [rax + 0x14]
	};
	static_assert(sizeof(Consumeable) == 0x18);

	struct LayeredArmour
	{
		uint32_t m_layered_head;  //0xE7434
		uint32_t m_layered_chest; //0xE7438
		uint32_t m_layered_arms;  //0xE743C
		uint32_t m_layered_waist; //0xE7440
		uint32_t m_layered_legs;  //0xE7444
	};

	struct Palico
	{
		char m_name[20];          //0x10261C
		char pad_102630[44];      //0x102630
		uint32_t m_experience;    //0x10265C
		uint32_t m_vigorwasp;     //0x102660
		uint32_t m_flashfly;      //0x102664
		uint32_t m_shieldspire;   //0x102668
		uint32_t m_coral;         //0x10266C
		uint32_t m_plunder_blade; //0x102670
		uint32_t m_cocktail;      //0x102674
		char pad_x102678[40];     //0x102678
		uint32_t m_bugtrappers;   //0x1026A0
		uint32_t m_protectors;    //0x1026A4
		uint32_t m_troupers;      //0x1026A8
		uint32_t m_plunderers;    //0x1026AC
		uint32_t m_linguist;      //0x1026B0
		uint32_t m_unk;           //0x1026B4
		uint32_t m_unity;         //0x1026B8
	};

	class Character
	{
	public:
		char pad_0000[0x50];               //0x0000
		char m_name[16];                   //0x0050
		char pad_0060[48];                 //0x0060
		uint32_t m_high_rank_level;        //0x0090
		uint32_t m_zeni;                   //0x0094
		uint32_t m_research_points;        //0x0098
		uint32_t m_high_rank_experience;   //0x009C
		uint32_t m_time_playing;           //0x00A0
		char pad_00A4[48];                 //0x00A4
		uint32_t m_master_rank_level;      //0x00D4
		uint32_t m_master_rank_experience; //0x00D8
		                                   //char pad_00DC[947032]; //0x00DC
		                                   //LayeredArmour m_layered_armour;
		                                   //char pad_E74448[111060]; //0xE7448
		                                   //Palico m_palico;
		                                   //char pad_1026BC[1541640]; //0x1026BC
		                                   //uint32_t m_lucky_voucher; //0x27ACC4
	};
	//static_assert(sizeof(Character) == 0x27ACC8);
	static_assert(sizeof(Character) == 0xDC);

	class Equipment
	{
	public:
		char pad_0000[11];        //0x00
		int8_t m_num_active_slot; //0x0B
		char pad_000C[8];         //0x0C
		short m_id_layered;       //0x14
		short m_defense;          //0x16
		char pad_0018[6];         //0x18
		short m_id_ed;            //0x1E
		char pad_0020[12];        //0x20
		int8_t m_fire_res;        //0x2C
		int8_t m_water_res;       //0x2D
		int8_t m_thunder_res;     //0x2E
		int8_t m_ice_res;         //0x2F
		int8_t m_dragon_res;      //0x30
		int8_t m_bonus_level;     //0x31
		char pad_0032;            //0x32
		int8_t m_skill_level_a;   //0x33
		int8_t m_skill_level_b;   //0x34
		int8_t m_skill_level_c;   //0x35
		int8_t m_set_bonus;       //0x36
		char m_pad_0037[3];       //0x37
		int8_t m_skill_a;         //0x3A
		char pad_003B;            //0x3B
		int8_t m_skill_b;         //0x3C
		char pad_003D;            //0x3D
		int8_t m_skill_c;         //0x3E
		int8_t m_slot_a_level;    //0x40
		int8_t m_slot_b_level;    //0x41
		int8_t m_slot_c_level;    //0x42
	};
	static_assert(sizeof(Equipment) == 0x42);

	class MeleeWeapon
	{
	public:
		char pad_0000[0x1C];                 //0x0000
		uint8_t m_rarity;                    //0x001C
		uint8_t m_sharpness_quality;         //0x001D
		uint8_t m_sharpness_amount;          //0x001E
		uint8_t pad_001F;                    //0x001F
		unsigned short m_attack;             //0x0020
		unsigned short m_defense;            //0x0022
		uint8_t m_affinity;                  //0x0024
		uint8_t m_element_type;              //0x0025
		unsigned short m_elemental_atk;      //0x0026
		uint8_t m_hidden_element_type;       //0x0028
		uint8_t m_pad_0029;                  //0x0029
		unsigned short m_hidden_element_atk; //0x002A
		uint8_t m_elderseal;                 //0x002C
		uint8_t m_active_slot;               //0x002D
		uint8_t m_slot_a_level;              //0x002E
		uint8_t m_slot_b_level;              //0x002F
		uint8_t m_slot_c_level;              //0x0030
		uint8_t m_special_bonus;             //0x0031 Shelling Level Phial Type (CB) Phial Type (SA)
		char pad_0032[16];                   //0x0032
		unsigned short m_weapon_id;          //0x0042
		char pad_0044[4];                    //0x0044
		uint8_t m_skill;                     //0x0048
	};
	static_assert(sizeof(MeleeWeapon) == 0x4A);

	class RangedWeapon
	{
	public:
		char pad_0000[32];             //0x0000
		uint8_t m_rarity;              //0x0020
		uint8_t m_unk_0021;            //0x0021
		uint16_t m_attack;             //0x0022
		uint16_t m_defense;            //0x0024
		uint8_t m_affinity;            //0x0026
		uint8_t m_element_type;        //0x0027
		uint16_t m_element_atk;        //0x0028
		uint8_t m_hidden_element_type; //0x002A
		uint8_t m_unk_002B;            //0x002B
		uint16_t m_hidden_element_atk; //0x002C
		char pad_002E[4];              //0x002E
		uint8_t m_deviation;           //0x0032
		uint8_t m_active_slot;         //0x0033
		uint8_t m_slot_a_level;        //0x0034
		uint8_t m_slot_b_level;        //0x0035
		uint8_t m_slot_c_level;        //0x0036
		char pad_0037[14];             //0x0037
		uint8_t m_bowgun_special_ammo; //0x0045
		char pad_0046[2];              //0x0046
		uint16_t m_weapon_id;          //0x0048
		char pad_004A[6];              //0x004A
		uint8_t m_skill;               //0x0050
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
		char pad_0000[352];   //0x0000
		Vector3 m_position;   //0x0160
		char pad_016C[2276];  //0x016C
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
		int64_t pad_0000;              //0x0000
		int32_t m_ingredient_slots[6]; //0x0008
		char pad_0020[48];             //0x0020
		int32_t m_skill_a;             //0x0050
		int32_t m_skill_category;      //0x0054
		int32_t pad_0058;              //0x0058
		int32_t m_skill_b;             //0x005C
		char pad_0060[2];              //0x0060
		int32_t m_skill_c;             //0x0062
		char pad_0066[58];             //0x0066
		int32_t m_fresh_ingredients;   //0x00A0
		char pad_00A4[12];             //0x00A4
		int32_t m_health_effect;       //0x00B0
		int32_t m_stamina_effect;      //0x00B4
		int32_t m_attack_effect;       //0x00B8
		int32_t m_defense_effect;      //0x00BC
		int32_t m_elemental_res;       //0x00C0
		char pad_00C4[20];             //0x00C4
		int32_t m_palico_health;       //0x00D8
	};
	static_assert(sizeof(MealStats) == 0xDC);
#pragma pack(pop)

	class PlayerParams
	{
	public:
		uint32_t m_base;                   //0x00
		char pad_0004[180];                //0x04
		float m_gravity;                   //0xB8
		float m_gravity_speed_rate;        //0xBC
		char pad_00C0[600];                //0xC0
		float m_health_initial;            //0x318
		float m_health_max;                //0x31C
		float m_health_recover_ratio;      //0x320
		float m_health_recover_delay;      //0x324
		float m_health_recover_interval;   //0x328
		float m_health_recover_amount;     //0x32C
		float m_stamina_initial;           //0x330
		float m_stamina_max;               //0x334
		float m_stamina_tired;             //0x338
		float m_stamina_recover;           //0x33C
		float m_stamina_regen_rate;        //0x340
		float m_stamina_reduction_eat;     //0x344
		float m_stamina_reduction_timer;   //0x348
		float m_stamina_escape_dash_cost;  //0x34C
		float m_stamina_cost_rate;         //0x350
		float unk_354;                     //0x354
		float m_stamina_multiplier_action; //0x358
		float m_stamina_multiplier_bow;    //0x35C
		float m_stamina_cost_action;       //0x360
		char pad_364[12];                  //0x364
		float m_dp_cost;                   //0x370
		char pad_0374[14];                 //0x374
		float unk_388;                     //0x388
		char pad_38C[212];                 //0x38C
		float m_bow_charge_step;           //0x460 (actually 0x45C)
		char pad_0460[0x938 - (0x45C + sizeof(float))]; // 0x460 to 0x938
		float m_walk_speed;                //0x0938
		char pad_093C[0x0954 - (0x0938 + sizeof(float))]; // 0x093C to 0x0954
		float m_room_run_speed;            //0x0954
		float m_run_speed;                 //0x0958
		char pad_095C[0x1574 - (0x0958 + sizeof(float))]; // 0x095C to 0x1574
		float m_carve_speeds[4];           //0x1574
	};
	static_assert(offsetof(PlayerParams, m_walk_speed) == 0x0938);
	static_assert(offsetof(PlayerParams, m_room_run_speed) == 0x0954);
	static_assert(offsetof(PlayerParams, m_carve_speeds) == 0x1574);

	class MonsterWorldStatus
	{
	public:
		char pad_0000[120328]; //0x0000
		float m_timer;         //0x1D608
		float m_max_timer;     //0x1D60C
		char pad_1D610[424];   //0x1D610
		int32_t m_monster_id;  //0x1D7B8
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
		char pad_0000[352];                        //0x0000
		Vector3 m_position;                        //0x0160
		char pad_016C[28];                         //0x016C
		float m_size;                              //0x0188
		char pad_018C[29924];                      //0x018C
		MonsterStats* m_monster_stats;             //0x7670
		char pad_7678[44032];                      //0x7678
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

	class World
	{
	public:
		char pad_0000[56];   //0x0000
		float m_time_of_day; //0x0038
		float m_unk_3C;      //0x003C
		float m_unk_40;      //0x0040
		char pad_0044[72];   //0x0044
		uint32_t m_weather;  //0x008C
	};
	static_assert(sizeof(World) == 0x90);

	// =========================================================================
	// In-World Hunter Buffs Container
	// =========================================================================
	class HunterBuffContainer
	{
	public:
		char pad_0000[0x690];                 // 0x0000
		float m_dash_juice;                   // 0x0690
		char pad_0694[0x6A0 - 0x694];         // 0x0694
		float m_might_seed;                   // 0x06A0
		char pad_06A4[0x6B0 - 0x6A4];         // 0x06A4
		float m_adamant_seed;                 // 0x06B0
		char pad_06B4[0x6C4 - 0x6B4];         // 0x06B4
		float m_demon_powder;                 // 0x06C4
		float m_hardshell_powder;             // 0x06C8
		float m_demondrug;                    // 0x06CC
		float m_armorskin;                    // 0x06D0
		char pad_06D4[0x6EC - 0x6D4];         // 0x06D4
		float m_cool_drink;                   // 0x06EC
		float m_hot_drink;                    // 0x06F0
	};
	static_assert(offsetof(HunterBuffContainer, m_dash_juice) == 0x0690);
	static_assert(offsetof(HunterBuffContainer, m_might_seed) == 0x06A0);
	static_assert(offsetof(HunterBuffContainer, m_adamant_seed) == 0x06B0);
	static_assert(offsetof(HunterBuffContainer, m_demon_powder) == 0x06C4);
	static_assert(offsetof(HunterBuffContainer, m_hardshell_powder) == 0x06C8);
	static_assert(offsetof(HunterBuffContainer, m_demondrug) == 0x06CC);
	static_assert(offsetof(HunterBuffContainer, m_armorskin) == 0x06D0);
	static_assert(offsetof(HunterBuffContainer, m_cool_drink) == 0x06EC);
	static_assert(offsetof(HunterBuffContainer, m_hot_drink) == 0x06F0);

	// =========================================================================
	// In-World Hunter & Character Entity
	// =========================================================================
	class sHunter
	{
	public:
		char pad_0000[3504];                  // 0x0000
		PlayerPosition* m_position_component; // 0x0DB0
		char pad_0DB8[84];                    // 0x0DB8
		float m_mantle_cooldown[2];           // 0x0E0C
		char pad_0E14[232];                   // 0x0E14
		float m_mantle_duration[2];           // 0x0EFC
		char pad_0F04[26412];                 // 0x0F04
		float m_health;                       // 0x7630
		float m_max_health;                   // 0x7634
		char pad_7638[1768];                  // 0x7638
		HunterBuffContainer* m_buffs_ptr;     // 0x7D20
		char pad_7D28[43456];                 // 0x7D28
		int32_t m_weapon_id;                  // 0x126E8
	};
	static_assert(offsetof(sHunter, m_position_component) == 0x0DB0);
	static_assert(offsetof(sHunter, m_mantle_cooldown) == 0x0E0C);
	static_assert(offsetof(sHunter, m_mantle_duration) == 0x0EFC);
	static_assert(offsetof(sHunter, m_health) == 0x7630);
	static_assert(offsetof(sHunter, m_max_health) == 0x7634);
	static_assert(offsetof(sHunter, m_buffs_ptr) == 0x7D20);
	static_assert(offsetof(sHunter, m_weapon_id) == 0x126E8);

	// =========================================================================
	// Player Data Manager (4 map slots)
	// =========================================================================
	class sPlayerSlot
	{
	public:
		sHunter* m_hunter;                    // 0x0000
		char pad_0008[0xA0 - sizeof(sHunter*)]; // 0x0008
	};
	static_assert(sizeof(sPlayerSlot) == 0xA0);

	class sPlayerData
	{
	public:
		char pad_0000[0x80];                  // 0x0000
		sPlayerSlot m_slots[4];               // 0x0080: slots 0..3
	};
	static_assert(offsetof(sPlayerData, m_slots) == 0x80);

	// =========================================================================
	// Multiplayer Network Session
	// =========================================================================
	class sHunterInfo
	{
	public:
		char pad_0000[0x14];                  // 0x0000
		uint32_t m_game_build;                // 0x0014
		char pad_0018[0x34 - 0x18];           // 0x0018
		int32_t m_weapon_id;                  // 0x0034
		char pad_0038[0x50 - 0x38];           // 0x0038
		char m_name_short[32];                // 0x0050
		char pad_0070[0x80 - 0x70];           // 0x0070
		char m_palico_name[32];               // 0x0080
		char pad_00A0[0x198 - 0xA0];          // 0x00A0
		char m_name[32];                      // 0x0198
	};
	static_assert(offsetof(sHunterInfo, m_weapon_id) == 0x34);
	static_assert(offsetof(sHunterInfo, m_name_short) == 0x50);
	static_assert(offsetof(sHunterInfo, m_palico_name) == 0x80);
	static_assert(offsetof(sHunterInfo, m_name) == 0x198);

	struct sSessionEntry
	{
		char pad_0000[0x28];                  // 0x0000
		sHunterInfo* m_hunter_info;           // 0x0028
	};
	static_assert(offsetof(sSessionEntry, m_hunter_info) == 0x28);

	class sSessionData
	{
	public:
		char pad_0000[0x53890];               // 0x00000
		uint32_t m_member_count;              // 0x53890
		char pad_53894[0x538A0 - 0x53894];    // 0x53894
		sSessionEntry** m_member_array;       // 0x538A0
	};
	static_assert(offsetof(sSessionData, m_member_count) == 0x53890);
	static_assert(offsetof(sSessionData, m_member_array) == 0x538A0);

	// =========================================================================
	// Camera & View Projection
	// =========================================================================
	struct Matrix4x4
	{
		float m[16];
	};
	static_assert(sizeof(Matrix4x4) == 64);

	class sCamera
	{
	public:
		char pad_0000[0xF0];                  // 0x0000
		Matrix4x4 m_view_matrix;              // 0x00F0
		Matrix4x4 m_proj_matrix;              // 0x0130
	};
	static_assert(offsetof(sCamera, m_view_matrix) == 0xF0);
	static_assert(offsetof(sCamera, m_proj_matrix) == 0x130);

	// =========================================================================
	// Weapon Specific Structures
	// =========================================================================
	struct BowgunMods
	{
		uint32_t m_recoil;                    // 0x00
		uint32_t m_reload;                    // 0x04
		uint32_t m_deviation;                 // 0x08
		uint32_t m_close_range;               // 0x0C
		uint32_t m_ranged_atk_up;             // 0x10
		uint32_t m_shield;                    // 0x14
		uint32_t m_special_scope;             // 0x18
		uint32_t m_evading_reload;            // 0x1C
		uint32_t m_wyvernheart_mod;           // 0x20
		uint32_t m_wyvernsnipe_mod;           // 0x24
		uint32_t m_wyvernblast_mod;           // 0x28
		uint32_t m_power_barrel;              // 0x2C
		uint32_t m_long_barrel;               // 0x30
	};
	static_assert(sizeof(BowgunMods) == 0x34);

	class BowgunAmmoParams
	{
	public:
		char pad_0000[0x13874];               // 0x0000
		int32_t m_ammo_type_1;                // 0x13874
		int32_t m_ammo_type_2;                // 0x13878
		int32_t m_ammo_type_3;                // 0x1387C
	};
	static_assert(offsetof(BowgunAmmoParams, m_ammo_type_1) == 0x13874);
	static_assert(offsetof(BowgunAmmoParams, m_ammo_type_2) == 0x13878);
	static_assert(offsetof(BowgunAmmoParams, m_ammo_type_3) == 0x1387C);

	class BowgunData
	{
	public:
		char pad_0000[2480];                  // 0x0000
		BowgunAmmoParams* m_param_ptr;        // 0x09B0 (2480)
		char pad_09B8[0x2348 - 0x09B8];       // 0x09B8
		BowgunMods m_mods;                    // 0x2348
		char pad_237C[0x23A8 - (0x2348 + sizeof(BowgunMods))]; // 0x237C
		int64_t m_active_clips[13];           // 0x23A8
		char pad_2410[0x2578 - (0x23A8 + sizeof(int64_t) * 13)]; // 0x2410
		int64_t m_ammo_clip[13];              // 0x2578
	};
	static_assert(offsetof(BowgunData, m_mods) == 0x2348);
	static_assert(offsetof(BowgunData, m_active_clips) == 0x23A8);
	static_assert(offsetof(BowgunData, m_ammo_clip) == 0x2578);

	class InsectGlaiveData
	{
	public:
		char pad_0000[0x2368];                // 0x0000
		float m_red_extract_duration;         // 0x2368
		float m_white_extract_duration;       // 0x236C
		uint32_t m_triple_buff_level;         // 0x2370
		float m_orange_extract_duration;      // 0x2374
	};
	static_assert(offsetof(InsectGlaiveData, m_red_extract_duration) == 0x2368);
	static_assert(offsetof(InsectGlaiveData, m_triple_buff_level) == 0x2370);

	// =========================================================================
	// Inventory Item Slot
	// =========================================================================
	class ItemSlot
	{
	public:
		char pad_0000[8];                     // 0x0000
		uint32_t m_item_id;                   // 0x0008
	};
	static_assert(offsetof(ItemSlot, m_item_id) == 0x08);


	// =========================================================================
	// Weapon States & Action Controllers (Mid-Hook Structures)
	// =========================================================================
	class GreatswordData
	{
	public:
		char pad_0000[0x2350];
		float m_charge_timer;          // 0x2350
		char pad_2354[4];
		uint32_t m_charge_level;       // 0x2358
	};
	static_assert(offsetof(GreatswordData, m_charge_timer) == 0x2350);
	static_assert(offsetof(GreatswordData, m_charge_level) == 0x2358);

	class LongswordData
	{
	public:
		char pad_0000[0x2368];
		float m_spirit_gauge;          // 0x2368
		char pad_236C[4];
		uint32_t m_spirit_level;       // 0x2370
		float m_spirit_duration;       // 0x2374
	};
	static_assert(offsetof(LongswordData, m_spirit_gauge) == 0x2368);
	static_assert(offsetof(LongswordData, m_spirit_level) == 0x2370);
	static_assert(offsetof(LongswordData, m_spirit_duration) == 0x2374);

	class DualBladesData
	{
	public:
		char pad_0000[0x2368];
		uint8_t pad_2368;
		uint8_t m_is_demon_mode;       // 0x2369
		char pad_236A[2];
		float m_demon_gauge;           // 0x236C
	};
	static_assert(offsetof(DualBladesData, m_is_demon_mode) == 0x2369);
	static_assert(offsetof(DualBladesData, m_demon_gauge) == 0x236C);

	class SwitchAxeData
	{
	public:
		char pad_0000[0x2350];
		float m_sword_gauge;           // 0x2350
	};
	static_assert(offsetof(SwitchAxeData, m_sword_gauge) == 0x2350);

	class GunlanceData
	{
	public:
		char pad_0000[0x2368];
		uint32_t m_max_ammo;           // 0x2368
		uint32_t m_ammo;               // 0x236C
		char pad_2370[8];
		uint32_t m_wyrmstake;          // 0x2378
	};
	static_assert(offsetof(GunlanceData, m_max_ammo) == 0x2368);
	static_assert(offsetof(GunlanceData, m_ammo) == 0x236C);
	static_assert(offsetof(GunlanceData, m_wyrmstake) == 0x2378);

	class ChargeBladeData
	{
	public:
		char pad_0000[0x2372];
		uint16_t m_phials_charge;      // 0x2372
		uint32_t m_phials_count;       // 0x2374
		float m_shield_charge_timer;   // 0x2378
		float m_sword_charge_timer;    // 0x237C
	};
	static_assert(offsetof(ChargeBladeData, m_phials_charge) == 0x2372);
	static_assert(offsetof(ChargeBladeData, m_phials_count) == 0x2374);
	static_assert(offsetof(ChargeBladeData, m_shield_charge_timer) == 0x2378);
	static_assert(offsetof(ChargeBladeData, m_sword_charge_timer) == 0x237C);

	class BowData
	{
	public:
		char pad_0000[0x45];
		uint8_t m_coatings;            // 0x0045
		char pad_0046[0x23B4 - 0x0046];
		uint32_t m_charge_level;       // 0x23B4
	};
	static_assert(offsetof(BowData, m_coatings) == 0x0045);
	static_assert(offsetof(BowData, m_charge_level) == 0x23B4);

	class HeavyBowgunSpecialData
	{
	public:
		char pad_0000[0x09C4];
		float m_wyvernheart_timer;     // 0x09C4
		char pad_09C8[0x2848 - 0x09C8];
		uint32_t m_wyvernblast_ammo;   // 0x2848
	};
	static_assert(offsetof(HeavyBowgunSpecialData, m_wyvernheart_timer) == 0x09C4);
	static_assert(offsetof(HeavyBowgunSpecialData, m_wyvernblast_ammo) == 0x2848);

	class PalicoGadgetData
	{
	public:
		char pad_0000[0xA340];
		float m_cooldown;              // 0xA340
	};
	static_assert(offsetof(PalicoGadgetData, m_cooldown) == 0xA340);

	class SlingerData
	{
	public:
		char pad_0000[0x7E20];
		uint32_t m_ammo;               // 0x7E20
	};
	static_assert(offsetof(SlingerData, m_ammo) == 0x7E20);

	class ArmorSlotData
	{
	public:
		char pad_0000[0x11];
		uint8_t m_slot_count;          // 0x0011
		char pad_0012[0x40 - 0x0012];
		uint8_t m_slot_levels[3];      // 0x0040
	};
	static_assert(offsetof(ArmorSlotData, m_slot_count) == 0x0011);
	static_assert(offsetof(ArmorSlotData, m_slot_levels) == 0x0040);

	class ArmorSlotMelee
	{
	public:
		char pad_0000[0x2D];
		uint8_t m_slot_count;          // 0x002D
		uint8_t m_slot_levels[3];      // 0x002E
	};
	static_assert(offsetof(ArmorSlotMelee, m_slot_count) == 0x002D);
	static_assert(offsetof(ArmorSlotMelee, m_slot_levels) == 0x002E);

	class ArmorSlotRanged
	{
	public:
		char pad_0000[0x33];
		uint8_t m_slot_count;          // 0x0033
		uint8_t m_slot_levels[3];      // 0x0034
	};
	static_assert(offsetof(ArmorSlotRanged, m_slot_count) == 0x0033);
	static_assert(offsetof(ArmorSlotRanged, m_slot_levels) == 0x0034);


	class WeaponSharpnessData

	{

	public:

		char pad_0000[0x20F8];

		uint32_t m_sharpness;          // 0x20F8

	};

	static_assert(offsetof(WeaponSharpnessData, m_sharpness) == 0x20F8);



	class SiegeCannonData

	{

	public:

		char pad_0000[0x0FB8];

		uint32_t m_cannon_balls;       // 0x0FB8

	};

	static_assert(offsetof(SiegeCannonData, m_cannon_balls) == 0x0FB8);



	class MonsterMountData

	{

	public:

		char pad_0000[0x1C170];

		float m_mount_gauge;           // 0x1C170

	};

	static_assert(offsetof(MonsterMountData, m_mount_gauge) == 0x1C170);



	class QuestVoucherData

	{

	public:

		char pad_0000[0x269154];

		uint32_t m_lucky_vouchers;     // 0x269154

	};

	static_assert(offsetof(QuestVoucherData, m_lucky_vouchers) == 0x269154);



	class ArmorSkillLevelData

	{

	public:

		char pad_0000[0x31];

		uint8_t m_skill_level;         // 0x0031

	};

	static_assert(offsetof(ArmorSkillLevelData, m_skill_level) == 0x0031);



	class ScoutflyData

	{

	public:

		char pad_0000[0x0F62A8];

		float m_levels[6];             // 0x0F62A8

	};

	static_assert(offsetof(ScoutflyData, m_levels) == 0x0F62A8);



	class KinsectData

	{

	public:

		char pad_0000[0x0A8C];

		float m_stamina;               // 0x0A8C

	};

	static_assert(offsetof(KinsectData, m_stamina) == 0x0A8C);



	class GlaiveAirActionData

	{

	public:

		char pad_0000[0x2428];

		uint32_t m_air_dash_count;     // 0x2428

	};

	static_assert(offsetof(GlaiveAirActionData, m_air_dash_count) == 0x2428);



	class BowgunRecoilData

	{

	public:

		char pad_0000[0x2368];

		int32_t m_recoil_levels[13];   // 0x2368

	};

	static_assert(offsetof(BowgunRecoilData, m_recoil_levels) == 0x2368);



	class MonsterPartEntry

	{

	public:

		char pad_0000[16];                    // 0x0000

		float m_current_hp;                   // 0x0010

		float m_max_hp;                       // 0x0014

		uint32_t m_break_count;               // 0x0018

	};

	static_assert(offsetof(MonsterPartEntry, m_current_hp) == 0x10);

	static_assert(offsetof(MonsterPartEntry, m_max_hp) == 0x14);

	static_assert(offsetof(MonsterPartEntry, m_break_count) == 0x18);



	class MonsterPartsManager

	{

	public:

		char pad_0000[16];                    // 0x0000

		uint32_t m_part_count;                // 0x0010

		char pad_0014[12];                    // 0x0014

		MonsterPartEntry** m_part_entries;    // 0x0020

	};

	static_assert(offsetof(MonsterPartsManager, m_part_count) == 0x10);

	static_assert(offsetof(MonsterPartsManager, m_part_entries) == 0x20);

}

