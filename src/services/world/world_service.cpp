#include "world_service.hpp"
#include "pointers.hpp"

namespace big
{
    world_service::world_service()
    {
        g_world_service = this;
    }

    world_service::~world_service() noexcept
    {
        g_world_service = nullptr;
    }

    std::string world_service::get_weather()
    {
        auto id = (*g_pointers->m_world_data)->m_weather;
        
        if (auto it = world_data::weather_data.find(id); it != world_data::weather_data.end())
        {
            return it->second;
        }

        return std::format("Weather Not Found, ID: {}", id);
    }

    float world_service::get_time_of_date()
    {
        if (auto wd = *g_pointers->m_world_data)
        {
            return wd->m_time_of_day;
        }

        return 0.0f;
    }

} // namespace big
