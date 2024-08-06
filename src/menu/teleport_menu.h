#pragma once
#include <mhw/vector.hpp>

namespace big
{
	static void to_json(nlohmann::json& j, const Vector3& attachment)
	{
		j = nlohmann::json{
			{"position_x", attachment.x},
			{"position_y", attachment.y},
			{"position_z", attachment.z},
		};
	}

	static void from_json(const nlohmann::json& j, Vector3& attachment)
	{
		j.at("position_x").get_to(attachment.x);
		j.at("position_y").get_to(attachment.y);
		j.at("position_z").get_to(attachment.z);
	}

	class teleport_menu
	{
	public:
		static void render_menu();

	private:
		static std::filesystem::path get_config_file();
		static nlohmann::json get_json();
		static std::vector<std::string> location_list();
		static void save_json(nlohmann::json const& json);
		static void save(Vector3 location, std::string const& name);
		static void load_location(std::string const& name);
		static void delete_location(std::string const& name);
	};
}