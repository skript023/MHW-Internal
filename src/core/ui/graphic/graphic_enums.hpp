#pragma once

namespace big
{
	// Monster Hunter: World only ships DirectX 11 and DirectX 12 builds, so the
	// framework's graphic layer only carries those two.
	enum class eGraphicsAPI
	{
		none,
		directx11,
		directx12
	};

	enum class eInitializationStatus
	{
		SUCCESS,
		FAILED,
		MODULE_NOT_FOUND_ERROR,
		CREATE_DUMMY_DEVICE_ERROR
	};
}
