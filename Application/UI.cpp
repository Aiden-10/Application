#include "UI.h"

#include "helper.hpp"
#include "UIs/settings.hpp"

void UI::Render()
{
    

	DrawRadar();

    settings::render();
}

void UI::DrawRadar()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 WindowPos = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y);
    ImVec2 windowSizeLast = windowSize;
    windowSize = viewport->WorkSize;
    
    if (windowSize.x != windowSizeLast.x || windowSize.x != windowSizeLast.x)
    {
        offset = { 0, 0 };
    }

	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(WindowPos, ImGuiCond_Always);
	ImGui::Begin("Radar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	radarHovered = ImGui::IsWindowHovered();
	RadarMousePos = { io.MousePos.x - ImGui::GetWindowPos().x, io.MousePos.y - ImGui::GetWindowPos().y };

	unsigned int image_dimension_x;
	unsigned int image_dimension_y;

	std::string path = "assets\\maps\\example.png";
	helper::get_png_image_dimensions(path, image_dimension_x, image_dimension_y);

	ImGui::SetCursorPos(offset);
	ImGui::Image(radarImage, ImVec2(image_dimension_x * zoom, image_dimension_y * zoom));

	HandleRadar();

    if (config::show_waypoints)
    {
        DrawWaypoints();
    }

    if (config::show_crosshair)
    {
        DrawCrosshair();
    }
}

void UI::HandleRadar()
{
    ImGuiIO& io = ImGui::GetIO();

    if (radarHovered)
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            if (!isMouseDragging) {
                // Mouse button was just pressed
                isMouseDragging = true;
                mouseDownPos = io.MousePos;
            }
            else {
                // Mouse button is being held down, update the image position
                ImVec2 currentMousePos = io.MousePos;
                ImVec2 delta = ImVec2(currentMousePos.x - mouseDownPos.x, currentMousePos.y - mouseDownPos.y);
                offset.x += delta.x;
                offset.y += delta.y;
                mouseDownPos = currentMousePos;
            }
        }
        else {
            if (isMouseDragging) {
                isMouseDragging = false;
            }
        }

        if (io.MouseWheel != 0) {

            if (io.MouseWheel > 0)
            {

                if (zoom < maxZoom)
                {
                    zoom *= 1.1;

                    offset.x -= (RadarMousePos.x - offset.x) * (1.1 - 1);
                    offset.y -= (RadarMousePos.y - offset.y) * (1.1 - 1);
                }
            }
            else
            {
                if (zoom > minZoom)
                {
                    zoom *= 1 / 1.1;
                    offset.x -= (RadarMousePos.x - offset.x) * (1 / 1.1 - 1);
                    offset.y -= (RadarMousePos.y - offset.y) * (1 / 1.1 - 1);
                }
            }
        }
    }
}

void UI::DrawWaypoints()
{
    for (auto i : waypoints)
    {
        float screenX = offset.x + (i.x * zoom);
        float screenY = offset.y + (i.y * zoom);

        ImGui::SetCursorPos({ screenX - 12.5f, screenY - 12.5f });
        ImGui::Image(waypoint_marker, { 25, 25 });
        ImGui::SetCursorPos({ screenX, screenY + 12.5f });
        ImGui::TextColored(ImColor(205, 0, 0), i.name);

        // std::cout << "Waypoint name: << " << i.name << " x: " << i.x << " y: " << i.y << std::endl;

    }
}

void UI::DrawCrosshair()
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 mouse = io.MousePos;
    std::cout << mouse.x << " " << mouse.y << std::endl;
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);

    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y));
    ImGui::SetNextWindowSize({ (float)windowRef.GetWindowSize().x, (float)windowRef.GetWindowSize().y });
    
    ImGui::Begin("Shadow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImVec2 windowPos = ImGui::GetWindowPos(); // Position of the current window
    ImVec2 localMousePos = ImVec2(mouse.x - windowPos.x, mouse.y - windowPos.y);

    ImGui::SetCursorPos({ localMousePos.x - 12.5f, localMousePos.y - 12.5f });
    ImGui::Image(icon_plus, { 25, 25 });

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !clicked)
    {
        clicked = true;
        newPoint.x = (localMousePos.x - offset.x) / zoom;
        newPoint.y = (localMousePos.y - offset.y) / zoom;
    }

    if (GetAsyncKeyState(VK_ESCAPE))
    {
        config::show_crosshair = false;
    }
    else if (clicked)
    {

        bool finished = false;

        ImVec2 windowSize = ImVec2((float)windowRef.GetWindowSize().x, (float)windowRef.GetWindowSize().y);
        ImVec2 viewportSize = viewport->Size;


        float settings_size_x = windowRef.GetWindowSize().x / 4;
        float settings_size_y = windowRef.GetWindowSize().y / 4;
        ImGui::SetNextWindowSize({ settings_size_x, settings_size_y });
        ImGui::SetNextWindowPos({ (viewport->WorkPos.x) + ((windowRef.GetWindowSize().x / 2) - (settings_size_x / 2)), (viewport->WorkPos.y) + ((windowRef.GetWindowSize().y / 2) - (settings_size_y / 2)) });

        ImGui::SetNextWindowFocus();
        ImGui::Begin("New Waypoint", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::Text("Enter a name for your new waypoint!");

        ImGui::InputText("Input Label", newPoint.name, sizeof(char) * 128);

        if (ImGui::Button("Submit"))
        {
            waypoints.push_back(newPoint);

            std::cout << "New Waypoint name: << " << newPoint.name << " x: " << newPoint.x << " y: " << newPoint.y << std::endl;

            config::show_crosshair = false; clicked = false;
        }

        ImGui::End();
    }

    ImGui::End();

    ImGui::StyleColorsDark();
}