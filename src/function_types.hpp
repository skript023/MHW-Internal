#pragma once
#include "common.hpp"
#include <class/fwddec.hpp>

namespace big::functions
{
	using create_d3d11_device_and_swapchain_t = long(__stdcall*)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
	using use_item_t = void(__fastcall*)(Consumeable* rax, int a2, int8_t a3);
	using select_weapon_t = __int64(__fastcall*)(uint32_t a1, uint32_t a2);
	using select_armour_t = Equipment*(__fastcall*)(uint32_t a1, uint32_t a2);
}
