#include "core/ui/renderer.hpp"

#include "core/ui/fonts/font_list.hpp"
#include "core/ui/fonts/icon_list.hpp"
#include "core/ui/gui.hpp"
#include <astra/host/canvas.hpp>

#include <imgui_internal.h>

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace big
{
	renderer::renderer()
	{
		g_renderer = this;
	}

	renderer::~renderer()
	{
		std::lock_guard lock(render_mutex);

		if (ImGui::GetCurrentContext())
		{
			if (m_api == renderer_api::dx11)
				ImGui_ImplDX11_Shutdown();
			else if (m_api == renderer_api::dx12)
				ImGui_ImplDX12_Shutdown();

			if (ImGui::GetIO().BackendPlatformUserData)
				ImGui_ImplWin32_Shutdown();

			ImGui::DestroyContext();
		}

		cleanup_dx11();
		cleanup_dx12();

		m_init = false;
		g_renderer = nullptr;
	}

	void renderer::cleanup_dx11()
	{
		if (m_d3d_render_target)
		{
			m_d3d_render_target->Release();
			m_d3d_render_target = nullptr;
		}

		if (m_d3d_context)
		{
			m_d3d_context->Release();
			m_d3d_context = nullptr;
		}

		if (m_d3d_device)
		{
			m_d3d_device->Release();
			m_d3d_device = nullptr;
		}
	}

	void renderer::cleanup_dx12()
	{
		for (auto& ctx : m_frame_contexts)
		{
			if (ctx.render_target)
			{
				ctx.render_target->Release();
				ctx.render_target = nullptr;
			}
			if (ctx.command_allocator)
			{
				ctx.command_allocator->Release();
				ctx.command_allocator = nullptr;
			}
		}
		m_frame_contexts.clear();

		if (m_d3d12_command_list)
		{
			m_d3d12_command_list->Release();
			m_d3d12_command_list = nullptr;
		}
		if (m_d3d12_rtv_heap)
		{
			m_d3d12_rtv_heap->Release();
			m_d3d12_rtv_heap = nullptr;
		}
		if (m_d3d12_srv_heap)
		{
			m_d3d12_srv_heap->Release();
			m_d3d12_srv_heap = nullptr;
		}
		if (m_d3d12_device)
		{
			m_d3d12_device->Release();
			m_d3d12_device = nullptr;
		}
	}

	void renderer::set_command_queue(ID3D12CommandQueue* queue)
	{
		std::lock_guard lock(render_mutex);
		if (!m_d3d12_command_queue && queue)
		{
			m_d3d12_command_queue = queue;
			LOG(INFO) << "Captured DX12 CommandQueue: " << (void*)queue;
		}
	}

	bool renderer::init(IDXGISwapChain* swapchain)
	{
		std::lock_guard lock(render_mutex);

		if (m_init)
			return true;

		if (!swapchain)
			return false;

		// 1. Check if DX11 device
		ID3D11Device* d3d11_device = nullptr;
		if (SUCCEEDED(swapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&d3d11_device))))
		{
			d3d11_device->Release();
			return init_dx11(swapchain);
		}

		// 2. Check if DX12 device
		ID3D12Device* d3d12_device = nullptr;
		if (SUCCEEDED(swapchain->GetDevice(__uuidof(ID3D12Device), reinterpret_cast<void**>(&d3d12_device))))
		{
			d3d12_device->Release();
			return init_dx12(swapchain);
		}

		return false;
	}

	bool renderer::init_dx11(IDXGISwapChain* swapchain)
	{
		if (FAILED(swapchain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&m_d3d_device))))
			return false;

		m_d3d_device->GetImmediateContext(&m_d3d_context);

		DXGI_SWAP_CHAIN_DESC sd;
		swapchain->GetDesc(&sd);
		m_window = sd.OutputWindow;

		ID3D11Texture2D* back_buffer = nullptr;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
		if (back_buffer && m_d3d_device)
		{
			m_d3d_device->CreateRenderTargetView(back_buffer, NULL, &m_d3d_render_target);
			back_buffer->Release();
		}
		else
		{
			return false;
		}

		this->imgui_init();

		ImGui_ImplDX11_Init(m_d3d_device, m_d3d_context);
		ImGui_ImplWin32_Init(m_window);

		m_api = renderer_api::dx11;
		m_init = true;
		LOG(INFO) << "Swapchain initialized (DirectX 11).";
		g_gui.load_textures();
		g_gui.script_init();

		return true;
	}

	bool renderer::init_dx12(IDXGISwapChain* swapchain)
	{
		if (FAILED(swapchain->GetDevice(__uuidof(ID3D12Device), reinterpret_cast<void**>(&m_d3d12_device))))
			return false;

		if (!m_d3d12_command_queue)
		{
			return false;
		}

		DXGI_SWAP_CHAIN_DESC sd;
		swapchain->GetDesc(&sd);
		m_window = sd.OutputWindow;
		m_buffer_count = sd.BufferCount;
		if (m_buffer_count == 0)
			m_buffer_count = 2;

		// Create RTV Descriptor Heap
		D3D12_DESCRIPTOR_HEAP_DESC rtv_desc = {};
		rtv_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtv_desc.NumDescriptors = m_buffer_count;
		rtv_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtv_desc.NodeMask = 1;
		if (FAILED(m_d3d12_device->CreateDescriptorHeap(&rtv_desc, IID_PPV_ARGS(&m_d3d12_rtv_heap))))
			return false;

		// Create SRV Descriptor Heap for ImGui Font Atlas and Textures
		D3D12_DESCRIPTOR_HEAP_DESC srv_desc = {};
		srv_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srv_desc.NumDescriptors = 64;
		srv_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (FAILED(m_d3d12_device->CreateDescriptorHeap(&srv_desc, IID_PPV_ARGS(&m_d3d12_srv_heap))))
			return false;

		SIZE_T rtv_descriptor_size = m_d3d12_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = m_d3d12_rtv_heap->GetCPUDescriptorHandleForHeapStart();

		m_frame_contexts.resize(m_buffer_count);
		for (UINT i = 0; i < m_buffer_count; ++i)
		{
			m_frame_contexts[i].rtv_descriptor = rtv_handle;
			if (FAILED(m_d3d12_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_frame_contexts[i].command_allocator))))
				return false;

			if (FAILED(swapchain->GetBuffer(i, IID_PPV_ARGS(&m_frame_contexts[i].render_target))))
				return false;

			m_d3d12_device->CreateRenderTargetView(m_frame_contexts[i].render_target, nullptr, rtv_handle);
			rtv_handle.ptr += rtv_descriptor_size;
		}

		if (FAILED(m_d3d12_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_frame_contexts[0].command_allocator, nullptr, IID_PPV_ARGS(&m_d3d12_command_list))))
			return false;

		m_d3d12_command_list->Close();

		this->imgui_init();

		ImGui_ImplDX12_InitInfo init_info = {};
		init_info.Device = m_d3d12_device;
		init_info.CommandQueue = m_d3d12_command_queue;
		init_info.NumFramesInFlight = m_buffer_count;
		init_info.RTVFormat = sd.BufferDesc.Format;
		init_info.DSVFormat = DXGI_FORMAT_UNKNOWN;
		init_info.SrvDescriptorHeap = m_d3d12_srv_heap;
		init_info.LegacySingleSrvCpuDescriptor = m_d3d12_srv_heap->GetCPUDescriptorHandleForHeapStart();
		init_info.LegacySingleSrvGpuDescriptor = m_d3d12_srv_heap->GetGPUDescriptorHandleForHeapStart();

		ImGui_ImplDX12_Init(&init_info);
		ImGui_ImplWin32_Init(m_window);

		m_api = renderer_api::dx12;
		m_init = true;
		LOG(INFO) << "Swapchain initialized (DirectX 12).";
		g_gui.load_textures();
		g_gui.script_init();

		return true;
	}

	void renderer::imgui_init()
	{
		auto file_path = std::filesystem::path(std::getenv("appdata"));
		file_path /= "Ellohim";
		if (!std::filesystem::exists(file_path))
		{
			std::filesystem::create_directory(file_path);
		}
		else if (!std::filesystem::is_directory(file_path))
		{
			std::filesystem::remove(file_path);
			std::filesystem::create_directory(file_path);
		}
		file_path /= "imgui.ini";

		ImGuiContext* ctx = ImGui::CreateContext();

		static std::string path = file_path.make_preferred().string();
		ctx->IO.IniFilename = path.c_str();

		ImFontConfig font_cfg{};
		font_cfg.FontDataOwnedByAtlas = false;
		std::strcpy(font_cfg.Name, "Rubik");

		m_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 14.f, &font_cfg);
		merge_icon_with_latest_font(14.f, false);

		m_font_sub_title = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 18.f, &font_cfg);
		merge_icon_with_latest_font(16.f, false);

		m_font_title = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 24.f, &font_cfg);
		merge_icon_with_latest_font(17.f, false);

		m_monospace_font = ImGui::GetIO().Fonts->AddFontDefault();

		g_gui.dx_init();
	}

	void renderer::on_present(IDXGISwapChain* swapchain)
	{
		std::lock_guard lock(render_mutex);

		if (!m_init || !ImGui::GetCurrentContext())
			return;

		g_gui.m_opened = canvas::is_opened();
		ImGui::GetIO().MouseDrawCursor = canvas::uses_mouse();
		if (canvas::uses_mouse())
		{
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
		else
		{
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}

		if (m_api == renderer_api::dx11)
		{
			canvas::prepare_imgui_mouse();
			canvas::apply_panel_theme();
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			canvas::keep_windows_off_menu();

			g_gui.dx_on_tick();
			g_gui.dx_on_opened();

			ImGui::Render();
			render_dx11();
		}
		else if (m_api == renderer_api::dx12)
		{
			canvas::prepare_imgui_mouse();
			canvas::apply_panel_theme();
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			canvas::keep_windows_off_menu();

			g_gui.dx_on_tick();
			g_gui.dx_on_opened();

			ImGui::Render();
			render_dx12(swapchain);
		}
	}

	void renderer::render_dx11()
	{
		if (m_d3d_context && m_d3d_render_target)
		{
			m_d3d_context->OMSetRenderTargets(1, &m_d3d_render_target, NULL);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void renderer::render_dx12(IDXGISwapChain* swapchain)
	{
		if (!m_d3d12_command_queue || !m_d3d12_command_list || m_frame_contexts.empty())
			return;

		auto swapchain3 = reinterpret_cast<IDXGISwapChain3*>(swapchain);
		UINT back_buffer_idx = swapchain3->GetCurrentBackBufferIndex();
		if (back_buffer_idx >= m_frame_contexts.size())
			return;

		auto& frame_ctx = m_frame_contexts[back_buffer_idx];
		if (!frame_ctx.command_allocator || !frame_ctx.render_target)
			return;

		frame_ctx.command_allocator->Reset();
		m_d3d12_command_list->Reset(frame_ctx.command_allocator, nullptr);

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = frame_ctx.render_target;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		m_d3d12_command_list->ResourceBarrier(1, &barrier);

		m_d3d12_command_list->OMSetRenderTargets(1, &frame_ctx.rtv_descriptor, FALSE, nullptr);
		m_d3d12_command_list->SetDescriptorHeaps(1, &m_d3d12_srv_heap);

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_d3d12_command_list);

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		m_d3d12_command_list->ResourceBarrier(1, &barrier);

		m_d3d12_command_list->Close();

		ID3D12CommandList* const command_lists[] = { m_d3d12_command_list };
		m_d3d12_command_queue->ExecuteCommandLists(1, command_lists);
	}

	void renderer::pre_reset()
	{
		std::lock_guard lock(render_mutex);
		if (!m_init)
			return;

		if (m_api == renderer_api::dx11)
		{
			if (m_d3d_render_target)
			{
				if (m_d3d_context)
					m_d3d_context->OMSetRenderTargets(0, 0, 0);
				m_d3d_render_target->Release();
				m_d3d_render_target = nullptr;
			}

			ImGui_ImplDX11_InvalidateDeviceObjects();
		}
		else if (m_api == renderer_api::dx12)
		{
			for (auto& ctx : m_frame_contexts)
			{
				if (ctx.render_target)
				{
					ctx.render_target->Release();
					ctx.render_target = nullptr;
				}
			}
			ImGui_ImplDX12_InvalidateDeviceObjects();
		}
	}

	void renderer::post_reset(IDXGISwapChain* this_)
	{
		std::lock_guard lock(render_mutex);
		if (!m_init || !this_)
			return;

		if (m_api == renderer_api::dx11)
		{
			ImGui_ImplDX11_CreateDeviceObjects();

			ID3D11Texture2D* back_buffer = nullptr;
			this_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));

			if (back_buffer)
			{
				if (m_d3d_device)
					m_d3d_device->CreateRenderTargetView(back_buffer, NULL, &m_d3d_render_target);
				back_buffer->Release();
			}

			if (m_d3d_context && m_d3d_render_target)
				m_d3d_context->OMSetRenderTargets(1, &m_d3d_render_target, NULL);
		}
		else if (m_api == renderer_api::dx12)
		{
			ImGui_ImplDX12_CreateDeviceObjects();

			if (m_d3d12_device && m_d3d12_rtv_heap)
			{
				SIZE_T rtv_descriptor_size = m_d3d12_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = m_d3d12_rtv_heap->GetCPUDescriptorHandleForHeapStart();

				for (UINT i = 0; i < m_buffer_count && i < m_frame_contexts.size(); ++i)
				{
					m_frame_contexts[i].rtv_descriptor = rtv_handle;
					if (SUCCEEDED(this_->GetBuffer(i, IID_PPV_ARGS(&m_frame_contexts[i].render_target))))
					{
						m_d3d12_device->CreateRenderTargetView(m_frame_contexts[i].render_target, nullptr, rtv_handle);
					}
					rtv_handle.ptr += rtv_descriptor_size;
				}
			}
		}
	}

	bool renderer::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		std::lock_guard lock(render_mutex);

		if (msg == WM_KEYUP && wparam == VK_END)
		{
			g_running = false;
		}

		if (m_init && ImGui::GetCurrentContext())
		{
			ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
		}

		return canvas::uses_mouse();
	}

	void renderer::release_texture(ImTextureID texture)
	{
		std::lock_guard lock(render_mutex);
		if (m_api == renderer_api::dx11)
		{
			std::erase_if(m_dx11_textures, [texture](const auto& view) {
				return reinterpret_cast<ImTextureID>(view.Get()) == texture;
			});
		}
		else if (m_api == renderer_api::dx12)
		{
			std::erase_if(m_dx12_textures, [texture](const auto& tex) {
				return (ImTextureID)tex.gpu_handle.ptr == texture;
			});
		}
	}

	ImTextureID renderer::upload_rgba(const unsigned char* pixels, int width, int height)
	{
		std::lock_guard lock(render_mutex);
		if (!pixels || width <= 0 || height <= 0)
			return 0;

		if (m_api == renderer_api::dx11 && m_d3d_device)
		{
			D3D11_TEXTURE2D_DESC desc{};
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			D3D11_SUBRESOURCE_DATA data{pixels, static_cast<UINT>(width * 4), 0};
			Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view;
			if (SUCCEEDED(m_d3d_device->CreateTexture2D(&desc, &data, &texture)) &&
			    SUCCEEDED(m_d3d_device->CreateShaderResourceView(texture.Get(), nullptr, &view)))
			{
				const auto id = reinterpret_cast<ImTextureID>(view.Get());
				m_dx11_textures.push_back(std::move(view));
				return id;
			}
		}
		else if (m_api == renderer_api::dx12 && m_d3d12_device && m_d3d12_srv_heap && m_d3d12_command_queue)
		{
			if (m_dx12_srv_allocated >= 64)
				return 0;

			D3D12_RESOURCE_DESC desc = {};
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			desc.Alignment = 0;
			desc.Width = width;
			desc.Height = height;
			desc.DepthOrArraySize = 1;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12_HEAP_PROPERTIES heap_props = {};
			heap_props.Type = D3D12_HEAP_TYPE_DEFAULT;

			Microsoft::WRL::ComPtr<ID3D12Resource> texture;
			if (FAILED(m_d3d12_device->CreateCommittedResource(
			        &heap_props,
			        D3D12_HEAP_FLAG_NONE,
			        &desc,
			        D3D12_RESOURCE_STATE_COPY_DEST,
			        nullptr,
			        IID_PPV_ARGS(&texture))))
				return 0;

			const UINT64 upload_pitch = (width * 4 + 255) & ~255;
			const UINT64 upload_size = upload_pitch * height;

			D3D12_RESOURCE_DESC upload_desc = {};
			upload_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			upload_desc.Alignment = 0;
			upload_desc.Width = upload_size;
			upload_desc.Height = 1;
			upload_desc.DepthOrArraySize = 1;
			upload_desc.MipLevels = 1;
			upload_desc.Format = DXGI_FORMAT_UNKNOWN;
			upload_desc.SampleDesc.Count = 1;
			upload_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			upload_desc.Flags = D3D12_RESOURCE_FLAG_NONE;

			D3D12_HEAP_PROPERTIES upload_heap = {};
			upload_heap.Type = D3D12_HEAP_TYPE_UPLOAD;

			Microsoft::WRL::ComPtr<ID3D12Resource> upload_buffer;
			if (FAILED(m_d3d12_device->CreateCommittedResource(
			        &upload_heap,
			        D3D12_HEAP_FLAG_NONE,
			        &upload_desc,
			        D3D12_RESOURCE_STATE_GENERIC_READ,
			        nullptr,
			        IID_PPV_ARGS(&upload_buffer))))
				return 0;

			void* mapped = nullptr;
			D3D12_RANGE range = {0, 0};
			if (SUCCEEDED(upload_buffer->Map(0, &range, &mapped)))
			{
				for (int y = 0; y < height; ++y)
				{
					memcpy(static_cast<BYTE*>(mapped) + y * upload_pitch, pixels + y * width * 4, width * 4);
				}
				upload_buffer->Unmap(0, nullptr);
			}

			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmd_alloc;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list;
			if (SUCCEEDED(m_d3d12_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmd_alloc))) &&
			    SUCCEEDED(m_d3d12_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmd_alloc.Get(), nullptr, IID_PPV_ARGS(&cmd_list))))
			{
				D3D12_TEXTURE_COPY_LOCATION src_loc = {};
				src_loc.pResource = upload_buffer.Get();
				src_loc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
				src_loc.PlacedFootprint.Offset = 0;
				src_loc.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				src_loc.PlacedFootprint.Footprint.Width = width;
				src_loc.PlacedFootprint.Footprint.Height = height;
				src_loc.PlacedFootprint.Footprint.Depth = 1;
				src_loc.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(upload_pitch);

				D3D12_TEXTURE_COPY_LOCATION dst_loc = {};
				dst_loc.pResource = texture.Get();
				dst_loc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				dst_loc.SubresourceIndex = 0;

				cmd_list->CopyTextureRegion(&dst_loc, 0, 0, 0, &src_loc, nullptr);

				D3D12_RESOURCE_BARRIER barrier = {};
				barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barrier.Transition.pResource = texture.Get();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				cmd_list->ResourceBarrier(1, &barrier);

				cmd_list->Close();
				ID3D12CommandList* lists[] = {cmd_list.Get()};
				m_d3d12_command_queue->ExecuteCommandLists(1, lists);

				Microsoft::WRL::ComPtr<ID3D12Fence> fence;
				if (SUCCEEDED(m_d3d12_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence))))
				{
					HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
					m_d3d12_command_queue->Signal(fence.Get(), 1);
					fence->SetEventOnCompletion(1, event);
					WaitForSingleObject(event, INFINITE);
					CloseHandle(event);
				}

				UINT srv_size = m_d3d12_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
				D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle = m_d3d12_srv_heap->GetCPUDescriptorHandleForHeapStart();
				D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle = m_d3d12_srv_heap->GetGPUDescriptorHandleForHeapStart();

				cpu_handle.ptr += static_cast<SIZE_T>(m_dx12_srv_allocated) * srv_size;
				gpu_handle.ptr += static_cast<UINT64>(m_dx12_srv_allocated) * srv_size;
				++m_dx12_srv_allocated;

				D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
				srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srv.Texture2D.MipLevels = 1;
				m_d3d12_device->CreateShaderResourceView(texture.Get(), &srv, cpu_handle);

				m_dx12_textures.push_back({std::move(texture), gpu_handle});
				return (ImTextureID)gpu_handle.ptr;
			}
		}
		return 0;
	}

	void renderer::merge_icon_with_latest_font(float font_size, bool FontDataOwnedByAtlas)
	{
		static const ImWchar icons_ranges[3] = {ICON_MIN_FA, ICON_MAX_FA, 0};

		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.FontDataOwnedByAtlas = FontDataOwnedByAtlas;

		m_font_icon = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)font_icons, sizeof(font_icons), font_size, &icons_config, icons_ranges);
	}
}
