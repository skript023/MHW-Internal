#pragma once
#include <chrono>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <imgui.h>

namespace big
{
	enum class NotificationType
	{
		INFO,
		SUCCESS,
		PROTECTED,
		WARNING,
		DANGER,
	};

	struct legacy_notification
	{
		NotificationType type;
		const std::string title;
		const std::string message;
		const std::chrono::time_point<std::chrono::system_clock> created_on;
		const float destroy_in;
		float alpha;
	};

	class notification_service final
	{
		static notification_service& get_service()
		{
			static notification_service instance;
			return instance;
		}

		std::unordered_map<std::size_t, legacy_notification> notifications;

		void push_impl(legacy_notification);
		void push_impl(std::string, std::string);
		void push_warning_impl(std::string, std::string);
		void push_error_impl(std::string, std::string);

		void success_impl(std::string, std::string);
		void info_impl(std::string, std::string);
		void warning_impl(std::string, std::string);
		void error_impl(std::string, std::string);

		void protection_impl(std::string);
		void report_impl(std::string);

		std::vector<legacy_notification> get_impl();

	public:
		notification_service() = default;
		~notification_service() = default;

		static void push(legacy_notification n)
		{
			get_service().push_impl(n);
		}
		static void push(std::string title, std::string message)
		{
			get_service().push_impl(title, message);
		}
		static void push_warning(std::string title, std::string message)
		{
			get_service().push_warning_impl(title, message);
		}
		static void push_error(std::string title, std::string message)
		{
			get_service().push_error_impl(title, message);
		}

		static void success(std::string title, std::string message)
		{
			get_service().success_impl(title, message);
		}
		static void info(std::string title, std::string message)
		{
			get_service().info_impl(title, message);
		}
		static void warning(std::string title, std::string message)
		{
			get_service().warning_impl(title, message);
		}
		static void error(std::string title, std::string message)
		{
			get_service().error_impl(title, message);
		}

		static void protection(std::string reason)
		{
			get_service().protection_impl(reason);
		}
		static void report(std::string reason)
		{
			get_service().report_impl(reason);
		}

		static std::vector<legacy_notification> get()
		{
			return get_service().get_impl();
		}

		inline static const std::map<NotificationType, ImVec4> notification_colors = {
		    {NotificationType::INFO, ImVec4(0.80f, 0.80f, 0.83f, 1.00f)},
		    {NotificationType::PROTECTED, ImVec4(0.29f, 0.34f, 0.69f, 1.00f)},
		    {NotificationType::SUCCESS, ImVec4(0.29f, 0.69f, 0.34f, 1.00f)},
		    {NotificationType::WARNING, ImVec4(0.69f, 0.49f, 0.29f, 1.00f)},
		    {NotificationType::DANGER, ImVec4(0.69f, 0.29f, 0.29f, 1.00f)},
		};
	};
}
