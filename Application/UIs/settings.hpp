#pragma once

#include "../UI.h"
#include "menuconfig.hpp"

class settings
{
public:
	static void render()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 WindowPos = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y);

		ImGui::SetNextWindowSize({200.f, 540.f}, ImGuiCond_Always);
		ImGui::SetNextWindowPos({ WindowPos.x + 10, WindowPos.y + 10}, ImGuiCond_Always);
		if (!config::show_crosshair)
		{
			ImGui::SetNextWindowFocus();
		}
		
		ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		ImGui::Text("Radar");
		ImGui::Spacing();

		// Todo: Add logic to select image from maps dir

        ImGui::Separator();
        ImGui::Spacing();

		ImGui::Text("Map Settings");
		ImGui::Spacing();

		// settings here

		ImGui::Checkbox("Center On Player", &config::center_on_player);
		ImGui::Checkbox("Show Waypoints", &config::show_waypoints);
		ImGui::Checkbox("Show Cars", &config::show_cars);
		ImGui::Checkbox("Show Animals", &config::show_animals);

		if (ImGui::Button("Add Waypoint"))
		{
			config::show_crosshair = !config::show_crosshair;
		}

		ImGui::Separator();
		ImGui::Spacing();
		
		ImGui::Text("Player Settings");
		ImGui::Spacing();

		// settings here 
		ImGui::Checkbox("Player Names", &config::show_player_names);
		ImGui::Checkbox("Player Distance", &config::show_player_distance);
		ImGui::Checkbox("Player Lines", &config::show_player_lines);
		ImGui::Checkbox("Player Hands", &config::show_player_hands);
		ImGui::Checkbox("Player Inventory", &config::show_player_inventory);

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Loot Settings");
		ImGui::Spacing();

		// settings here
		ImGui::Checkbox("Show Loot", &config::show_loot);
		ImGui::Checkbox("Container Inventory", &config::show_loot_inventory);

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Misc Settings");
		ImGui::Spacing();

		// settings here
		ImGui::Checkbox("Bright Night", &config::bright_night);
		ImGui::Checkbox("No Grass", &config::no_grass);
        
		ImGui::End();
	}
};