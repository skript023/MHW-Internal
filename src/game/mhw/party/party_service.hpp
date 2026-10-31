#pragma once
#include "core/common.hpp"
#include "core/utility/vector.hpp"
#include <string>
#include <vector>
#include <cstdint>
#include <chrono>

namespace big
{
	struct teammate_info
	{
		int player_id = 0;              // 0 = Local Hunter, 1..3 = Co-op Teammates
		std::string name = "Teammate";
		std::string weapon = "Great Sword";
		int weapon_id = 0;
		Vector3 position{0.0f, 0.0f, 0.0f};
		float health = 150.0f;
		float max_health = 150.0f;
		float distance = 0.0f;          // In-game meters
		bool is_local = false;
		bool is_active = false;
		uintptr_t entity_ptr = 0;
		std::chrono::steady_clock::time_point last_seen{};
	};

	class party_service
	{
		static party_service& get()
		{
			static party_service instance;
			return instance;
		}

		void update_impl();
		void register_player_entity_impl(uintptr_t entity, Vector3 pos);
		[[nodiscard]] std::vector<teammate_info> get_teammates_impl();
		[[nodiscard]] std::vector<teammate_info> get_active_teammates_impl();
		bool teleport_to_teammate_impl(int player_id);
		bool teleport_to_teammate_impl(const teammate_info& teammate);
		void set_preview_mode_impl(bool enabled);
		[[nodiscard]] bool is_preview_mode_impl() const;

	public:
		explicit party_service();
		~party_service() noexcept = default;

		static void update()
		{
			get().update_impl();
		}

		static void register_player_entity(uintptr_t entity, Vector3 pos)
		{
			get().register_player_entity_impl(entity, pos);
		}

		[[nodiscard]] static std::vector<teammate_info> get_teammates()
		{
			return get().get_teammates_impl();
		}

		[[nodiscard]] static std::vector<teammate_info> get_active_teammates()
		{
			return get().get_active_teammates_impl();
		}

		static bool teleport_to_teammate(int player_id)
		{
			return get().teleport_to_teammate_impl(player_id);
		}

		static bool teleport_to_teammate(const teammate_info& teammate)
		{
			return get().teleport_to_teammate_impl(teammate);
		}

		static void set_preview_mode(bool enabled)
		{
			get().set_preview_mode_impl(enabled);
		}

		[[nodiscard]] static bool is_preview_mode()
		{
			return get().is_preview_mode_impl();
		}

		[[nodiscard]] static const char* get_weapon_name(int weapon_id);
		[[nodiscard]] static const char* get_weapon_icon(int weapon_id);

	private:
		std::vector<teammate_info> m_real_teammates;
		std::vector<teammate_info> m_preview_teammates;
		bool m_preview_mode = false;
		std::chrono::steady_clock::time_point m_last_update{};
	};
}
