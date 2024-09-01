#pragma once
#include "alpha_module.hpp"
#include "interfaces/subscribe/message.hpp"

namespace big
{
	class alpha_service
	{
	public:
		explicit alpha_service(alpha_module client_module) :
			m_alpha_gateway(client_module.getClient())
		{
		}

		~alpha_service() noexcept = default;

		void find()
		{
			m_event.event = "findAllAlpha";

			m_alpha_gateway->send_message(m_event.to_json_string());
			m_alpha_gateway->on_message_received([](std::string const& message) {
				if (message.empty())
				{
					LOG(HACKER) << "Received an empty message";
				}
				else
				{
					LOG(HACKER) << "Received: " << message;
				}
			});
		}

		void ping()
		{
			m_event.event = "pingAlpha";

			m_alpha_gateway->send_message(m_event.to_json_string());
			m_alpha_gateway->on_message_received([](std::string const& message) {
				if (message.empty())
				{
					LOG(HACKER) << "Received an empty message";
				}
				else
				{
					LOG(HACKER) << "Received: " << message;
				}
			});
		}

		void poll()
		{
			m_alpha_gateway->poll();
		}
	private:
		std::shared_ptr<socket_client> m_alpha_gateway;
		Gateway m_event;
	};
}
