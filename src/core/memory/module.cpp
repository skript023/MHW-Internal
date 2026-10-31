#include "core/memory/module.hpp"

namespace big::memory
{
	module::module(LDR_DATA_TABLE_ENTRY* dll_entry) :
		range(handle(dll_entry->DllBase), 0),
		m_path(std::wstring_view(dll_entry->FullDllName.Buffer, dll_entry->FullDllName.Length / sizeof(wchar_t))),
		m_name(m_path.filename().string())
	{
		const auto nt_header = get_nt_header();
		if (nt_header)
		{
			m_size = nt_header->OptionalHeader.SizeOfImage;
		}
	}

	module::module(HMODULE mod) :
		range(mod, 0)
	{
		const auto nt_header = get_nt_header();
		if (nt_header)
		{
			m_size = nt_header->OptionalHeader.SizeOfImage;
		}

		wchar_t path_buf[MAX_PATH];
		if (GetModuleFileNameW(mod, path_buf, MAX_PATH))
		{
			m_path = path_buf;
			m_name = m_path.filename().string();
		}
	}

	module::module(std::nullptr_t) :
		module(GetModuleHandle(nullptr))
	{
	}

	module::module(std::string_view name) :
		module(GetModuleHandleA(name.data()))
	{
	}

	module::module(std::wstring_view name) :
		module(GetModuleHandleW(name.data()))
	{
	}

	IMAGE_NT_HEADERS* module::get_nt_header() const
	{
		if (!m_base)
			return nullptr;

		const auto dos_header = m_base.as<IMAGE_DOS_HEADER*>();
		if (!dos_header || dos_header->e_magic != IMAGE_DOS_SIGNATURE)
			return nullptr;

		return m_base.add(dos_header->e_lfanew).as<IMAGE_NT_HEADERS*>();
	}

	void** module::get_import(const std::string_view module_name, const std::string_view symbol_name) const
	{
		const auto nt_header = get_nt_header();
		if (!nt_header)
			return nullptr;

		const auto image_data_directory = nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
		if (!image_data_directory.VirtualAddress || !image_data_directory.Size)
			return nullptr;

		auto import_descriptor = m_base.add(image_data_directory.VirtualAddress).as<IMAGE_IMPORT_DESCRIPTOR*>();
		for (; import_descriptor->Name; import_descriptor++)
		{
			const auto mod_name = m_base.add(import_descriptor->Name).as<const char*>();
			if (!mod_name || _stricmp(mod_name, module_name.data()) != 0)
				continue;

			auto first_thunk = m_base.add(import_descriptor->FirstThunk).as<IMAGE_THUNK_DATA*>();
			auto orig_thunk = m_base.add(import_descriptor->OriginalFirstThunk).as<IMAGE_THUNK_DATA*>();
			for (; orig_thunk->u1.AddressOfData != 0; first_thunk++, orig_thunk++)
			{
				if (IMAGE_SNAP_BY_ORDINAL(orig_thunk->u1.Ordinal))
					continue;

				const auto import_data = m_base.add(orig_thunk->u1.AddressOfData).as<IMAGE_IMPORT_BY_NAME*>();
				if (import_data && strcmp(import_data->Name, symbol_name.data()) == 0)
				{
					return reinterpret_cast<void**>(&first_thunk->u1.Function);
				}
			}
		}
		return nullptr;
	}
}
