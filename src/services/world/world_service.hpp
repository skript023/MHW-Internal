#pragma once
#include "data/world_data.hpp"
#include "mhw/fwddec.hpp"

namespace big
{
	class world_service
	{
	public:
		explicit world_service();
		~world_service() noexcept;

		world_service(world_service const&) = delete;
		world_service& operator=(world_service const&) = delete;

		world_service(world_service&&) = delete;
		world_service& operator=(world_service&&) = delete;

		_NODISCARD std::string get_weather();
		_NODISCARD float get_time_of_date();

	private:
	};

	inline world_service* g_world_service{};
}