#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace big
{
	struct party_damage_entry
	{
		int player_id = 0;
		std::string name = "Hunter";
		std::string weapon = "Great Sword";
		uint32_t total_damage = 0;
		float dps = 0.0f;
		float damage_percent = 0.0f;
		uint32_t cart_count = 0;
		bool is_local = false;
	};

	class damage_service
	{
		static damage_service& get()
		{
			static damage_service instance;
			return instance;
		}

		void update_impl();
		void record_damage_impl(int player_id, uint32_t damage);
		void record_cart_impl(int player_id);
		void reset_impl();

		[[nodiscard]] std::vector<party_damage_entry> get_party_damage_impl();
		[[nodiscard]] float get_hunt_time_impl() const;
		[[nodiscard]] uint32_t get_total_party_damage_impl() const;
		[[nodiscard]] float get_total_party_dps_impl() const;

	public:
		explicit damage_service();
		~damage_service() noexcept = default;

		static void update()
		{
			get().update_impl();
		}
		static void record_damage(int player_id, uint32_t damage)
		{
			get().record_damage_impl(player_id, damage);
		}
		static void record_cart(int player_id)
		{
			get().record_cart_impl(player_id);
		}
		static void reset()
		{
			get().reset_impl();
		}

		[[nodiscard]] static std::vector<party_damage_entry> get_party_damage()
		{
			return get().get_party_damage_impl();
		}
		[[nodiscard]] static float get_hunt_time()
		{
			return get().get_hunt_time_impl();
		}
		[[nodiscard]] static uint32_t get_total_party_damage()
		{
			return get().get_total_party_damage_impl();
		}
		[[nodiscard]] static float get_total_party_dps()
		{
			return get().get_total_party_dps_impl();
		}

	private:
		std::vector<party_damage_entry> m_players;
		float m_hunt_time = 0.0f;
		float m_last_monster_hp[3] = {0.0f, 0.0f, 0.0f};
		bool m_initialized_hp = false;
	};
}
