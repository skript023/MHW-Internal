#pragma once
#include "mhw/fwddec.hpp"
#include "meal_data.hpp"

namespace big
{
	class meal_service
	{
		static meal_service& get()
		{
			static meal_service instance;
			return instance;
		}

		void init_impl(MealStats* instance);
		std::string_view get_buff_impl(uint32_t skill);
		std::string_view get_buff_category_impl();
		MealStats* get_stats_impl() { return m_meal_stat == nullptr ? nullptr : m_meal_stat; };
	public:
		explicit meal_service() = default;
		~meal_service() noexcept = default;

		static void init(MealStats* instance){ get().init_impl(instance); }
		static std::string_view get_buff(uint32_t skill) { return get().get_buff_impl(skill); };
		static std::string_view get_buff_category() { return get().get_buff_category_impl(); };
		static MealStats* get_stats() { return get().get_stats_impl(); };
	private:
		MealStats* m_meal_stat = nullptr;
	};
}