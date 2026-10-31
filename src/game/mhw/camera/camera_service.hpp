#pragma once
#include "core/common.hpp"
#include "core/utility/vector.hpp"

namespace big
{
	class camera_service
	{
		static camera_service& get()
		{
			static camera_service instance;
			return instance;
		}

		[[nodiscard]] bool world_to_screen_impl(const Vector3& world_pos, Vector2& screen_pos) const;
		[[nodiscard]] Vector3 get_camera_pos_impl() const;
		[[nodiscard]] Vector3 get_camera_forward_impl() const;
		[[nodiscard]] Vector3 get_camera_right_impl() const;
		[[nodiscard]] uintptr_t get_camera_ptr_impl() const;

	public:
		explicit camera_service() = default;
		~camera_service() noexcept = default;

		[[nodiscard]] static bool world_to_screen(const Vector3& world_pos, Vector2& screen_pos)
		{
			return get().world_to_screen_impl(world_pos, screen_pos);
		}

		[[nodiscard]] static Vector3 get_camera_pos()
		{
			return get().get_camera_pos_impl();
		}

		[[nodiscard]] static Vector3 get_camera_forward()
		{
			return get().get_camera_forward_impl();
		}

		[[nodiscard]] static Vector3 get_camera_right()
		{
			return get().get_camera_right_impl();
		}

		[[nodiscard]] static uintptr_t get_camera_ptr()
		{
			return get().get_camera_ptr_impl();
		}
	};
}
