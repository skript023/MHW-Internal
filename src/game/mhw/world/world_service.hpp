#pragma once
#include "data/world_data.hpp"
#include "game/classes/fwddec.hpp"

#include <string>

namespace big
{
	class world_service
	{
		static world_service& get()
		{
			static world_service instance;
			return instance;
		}

		_NODISCARD std::string get_weather_impl();
		_NODISCARD float get_time_of_date_impl();

	public:
		explicit world_service() = default;
		~world_service() noexcept = default;

		world_service(world_service const&) = delete;
		world_service& operator=(world_service const&) = delete;

		world_service(world_service&&) = delete;
		world_service& operator=(world_service&&) = delete;

		_NODISCARD static std::string get_weather()
		{
			return get().get_weather_impl();
		}
		_NODISCARD static float get_time_of_date()
		{
			return get().get_time_of_date_impl();
		}
	};
}
