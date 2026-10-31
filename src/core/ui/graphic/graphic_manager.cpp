#include "core/ui/graphic/graphic_manager.hpp"

#include "core/common.hpp"

#include <d3d11.h>
#include <d3d12.h>
#include <dxgi.h>

namespace big
{
	namespace
	{
		constexpr const wchar_t* dummy_window_class = L"EllohimGraphicManager";
		constexpr const wchar_t* dummy_window_title = L"Ellohim Dummy Window";

		// Throwaway window/class used to build the dummy device and swapchain.
		struct dummy_window
		{
			WNDCLASSEXW wc{};
			HWND handle{};

			dummy_window()
			{
				wc.cbSize        = sizeof(WNDCLASSEXW);
				wc.style         = CS_HREDRAW | CS_VREDRAW;
				wc.lpfnWndProc   = DefWindowProc;
				wc.hInstance     = GetModuleHandleW(nullptr);
				wc.lpszClassName = dummy_window_class;

				RegisterClassExW(&wc);

				handle = CreateWindowExW(0, dummy_window_class, dummy_window_title, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100,
				                         nullptr, nullptr, wc.hInstance, nullptr);
			}

			~dummy_window()
			{
				if (handle)
					DestroyWindow(handle);
				UnregisterClassW(dummy_window_class, wc.hInstance);
			}

			explicit operator bool() const
			{
				return handle != nullptr;
			}
		};

		template<typename T>
		std::vector<std::uintptr_t> vtable_range(T* object, std::size_t count)
		{
			auto* table = *reinterpret_cast<std::uintptr_t**>(object);

			return {table, table + count};
		}

