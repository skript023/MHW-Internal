#include "../common.hpp"
#include "pattern_batch.hpp"
#include "range.hpp"

namespace memory
{
	pattern_batch::pattern_batch(std::shared_ptr<pattern_cache> pattern_cache) : m_pattern_cache(pattern_cache)
	{

	}
	pattern_batch::~pattern_batch() noexcept
	{
		m_pattern_cache.reset();
	}
	void pattern_batch::add(std::string name, pattern pattern, std::function<void(handle)> callback)
	{
		m_entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
	}

	void pattern_batch::run(range region)
	{
		bool all_found = true;
		for (auto& entry : m_entries)
		{
			if (m_pattern_cache->is_initialized())
			{
				auto offset = m_pattern_cache->get_cached_offset(entry.m_hash.update(region.size()));

				if (offset.has_value())
				{
					LOG(VERBOSE) << "Using cached pattern [" << entry.m_name << "] : [" << HEX_TO_UPPER(region.begin().as<DWORD64>() + offset.value()) << "]";
					std::invoke(entry.m_callback, handle(region.begin().as<DWORD64>() + offset.value()));

					continue;
				}
			}

			if (auto result = region.scan(entry.m_pattern))
			{
				if (entry.m_callback)
				{
					std::invoke(std::move(entry.m_callback), result);
					LOG(VERBOSE) << "Found '" << entry.m_name << std::format("' {}+", TARGET_PROCESS) << HEX_TO_UPPER(result.as<DWORD64>() - region.begin().as<DWORD64>());

					if (m_pattern_cache->is_initialized())
					{
						LOG(VERBOSE) << "Save " << entry.m_name << " to cache";
						m_pattern_cache->update_cached_offset(entry.m_hash.update(region.size()), result.as<DWORD64>() - region.begin().as<DWORD64>());
					}
				}
				else
				{
					all_found = false;
					LOG(WARNING) << "Failed to find '" << entry.m_name << "'.";
				}
			}
			else
			{
				all_found = false;
				LOG(WARNING) << "Failed to find '" << entry.m_name << "'.";
			}
		}

		m_entries.clear();
		if (!all_found)
		{
			throw std::runtime_error("Failed to find some patterns.");
		}
	}
}
