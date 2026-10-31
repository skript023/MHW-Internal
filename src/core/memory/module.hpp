#pragma once
#include "core/memory/handle.hpp"
#include "core/memory/range.hpp"

#include <concepts>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <type_traits>
#include <windows.h>
#include <winternl.h>

namespace big::memory
{
	template<typename T>
	concept symbol = std::is_convertible_v<T, int> || std::is_convertible_v<T, std::string_view>;

	class module : public range
	{
	public:
		module(LDR_DATA_TABLE_ENTRY* dll_entry);
		module(HMODULE mod);
		explicit module(std::nullptr_t);
		explicit module(std::string_view name);
		explicit module(std::wstring_view name);
		~module() = default;

		const std::string_view name() const;
		const std::filesystem::path& path() const;
		std::uintptr_t base() const;
		std::uintptr_t end_address() const;
		bool valid() const;

		IMAGE_NT_HEADERS* get_nt_header() const;

		template<typename R = memory::handle>
		R get_export(const std::string_view symbol_name) const;

		template<typename R = memory::handle>
		R get_export(const int ordinal) const;

		bool is_exported(const std::string_view symbol_name) const;
		bool is_exported(const int ordinal) const;

		void** get_import(const std::string_view module_name, const std::string_view symbol_name) const;

	private:
		template<typename R = memory::handle, symbol T>
		R get_export_impl(const T sym) const;

	private:
		std::filesystem::path m_path{};
		std::string m_name{};
	};

	inline std::uintptr_t module::base() const
	{
		return m_base.as<std::uintptr_t>();
	}

	inline std::uintptr_t module::end_address() const
	{
		return base() + m_size;
	}

	inline const std::string_view module::name() const
	{
		return m_name;
	}

	inline const std::filesystem::path& module::path() const
	{
		return m_path;
	}

	inline bool module::valid() const
	{
		return m_size != 0;
	}

	template<typename R>
	inline R module::get_export(const std::string_view symbol_name) const
	{
		return get_export_impl<R, const std::string_view>(symbol_name);
	}

	template<typename R>
	inline R module::get_export(const int ordinal) const
	{
		return get_export_impl<R, const int>(ordinal);
	}

	inline bool module::is_exported(const std::string_view symbol_name) const
	{
		return get_export_impl<void*, const std::string_view>(symbol_name) != nullptr;
	}

	inline bool module::is_exported(const int ordinal) const
	{
		return get_export_impl<void*, const int>(ordinal) != nullptr;
	}

	template<typename R, symbol T>
	inline R module::get_export_impl(const T sym) const
	{
		const auto nt_header = get_nt_header();
		if (!nt_header)
			return {};

		const auto image_data_directory = nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
		if (!image_data_directory.VirtualAddress || !image_data_directory.Size)
		{
			if constexpr (std::is_convertible_v<T, std::string_view>)
			{
				if (auto proc = GetProcAddress(m_base.as<HMODULE>(), std::string_view(sym).data()))
				{
					if constexpr (std::is_same_v<R, memory::handle>)
						return memory::handle(proc);
					else
						return reinterpret_cast<R>(proc);
				}
			}
			return {};
		}

		const auto export_directory = m_base.add(image_data_directory.VirtualAddress).as<IMAGE_EXPORT_DIRECTORY*>();
		const auto names_offsets = m_base.add(export_directory->AddressOfNames).as<DWORD*>();
		const auto ordinal_offsets = m_base.add(export_directory->AddressOfNameOrdinals).as<uint16_t*>();
		const auto func_offsets = m_base.add(export_directory->AddressOfFunctions).as<DWORD*>();

		for (std::size_t i = 0; i < export_directory->NumberOfNames; i++)
		{
			if constexpr (std::is_convertible_v<T, int>)
			{
				if (export_directory->Base + ordinal_offsets[i] != sym)
					continue;

				if constexpr (std::is_same_v<R, memory::handle>)
					return m_base.add(func_offsets[ordinal_offsets[i]]);
				else
					return reinterpret_cast<R>(m_base.add(func_offsets[ordinal_offsets[i]]).as<void*>());
			}
			else if constexpr (std::is_convertible_v<T, std::string_view>)
			{
				const auto function_name = m_base.add(names_offsets[i]).as<const char*>();
				if (strcmp(function_name, std::string_view(sym).data()) != 0)
					continue;

				if constexpr (std::is_same_v<R, memory::handle>)
					return m_base.add(func_offsets[ordinal_offsets[i]]);
				else
					return reinterpret_cast<R>(m_base.add(func_offsets[ordinal_offsets[i]]).as<void*>());
			}
		}

		if constexpr (std::is_convertible_v<T, std::string_view>)
		{
			if (auto proc = GetProcAddress(m_base.as<HMODULE>(), std::string_view(sym).data()))
			{
				if constexpr (std::is_same_v<R, memory::handle>)
					return memory::handle(proc);
				else
					return reinterpret_cast<R>(proc);
			}
		}

		return {};
	}
}