		using create_d3d11_device_and_swapchain_t = HRESULT(WINAPI*)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT,
		                                                             const D3D_FEATURE_LEVEL*, UINT, UINT,
		                                                             const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**,
		                                                             ID3D11Device**, D3D_FEATURE_LEVEL*,
		                                                             ID3D11DeviceContext**);
	}

	graphic_manager& graphic_manager::get_instance()
	{
		static graphic_manager instance;
		return instance;
	}

	eGraphicsAPI graphic_manager::detect_graphics_api()
	{
		if (GetModuleHandleW(L"d3d12.dll"))
			return eGraphicsAPI::directx12;

		if (GetModuleHandleW(L"d3d11.dll"))
			return eGraphicsAPI::directx11;

		return eGraphicsAPI::none;
	}

	eGraphicsAPI graphic_manager::get_render_type()
	{
		return get_instance().m_render_type;
	}

	std::string graphic_manager::get_render_type_name()
	{
		switch (get_instance().m_render_type)
		{
		case eGraphicsAPI::directx11: return "DirectX 11";
		case eGraphicsAPI::directx12: return "DirectX 12";
		default: return "No Render Type";
		}
	}

	eInitializationStatus graphic_manager::get_swapchain(eGraphicsAPI render_type)
	{
		return get_instance().create_dummy_device(render_type);
	}

	eInitializationStatus graphic_manager::get_swapchain()
	{
		auto& self = get_instance();

		// The method tables do not change between frames, so this only runs once.
		if (!self.m_swapchain_methods.empty())
			return eInitializationStatus::SUCCESS;

		const auto detected = detect_graphics_api();
		if (detected == eGraphicsAPI::none)
		{
			LOG(WARNING) << "Neither d3d11.dll nor d3d12.dll is loaded; no swapchain to hook.";
			return eInitializationStatus::MODULE_NOT_FOUND_ERROR;
		}

		auto status = self.create_dummy_device(detected);

		// A DX12 dummy can fail on adapters without a hardware queue (VMs, some
		// laptops); the DXGI swapchain vtable is the same either way.
		if (status != eInitializationStatus::SUCCESS && detected == eGraphicsAPI::directx12)
			status = self.create_dummy_device(eGraphicsAPI::directx11);

		return status;
	}

	void* graphic_manager::get_method_table(int index)
	{
		auto& self = get_instance();

		if (index < 0 || static_cast<std::size_t>(index) >= self.m_swapchain_methods.size())
			return nullptr;

		return reinterpret_cast<void*>(self.m_swapchain_methods[index]);
	}

	void* graphic_manager::get_execute_command_lists()
	{
		return get_instance().m_execute_command_lists;
	}

	eInitializationStatus graphic_manager::create_dummy_device(eGraphicsAPI render_type)
	{
		dummy_window window;
		if (!window)
		{
			LOG(WARNING) << "Cannot create the dummy window for the graphic manager.";
			return eInitializationStatus::FAILED;
		}

		switch (render_type)
		{
		case eGraphicsAPI::directx11: return create_dx11_dummy(window.handle);
		case eGraphicsAPI::directx12: return create_dx12_dummy(window.handle);
		default: return eInitializationStatus::FAILED;
		}
	}

	eInitializationStatus graphic_manager::create_dx11_dummy(HWND window)
	{
		auto d3d11 = GetModuleHandleW(L"d3d11.dll");
		if (!d3d11)
			d3d11 = LoadLibraryW(L"d3d11.dll");
		if (!d3d11)
			return eInitializationStatus::MODULE_NOT_FOUND_ERROR;

		auto create = reinterpret_cast<create_d3d11_device_and_swapchain_t>(GetProcAddress(d3d11, "D3D11CreateDeviceAndSwapChain"));
		if (!create)
			return eInitializationStatus::FAILED;

		const D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1};

		DXGI_SWAP_CHAIN_DESC desc{};
		desc.BufferCount        = 1;
		desc.BufferDesc.Width   = 100;
		desc.BufferDesc.Height  = 100;
		desc.BufferDesc.Format  = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow       = window;
		desc.SampleDesc.Count   = 1;
		desc.Windowed           = TRUE;
		desc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;

		IDXGISwapChain* swapchain = nullptr;
		ID3D11Device* device      = nullptr;
		ID3D11DeviceContext* context = nullptr;
		D3D_FEATURE_LEVEL level{};

		if (FAILED(create(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, levels, 2, D3D11_SDK_VERSION, &desc,
		                  &swapchain, &device, &level, &context)))
		{
			LOG(WARNING) << "D3D11CreateDeviceAndSwapChain failed for the dummy swapchain.";
			return eInitializationStatus::CREATE_DUMMY_DEVICE_ERROR;
		}

		m_swapchain_methods = vtable_range(swapchain, swapchain_num_funcs);
		m_render_type       = eGraphicsAPI::directx11;

		LOG(INFO) << "DX11 dummy swapchain created (" << m_swapchain_methods.size() << " methods).";

		swapchain->Release();
		device->Release();
		context->Release();

		return eInitializationStatus::SUCCESS;
	}

	eInitializationStatus graphic_manager::create_dx12_dummy(HWND window)
	{
		auto d3d12 = GetModuleHandleW(L"d3d12.dll");
		auto dxgi  = GetModuleHandleW(L"dxgi.dll");

		if (!d3d12 || !dxgi)
			return eInitializationStatus::MODULE_NOT_FOUND_ERROR;

		auto create_factory = reinterpret_cast<HRESULT(WINAPI*)(const IID&, void**)>(GetProcAddress(dxgi, "CreateDXGIFactory1"));
		if (!create_factory)
			create_factory = reinterpret_cast<HRESULT(WINAPI*)(const IID&, void**)>(GetProcAddress(dxgi, "CreateDXGIFactory"));

		auto create_device = reinterpret_cast<HRESULT(WINAPI*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**)>(
		    GetProcAddress(d3d12, "D3D12CreateDevice"));

		if (!create_factory || !create_device)
			return eInitializationStatus::FAILED;

		IDXGIFactory4* factory = nullptr;
		if (FAILED(create_factory(__uuidof(IDXGIFactory4), reinterpret_cast<void**>(&factory)))
		    && FAILED(create_factory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory))))
		{
			LOG(WARNING) << "CreateDXGIFactory failed for the dummy swapchain.";
			return eInitializationStatus::FAILED;
		}

		eInitializationStatus status = eInitializationStatus::FAILED;

		IDXGIAdapter* adapter = nullptr;
		if (factory->EnumAdapters(0, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			ID3D12Device* device = nullptr;
			if (SUCCEEDED(create_device(adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), reinterpret_cast<void**>(&device))))
			{
				D3D12_COMMAND_QUEUE_DESC queue_desc{};
				queue_desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
				queue_desc.Priority = 0;
				queue_desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;

				ID3D12CommandQueue* command_queue = nullptr;
				if (SUCCEEDED(device->CreateCommandQueue(&queue_desc, __uuidof(ID3D12CommandQueue), reinterpret_cast<void**>(&command_queue))))
				{
					DXGI_SWAP_CHAIN_DESC desc{};
					desc.BufferCount      = 2;
					desc.BufferDesc.Width = 100;
					desc.BufferDesc.Height = 100;
					desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.BufferUsage      = DXGI_USAGE_RENDER_TARGET_OUTPUT;
					desc.OutputWindow     = window;
					desc.SampleDesc.Count = 1;
					desc.Windowed         = TRUE;
					desc.SwapEffect       = DXGI_SWAP_EFFECT_FLIP_DISCARD;

					IDXGISwapChain* swapchain = nullptr;
					if (SUCCEEDED(factory->CreateSwapChain(command_queue, &desc, &swapchain)))
					{
						m_swapchain_methods = vtable_range(swapchain, swapchain_num_funcs);

						// The queue vtable gives us ExecuteCommandLists, which is how the
						// renderer learns the game's real command queue.
						auto queue_methods = vtable_range(command_queue, command_queue_execute_command_lists_index + 1);
						m_execute_command_lists = reinterpret_cast<void*>(queue_methods[command_queue_execute_command_lists_index]);

						m_render_type = eGraphicsAPI::directx12;
						status        = eInitializationStatus::SUCCESS;

						LOG(INFO) << "DX12 dummy swapchain created (" << m_swapchain_methods.size() << " methods).";

						swapchain->Release();
					}

					command_queue->Release();
				}

				device->Release();
			}

			adapter->Release();
		}

		factory->Release();

		return status;
	}
}
