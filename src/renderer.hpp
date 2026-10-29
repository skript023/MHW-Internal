#pragma once
#include "common.hpp"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>
#include <mutex>
#include <vector>

namespace big
{
	enum class renderer_api
	{
		unknown,
		dx11,
		dx12
	};

	struct dx12_frame_context
	{
		ID3D12CommandAllocator* command_allocator = nullptr;
		ID3D12Resource* render_target = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE rtv_descriptor{};
	};

	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		bool m_init = false;
		bool init(IDXGISwapChain* swapchain);
		void imgui_init();

		void on_present(IDXGISwapChain* swapchain);

		void pre_reset();
		void post_reset(IDXGISwapChain* this_);
		void merge_icon_with_latest_font(float font_size, bool FontDataOwnedByAtlas = false);

		bool wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		ImTextureID upload_rgba(const unsigned char* pixels, int width, int height);
		void release_texture(ImTextureID texture);

		void set_command_queue(ID3D12CommandQueue* queue);
		renderer_api api() const { return m_api; }

	public:
		ImFont* m_font = nullptr;
		ImFont* m_monospace_font = nullptr;

		HWND m_window = NULL;
		std::recursive_mutex render_mutex;

	private:
		renderer_api m_api = renderer_api::unknown;

		// DX11 resources
		ID3D11Device* m_d3d_device = nullptr;
		ID3D11DeviceContext* m_d3d_context = nullptr;
		ID3D11RenderTargetView* m_d3d_render_target = nullptr;
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_dx11_textures;

		// DX12 resources
		ID3D12Device* m_d3d12_device = nullptr;
		ID3D12CommandQueue* m_d3d12_command_queue = nullptr;
		ID3D12DescriptorHeap* m_d3d12_rtv_heap = nullptr;
		ID3D12DescriptorHeap* m_d3d12_srv_heap = nullptr;
		ID3D12GraphicsCommandList* m_d3d12_command_list = nullptr;
		std::vector<dx12_frame_context> m_frame_contexts;
		UINT m_buffer_count = 0;
		UINT m_dx12_srv_allocated = 1;
		struct dx12_texture
		{
			Microsoft::WRL::ComPtr<ID3D12Resource> resource;
			D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle{};
		};
		std::vector<dx12_texture> m_dx12_textures;

		bool init_dx11(IDXGISwapChain* swapchain);
		bool init_dx12(IDXGISwapChain* swapchain);
		void cleanup_dx11();
		void cleanup_dx12();
		void render_dx11();
		void render_dx12(IDXGISwapChain* swapchain);
	};

	inline renderer* g_renderer{};
}
