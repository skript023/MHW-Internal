#pragma once
#include "module.hpp"
#include "common.hpp"
#include "utility/joaat.hpp"

#include <memory>
#include <string_view>
#include <unordered_map>

namespace big
{
	using module = memory::module;

	class module_mgr
	{
	public:
		module_mgr() = default;
		virtual ~module_mgr() = default;
		module_mgr(const module_mgr&) = delete;
		module_mgr(module_mgr&&) noexcept = delete;
		module_mgr& operator=(const module_mgr&) = delete;
		module_mgr& operator=(module_mgr&&) noexcept = delete;

		static module_mgr& get_instance()
		{
			static module_mgr instance;
			return instance;
		}

		static module* get(const std::string_view name)
		{
			return get_instance().get_module(name);
		}

		static module* get(joaat_t hash)
		{
			return get_instance().get_module(hash);
		}

		static bool load_modules()
		{
			return get_instance().load_modules_impl();
		}

		static std::unordered_multimap<std::uint32_t, std::unique_ptr<module>>& get_modules()
		{
			return get_instance().get_modules_impl();
		}

		static bool is_manual_mapped()
		{
			return get_instance().is_manual_mapped_impl();
		}

		module* get_module(const std::string_view name);
		module* get_module(joaat_t hash);

		bool load_modules_impl();
		std::unordered_multimap<std::uint32_t, std::unique_ptr<module>>& get_modules_impl();
		bool is_manual_mapped_impl() const;

	private:
		bool m_manual_mapped = false;
		std::unordered_multimap<std::uint32_t, std::unique_ptr<module>> m_cached_modules;
	};

	inline module_mgr& g_module_mgr = module_mgr::get_instance();
}