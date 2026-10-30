#include "camera_service.hpp"
#include <pointers.hpp>
#include <imgui.h>
#include <algorithm>
#include <cstring>

namespace
{
	template <typename T>
	static bool safe_read(uintptr_t addr, T* out)
	{
		if (addr < 0x10000)
			return false;

		__try
		{
			*out = *reinterpret_cast<const T*>(addr);
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
	}

	static bool safe_read_bytes(uintptr_t addr, void* out, size_t size)
	{
		if (addr < 0x10000)
			return false;

		__try
		{
			std::memcpy(out, reinterpret_cast<const void*>(addr), size);
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			return false;
		}
	}
}

namespace big
{
	uintptr_t camera_service::get_camera_ptr_impl() const
	{
		if (!g_pointers || !g_pointers->m_camera)
			return 0;

		uintptr_t cam = 0;
		if (safe_read(reinterpret_cast<uintptr_t>(g_pointers->m_camera), &cam) && cam >= 0x10000)
		{
			return cam;
		}
		return 0;
	}

	Vector3 camera_service::get_camera_pos_impl() const
	{
		uintptr_t cam = get_camera_ptr_impl();
		if (!cam)
			return Vector3(0.0f, 0.0f, 0.0f);

		float v[16]{};
		if (safe_read_bytes(cam + 0xF0, v, sizeof(v)))
		{
			// World Camera Position is -transpose(R) * T
			float rx = -(v[0] * v[12] + v[1] * v[13] + v[2] * v[14]);
			float ry = -(v[4] * v[12] + v[5] * v[13] + v[6] * v[14]);
			float rz = -(v[8] * v[12] + v[9] * v[13] + v[10] * v[14]);
			return Vector3(rx, ry, rz);
		}

		return Vector3(0.0f, 0.0f, 0.0f);
	}

	Vector3 camera_service::get_camera_forward_impl() const
	{
		uintptr_t cam = get_camera_ptr_impl();
		if (!cam)
			return Vector3(0.0f, 0.0f, 1.0f);

		float v[16]{};
		if (safe_read_bytes(cam + 0xF0, v, sizeof(v)))
		{
			// Column 2 of row-major view matrix is camera forward
			return Vector3(v[2], v[6], v[10]);
		}

		return Vector3(0.0f, 0.0f, 1.0f);
	}

	Vector3 camera_service::get_camera_right_impl() const
	{
		uintptr_t cam = get_camera_ptr_impl();
		if (!cam)
			return Vector3(1.0f, 0.0f, 0.0f);

		float v[16]{};
		if (safe_read_bytes(cam + 0xF0, v, sizeof(v)))
		{
			// Column 0 of row-major view matrix is camera right
			return Vector3(v[0], v[4], v[8]);
		}

		return Vector3(1.0f, 0.0f, 0.0f);
	}

	bool camera_service::world_to_screen_impl(const Vector3& world_pos, Vector2& screen_pos) const
	{
		uintptr_t cam = get_camera_ptr_impl();
		if (!cam)
			return false;

		float v[16]{};
		float p[16]{};

		// In sCamera sub-camera 0:
		// View Matrix is at cam + 0xF0 (64 bytes)
		// Projection Matrix is at cam + 0x130 (64 bytes)
		if (!safe_read_bytes(cam + 0xF0, v, sizeof(v)) ||
		    !safe_read_bytes(cam + 0x130, p, sizeof(p)))
		{
			return false;
		}

		// 1. World Space -> Camera View Space: [world_x, world_y, world_z, 1.0] @ V
		float cx = world_pos.x * v[0] + world_pos.y * v[4] + world_pos.z * v[8]  + v[12];
		float cy = world_pos.x * v[1] + world_pos.y * v[5] + world_pos.z * v[9]  + v[13];
		float cz = world_pos.x * v[2] + world_pos.y * v[6] + world_pos.z * v[10] + v[14];
		float cw = world_pos.x * v[3] + world_pos.y * v[7] + world_pos.z * v[11] + v[15];

		// 2. Camera View Space -> Clip Space: [cx, cy, cz, cw] @ P
		float clip_x = cx * p[0] + cy * p[4] + cz * p[8]  + cw * p[12];
		float clip_y = cx * p[1] + cy * p[5] + cz * p[9]  + cw * p[13];
		float clip_z = cx * p[2] + cy * p[6] + cz * p[10] + cw * p[14];
		float clip_w = cx * p[3] + cy * p[7] + cz * p[11] + cw * p[15];

		// Behind the camera check (clip_w is positive in front of camera)
		if (clip_w <= 0.001f)
			return false;

		// 3. Perspective Divide -> Normalized Device Coordinates (NDC)
		float inv_w = 1.0f / clip_w;
		float ndc_x = clip_x * inv_w;
		float ndc_y = clip_y * inv_w;

		// 4. Viewport Mapping -> Screen Pixels
		const auto* viewport = ImGui::GetMainViewport();
		if (!viewport)
			return false;

		float screen_w = viewport->WorkSize.x;
		float screen_h = viewport->WorkSize.y;

		screen_pos.x = viewport->WorkPos.x + (ndc_x * 0.5f + 0.5f) * screen_w;
		screen_pos.y = viewport->WorkPos.y + (1.0f - (ndc_y * 0.5f + 0.5f)) * screen_h;

		return true;
	}
}
