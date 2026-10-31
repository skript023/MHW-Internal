#pragma once
#include <vector>
#include "core/memory/fwddec.hpp"
#include "core/memory/handle.hpp"

namespace big::memory
{
	class range
	{
	public:
		range(handle base, std::size_t size);

		handle begin();
		handle end();
		std::size_t size();

		bool contains(handle h);

		handle scan(pattern const& sig);
		std::vector<handle> scan_all(pattern const& sig);
		static handle fullscan(pattern const& sig);

	protected:
		handle m_base;
		std::size_t m_size;
	};
}
