#pragma once
#include "core/ui/graphic/graphic_enums.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace big
{
	// Owns the framework's view of the game's graphics API: it detects whether the
	// process runs DirectX 11 or 12, creates a throwaway device/swapchain for that
	// API and keeps the resulting method tables. The renderer and hook manager
	// only ever ask this class, so no game layer has to pattern scan or copy dev
	// swapchain vtables.
	class graphic_manager
	{
	public:
		// Index of Present/ResizeBuffers in the IDXGISwapChain vtable.
		static constexpr int swapchain_num_funcs           = 19;
		static constexpr int swapchain_present_index       = 8;
		static constexpr int swapchain_resizebuffers_index = 13;

		// Index of ExecuteCommandLists in the ID3D12CommandQueue vtable.
		static constexpr int command_queue_execute_command_lists_index = 10;

		// DirectX 12 when d3d12.dll is loaded in the process, otherwise DirectX 11.
		static eGraphicsAPI detect_graphics_api();

		static eGraphicsAPI get_render_type();
		static std::string get_render_type_name();

		// Creates the dummy device/swapchain for `render_type` (defaults to the
		// detected API) and caches the method tables. Calling it twice is a no-op.
		static eInitializationStatus get_swapchain(eGraphicsAPI render_type);
		static eInitializationStatus get_swapchain();

		// Slot of the cached IDXGISwapChain vtable (Present = 8, ResizeBuffers = 13).
		static void* get_method_table(int index);

		// ID3D12CommandQueue::ExecuteCommandLists, captured from a dummy queue;
		// nullptr when the process is not DirectX 12.
		static void* get_execute_command_lists();

	private:
		graphic_manager() = default;
		graphic_manager(const graphic_manager&) = delete;
		graphic_manager& operator=(const graphic_manager&) = delete;

		static graphic_manager& get_instance();

		eInitializationStatus create_dummy_device(eGraphicsAPI render_type);
		eInitializationStatus create_dx11_dummy(HWND window);
		eInitializationStatus create_dx12_dummy(HWND window);

		eGraphicsAPI m_render_type{eGraphicsAPI::none};
		std::vector<std::uintptr_t> m_swapchain_methods;
		void* m_execute_command_lists{};
	};
}
