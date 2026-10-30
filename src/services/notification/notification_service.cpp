#include "notification_service.hpp"

#include "fonts/font_list.hpp"
#include "fonts/icon_list.hpp"
#include <astra/host/notification.hpp>

namespace big
{
	void notification_service::push_impl(legacy_notification n)
	{
		this->notifications.emplace(std::hash<std::string>{}(n.message + n.title), n);
	}

	void notification_service::push_impl(std::string title, std::string message)
	{
		big::notification::info(title, message);
		this->push_impl({NotificationType::INFO, ICON_FA_INFO_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f, 1.f});
	}

	void notification_service::push_warning_impl(std::string title, std::string message)
	{
		big::notification::warning(title, message);
		this->push_impl({NotificationType::WARNING, ICON_FA_EXCLAMATION_TRIANGLE " " + title, message, std::chrono::system_clock::now(), 3000.f, 1.f});
	}

	void notification_service::push_error_impl(std::string title, std::string message)
	{
		big::notification::error(title, message);
		this->push_impl({NotificationType::DANGER, ICON_FA_TIMES_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f, 1.f});
	}

	void notification_service::success_impl(std::string title, std::string message)
	{
		big::notification::success(title, message);
		this->push_impl({NotificationType::SUCCESS, ICON_FA_CHECK_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f, 1.f});
	}

	void notification_service::info_impl(std::string title, std::string message)
	{
		big::notification::info(title, message);
		this->push_impl({NotificationType::INFO, ICON_FA_INFO_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f, 1.f});
	}

	void notification_service::warning_impl(std::string title, std::string message)
	{
		big::notification::warning(title, message);
		this->push_impl({NotificationType::WARNING, ICON_FA_EXCLAMATION_TRIANGLE " " + title, message, std::chrono::system_clock::now(), 3000.f, 1.f});
	}

	void notification_service::error_impl(std::string title, std::string message)
	{
		big::notification::error(title, message);
		this->push_impl({NotificationType::DANGER, ICON_FA_TIMES_CIRCLE " " + title, message, std::chrono::system_clock::now(), 3000.f, 1.f});
	}

	void notification_service::protection_impl(std::string message)
	{
		big::notification::protection(message);
		this->push_impl({NotificationType::PROTECTED, ICON_FA_SHIELD_ALT " Illegal Event Protection", message, std::chrono::system_clock::now(), 3000.f, 1.f});
	}

	void notification_service::report_impl(std::string message)
	{
		big::notification::report(message);
		this->push_impl({NotificationType::PROTECTED, ICON_FA_USER_SHIELD " Report Protection", message, std::chrono::system_clock::now(), 3000.f, 1.f});
	}

	std::vector<legacy_notification> notification_service::get_impl()
	{
		std::vector<legacy_notification> notifications_to_sent;
		std::vector<std::size_t> to_remove;
		for (auto& n : this->notifications)
		{
			std::chrono::time_point<std::chrono::system_clock> curTime = std::chrono::system_clock::now();
			const float time_diff =
			    (float)std::chrono::duration_cast<std::chrono::milliseconds>(curTime - n.second.created_on).count();
			n.second.alpha = 1;
			if (n.second.destroy_in <= time_diff)
			{
				n.second.alpha = 1.f - ((time_diff - n.second.destroy_in) / 600);
				n.second.alpha = n.second.alpha < 0.f ? 0.f : n.second.alpha;
			}

			if (n.second.alpha > 0.f)
				notifications_to_sent.push_back(n.second);
			else
				to_remove.push_back(n.first);
		}
		for (std::size_t k : to_remove)
			this->notifications.erase(k);

		return notifications_to_sent;
	}
}
