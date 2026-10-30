#include "world_service.hpp"

#include "pointers.hpp"

#include <format>

namespace big
{
	std::string world_service::get_weather_impl()
	{
		if (!g_pointers || !g_pointers->m_world_data || !*g_pointers->m_world_data)
			return "Unknown";

		auto id = (*g_pointers->m_world_data)->m_weather;

		if (auto it = world_data::weather_data.find(id); it != world_data::weather_data.end())
		{
			return it->second;
		}

		return std::format("Weather Not Found, ID: {}", id);
	}

	float world_service::get_time_of_date_impl()
	{
		if (!g_pointers || !g_pointers->m_world_data)
			return 0.0f;

		if (auto wd = *g_pointers->m_world_data)
		{
			return wd->m_time_of_day;
		}

		return 0.0f;
	}
} // namespace big
