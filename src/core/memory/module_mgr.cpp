#include "module_mgr.hpp"
#include "common.hpp"
#include "utility/joaat.hpp"

namespace big
{
	module* module_mgr::get_module(const std::string_view name)
	{
		return get_module(joaat(name));
	}

	module* module_mgr::get_module(joaat_t hash)
	{
		if (m_cached_modules.empty())
		{
			load_modules_impl();
		}

		if (const auto& it = m_cached_modules.find(hash); it != m_cached_modules.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	bool module_mgr::load_modules_impl()
	{
		m_cached_modules.clear();
		m_manual_mapped = true;

		const auto peb = reinterpret_cast<PPEB>(NtCurrentTeb()->ProcessEnvironmentBlock);
		if (!peb)
		{
			return false;
		}

		const auto ldr_data = peb->Ldr;
		if (!ldr_data)
		{
			return false;
		}

		const auto module_list = &ldr_data->InMemoryOrderModuleList;
		auto module_entry = module_list->Flink;
		for (; module_list != module_entry; module_entry = module_entry->Flink)
		{
			const auto table_entry = CONTAINING_RECORD(module_entry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			if (!table_entry)
			{
				continue;
			}

			if (table_entry->FullDllName.Buffer)
			{
				auto mod = std::make_unique<module>(table_entry);

				if (mod->base() == reinterpret_cast<uintptr_t>(g_hmodule))
				{
					m_manual_mapped = false;
				}

				m_cached_modules.insert({joaat(mod->name()), std::move(mod)});
			}
		}

		return true;
	}

	std::unordered_multimap<std::uint32_t, std::unique_ptr<module>>& module_mgr::get_modules_impl()
	{
		if (m_cached_modules.empty())
		{
			load_modules_impl();
		}
		return m_cached_modules;
	}

	bool module_mgr::is_manual_mapped_impl() const
	{
		return m_manual_mapped;
	}
}