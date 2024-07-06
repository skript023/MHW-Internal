#pragma once

namespace big
{
	class player
	{
	public:
		player() = delete;
		~player() = delete;

		player(player const& that) = delete;
		player(player&& that) = delete;

		player& operator=(player const& that) = delete;
		player& operator=(player&& that) = delete;

		static void invulnerable();

		static void infinite_stamina();

		static void critical_boost();

		static void easy_part_breaker();

		static void part_breaker();

		static void max_stat_bonus();

		static void max_armor_skill();

		static void max_sharpness();

		static void mount_reset();

		static void inf_mount();

		static void inf_gathering();

		static void inf_lucky_voucher();

		static void cannon_always_loaded();

		static void inf_cannon_fire();

		static void unlock_all_pet();
	};
}