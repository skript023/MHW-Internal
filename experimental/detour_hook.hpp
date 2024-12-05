#pragma once
#include "hook.hpp"

namespace big
{
	class detour_hook : public base_hook
	{
	public:
		explicit detour_hook(const std::string_view name, void* target, void* detour);
		~detour_hook() noexcept;

		bool enable();
		bool disable();

		void enable_immediately() const;
		void disable_immediately() const;

		template <auto T>
		auto get_original();

		void fix_hook_address();
	private:
		void* m_target;
		void* m_detour;
		void* m_original{};
	};

	template <auto T>
	inline auto detour_hook::get_original()
	{
		return static_cast<decltype(T)>(m_original);
	}
}
