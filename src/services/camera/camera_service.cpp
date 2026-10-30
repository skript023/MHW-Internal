#include "camera_service.hpp"
#include <pointers.hpp>
#include <mhw/fwddec.hpp>
#include <imgui.h>
#include <algorithm>
#include <cstring>

namespace big
{
	uintptr_t camera_service::get_camera_ptr_impl() const
	{
		if (!g_pointers || !g_pointers->m_camera || !*g_pointers->m_camera)
			return 0;

		return reinterpret_cast<uintptr_t>(*g_pointers->m_camera);
	}

	Vector3 camera_service::get_camera_pos_impl() const
	{
		auto* cam = reinterpret_cast<const sCamera*>(get_camera_ptr_impl());
		if (!cam)
			return Vector3(0.0f, 0.0f, 0.0f);

		const auto& v = cam->m_view_matrix;
		// World Camera Position is -transpose(R) * T
		float rx = -(v.m[0] * v.m[12] + v.m[1] * v.m[13] + v.m[2] * v.m[14]);
		float ry = -(v.m[4] * v.m[12] + v.m[5] * v.m[13] + v.m[6] * v.m[14]);
		float rz = -(v.m[8] * v.m[12] + v.m[9] * v.m[13] + v.m[10] * v.m[14]);
		return Vector3(rx, ry, rz);
	}

	Vector3 camera_service::get_camera_forward_impl() const
	{
		auto* cam = reinterpret_cast<const sCamera*>(get_camera_ptr_impl());
		if (!cam)
			return Vector3(0.0f, 0.0f, 1.0f);

		const auto& v = cam->m_view_matrix;
		// Column 2 of row-major view matrix is camera forward
		return Vector3(v.m[2], v.m[6], v.m[10]);
	}

	Vector3 camera_service::get_camera_right_impl() const
	{
		auto* cam = reinterpret_cast<const sCamera*>(get_camera_ptr_impl());
		if (!cam)
			return Vector3(1.0f, 0.0f, 0.0f);

		const auto& v = cam->m_view_matrix;
		// Column 0 of row-major view matrix is camera right
		return Vector3(v.m[0], v.m[4], v.m[8]);
	}

	bool camera_service::world_to_screen_impl(const Vector3& world_pos, Vector2& screen_pos) const
	{
		auto* cam = reinterpret_cast<const sCamera*>(get_camera_ptr_impl());
		if (!cam)
			return false;

		const auto& v = cam->m_view_matrix;
		const auto& p = cam->m_proj_matrix;

		// 1. World Space -> Camera View Space: [world_x, world_y, world_z, 1.0] @ V
		float cx = world_pos.x * v.m[0] + world_pos.y * v.m[4] + world_pos.z * v.m[8]  + v.m[12];
		float cy = world_pos.x * v.m[1] + world_pos.y * v.m[5] + world_pos.z * v.m[9]  + v.m[13];
		float cz = world_pos.x * v.m[2] + world_pos.y * v.m[6] + world_pos.z * v.m[10] + v.m[14];
		float cw = world_pos.x * v.m[3] + world_pos.y * v.m[7] + world_pos.z * v.m[11] + v.m[15];

		// 2. Camera View Space -> Clip Space: [cx, cy, cz, cw] @ P
		float clip_x = cx * p.m[0] + cy * p.m[4] + cz * p.m[8]  + cw * p.m[12];
		float clip_y = cx * p.m[1] + cy * p.m[5] + cz * p.m[9]  + cw * p.m[13];
		float clip_z = cx * p.m[2] + cy * p.m[6] + cz * p.m[10] + cw * p.m[14];
		float clip_w = cx * p.m[3] + cy * p.m[7] + cz * p.m[11] + cw * p.m[15];

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
