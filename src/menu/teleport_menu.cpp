#include "teleport_menu.h"
#include <character/character_service.hpp>

namespace big
{
    void teleport_menu::render_menu()
    {
        ImGui::BeginGroup();

        if (ImGui::Button("Teleport Waypoint"))
        {
            g_character_service->set_player_to_waypoint();
        }

        ImGui::SameLine();

        if (ImGui::Button("Teleport Forward"))
        {
            g_character_service->set_player_forward();
        }

        ImGui::EndGroup();
        auto teleport_locations = location_list();

        static std::string selected_location;

        static char teleport_name[50]{};

        ImGui::PushItemWidth(200);
        ImGui::InputText("Location Name", teleport_name, IM_ARRAYSIZE(teleport_name));
        ImGui::PopItemWidth();

        ImGui::SameLine();

        if (ImGui::Button("Save Location"))
        {
            save(g_character_service->get_position(), teleport_name);
            ZeroMemory(teleport_name, sizeof(teleport_name));
        }

        ImGui::PushItemWidth(250);
        ImGui::Text("Saved Locations");
        if (ImGui::BeginListBox("##empty", ImVec2(200, 200)))
        {
            for (auto pair : teleport_locations)
            {
                if (ImGui::Selectable(pair.c_str(), selected_location == pair))
                    selected_location = pair;
            }
            ImGui::EndListBox();
        }
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::BeginGroup();

        if (ImGui::Button("Load Location"))
        {
            load_location(selected_location);
        }

        if (ImGui::Button("Delete Location"))
        {
            if (!selected_location.empty())
            {
                delete_location(selected_location);
                selected_location.clear();
            }
        }
        ImGui::EndGroup();
    }
    std::filesystem::path teleport_menu::get_config_file()
	{
        auto file_path = std::filesystem::path(std::getenv("appdata"));

        file_path /= FOLDER;

        if (!std::filesystem::exists(file_path))
        {
            std::filesystem::create_directory(file_path);
        }
        else if (!std::filesystem::is_directory(file_path))
        {
            std::filesystem::remove(file_path);
            std::filesystem::create_directory(file_path);
        }

        file_path /= "Teleport.json";

        return file_path;
	}
    nlohmann::json teleport_menu::get_json()
    {
        auto file_path = get_config_file();

        nlohmann::json locations;

        std::ifstream file(file_path);

        if (!file.fail())
            file >> locations;

        return locations;
    }
    std::vector<std::string> teleport_menu::location_list()
    {
        std::vector<std::string> return_value;

        auto json = get_json();

        for (auto& item : json.items())
            return_value.push_back(item.key());

        return return_value;
    }
    void teleport_menu::save_json(nlohmann::json const& json)
    {
        auto file_path = get_config_file();

        std::ofstream file(file_path, std::ios::out | std::ios::trunc);

        file << json.dump(4);

        file.close();
    }
    void teleport_menu::save(Vector3 location, std::string const& name)
    {
        auto json = get_json();

        json[name] = location;

        save_json(json);
    }
    void teleport_menu::load_location(std::string const& name)
    {
        auto locations = get_json();

        if (locations[name].is_null())
            return;

        auto target_position = locations[name].get<Vector3>();

        g_character_service->set_player_coords(target_position);
    }
    void teleport_menu::delete_location(std::string const& name)
    {
        auto locations = get_json();

        if (locations[name].is_null())
            return;

        locations.erase(name);

        save_json(locations);
    }
}