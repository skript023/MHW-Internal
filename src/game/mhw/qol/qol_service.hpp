#pragma once
#include "game/settings.hpp"
#include <cstdint>

namespace big
{
	class qol_service
	{
		static qol_service& get()
		{
			static qol_service instance;
			return instance;
		}

		bool m_initialized{false};
		bool m_hub_speed_applied{false};
		bool m_carve_speed_applied{false};
		bool m_tic_rate_fix_active{false};
		float m_current_tick_scale{1.0f};
		uint32_t m_last_quest_id{0};
		float m_orig_room_run_speed{1.0f};
		float m_orig_carve_speeds[4]{1.0f, 1.0f, 1.0f, 1.0f};

		void init_impl();
		void tick_impl();
		void skip_cutscene_impl();
		void skip_post_quest_timer_impl();
		void apply_hub_speed_impl(bool enable, float mult = 2.0f);
		void apply_fast_carve_impl(bool enable, float mult = 10.0f);
		void apply_tic_rate_fix_impl(bool enable);
		bool is_tic_rate_fix_active_impl() const;
		float get_current_tick_scale_impl() const;
		void open_remote_item_box_impl();
		void open_remote_equipment_impl();
		void quick_restart_quest_impl();
		void quick_reset_quest_impl();
		void on_gather_node_impl();

	public:
		explicit qol_service() = default;
		~qol_service() noexcept = default;

		qol_service(const qol_service&) = delete;
		qol_service& operator=(const qol_service&) = delete;
		qol_service(qol_service&&) = delete;
		qol_service& operator=(qol_service&&) = delete;

		static void init()
		{
			get().init_impl();
		}

		static void tick()
		{
			get().tick_impl();
		}

		static void skip_cutscene()
		{
			get().skip_cutscene_impl();
		}

		static void skip_post_quest_timer()
		{
			get().skip_post_quest_timer_impl();
		}

		static void apply_hub_speed(bool enable, float mult = 2.0f)
		{
			get().apply_hub_speed_impl(enable, mult);
		}

		static void apply_fast_carve(bool enable, float mult = 10.0f)
		{
			get().apply_fast_carve_impl(enable, mult);
		}

		static void apply_tic_rate_fix(bool enable)
		{
			get().apply_tic_rate_fix_impl(enable);
		}

		static bool is_tic_rate_fix_active()
		{
			return get().is_tic_rate_fix_active_impl();
		}

		static float get_current_tick_scale()
		{
			return get().get_current_tick_scale_impl();
		}

		static void open_remote_item_box()
		{
			get().open_remote_item_box_impl();
		}

		static void open_remote_equipment()
		{
			get().open_remote_equipment_impl();
		}

		static void quick_restart_quest()
		{
			get().quick_restart_quest_impl();
		}

		static void quick_reset_quest()
		{
			get().quick_reset_quest_impl();
		}

		static void on_gather_node()
		{
			get().on_gather_node_impl();
		}
	};
}
