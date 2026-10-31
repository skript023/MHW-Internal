#pragma once
#include "core/utility/vector.hpp"

#include <string>
#include <vector>
#include <cstdint>

namespace big
{
	struct dropped_material_entry
	{
		uint32_t id = 0;
		std::string name;
		Vector3 position;
		float distance = 0.0f;
		float delta_y = 0.0f;
		std::string direction = "N";
		float time_remaining = 38.0f;
		float max_time = 38.0f;
		bool is_guiding_lands = false;
	};

	class drop_service
	{
		static drop_service& get()
		{
			static drop_service instance;
			return instance;
		}

		void update_impl();
		void register_drop_impl(uint32_t id, const std::string& name, const Vector3& pos, bool guiding_lands, float timer);
		void clear_impl();

		[[nodiscard]] std::vector<dropped_material_entry> get_active_drops_impl();
		[[nodiscard]] int get_total_active_drops_impl() const;
		[[nodiscard]] float get_closest_drop_distance_impl() const;

	public:
		explicit drop_service() = default;
		~drop_service() noexcept = default;

		static void update()
		{
			get().update_impl();
		}
		static void register_drop(uint32_t id, const std::string& name, const Vector3& pos, bool guiding_lands = false, float timer = 38.0f)
		{
			get().register_drop_impl(id, name, pos, guiding_lands, timer);
		}
		static void clear()
		{
			get().clear_impl();
		}

		[[nodiscard]] static std::vector<dropped_material_entry> get_active_drops()
		{
			return get().get_active_drops_impl();
		}
		[[nodiscard]] static int get_total_active_drops()
		{
			return get().get_total_active_drops_impl();
		}
		[[nodiscard]] static float get_closest_drop_distance()
		{
			return get().get_closest_drop_distance_impl();
		}

	private:
		std::vector<dropped_material_entry> m_drops;
		float m_last_update_time = 0.0f;
	};
}
